#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <mpi.h>
#include <sys/time.h>
#include <limits>
#include <cmath>

using namespace std;

void generarMatriz1(vector<vector<float>>& matriz, int N) {
    matriz.resize(N, vector<float>(N, 0.1));
}

void generarMatriz2(vector<vector<float>>& matriz, int N) {
    matriz.resize(N, vector<float>(N, 0.2));
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int num_processes, process_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
    MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);

    int N = 300;  // Cambia el valor de N según sea necesario

    vector<vector<float>> matriz1, matriz2, matriz_resultante;
    generarMatriz1(matriz1, N);
    generarMatriz2(matriz2, N);
    matriz_resultante.resize(N, vector<float>(N, std::numeric_limits<double>::quiet_NaN()));

    int filas_por_proceso = N / num_processes;
    int inicio = process_rank * filas_por_proceso;
    int fin = inicio + filas_por_proceso;

    for (int i = inicio; i < fin; i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < N; k++) {
                if (std::isnan(matriz_resultante[i][j])) matriz_resultante[i][j] = 0.0;
                matriz_resultante[i][j] += matriz1[i][k] * matriz2[k][j];
            }
        }
    }

    double suma_local = 0.0;
    for (int i = inicio; i < fin; i++) {
        for (int j = 0; j < N; j++) {
            suma_local += matriz_resultante[i][j];
        }
    }

    double suma_total = 0.0;
    MPI_Reduce(&suma_local, &suma_total, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    // Crear un array para almacenar los resultados de las esquinas
    double esquinas[4] = {0.0};

    // Obtener los resultados de las esquinas
    if (std::isnan(matriz_resultante[0][0])) {
        esquinas[0] = matriz_resultante[0][0];
    }
    if (std::isnan(matriz_resultante[0][N - 1])) {
        esquinas[1] = matriz_resultante[0][N - 1];
    }
    if (std::isnan(matriz_resultante[N - 1][0])) {
        esquinas[2] = matriz_resultante[N - 1][0];
    }
    if (std::isnan(matriz_resultante[N - 1][N - 1])) {
        esquinas[3] = matriz_resultante[N - 1][N - 1];
    }

    // Recopilar los resultados de las esquinas en el proceso raíz
    double esquinas_globales[4] = {0.0};
    MPI_Gather(esquinas, 4, MPI_DOUBLE, esquinas_globales, 4, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Imprimir los resultados de las esquinas solo en el proceso raíz
    if (process_rank == 0) {
        struct timeval start, end;
        gettimeofday(&start, NULL);

        cout << "Elementos de las esquinas:" << endl;
        cout << "Matriz Resultante[0][0]: " << esquinas_globales[0] << endl;
        cout << "Matriz Resultante[0][N-1]: " << esquinas_globales[1] << endl;
        cout << "Matriz Resultante[N-1][0]: " << esquinas_globales[2] << endl;
        cout << "Matriz Resultante[N-1][N-1]: " << esquinas_globales[3] << endl;

        cout << "Resultado de la sumatoria: " << suma_total << endl;

        gettimeofday(&end, NULL);
        double tiempo_ejecucion = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
        cout << "Tiempo de ejecución (segundos): " << tiempo_ejecucion << endl;
    }

    MPI_Finalize();

    return 0;
}
