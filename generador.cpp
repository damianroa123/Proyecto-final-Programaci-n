#include <iostream>
#include <fstream>

using namespace std;

int main() {
    ofstream f("data/readings.bsf", ios::binary);
    
    unsigned char salaID = 10;
    unsigned char numMaquinas = 1;
    f.write((char*)&salaID, 1);
    f.write((char*)&numMaquinas, 1);

    unsigned char maqID = 1;
    unsigned int numMediciones = 1;
    f.write((char*)&maqID, 1);
    f.write((char*)&numMediciones, 4);

    // USAMOS EL ID DE CARLOS MORALES PARA QUE EL REPORTE LO ENCUENTRE
    char idPac[11] = "7177937723"; 
    f.write(idPac, 11);
    
    char fecha[24] = "2026-03-11 08:30:00";
    f.write(fecha, 24);

    unsigned int numLecturas = 2;
    f.write((char*)&numLecturas, 4);

    // Lectura 1: Temperatura alta (Fiebre)
    char t1 = 'T'; double v1 = 39.2; 
    f.write(&t1, 1); f.write((char*)&v1, 8);

    // Lectura 2: Oxígeno bajo
    char t2 = 'O'; double v2 = 88.0;
    f.write(&t2, 1); f.write((char*)&v2, 8);

    f.close();
    cout << "Binario generado para Carlos Morales (7177937723)" << endl;
    return 0;
}