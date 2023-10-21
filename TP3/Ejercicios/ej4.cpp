#include <mpi.h>
#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>

using namespace std;


bool es_primo(long long int numero) {
    if (numero <= 3) {
        return true; // 2 es primo
    }
    if (numero % 2 == 0 || numero % 3 == 0) {
        return false; // Los múltiplos de 2 o 3 no son primos
    }

    // Verificar divisibilidad por números mayores a 3
    for (long long int i = 5; i * i <= numero; i += 6) {
        if (numero % i == 0 || numero % (i + 2) == 0) {
            return false;
        }
    }

    return true;
}

bool esPrimoVector(int numero, vector<int> primos) {
    // Verificar si el número es divisible por los primos menores o iguales a la raíz cuadrada de N
    for (long long int primo : primos) {
        if (primo * primo > numero) {
            break;
        }
        if (numero % primo == 0) {
            return false;
        }
    }

    return true;
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int N = 1000000;
    int raiz_N = sqrt(N);

    //Busco los primos hasta raiz de N
    vector<int> primos_hasta_raiz_N;
    for (int i = 2; i <= raiz_N; ++i) {
        if (es_primo(i)) {
            primos_hasta_raiz_N.push_back(i);
        }
    }

    // Dividir los números entre raiz de n y n entre los procesos
    int numeros_por_proceso = (N - raiz_N) / size;
    int inicio = raiz_N + rank * numeros_por_proceso;
    int fin = inicio + numeros_por_proceso;


    // Busco los primos mayores a raiz de N verificando si son multiplos de los primos hasta raiz de N
    int count = 0;
    vector<int> primos;
    for (int i = inicio; i <= fin; ++i) {
        if (esPrimoVector(i, primos_hasta_raiz_N)) {
            count++;
            primos.push_back(i);
        }
    }

    // Envio los resultados al proceso maestro
    if(rank != 0){
        MPI_Send(&count, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Send(&primos [0], primos.size(), MPI_INT, 0, 0, MPI_COMM_WORLD);
    }


    // Recibir los resultados de los otros procesos
    if (rank == 0) {
        int total_count = count + primos_hasta_raiz_N.size();
        vector<int> total_primos = primos_hasta_raiz_N;

        // Recibir resultados de otros procesos
        for (int i = 1; i < size; ++i) {
            int count_received;
            MPI_Recv(&count_received, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            total_count += count_received;

            // Recibir primos del proceso i
            vector<int> primos_received(count_received);
            MPI_Recv(primos_received.data(), count_received, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            // Agregar los primos recibidos a la lista total
            total_primos.insert(total_primos.end(), primos_received.begin(), primos_received.end());
        }

        // Ordenar los primos de manera ascendente
        sort(total_primos.begin(), total_primos.end());

        // Imprimir resultados
        cout << "Total de primos menores a n: " << total_count << endl;
        cout << "Los 10 mayores son: ";
        for (int i = total_primos.size() - 10; i < total_primos.size(); ++i) {
            cout << total_primos[i] << " ";
        }
        cout << endl;
    }
    MPI_Finalize();
    return 0;
}