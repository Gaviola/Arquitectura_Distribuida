#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <thread>
#include <vector>

using namespace std;

// Función para contar las ocurrencias de un patrón en una línea
int matchPattern(string patronActual, string linea) {
    int count = 0;
    size_t posicion = 0;
    while ((posicion = linea.find(patronActual, posicion)) != string::npos) {
        // Encontrado un patrón, incrementar el contador
        count++;
        posicion += patronActual.length();
    }
    return count;
}

// Función para contar patrones en un rango de líneas
void contarPatronesEnRango(ifstream& textoFile, const vector<string>& patrones, map<string, int>& contadorPatrones, int inicio, int fin) {
    for (int i = inicio; i <= fin; i++) {
        string linea;
        if (getline(textoFile, linea)) {
            for (const string& patron : patrones) {
                contadorPatrones[patron] += matchPattern(patron, linea);
            }
        }
    }
}

int main() {
    ifstream patronesFile("TP1/Ejercicio_2/patrones.txt");
    ifstream textoFile("TP1/Ejercicio_2/texto.txt");

    if (!patronesFile.is_open() || !textoFile.is_open()) {
        cerr << "No se pudo abrir uno o ambos archivos." << endl;
        return 1;
    }
    
    // Leer patrones y almacenarlos en un vector
    vector<string> patrones;
    string patron;
    
    while (getline(patronesFile, patron)) {
        patrones.push_back(patron);
    }
    
    // Leer el contenido del archivo de texto línea por línea
    map<string, int> contadorPatrones;

    // Obtener el número de núcleos de CPU disponibles
    unsigned int numThreads = thread::hardware_concurrency();
    
    // Crear un vector de hilos
    vector<thread> threads;
    
    // Leer el contenido del archivo de texto línea por línea y contar patrones en paralelo
    for (unsigned int i = 0; i < numThreads; i++) {
        threads.emplace_back(contarPatronesEnRango, ref(textoFile), ref(patrones), ref(contadorPatrones), i * (patrones.size() / numThreads), (i + 1) * (patrones.size() / numThreads) - 1);
    }
    
    // Esperar a que todos los hilos terminen
    for (thread& thread : threads) {
        thread.join();
    }
    
    // Imprimir resultados
    for (const auto& par : contadorPatrones) {
        cout << "Patrón: " << par.first << " - Cantidad de veces: " << par.second << endl;
    }
    
    patronesFile.close();
    textoFile.close();
    
    return 0;
}
