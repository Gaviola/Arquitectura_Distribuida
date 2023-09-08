#include <iostream>
#include <vector>

using namespace std;

// Función para multiplicar dos matrices NxN
vector<vector<float>> multiplicarMatrices(const vector<vector<float>>& matrizA, const vector<vector<float>>& matrizB) {
    int N = matrizA.size();
    vector<vector<float>> resultado(N, vector<float>(N, 0.0));

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < N; k++) {
                resultado[i][j] += matrizA[i][k] * matrizB[k][j];
            }
        }
    }

    return resultado;
}

void imprimirMatriz(const vector<vector<float>>& matriz) {
    int N = matriz.size();
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cout << matriz[i][j] << " ";
        }
        cout << endl;
    }
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

    cout << "Ingrese el valor de N (tamaño de las matrices NxN): ";
    cin >> N;

    // Crear matrices NxN y llenarlas con valores
    vector<vector<float>> matrizA(N, vector<float>(N, 0.1));  // Llenar matrizA con 0.1
    vector<vector<float>> matrizB(N, vector<float>(N, 0.2));  // Llenar matrizB con 0.2

    // Multiplicar las matrices
    vector<vector<float>> matrizResultado = multiplicarMatrices(matrizA, matrizB);

    // Mostrar la matriz resultado
    cout << "Matriz resultado de la multiplicación:" << endl;
    imprimirMatriz(matrizResultado);

    // Calcular y mostrar la sumatoria de los elementos de la matriz resultado
    sumatoriaMatriz(matrizResultado);

    return 0;
}
