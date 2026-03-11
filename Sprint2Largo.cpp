#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>

using namespace std;

struct Lectura {
    char tipo; 
    double valorD;   
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

// Funcion para detectar anomalias en sensores que no sean ECG
bool esAnomalaSimple(double valor, char tipo, Configuracion* configs, int nC) {
    if (tipo == 'E') return false; 

    for (int i = 0; i < nC; i++) {
        if (configs[i].tipo[0] == tipo) {
            return (valor < configs[i].min || valor > configs[i].max);
        }
    }
    return false;
}


void generarReportePaciente(const char* idBuscado, SalaUCI* sala, Configuracion* configs, int nC) {
    if (sala == nullptr) return;

    char nombreArchivo[100] = "mediciones_paciente_";
    strcat(nombreArchivo, idBuscado);
    strcat(nombreArchivo, ".txt");

    ofstream archivo(nombreArchivo);
    if (!archivo) return;

    
    archivo << "Paciente: " << idBuscado << endl;

    for (int i = 0; i < sala->numMaquinas; i++) {
        for (int j = 0; j < sala->maquinas[i].numMediciones; j++) {
            Medicion& m = sala->maquinas[i].mediciones[j];

            if (strcmp(m.idPaciente, idBuscado) == 0) {
                archivo << "Fecha: " << m.fechaHora << endl;
                archivo << "Maquina: " << (int)sala->maquinas[i].id << endl;

                double suma = 0;
                double minV = 1000000, maxV = -1000000;

                for (int k = 0; k < m.numLecturas; k++) {
                    Lectura& l = m.lecturas[k];
                    archivo << "Tipo: " << l.tipo << " Valor: " << l.valorD;

                    if (l.tipo != 'E') {
                        if (esAnomalaSimple(l.valorD, l.tipo, configs, nC)) {
                            archivo << " (ANOMALIA)";
                        } else {
                            archivo << " (NORMAL)";
                        }
                    }
                    archivo << endl;

                    suma += l.valorD;
                    if (l.valorD < minV) minV = l.valorD;
                    if (l.valorD > maxV) maxV = l.valorD;
                }

                archivo << "Min: " << minV << " Max: " << maxV;
                archivo << " Promedio: " << (suma / m.numLecturas) << endl << endl;
            }
        }
    }
    archivo.close();
    cout << "Reporte creado." << endl;
}


void crearDatosPrueba(SalaUCI*& sala) {
    sala = new SalaUCI;
    sala->numMaquinas = 1;
    sala->maquinas = new MaquinaUCI[1];
    sala->maquinas[0].id = 5;
    sala->maquinas[0].numMediciones = 1;
    sala->maquinas[0].mediciones = new Medicion[1];

    Medicion& m = sala->maquinas[0].mediciones[0];
    strcpy(m.idPaciente, "123");
    strcpy(m.fechaHora, "2026-03-09");
    m.numLecturas = 2;
    m.lecturas = new Lectura[2];

    m.lecturas[0].tipo = 'T'; m.lecturas[0].valorD = 38.5; 
    m.lecturas[1].tipo = 'E'; m.lecturas[1].valorD = 0.8;
}

int main() {
    Configuracion c[1];
    strcpy(c[0].tipo, "T"); c[0].min = 36.0; c[0].max = 37.5;

    SalaUCI* miSala = nullptr;
    crearDatosPrueba(miSala);

    generarReportePaciente("123", miSala, c, 1);

    // Limpieza
    delete[] miSala->maquinas[0].mediciones[0].lecturas;
    delete[] miSala->maquinas[0].mediciones;
    delete[] miSala->maquinas;
    delete miSala;

    return 0;
}