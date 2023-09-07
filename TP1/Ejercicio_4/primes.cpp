#include <iostream>
#include <vector>
#include <algorithm>
#include <sys/time.h>

const long long int N = 1000000;

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

int main() {
    std::cout << "Ingrese un número mayor o igual a 10000000: ";
    long long int n;
    std::cin >> n;

    while (n < 10000000) {
        std::cout << "El número debe ser mayor a 10000000." << std::endl << std::endl;
        
        std::cout << "Ingrese un número mayor a 10000000: ";
        std::cin >> n;
    }

    struct timeval start_time, end_time;
    gettimeofday(&start_time, nullptr);

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

    gettimeofday(&end_time, nullptr);

    double elapsed_time = (end_time.tv_sec - start_time.tv_sec) + (end_time.tv_usec - start_time.tv_usec) / 1000000.0;

    std::cout << "Tiempo de ejecución: " << elapsed_time << " segundos" << std::endl;

    return 0;
}
