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



void redimensionarLecturas(Medicion& m, int nuevaCapacidad) {
    Lectura* nuevoArreglo = new Lectura[nuevaCapacidad];
    
    
    for (unsigned int i = 0; i < m.numLecturas; i++) {
        nuevoArreglo[i] = m.lecturas[i];
    }
    
    
    if (m.lecturas != nullptr) {
        delete[] m.lecturas;
    }
    
    m.lecturas = nuevoArreglo;
}



SalaUCI* leerBSF_Dinamico(const char* nombre) {
    ifstream f(nombre, ios::binary);
    if (!f) return nullptr;

    SalaUCI* s = new SalaUCI;
    f.read((char*)&s->id, 1);
    f.read((char*)&s->numMaquinas, 1);
    
    
    s->maquinas = new MaquinaUCI[s->numMaquinas];

    for (int i = 0; i < s->numMaquinas; i++) {
        f.read((char*)&s->maquinas[i].id, 1);
        f.read((char*)&s->maquinas[i].numMediciones, 4);
        
        
        s->maquinas[i].mediciones = new Medicion[s->maquinas[i].numMediciones];

        for (unsigned int j = 0; j < s->maquinas[i].numMediciones; j++) {
            Medicion& m = s->maquinas[i].mediciones[j];
            f.read(m.idPaciente, 11); m.idPaciente[11] = '\0';
            f.read(m.fechaHora, 24); m.fechaHora[24] = '\0';
            f.read((char*)&m.numLecturas, 4);

            
            m.lecturas = new Lectura[m.numLecturas];

            for (unsigned int k = 0; k < m.numLecturas; k++) {
                f.read(&m.lecturas[k].tipo, 1);
                f.read((char*)&m.lecturas[k].valorD, 8);
            }
        }
    }
    f.close();
    return s;
}


void liberarMemoria(SalaUCI* s) {
    if (s == nullptr) return;

    for (int i = 0; i < s->numMaquinas; i++) {
        for (unsigned int j = 0; j < s->maquinas[i].numMediciones; j++) {
            
            delete[] s->maquinas[i].mediciones[j].lecturas;
        }
        
        delete[] s->maquinas[i].mediciones;
    }
    
    delete[] s->maquinas;
  
    delete s;
}

int main() {

    cout << "Sistema dinamico listo (sin vectores ni tamaños fijos)." << endl;
    return 0;
}