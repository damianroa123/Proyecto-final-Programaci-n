#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <cstdlib>

using namespace std;

struct Lectura{
    char tipoSensor;
    double* valores;
    int numValores;
};

struct Medicion{
    char idPaciente;
    char fechaHora[25];
    int numLecturas;
    Lectura* lecturas;
};

struct Maquina{
    char id;
    int numMediciones;
    Medicion* mediciones;
};

struct SalaUCI{
    char idSala;
    int numMaquinas;
    Maquina* maquinas;
};

struct ConfigSensor{
    char tipo[10];
    double min;
    double max;
};

struct Configuracion{
    int numSensores;
    ConfigSensor* sensores;
};

struct Paciente{
    char id;
    char tipoDocumento[5];
    char documento[20];
    char nombres[50];
    char apellidos[50];
    char fechaNacimiento[15];
    char telefono[20];
    char email[60];
    char tipoSangre[5];
    char entidadSalud[40];
    char medicinaPrepagada[40];
};

struct ListaPacientes{
    int numPacientes;
    Paciente* pacientes;
};

void menu(){
    cout<<"\n===== SISTEMA UCI =====\n";
    cout<<"1. Cargar configuracion y pacientes\n";
    cout<<"2. Leer archivo BSF\n";
    cout<<"3. Generar reporte de anomalias\n";
    cout<<"4. Calcular estadisticas\n";
    cout<<"5. Exportar datos procesados\n";
    cout<<"6. Salir\n";
}

char** separarLinea(string linea,char separador,int &n){

    n=0;
    char** datos=nullptr;

    stringstream ss(linea);
    string token;

    while(getline(ss,token,separador)){

        char** temp=new char*[n+1];

        for(int i=0;i<n;i++)
            temp[i]=datos[i];

        temp[n]=new char[token.size()+1];
        strcpy(temp[n],token.c_str());

        delete[] datos;
        datos=temp;

        n++;
    }

    return datos;
}

bool fechaMayor(const char* f1,const char* f2){
    return strcmp(f1,f2)>0;
}

void cargarConfiguracion(const char* archivo,Configuracion &config){

    ifstream file(archivo);

    if(!file){
        cout<<"Error al abrir "<<archivo<<endl;
        return;
    }

    string linea;

    config.numSensores=0;
    config.sensores=nullptr;

    while(getline(file,linea)){

        if(linea.size()==0 || linea[0]=='#') continue;

        int n;
        char** datos=separarLinea(linea,',',n);

        if(n>=3){

            ConfigSensor* temp=new ConfigSensor[config.numSensores+1];

            for(int i=0;i<config.numSensores;i++)
                temp[i]=config.sensores[i];

            strcpy(temp[config.numSensores].tipo,datos[0]);
            temp[config.numSensores].min=atof(datos[1]);
            temp[config.numSensores].max=atof(datos[2]);

            delete[] config.sensores;
            config.sensores=temp;

            config.numSensores++;
        }

        for(int i=0;i<n;i++)
            delete[] datos[i];
        delete[] datos;
    }

    file.close();
}

void cargarPacientes(const char* archivo,ListaPacientes &lista){

    ifstream file(archivo);

    if(!file){
        cout<<"Error al abrir "<<archivo<<endl;
        return;
    }

    string linea;

    lista.numPacientes=0;
    lista.pacientes=nullptr;

    while(getline(file,linea)){

        if(linea.size()==0 || linea[0]=='#') continue;

        int n;
        char** datos=separarLinea(linea,';',n);

        if(n>=11){

            Paciente* temp=new Paciente[lista.numPacientes+1];

            for(int i=0;i<lista.numPacientes;i++)
                temp[i]=lista.pacientes[i];

            Paciente &p=temp[lista.numPacientes];

            p.id=datos[0][0];

            strcpy(p.tipoDocumento,datos[1]);
            strcpy(p.documento,datos[2]);
            strcpy(p.nombres,datos[3]);
            strcpy(p.apellidos,datos[4]);
            strcpy(p.fechaNacimiento,datos[5]);
            strcpy(p.telefono,datos[6]);
            strcpy(p.email,datos[7]);
            strcpy(p.tipoSangre,datos[8]);
            strcpy(p.entidadSalud,datos[9]);
            strcpy(p.medicinaPrepagada,datos[10]);

            delete[] lista.pacientes;
            lista.pacientes=temp;

            lista.numPacientes++;
        }

        for(int i=0;i<n;i++)
            delete[] datos[i];
        delete[] datos;
    }

    file.close();
}

int main(){

    Configuracion config;
    ListaPacientes pacientes;

    int opcion;

    do{

        menu();
        cin>>opcion;

        switch(opcion){

            case 1:
                cargarConfiguracion("configuracion.txt",config);
                cargarPacientes("pacientes.csv",pacientes);
                cout<<"Datos cargados correctamente\n";
                break;

            case 2:
                cout<<"Lectura BSF se implementa en Sprint 3\n";
                break;

            case 3:
                cout<<"Reporte de anomalias disponible en Sprint 3\n";
                break;

            case 4:
                cout<<"Estadisticas disponibles en Sprint 3\n";
                break;

            case 5:
                cout<<"Exportacion disponible en Sprint 3\n";
                break;

        }

    }while(opcion!=6);

    return 0;
}