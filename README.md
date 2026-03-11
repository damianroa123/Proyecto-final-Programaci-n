# Proyecto-final-Programacion -   CODIGO FINAL ES COMPLETO4
Sistema de Monitoreo de Unidades de Cuidados Intensivos (UCI)
Este sistema es una aplicacion desarrollada en C++ para el procesamiento y analisis de datos biometricos de pacientes. El software integra la lectura de archivos de configuracion, bases de datos de pacientes en formato CSV y registros biometricos en formato binario (BSF) para la deteccion automatica de anomalias en signos vitales.

Caracteristicas Tecnicas
Gestion de Memoria Dinamica: Implementacion de estructuras jerarquicas (Sala, Maquina, Medicion, Lectura) utilizando punteros y gestion manual de memoria (new/delete) para garantizar eficiencia en sistemas con recursos limitados.

Procesamiento de Archivos:

Analisis de archivos de texto con delimitadores personalizados.

Lectura de archivos binarios mediante el mapeo directo de bytes a estructuras de memoria.

Algoritmos de Analisis:

Ordenamiento de registros mediante el algoritmo de la burbuja.

Busqueda binaria para la vinculacion de registros biometricos con la base de datos de pacientes en tiempo O(log n).

Deteccion de alertas basada en rangos dinamicos configurables.

Estructura de Datos
La informacion se organiza bajo la siguiente jerarquia de estructuras:

SalaUCI: Nodo raiz que contiene el identificador de la sala y un arreglo dinamico de maquinas.

MaquinaUCI: Gestiona el identificador de la unidad y el conjunto de mediciones capturadas.

Medicion: Almacena el ID del paciente, estampa de tiempo y el listado de lecturas de sensores.

Lectura: Almacena el tipo de signo vital (Temperatura, Oxigeno, Presion o ECG) y sus valores correspondientes.

Instrucciones de Compilacion y Ejecucion
Requisitos

Compilador de C++ (g++ o clang++).

Directorio de trabajo con una carpeta interna denominada data.

Compilacion

Para compilar el programa principal, utilice el siguiente comando en la terminal:
g++ -o monitor_uci completo4.cpp

Ejecucion

Verifique que los archivos configuracion.txt, pacientes.csv y patient_readings_simulation.bsf se encuentren dentro de la carpeta data.

Ejecute el binario generado:
./monitor_uci

Guia de Operacion
El programa opera mediante un menu interactivo:

Cargar Maestros: Inicializa los limites de referencia y carga la base de datos de pacientes desde el CSV.

Cargar Binario: Carga las mediciones del archivo .bsf en la estructura dinamica de la sala.

Generar Reporte de Anomalias: Exporta el analisis final al archivo reporte_final.txt, indicando pacientes con signos fuera de rango.

Ver Estadisticas en Pantalla: Muestra un resumen de promedios de temperatura y valores maximos de oxigeno por cada maquina.

Notas sobre el Procesamiento de Datos
El sistema cuenta con una logica robusta para el manejo de archivos CSV que presentan delimitadores adicionales al inicio de cada linea, utilizando un desplazamiento (offset) dinamico para localizar el ID del paciente, nombres y apellidos. Ademas, incluye funciones de limpieza de cadenas para evitar errores de coincidencia por caracteres no imprimibles o espacios residuales.
