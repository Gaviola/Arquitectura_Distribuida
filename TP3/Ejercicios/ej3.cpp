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

void imprimirMatriz(const vector<vector<float>> &matriz)
{
    for (const vector<float> &fila : matriz)
    {
        for (float elemento : fila)
        {
            cout << elemento << " ";
        }
        cout << endl; // Saltar a la siguiente línea para la siguiente fila
    }
}

int main(int argc, char** argv) {
    struct timeval start, end;
    gettimeofday(&start, NULL);
    MPI_Init(&argc, &argv);

    int num_processes, process_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
    MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);

    int n = 300;

    vector<vector<float>> matriz1, matriz2;

    generarMatriz1(matriz1, n);
    generarMatriz2(matriz2, n);

    int filas_por_proceso = n / num_processes;
    int inicio = process_rank * filas_por_proceso;
    int fin = inicio + filas_por_proceso;
    float* matriz_resultante = new float[(fin-inicio)*n];

    for (int i = inicio; i < fin; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < n; k++) {
                matriz_resultante[(i-inicio)*n + j] += matriz1[i][k] * matriz2[k][j];
            }
        }
    }

    float* matriz_global = new float[n*n];
    MPI_Gather(matriz_resultante, filas_por_proceso*n, MPI_FLOAT, matriz_global, filas_por_proceso*n, MPI_FLOAT, 0, MPI_COMM_WORLD);

    if (process_rank == 0) {
        // Convertir matriz global a vector
        float sum = 0.0;
        vector<vector<float>> vector_global(n, vector<float>(n));
        for (int i = 0; i < n*n; i++)
        {
            sum += matriz_global[i];
            vector_global[i/n][i%n] = matriz_global[i];
        }

        cout << "|" << vector_global[0][0] << "..." << vector_global[0][n-1] << "|" << endl;
        cout << "|.....|" << endl;
        cout << "|" << vector_global[n-1][0] << "..." << vector_global[n-1][n-1] << "|" << endl;

        cout << "Resultado de la sumatoria: " << sum << endl;

        gettimeofday(&end, NULL);
        double tiempo_ejecucion = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
        cout << "Tiempo de ejecución (segundos): " << tiempo_ejecucion << endl;
    }

    MPI_Finalize();

    return 0;
}
