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

    const long double x = 1500000.0L;
    const int num_terms = 10000000;
    const int terms_per_process = num_terms / num_processes;

    // Medir el tiempo de inicio
    double start_time, end_time;
    start_time = MPI_Wtime();

    long double local_sum = 0.0L;
    for (int n = process_rank * terms_per_process; n < (process_rank + 1) * terms_per_process; ++n) {
        local_sum += taylor_term(x, n);
    }

    // Calcula el tiempo de ejecución de cada proceso
    end_time = MPI_Wtime();
    double elapsed_time = end_time - start_time;

    // Envía los resultados locales al proceso maestro (rank 0)
    MPI_Send(&local_sum, 1, MPI_LONG_DOUBLE, 0, 0, MPI_COMM_WORLD);

    if (process_rank == 0) {
        long double global_sum = 0.0L;
        int fastest_process = -1;
        long double fastest_time = std::numeric_limits<long double>::max();

        for (int i = 0; i < num_processes; ++i) {
            MPI_Recv(&local_sum, 1, MPI_LONG_DOUBLE, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            global_sum += local_sum;
            
            // Actualiza el proceso más rápido
            if (elapsed_time < fastest_time) {
                fastest_time = elapsed_time;
                fastest_process = i;
            }
        }

        // Imprime el resultado con 15 dígitos
        std::cout.precision(15);
        std::cout << "El logaritmo natural de " << x << " es aproximadamente: " << global_sum << std::endl;

        // Imprime cuál fue el proceso más rápido
        std::cout << "El proceso más rápido fue el proceso " << fastest_process << " con un tiempo de ejecución de " << fastest_time << " segundos." << std::endl;
    }

    MPI_Finalize();
    return 0;
}
