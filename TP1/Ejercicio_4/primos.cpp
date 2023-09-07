#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <algorithm>

const long long int N = 1000000; // Número hasta el cual buscar números primos
const int NumThreads = 4; // Número de hilos

std::vector<long long int> primes; // Vector para almacenar números primos
std::mutex mtx; // Mutex para la exclusión mutua al actualizar el vector

// Función para verificar si un número es primo
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

// Función para buscar números primos en un rango dado
void find_primes_range(long long int start, long long int end) {
    for (long long int num = start; num <= end; ++num) {
        if (is_prime(num)) {
            std::lock_guard<std::mutex> lock(mtx); // Exclusión mutua para actualizar el vector
            primes.push_back(num);
        }
    }
}

int main() {
    std::cout << "Ingrese un número N: ";
    long long int N;
    std::cin >> N;

    // Inicialmente, buscamos números primos hasta la raíz cuadrada de N en un solo hilo
    find_primes_range(2, std::sqrt(N));

    // Luego, creamos hilos adicionales para buscar números primos en el rango restante
    std::vector<std::thread> threads;
    long long int start = std::sqrt(N) + 1;
    long long int range = (N - start + 1) / NumThreads;

    for (int i = 0; i < NumThreads; ++i) {
        long long int thread_start = start + i * range;
        long long int thread_end = (i == NumThreads - 1) ? N : thread_start + range - 1;
        threads.emplace_back(find_primes_range, thread_start, thread_end);
    }

    // Esperamos a que todos los hilos terminen
    for (auto &thread : threads) {
        thread.join();
    }

    // Ordenamos los números primos encontrados
    std::sort(primes.begin(), primes.end(), std::greater<long long int>());

    // Mostramos los 10 mayores números primos y la cantidad total de números primos
    std::cout << "Los 10 mayores números primos menores que " << N << " son: ";
    for (int i = 0; i < 10 && i < primes.size(); ++i) {
        std::cout << primes[i] << " ";
    }
    std::cout << std::endl;

    std::cout << "Cantidad total de números primos menores que " << N << ": " << primes.size() << std::endl;

    return 0;
}
