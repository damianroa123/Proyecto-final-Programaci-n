#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <limits>

using namespace std;


struct Lectura {
    char tipo; 
    double valorD;   
    int sis, dia; 
};

struct Medicion {
    char idPaciente[12]; 
    char fechaHora[25];  
    unsigned int numLecturas; 
    Lectura* lecturas;
};

struct MaquinaUCI {
    unsigned char id; 
    unsigned int numMediciones;
    Medicion* mediciones;
};

struct SalaUCI {
    unsigned char id;
    unsigned char numMaquinas;
    MaquinaUCI* maquinas;
};

struct Configuracion {
    char tipo[10]; 
    double min, max;
};

struct Paciente {
    char id[12];
    char tipoDoc[5];
    char documento[20];
    char nombres[50];
    char apellidos[50];
    char fechaNac[12];
    char telefono[20];
    char email[61];
    char tipoSangre[5];
    char entidadSalud[50];
    char medicinaPrepagada[50];
};

struct ArchivosPacientes {
    unsigned int numPacientes;
    Paciente* pacientes;
};


void limpiarCadena(char* cadena) {
    if (!cadena) return;
    int i = strlen(cadena) - 1;
    while (i >= 0 && (unsigned char)cadena[i] <= 32) {
        cadena[i] = '\0';
        i--;
    }
}

void copiarSeguro(char* destino, const char* origen, int maxTam) {
    if (!origen || !destino) return;
    strncpy(destino, origen, maxTam - 1);
    destino[maxTam - 1] = '\0';
    limpiarCadena(destino);
}

void liberarTokens(char** tokens, int n) {
    if (!tokens) return;
    for (int i = 0; i < n; i++) delete[] tokens[i];
    delete[] tokens;
}

char** parsearLinea(const char* linea, int& numTokens, char delim) {
    if (!linea || linea[0] == '#' || strlen(linea) < 2) {
        numTokens = 0; return nullptr;
    }
    char* copia = new char[strlen(linea) + 1];
    strcpy(copia, linea);
    
    // Contar tokens reales
    numTokens = 1;
    for (int i = 0; copia[i]; i++) if (copia[i] == delim) numTokens++;
    
    char** tokens = new char*[numTokens];
    char delimStr[2] = {delim, '\0'};
    

    char* t = strtok(copia, delimStr);
    int i = 0;
    while (t != nullptr && i < numTokens) {
        tokens[i] = new char[strlen(t) + 1];
        strcpy(tokens[i], t);
        i++;
        t = strtok(NULL, delimStr);
    }
    delete[] copia;
    return tokens;
}

void liberarSala(SalaUCI*& s) {
    if (!s) return;
    for (int i = 0; i < s->numMaquinas; i++) {
        if (s->maquinas[i].mediciones) {
            for (unsigned int j = 0; j < s->maquinas[i].numMediciones; j++) {
                if (s->maquinas[i].mediciones[j].lecturas)
                    delete[] s->maquinas[i].mediciones[j].lecturas;
            }
            delete[] s->maquinas[i].mediciones;
        }
    }
    delete[] s->maquinas;
    delete s;
    s = nullptr;
}


void ordenarPacientesID(ArchivosPacientes& p) {
    if (p.numPacientes < 2) return;
    for (unsigned int i = 0; i < p.numPacientes - 1; i++) {
        for (unsigned int j = 0; j < p.numPacientes - i - 1; j++) {
            if (strcmp(p.pacientes[j].id, p.pacientes[j+1].id) > 0) {
                Paciente temp = p.pacientes[j];
                p.pacientes[j] = p.pacientes[j+1];
                p.pacientes[j+1] = temp;
            }
        }
    }
}

int buscarPacienteBinario(const char* idBuscado, const ArchivosPacientes& p) {
    if (!p.pacientes) return -1;
    int inicio = 0, fin = (int)p.numPacientes - 1;
    while (inicio <= fin) {
        int medio = inicio + (fin - inicio) / 2;
        int comp = strcmp(p.pacientes[medio].id, idBuscado);
        if (comp == 0) return medio;
        if (comp < 0) inicio = medio + 1;
        else fin = medio - 1;
    }
    return -1;
}



void leerConfig(const char* path, Configuracion*& c, int& n) {
    ifstream archivo(path);
    if (!archivo) { cout << "[!] No se pudo abrir: " << path << endl; return; }
    if (c) { delete[] c; c = nullptr; }
    int cap = 10; c = new Configuracion[cap]; n = 0;
    char linea[256];
    while (archivo.getline(linea, 256)) {
        int nt; char** tk = parsearLinea(linea, nt, ',');
        if (tk && nt >= 3) {
            if (n >= cap) {
                cap *= 2; Configuracion* nuevo = new Configuracion[cap];
                for (int j = 0; j < n; j++) nuevo[j] = c[j];
                delete[] c; c = nuevo;
            }
            copiarSeguro(c[n].tipo, tk[0], 10);
            c[n].min = atof(tk[1]);
            c[n].max = atof(tk[2]);
            n++;
        }
        if (tk) liberarTokens(tk, nt);
    }
    archivo.close();
}


