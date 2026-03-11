#include <iostream>
#include <fstream>
#include <cstring>
#include <cmath>

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



SalaUCI* leerBSF(const char* nombreArchivo) {
    ifstream archivo(nombreArchivo, ios::binary);
    if (!archivo) return nullptr;

    SalaUCI* s = new SalaUCI;
    // Leemos los datos de la sala
    archivo.read((char*)&s->id, 1);
    archivo.read((char*)&s->numMaquinas, 1);
    s->maquinas = new MaquinaUCI[s->numMaquinas];

    for (int i = 0; i < s->numMaquinas; i++) {
        archivo.read((char*)&s->maquinas[i].id, 1);
        archivo.read((char*)&s->maquinas[i].numMediciones, 4);
        s->maquinas[i].mediciones = new Medicion[s->maquinas[i].numMediciones];

        for (int j = 0; j < s->maquinas[i].numMediciones; j++) {
            Medicion& m = s->maquinas[i].mediciones[j];
            archivo.read(m.idPaciente, 11); 
            m.idPaciente[11] = '\0';
            archivo.read(m.fechaHora, 24); 
            m.fechaHora[24] = '\0';
            
            archivo.read((char*)&m.numLecturas, 4);
            m.lecturas = new Lectura[m.numLecturas];

            for (int k = 0; k < m.numLecturas; k++) {
                archivo.read(&m.lecturas[k].tipo, 1);
                if (m.lecturas[k].tipo == 'P') { 
                    archivo.read((char*)&m.lecturas[k].sis, 4);
                    archivo.read((char*)&m.lecturas[k].dia, 4);
                } else {
                    archivo.read((char*)&m.lecturas[k].valorD, 8);
                }
            }
        }
    }
    archivo.close();
    return s;
}



bool chequeoECG(Medicion& m, double limInf, double limSup) {
    double elMinimo = 1000.0, elMaximo = -1000.0;
    bool tieneDatos = false;

    for (int k = 0; k < m.numLecturas; k++) {
        if (m.lecturas[k].tipo == 'E') {
            tieneDatos = true;
            if (m.lecturas[k].valorD < elMinimo) elMinimo = m.lecturas[k].valorD;
            if (m.lecturas[k].valorD > elMaximo) elMaximo = m.lecturas[k].valorD;
        }
    }

    if (!tieneDatos) return false;

    
    double sumaMedida = abs(elMinimo) + abs(elMaximo);
    double sumaLimites = abs(limInf) + abs(limSup);

    return (sumaMedida > sumaLimites);
}



void guardarEcgMalos(SalaUCI* sala, double l1, double l2) {
    if (sala == nullptr) return;

    ofstream bin("pacientes_ecg_anomalos.dat", ios::binary);
    
    for (int i = 0; i < sala->numMaquinas; i++) {
        for (int j = 0; j < sala->maquinas[i].numMediciones; j++) {
            Medicion& m = sala->maquinas[i].mediciones[j];

            if (chequeoECG(m, l1, l2)) {
                
                bin.write(m.idPaciente, 11);

                
                unsigned int cuantosE = 0;
                for(int k=0; k < m.numLecturas; k++) {
                    if(m.lecturas[k].tipo == 'E') cuantosE++;
                }

                 //(4 bytes)
                bin.write((char*)&cuantosE, 4);

                
                for (int k = 0; k < m.numLecturas; k++) {
                    if (m.lecturas[k].tipo == 'E') {
                        bin.write(m.fechaHora, 24); 
                        bin.write((char*)&m.lecturas[k].valorD, 8);
                    }
                }
            }
        }
    }
    bin.close();
    cout << "Archivo binario de anomalos guardado." << endl;
}



void reporteAnomaliasTexto(SalaUCI* sala, Configuracion* configs, int nC) {
    if (sala == nullptr || configs == nullptr) return;

    ofstream txt("anomalias.txt");
    txt << "REPORTE DE LECTURAS FUERA DE RANGO" << endl;

    for (int i = 0; i < sala->numMaquinas; i++) {
        for (int j = 0; j < sala->maquinas[i].numMediciones; j++) {
            Medicion& m = sala->maquinas[i].mediciones[j];
            
            for (int k = 0; k < m.numLecturas; k++) {
                char tipoActual = m.lecturas[k].tipo;
                if (tipoActual == 'E') continue; 

                for (int c = 0; c < nC; c++) {
                    if (configs[c].tipo[0] == tipoActual) {
                        double val = m.lecturas[k].valorD;
                        if (val < configs[c].min || val > configs[c].max) {
                            txt << "ID: " << m.idPaciente << " | Tipo: " << tipoActual 
                                << " | Valor: " << val << " | Fecha: " << m.fechaHora << endl;
                        }
                    }
                }
            }
        }
    }
    txt.close();
    cout << "Archivo anomalias.txt listo." << endl;
}