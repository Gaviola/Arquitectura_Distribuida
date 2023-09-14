#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>

using namespace std;

// Función para contar las ocurrencias de un patrón en una línea
int matchPattern(const string& pattern, const string& line) {
    int count = 0;
    size_t pos = 0;
    while ((pos = line.find(pattern, pos)) != string::npos) {
        count++;
        pos += pattern.length();
    }
    return count;
}

int main() {
    //Leer patrones y almacenarlos en un vector
    ifstream patronesFile("/home/fgaviola/Desktop/Facultad/3er Año/2do Semestre/Arquitecturas Distribuidas/arquitectura/Arquitectura_Distribuida/TP1/Ejercicio_2/patrones.txt");
    ifstream textoFile("/home/fgaviola/Desktop/Facultad/3er Año/2do Semestre/Arquitecturas Distribuidas/arquitectura/Arquitectura_Distribuida/TP1/Ejercicio_2/texto.txt");

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

    // buscar los patrones en el archivo de texto
    string linea;
    while (getline(textoFile, linea)) {
        for (const string& patron : patrones) {
            int count = matchPattern(patron, linea);
            contadorPatrones[patron] += count;
        }
    }

    // Imprimir resultados
    int i = 0;
    for (const string& patron : patrones) {
        cout << "El patron " << i << " aparece: " << contadorPatrones[patron] << endl;
        i++;
    }

    return 0;
}