void leerPacientes(const char* path, ArchivosPacientes& p) {
    ifstream archivo(path);
    if (!archivo) { cout << "[!] No se pudo abrir: " << path << endl; return; }
    if (p.pacientes) { delete[] p.pacientes; p.pacientes = nullptr; }
    int cap = 10; p.pacientes = new Paciente[cap]; p.numPacientes = 0;
    char linea[1024];
    while (archivo.getline(linea, 1024)) {
        int nt; char** tk = parsearLinea(linea, nt, ';');
        if (tk && nt >= 1) {
            // AJUSTE: Si el archivo empieza con ';', el ID estará en tk[1] en lugar de tk[0]
            int offset = (strlen(tk[0]) == 0 || (unsigned char)tk[0][0] <= 32) ? 1 : 0;
            
            if (nt >= (offset + 5)) { // Verificamos que al menos haya ID, Nombres y Apellidos
                if (p.numPacientes >= (unsigned int)cap) {
                    cap *= 2; Paciente* nuevo = new Paciente[cap];
                    for (unsigned int j = 0; j < p.numPacientes; j++) nuevo[j] = p.pacientes[j];
                    delete[] p.pacientes; p.pacientes = nuevo;
                }
                
                // Mapeo dinámico basado en el offset
                copiarSeguro(p.pacientes[p.numPacientes].id, tk[offset], 12);
                copiarSeguro(p.pacientes[p.numPacientes].nombres, tk[offset + 2], 50);
                copiarSeguro(p.pacientes[p.numPacientes].apellidos, tk[offset + 3], 50);
                p.numPacientes++; 
            }
        }
        if (tk) liberarTokens(tk, nt);
    }
    archivo.close();
}

SalaUCI* leerBSF(const char* path) {
    ifstream f(path, ios::binary);
    if (!f) return nullptr;
    
    SalaUCI* s = new SalaUCI;
    f.read((char*)&s->id, 1);
    f.read((char*)&s->numMaquinas, 1);
    
    if (s->numMaquinas > 100) { 
        cout << "[!] Archivo corrupto: Demasiadas maquinas.\n";
        s->numMaquinas = 0; s->maquinas = nullptr; return s; 
    }

    s->maquinas = new MaquinaUCI[s->numMaquinas];
    for (int i = 0; i < s->numMaquinas; i++) {
        f.read((char*)&s->maquinas[i].id, 1);
        f.read((char*)&s->maquinas[i].numMediciones, 4);
        
        if (s->maquinas[i].numMediciones > 10000) {
            s->maquinas[i].numMediciones = 0;
            s->maquinas[i].mediciones = nullptr;
        } else {
            s->maquinas[i].mediciones = new Medicion[s->maquinas[i].numMediciones];
            for (unsigned int j = 0; j < s->maquinas[i].numMediciones; j++) {
                Medicion& m = s->maquinas[i].mediciones[j];
                f.read(m.idPaciente, 11); m.idPaciente[11] = '\0';
                limpiarCadena(m.idPaciente);
                f.read(m.fechaHora, 24); m.fechaHora[24] = '\0';
                f.read((char*)&m.numLecturas, 4);

                if (m.numLecturas > 10000) {
                    m.numLecturas = 0; m.lecturas = nullptr;
                } else {
                    m.lecturas = new Lectura[m.numLecturas];
                    for (unsigned int k = 0; k < m.numLecturas; k++) {
                        f.read(&m.lecturas[k].tipo, 1);
                        if (m.lecturas[k].tipo == 'P') {
                            f.read((char*)&m.lecturas[k].sis, 4);
                            f.read((char*)&m.lecturas[k].dia, 4);
                        } else f.read((char*)&m.lecturas[k].valorD, 8);
                    }
                }
            }
        }
    }
    f.close();
    return s;
}


bool esAnomaloECG(Medicion& m, double minE, double maxE) {
    double vMin = 1e9, vMax = -1e9; bool tieneE = false;
    if (!m.lecturas) return false;
    for (unsigned int k = 0; k < m.numLecturas; k++) {
        if (m.lecturas[k].tipo == 'E') {
            tieneE = true;
            if (m.lecturas[k].valorD < vMin) vMin = m.lecturas[k].valorD;
            if (m.lecturas[k].valorD > vMax) vMax = m.lecturas[k].valorD;
        }
    }
    return tieneE && ((vMax - vMin) < minE || (vMax - vMin) > maxE);
}

