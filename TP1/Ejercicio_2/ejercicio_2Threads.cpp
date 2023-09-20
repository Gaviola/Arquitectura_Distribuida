#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <thread>
#include <vector>
#include <mutex>
#include <sys/time.h>

using namespace std;

// Función para contar las ocurrencias de un patrón en una línea con hilos
void matchPatternConHilos(string patronActual, string linea, map<string, int>& contadorPatrones) {
    int count = 0;
    size_t posicion = 0;
    while ((posicion = linea.find(patronActual, posicion)) != string::npos) {
        // Encontrado un patrón, incrementar el contador
        count++;
        posicion += patronActual.length();
    }
    contadorPatrones[patronActual] = count;
}

// Función para contar las ocurrencias de un patrón en una línea
int matchPattern(const string pattern, const string line) {
    int count = 0;
    size_t pos = 0;
    while ((pos = line.find(pattern, pos)) != string::npos) {
        count++;
        pos += pattern.length();
    }
    return count;
}

void sinHilos(ifstream& patronesFile, ifstream& textoFile) {
    // Leer patrones y almacenarlos en un vector
    vector<string> patrones;
    string patron;

    while (getline(patronesFile, patron)) {
        patrones.push_back(patron);
    }

    // Leer el contenido del archivo de texto línea por línea
    map<string, int> contadorPatrones;

    // buscar los patrones en el archivo de texto
    string linea;
    while (getline(textoFile, linea)) {
        for (const string& patron : patrones) {
            int count = matchPattern(patron, linea);
            contadorPatrones[patron] = count;
        }
    }

    // Imprimir resultados
    int i = 0;
    for (const string& patron : patrones) {
        cout << "El patron " << i << " aparece: " << contadorPatrones[patron] << endl;
        i++;
    }
}

void conHilos(ifstream& patronesFile, ifstream& textoFile) {

    // Leer patrones y almacenarlos en un vector
    vector<string> patrones;
    string patron;

    while (getline(patronesFile, patron)) {
        patrones.push_back(patron);
    }

    // Leeo el archivo de texto
    string linea;
    getline(textoFile, linea);

    // Contador de patrones
    map<string, int> contadorPatrones;

    // Creo tantos hilos como patrones tengo que buscar
    unsigned int numThreads = 32;

    // Crear un vector de hilos
    vector<thread> threads;

    // Leer el contenido del archivo de texto línea y busco un patron con cada hilo
    for (unsigned int i = 0; i < numThreads; i++) {
        threads.emplace_back(thread(matchPatternConHilos,patrones[i], linea, ref(contadorPatrones)));
    }

    // Esperar a que todos los hilos terminen
    for (thread& thread : threads) {
        thread.join();
    }

    // Imprimir resultados
    int i = 0;
    for (const string& patron : patrones) {
        cout << "El patron " << i << " aparece: " << contadorPatrones[patron] << endl;
        i++;
    }
}


int main() {
    //Mido tiempo
    timeval time1{},time2{};


    //Abrir archivos
    ifstream patronesFile("/home/bauti/Documents/Facultad/3°/Arquitectura_Distribuida/TP1/Ejercicio_2/patrones.txt");
    ifstream textoFile("/home/bauti/Documents/Facultad/3°/Arquitectura_Distribuida/TP1/Ejercicio_2/texto.txt");

    if (!patronesFile.is_open() || !textoFile.is_open()) {
        cerr << "No se pudo abrir uno o ambos archivos." << endl;
        return 1;
    }

    // Mido tiempo sin hilos
    gettimeofday(&time1,nullptr);
    sinHilos(ref(patronesFile), ref(textoFile));
    gettimeofday(&time2,nullptr);
    double sinHilos = double(time2.tv_sec - time1.tv_sec) + double(time2.tv_usec-time1.tv_usec)/1000000;
    cout << "Tiempo sin hilos: " << sinHilos << endl;

    // Ni idea porque tengo que cerrar y volver a abrir los archivos
    ifstream patronesFile2("/home/bauti/Documents/Facultad/3°/Arquitectura_Distribuida/TP1/Ejercicio_2/patrones.txt");
    ifstream textoFile2("/home/bauti/Documents/Facultad/3°/Arquitectura_Distribuida/TP1/Ejercicio_2/texto.txt");

    // Mido tiempo con hilos
    gettimeofday(&time1,nullptr);
    conHilos(ref(patronesFile2), ref(textoFile2));
    gettimeofday(&time2,nullptr);
    double conHilos = double(time2.tv_sec - time1.tv_sec) + double(time2.tv_usec-time1.tv_usec)/1000000;
    cout << "Tiempo con hilos: " << conHilos << endl;
    cout << "Speedup: " << (sinHilos / conHilos) << endl;

    patronesFile.close();
    textoFile.close();
    patronesFile2.close();
    textoFile2.close();
    
    return 0;
}
