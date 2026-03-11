#include <iostream>
#include <cstring>
#include <cstdlib>
#include <sstream>
//#include <cstdio>
using namespace std;

struct Lectura {
    char tipoSensor;     
    double valor1;
    double valor2;       
};

struct Medicion {
    char idPaciente;
    char fechaHora[25];
    int numLecturas;
    Lectura lecturas[2];
};

struct Maquina {
    char id;
    int numMediciones;
    Medicion mediciones[2];
};

struct SalaUCI {
    char idSala;
    int numMaquinas;
    Maquina maquinas[2];
};

struct ConfigSensor {
    char tipo[6];
    double min;
    double max;
};

struct Configuracion {
    int numSensores;
    ConfigSensor sensores[10];
};

struct Paciente {
    char id[12];
    char tipoDocumento[4];
    char documento[15];
    char nombres[30];
    char apellidos[30];
    char fechaNacimiento[11];
    char telefono[15];
    char email[40];
    char tipoSangre[5];
    char entidadSalud[20];
    char medicinaPrepagada[20];
};


void separarConfiguracion(string linea, char datos[][50], int &cantidad) {
    cantidad = 0;

    stringstream ss(linea);
    string valor;

    while (getline(ss, valor, ',')) {
        strcpy(datos[cantidad], valor.c_str());
        cantidad++;
    }
}

void separarPaciente(string linea, char datos[][100], int &cantidad) {
    cantidad = 0;

    stringstream ss(linea);
    string valor;

    while (getline(ss, valor, ';')) {
        strcpy(datos[cantidad], valor.c_str());
        cantidad++;
    }
}


void cargarConfig(ConfigSensor &sensor, char datos[][50]) {
    strcpy(sensor.tipo, datos[0]);
    sensor.min = atof(datos[1]);
    sensor.max = atof(datos[2]);
}

void cargarPaciente(Paciente &p, char datos[][100]) {
    strcpy(p.id, datos[0]);
    strcpy(p.tipoDocumento, datos[1]);
    strcpy(p.documento, datos[2]);
    strcpy(p.nombres, datos[3]);
    strcpy(p.apellidos, datos[4]);
    strcpy(p.fechaNacimiento, datos[5]);
    strcpy(p.telefono, datos[6]);
    strcpy(p.email, datos[7]);
    strcpy(p.tipoSangre, datos[8]);
    strcpy(p.entidadSalud, datos[9]);
    strcpy(p.medicinaPrepagada, datos[10]);
}



bool fechaMayor(char f1[], char f2[]) {

    int dia1, mes1, anio1, hora1, min1, seg1, mil1;
    int dia2, mes2, anio2, hora2, min2, seg2, mil2;

    sscanf(f1, "%d/%d/%d %d:%d:%d.%d",
           &dia1, &mes1, &anio1,
           &hora1, &min1, &seg1, &mil1);

    sscanf(f2, "%d/%d/%d %d:%d:%d.%d",
           &dia2, &mes2, &anio2,
           &hora2, &min2, &seg2, &mil2);

    if (anio1 != anio2) return anio1 > anio2;
    if (mes1 != mes2) return mes1 > mes2;
    if (dia1 != dia2) return dia1 > dia2;
    if (hora1 != hora2) return hora1 > hora2;
    if (min1 != min2) return min1 > min2;
    if (seg1 != seg2) return seg1 > seg2;
    return mil1 > mil2;
}



int main() {
    cout << "===== PRUEBA CONFIGURACION =====" << endl;

    string lineaConfig = "TEMP,36.5,39.0";
    char datosConfig[10][50];
    int cantidadConfig;

    separarConfiguracion(lineaConfig, datosConfig, cantidadConfig);

    ConfigSensor sensor;
    cargarConfig(sensor, datosConfig);

    cout << "Tipo: " << sensor.tipo << endl;
    cout << "Min: " << sensor.min << endl;
    cout << "Max: " << sensor.max << endl;


    cout << "\n===== PRUEBA PACIENTE =====" << endl;

    string lineaPaciente = "P01;CC;12345678;Juan;Perez;10/05/2000;3001234567;juan@mail.com;O+;Sura;No";
    char datosPaciente[20][100];
    int cantidadPaciente;

    separarPaciente(lineaPaciente, datosPaciente, cantidadPaciente);

    Paciente p;
    cargarPaciente(p, datosPaciente);

    cout << "ID: " << p.id << endl;
    cout << "Nombre: " << p.nombres << " " << p.apellidos << endl;
    cout << "Documento: " << p.documento << endl;
    cout << "Email: " << p.email << endl;


    cout << "\n===== PRUEBA FECHAS =====" << endl;

    char fecha1[] = "10/02/2026 10:30:15.123";
    char fecha2[] = "09/02/2026 10:30:15.123";

    if (fechaMayor(fecha1, fecha2))
        cout << "Fecha 1 es mayor (mas reciente)" << endl;
    else
        cout << "Fecha 2 es mayor (mas reciente)" << endl;

    return 0;
}

