#include <iostream>
#include <vector>
#include <cmath>
#include <mpi.h>

bool es_primo(long long int n, const std::vector<long long int>& primos_hasta_raiz) {
    for (long long int primo : primos_hasta_raiz) {
        if (primo * primo > n) break;
        if (n % primo == 0) return false;
    }
    return true;
}

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int mi_rango, num_procesos;
    MPI_Comm_rank(MPI_COMM_WORLD, &mi_rango);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procesos);

    long long int N = 1000000;
    long long int raiz_N = std::sqrt(N);
    long long int trozo = (raiz_N + num_procesos - 1) / num_procesos;
    long long int inicio = mi_rango * trozo;
    long long int fin = std::min((mi_rango + 1) * trozo, raiz_N);

    std::vector<long long int> primos_hasta_raiz;
    if (mi_rango == 0) {
        primos_hasta_raiz.push_back(2);
        for (long long int i = 3; i <= fin; i += 2) {
            if (es_primo(i, primos_hasta_raiz)) {
                primos_hasta_raiz.push_back(i);
            }
        }
    }

    MPI_Bcast(&primos_hasta_raiz[0], primos_hasta_raiz.size(), MPI_LONG_LONG, 0, MPI_COMM_WORLD);

    long long int contador = 0;
    for (long long int i = inicio * inicio + 1; i <= N; i += 2) {
        if (es_primo(i, primos_hasta_raiz)) {
            contador++;
        }
    }

    long long int total_primos;
    MPI_Reduce(&contador, &total_primos, 1, MPI_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);

    if (mi_rango == 0) {
        std::cout << "Hay " << total_primos << " números primos menores que " << N << ", siendo los 5 mayores:" << std::endl;
    }

    MPI_Barrier(MPI_COMM_WORLD);

    std::vector<long long int> mayores_primos;

    for (long long int i = N; i >= 2; i--) {
        if (es_primo(i, primos_hasta_raiz) && mayores_primos.size() < 5) {
            mayores_primos.push_back(i);
        }
    }

    if (mi_rango == 0) {
        for (int i = 0; i < 5; i++) {
            std::cout << mayores_primos[i] << " ";
        }
        std::cout << std::endl;
    }

    MPI_Finalize();
    return 0;
}