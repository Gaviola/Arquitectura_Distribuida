#include <iostream>
#include <vector>
#include <thread>
#include <mutex>

using namespace std;

// Función para multiplicar un grupo de filas de las matrices A y B y almacenar el resultado en la matriz resultado
void multiplicarFilas(const vector<vector<float>>& matrizA, const vector<vector<float>>& matrizB, vector<vector<float>>& matrizResultado, int inicio, int fin) {
    int N = matrizA.size();
    for (int i = inicio; i < fin; i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < N; k++) {
                matrizResultado[i][j] += matrizA[i][k] * matrizB[k][j];
            }
        }
    }
}

//Imprime las 4 esqinas de una matriz
void imprimirMatriz(const vector<vector<float>>& matriz) {
    int N = matriz.size() - 1;
    cout << "|" << matriz[0][0] << "......" << matriz[0][N] << "|" << endl;
    cout << "|" << ":"  << "\t" << ":" << "|" << endl;
    cout << "|" << ":"  << "\t" << ":" << "|" << endl;
    cout << "|" << matriz[N][0] << "......" << matriz[N][N] << "|" << endl;
    cout << endl;

    return;
}

void sumatoriaMatriz(const vector<vector<float>>& matriz) {
    int N = matriz.size();
    float suma = 0.0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            suma += matriz[i][j]; 
        }
    }
    cout << "Sumatoria de los elementos de la matriz resultado: " << suma << endl;
    return;
}

int main() {
    int N;
    int numThreads = 10;

    cout << "Ingrese el valor de N (tamaño de las matrices NxN): ";
    cin >> N;

    // Crear matrices NxN y llenarlas con valores
    vector<vector<float>> matrizA(N, vector<float>(N, 0.1));  // Llenar matrizA con 0.1
    vector<vector<float>> matrizB(N, vector<float>(N, 0.2));  // Llenar matrizB con 0.2
    vector<vector<float>> matrizResultado(N, vector<float>(N, 0.0));  // Matriz de resutados 

    // Crear vector de hilos
    vector<thread> threads;

    // Dividir el trabajo entre los hilos
    int filasPorHilo = N / numThreads;
    int inicio = 0;
    int fin = filasPorHilo;

    for (int i = 0; i < numThreads; i++) {
        threads.emplace_back(multiplicarFilas, ref(matrizA), ref(matrizB), ref(matrizResultado), inicio, fin);
        inicio = fin;
        fin += filasPorHilo;
    }

    // Esperar a que todos los hilos terminen
    for (auto& thread : threads) {
        thread.join();
    }

    // Mostrar la matriz resultado
    cout << "Matriz resultado de la multiplicación:" << endl;
    imprimirMatriz(matrizResultado);

    // Calcular y mostrar la sumatoria de los elementos de la matriz resultado
    sumatoriaMatriz(matrizResultado);

    return 0;
}