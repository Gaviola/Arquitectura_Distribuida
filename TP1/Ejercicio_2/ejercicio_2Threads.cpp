#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <thread>
#include <vector>

// Función para contar las ocurrencias de un patrón en una línea
int matchPattern(std::string patronActual, std::string linea) {
    int count = 0;
    size_t posicion = 0;
    while ((posicion = linea.find(patronActual, posicion)) != std::string::npos) {
        // Encontrado un patrón, incrementar el contador
        count++;
        posicion += patronActual.length();
    }
    return count;
}

// Función para contar patrones en un rango de líneas
void contarPatronesEnRango(std::ifstream& textoFile, const std::vector<std::string>& patrones, std::map<std::string, int>& contadorPatrones, int inicio, int fin) {
    for (int i = inicio; i <= fin; i++) {
        std::string linea;
        if (std::getline(textoFile, linea)) {
            for (const std::string& patron : patrones) {
                contadorPatrones[patron] += matchPattern(patron, linea);
            }
        }
    }
}

int main() {
    std::ifstream patronesFile("patrones.txt");
    std::ifstream textoFile("texto.txt");

    if (!patronesFile.is_open() || !textoFile.is_open()) {
        std::cerr << "No se pudo abrir uno o ambos archivos." << std::endl;
        return 1;
    }
    
    // Leer patrones y almacenarlos en un vector
    std::vector<std::string> patrones;
    std::string patron;
    
    while (std::getline(patronesFile, patron)) {
        patrones.push_back(patron);
    }
    
    // Leer el contenido del archivo de texto línea por línea
    std::map<std::string, int> contadorPatrones;

    // Obtener el número de núcleos de CPU disponibles
    unsigned int numThreads = std::thread::hardware_concurrency();
    
    // Crear un vector de hilos
    std::vector<std::thread> threads;
    
    // Leer el contenido del archivo de texto línea por línea y contar patrones en paralelo
    for (unsigned int i = 0; i < numThreads; i++) {
        threads.emplace_back(contarPatronesEnRango, std::ref(textoFile), std::ref(patrones), std::ref(contadorPatrones), i * (patrones.size() / numThreads), (i + 1) * (patrones.size() / numThreads) - 1);
    }
    
    // Esperar a que todos los hilos terminen
    for (std::thread& thread : threads) {
        thread.join();
    }
    
    // Imprimir resultados
    for (const auto& par : contadorPatrones) {
        std::cout << "Patrón: " << par.first << " - Cantidad de veces: " << par.second << std::endl;
    }
    
    patronesFile.close();
    textoFile.close();
    
    return 0;
}