void calcularEstadisticas(SalaUCI* s) {
    if (!s) { cout << "[!] No hay datos binarios cargados.\n"; return; }
    cout << "\n--- ESTADISTICAS SALA #" << (int)s->id << " ---\n";
    for (int i = 0; i < s->numMaquinas; i++) {
        double sumaT = 0, maxO = 0;
        int cT = 0, cO = 0;
        for (unsigned int j = 0; j < s->maquinas[i].numMediciones; j++) {
            if (!s->maquinas[i].mediciones[j].lecturas) continue;
            for (unsigned int k = 0; k < s->maquinas[i].mediciones[j].numLecturas; k++) {
                Lectura& l = s->maquinas[i].mediciones[j].lecturas[k];
                if (l.tipo == 'T') { sumaT += l.valorD; cT++; }
                if (l.tipo == 'O') { if (l.valorD > maxO) maxO = l.valorD; cO++; }
            }
        }
        cout << "Maq " << (int)s->maquinas[i].id << ": ";
        if (cT > 0) cout << "Temp Prom: " << (sumaT / cT) << "C | ";
        if (cO > 0) cout << "Oxi Max: " << maxO << "%";
        else cout << "Sin datos de O2";
        cout << endl;
    }
}

void exportarReporte(SalaUCI* s, Configuracion* c, int nC, ArchivosPacientes& p) {
    if (!s || !c || !p.pacientes) return;
    ordenarPacientesID(p);
    double minE = 0, maxE = 0;
    for(int ic = 0; ic < nC; ic++) {
        if(c[ic].tipo[0] == 'E') { minE = c[ic].min; maxE = c[ic].max; break; }
    }
    ofstream rep("reporte_final.txt");
    rep << "REPORTE MONITORIZACION UCI - JAAVERIANA\n========================================\n";
    for (int i = 0; i < s->numMaquinas; i++) {
        rep << "\nMAQUINA ID: " << (int)s->maquinas[i].id << "\n";
        for (unsigned int j = 0; j < s->maquinas[i].numMediciones; j++) {
            Medicion& m = s->maquinas[i].mediciones[j];
            int idx = buscarPacienteBinario(m.idPaciente, p);
            rep << "Paciente: " << (idx != -1 ? p.pacientes[idx].nombres : "N/A") << " [" << m.idPaciente << "]\n";
            char fallos[256] = ""; 
            if (m.lecturas) {
                for (unsigned int k = 0; k < m.numLecturas; k++) {
                    Lectura& l = m.lecturas[k];
                    for(int ic = 0; ic < nC; ic++) {
                        if (l.tipo == 'P') {
                            if (strcmp(c[ic].tipo, "P_SIS") == 0 && (l.sis < c[ic].min || l.sis > c[ic].max)) strcat(fallos, "[SIS] ");
                            if (strcmp(c[ic].tipo, "P_DIA") == 0 && (l.dia < c[ic].min || l.dia > c[ic].max)) strcat(fallos, "[DIA] ");
                        } else if (l.tipo == c[ic].tipo[0] && l.tipo != 'E') {
                            if (l.valorD < c[ic].min || l.valorD > c[ic].max) {
                                if (l.tipo == 'T') strcat(fallos, "[T] ");
                                if (l.tipo == 'O') strcat(fallos, "[O] ");
                            }
                        }
                    }
                }
                if (esAnomaloECG(m, minE, maxE)) strcat(fallos, "[ECG] ");
            }
            rep << (strlen(fallos) > 0 ? "  ALERTA: " : "  OK: ") << fallos << "\n";
        }
    }
    rep.close();
}

int main() {
    Configuracion* configs = nullptr; int nC = 0;
    ArchivosPacientes pacs = {0, nullptr}; SalaUCI* sala = nullptr;
    int opcion;

    while (true) {
        cout << "\n--- SISTEMA UCI JAAVERIANA (SEM 8) ---\n";
        cout << "1. Cargar Maestros (Config/CSV)\n2. Cargar Binario (BSF)\n3. Generar Reporte de Anomalias\n4. Ver Estadisticas en Pantalla\n5. Salir\n>> ";
        if (!(cin >> opcion)) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); continue; }
        if (opcion == 5) break;

        switch(opcion) {
            case 1:
                leerConfig("data/configuracion.txt", configs, nC);
                leerPacientes("data/pacientes.csv", pacs);
                cout << "[Ok] Datos maestros cargados. (" << pacs.numPacientes << " pacientes encontrados)\n";
                break;
            case 2:
                if (sala) liberarSala(sala);
                sala = leerBSF("data/readings.bsf");
                if (!sala) {
                    cout << "[!] 'readings.bsf' no hallado, intentando simulacion...\n";
                    sala = leerBSF("data/patient_readings_simulation.bsf");
                }
                if (sala) cout << "[Ok] Archivo binario cargado.\n";
                else cout << "[!] Error: No se encontraron archivos .bsf en data/\n";
                break;
            case 3:
                if (sala && configs && pacs.pacientes) {
                    exportarReporte(sala, configs, nC, pacs);
                    cout << "[Ok] Reporte 'reporte_final.txt' exportado.\n";
                } else cout << "[!] Error: Faltan datos (Cargue Maestros y Binario).\n";
                break;
            case 4:
                calcularEstadisticas(sala);
                break;
            default:
                cout << "[!] Opcion no valida.\n";
        }
    }

    if (configs) delete[] configs;
    if (pacs.pacientes) delete[] pacs.pacientes;
    liberarSala(sala);
    return 0;
}