#include <iostream>
#include <cmath>
#include <mpi.h>
#include <sys/time.h>

using namespace std;

long double taylor_term(long double x, int n) {
    long double div = (x - 1.0L) / (x + 1.0L);
    long double div_pow = pow(div, 2.0L * n + 1.0L);
    return 2.0L * (1.0L / (2.0L * n + 1.0L)) * div_pow;
}

int main(int argc, char* argv[]) {
    timeval startTime{},endTime{};
    gettimeofday(&startTime,nullptr);

    MPI_Init(&argc, &argv);

    int num_processes, process_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
    MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);

    const long double x = 1500000.0L;
    const int num_terms = 10000000;
    const int terms_per_process = num_terms / num_processes;

    long double local_sum = 0.0L;
    for (int n = process_rank * terms_per_process; n < (process_rank + 1) * terms_per_process; ++n) {
        local_sum += taylor_term(x, n);
    }

    // Envía los resultados locales al proceso maestro (rank 0)
    MPI_Send(&local_sum, 1, MPI_LONG_DOUBLE, 0, 0, MPI_COMM_WORLD);

    if (process_rank == 0) {
        long double global_sum = 0.0L;

        for (int i = 0; i < num_processes; ++i) {
            MPI_Recv(&local_sum, 1, MPI_LONG_DOUBLE, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            global_sum += local_sum;
        }

        // Imprime el resultado con 15 dígitos
        std::cout.precision(15);
        std::cout << "El logaritmo natural de " << x << " es aproximadamente: " << global_sum << std::endl;

        gettimeofday(&endTime,nullptr);
        double executionTime = double(endTime.tv_sec - startTime.tv_sec) + double(endTime.tv_usec-startTime.tv_usec)/1000000;
        cout << "Tiempo de ejecucion: " << executionTime << endl;
    }

    MPI_Finalize();
    return 0;
}
