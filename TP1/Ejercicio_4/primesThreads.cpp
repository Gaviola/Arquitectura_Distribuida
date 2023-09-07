#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <cmath>
#include <algorithm>
#include <sys/time.h>

const long long int N = 1000000;
const int NumThreads = 4;

std::vector<long long int> primes;
std::mutex mtx; // Mutex para la exclusión mutua al actualizar el vector

bool is_prime(long long int num) {
    if (num <= 1) {
        return false;
    }
    if (num == 2) {
        return true;
    }
    if (num % 2 == 0) {
        return false;
    }
    for (long long int i = 3; i * i <= num; i += 2) {
        if (num % i == 0) {
            return false;
        }
    }
    return true;
}

void find_primes_range(long long int start, long long int end) {
    for (long long int num = start; num <= end; ++num) {
        if (is_prime(num)) {
            std::lock_guard<std::mutex> lock(mtx); // Exclusión mutua para actualizar el vector
            primes.push_back(num);
        }
    }
}

void getPrimesWithoutThreads(long long int n){
    std::vector<long long int> primes;

    // Buscar números primos desde 2 hasta N
    for (long long int num = 2; num <= n; ++num) {
        if (is_prime(num)) {
            primes.push_back(num);
        }
    }

    // Ordenar los números primos en orden descendente
    std::sort(primes.rbegin(), primes.rend());

    std::cout << "Los 10 mayores números primos menores que " << n << " son: ";
    for (int i = 0; i < 10 && i < primes.size(); ++i) {
        std::cout << primes[i] << " ";
    }
    std::cout << std::endl;

    std::cout << "Cantidad total de números primos menores que " << n << ": " << primes.size() << std::endl;
}

void getPrimesWithThreads(long long int n){
    // Inicialmente, buscamos números primos hasta la raíz cuadrada de N en un solo hilo
    find_primes_range(2, static_cast<long long int>(std::sqrt(static_cast<double>(n))));

    // Luego, creamos hilos adicionales para buscar números primos en el rango restante
    std::vector<std::thread> threads(NumThreads);

    long long int start = std::sqrt(n) + 1;
    long long int range = (n - start + 1) / NumThreads;

    for (int i = 0; i < NumThreads; ++i) {
        long long int thread_start = start + i * range;
        long long int thread_end = (i == NumThreads - 1) ? n : thread_start + range - 1;
        threads[i] = std::thread(find_primes_range, thread_start, thread_end);
    }

    for (int i = 0; i < NumThreads; ++i) {
        threads[i].join();
    }

    // Ordenamos los números primos encontrados en orden descendente
    std::sort(primes.rbegin(), primes.rend());

    std::cout << "Los 10 mayores números primos menores que " << n << " son: ";
    for (int i = 0; i < 10 && i < primes.size(); ++i) {
        std::cout << primes[i] << " ";
    }
    std::cout << std::endl;

    std::cout << "Cantidad total de números primos menores que " << n << ": " << primes.size() << std::endl;
}

int main() {
    std::cout << "Ingrese un número mayor o igual a 10000000: ";
    long long int n;
    std::cin >> n;

    while (n < 10000000) {
        std::cout << "El número debe ser mayor a 10000000." << std::endl << std::endl;
        
        std::cout << "Ingrese un número mayor a 10000000: ";
        std::cin >> n;
    }

    struct timeval start_time1, end_time1, start_time2, end_time2;
    gettimeofday(&start_time1, nullptr);
    getPrimesWithoutThreads(n);
    gettimeofday(&end_time1, nullptr);

    double time_without_threads = (end_time1.tv_sec - start_time1.tv_sec) + (end_time1.tv_usec - start_time1.tv_usec) / 1000000.0;
    std::cout << "Tiempo sin hilos: " << time_without_threads << " segundos" << std::endl << std::endl;

    gettimeofday(&start_time2, nullptr);
    getPrimesWithThreads(n);
    gettimeofday(&end_time2, nullptr);

    double time_with_threads = (end_time2.tv_sec - start_time2.tv_sec) + (end_time2.tv_usec - start_time2.tv_usec) / 1000000.0;
    std::cout << "Tiempo con hilos: " << time_with_threads << " segundos" << std::endl;

    double speed_up = time_without_threads / time_with_threads;
    std::cout << "Speed up: " << speed_up << std::endl;

    return 0;
}
