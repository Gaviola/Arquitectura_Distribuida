#include <iostream>
#include <cmath>
#include <mpi.h>

using namespace std;

long double taylor_term(long double x, int n) {
    long double div = (x - 1.0L) / (x + 1.0L);
    long double div_pow = pow(div, 2.0L * n + 1.0L);
    return 2.0L * (1.0L / (2.0L * n + 1.0L)) * div_pow;
}

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int num_processes, process_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
    MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);

    const long double x = 1500000.0L;  // El número para el cual se calcula el logaritmo
    const int num_terms = 10000000;    // Número de términos en la serie de Taylor
    const int terms_per_process = num_terms / num_processes;

    long double local_sum = 0.0L;
    for (int n = process_rank * terms_per_process; n < (process_rank + 1) * terms_per_process; ++n) {
        local_sum += taylor_term(x, n);
    }

    // Sumamos los resultados locales de cada proceso utilizando MPI_Reduce
    long double global_sum = 0.0L;
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_LONG_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (process_rank == 0) {
        // Imprimimos el resultado con 15 dígitos
        cout.precision(15);
        cout << "El logaritmo natural de " << x << " es aproximadamente: " << global_sum << endl;
    }

    MPI_Finalize();
    return 0;
}
