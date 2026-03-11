#include <iostream>
#include <cstring>
#include <cstdlib>

using namespace std;

struct Lectura {
    char tipoSensor;     // T, P, E, O
    double valor1;
    double valor2;       // solo presión
};

struct Medicion {
    char idPaciente;
    char fechaHora[25];  // DD/MM/AAAA HH:MM:ss.sss
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
    char tipo[6];  // T, P_SIS, P_DIA, E, O
    double min;
    double max;
};

struct Configuracion {
    int numSensores;
    ConfigSensor sensores[2];
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

char** separarConfiguracion(const char* linea, int& cantidad) {
    cantidad = 1;
    for (int i = 0; linea[i]; i++)
        if (linea[i] == ',') cantidad++;

    char** tokens = new char*[cantidad];
    int inicio = 0, idx = 0;

    for (int i = 0; ; i++) {
        if (linea[i] == ',' || linea[i] == '\0') {
            int len = i - inicio;
            tokens[idx] = new char[len + 1];
            for (int j = 0; j < len; j++)
                tokens[idx][j] = linea[inicio + j];
            tokens[idx][len] = '\0';
            idx++;
            inicio = i + 1;
        }
        if (linea[i] == '\0') break;
    }
    return tokens;
}

void separarPaciente(const char* linea, int& cantidad, char tokens[10][50]) {
    cantidad = 1;
    for (int i = 0; linea[i]; i++)
        if (linea[i] == ';') cantidad++;

    int inicio = 0, idx = 0;

    for (int i = 0; ; i++) {
        if (linea[i] == ';' || linea[i] == '\0') {
            int len = i - inicio;
            for (int j = 0; j < len; j++)
                tokens[idx][j] = linea[inicio + j];
            tokens[idx][len] = '\0';
            idx++;
            inicio = i + 1;
        }
        if (linea[i] == '\0') break;
    }
    int inicio = 0, idx = 0;

    for (int i = 0; ; i++) {
        if (linea[i] == ';' || linea[i] == '\0') {
            int len = i - inicio;
            for (int j = 0; j < len; j++)
                tokens[idx][j] = linea[inicio + j];
            tokens[idx][len] = '\0';
            idx++;
            inicio = i + 1;
        }
        if (linea[i] == '\0') break;
    }
}

/* =========================
   CARGA A ESTRUCTURAS
   ========================= */

ConfigSensor cargarConfig(char** d) {
    ConfigSensor c;
    strcpy(c.tipo, d[0]);
    c.min = atof(d[1]);
    c.max = atof(d[2]);
    return c;
}

Paciente cargarPaciente(char** d) {
    Paciente p;
    strcpy(p.id, d[0]);
    strcpy(p.tipoDocumento, d[1]);
    strcpy(p.documento, d[2]);
    strcpy(p.nombres, d[3]);
    strcpy(p.apellidos, d[4]);
    strcpy(p.fechaNacimiento, d[5]);
    strcpy(p.telefono, d[6]);
    strcpy(p.email, d[7]);
    strcpy(p.tipoSangre, d[8]);
    strcpy(p.entidadSalud, d[9]);
    strcpy(p.medicinaPrepagada, d[10]);
    return p;
}

/* =========================
   COMPARACIÓN DE FECHAS
   ========================= */

bool fechaMayor(const char* f1, const char* f2) {
    int orden[] = {6,7,8,9, 3,4, 0,1, 11,12,13,14,15,16,17,18,19,20,21,22};
    int n = 21;

    for (int i = 0; i < n; i++) {
        if (f1[orden[i]] != f2[orden[i]])
            return f1[orden[i]] > f2[orden[i]];
    }
    return false;
}

void menu() {
    cout << "\n--- SISTEMA DE MONITOREO BIOMÉDICO ---\n";
    cout << "1. Cargar archivo de configuración y pacientes\n";
    cout << "2. Leer archivo .bsf\n";
    cout << "3. Generar reporte de anomalías\n";
    cout << "4. Calcular estadísticas\n";
    cout << "5. Exportar datos procesados\n";
    cout << "6. Salir\n";
    cout << "Seleccione una opción: ";
}



int main() {
    int opcion;

    do {
        menu();
        cin >> opcion;

        switch (opcion) {
            case 1:
                cout << "Cargando configuración y pacientes (pendiente sprint 2)\n";
                break;
            case 2:
                cout << "Lectura de archivo BSF (pendiente sprint 3)\n";
                break;
            case 3:
                cout << "Reporte de anomalías (pendiente)\n";
                break;
            case 4:
                cout << "Cálculo de estadísticas (pendiente)\n";
                break;
            case 5:
                cout << "Exportación de datos (pendiente)\n";
                break;
            case 6:
                cout << "Saliendo del sistema...\n";
                break;
            default:
                cout << "Opción inválida\n";
        }

    } while (opcion != 6);

    /* ===== PRUEBA DE FECHAS ===== */
    char f1[] = "02/06/2024 10:30:15.123";
    char f2[] = "01/06/2024 12:00:00.000";
    cout << "\nPrueba fechas (f1 > f2): " << fechaMayor(f1, f2) << endl;
    
    return 0;
}
