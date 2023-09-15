#include <iostream>
#include <iomanip>
#include <thread>
#include <cmath>
#include <vector>
#include <mutex>
#include <sys/time.h>

using namespace std;

long double calculateTerm(long double x, long double n) {
    long double div = (x - 1.0L) / (x + 1.0L);
    long double div_pow = pow(div, (2.0L * n + 1.0L));
    return (1.0L / (2.0L * n + 1.0L)) * div_pow;
}


const int num_threads = 4; // Puedes ajustar el número de hilos según tu sistema y preferencias
const int num_terms = 10000000;
const long double x = 1500000.0; // Número para el cual calcular el logaritmo

long double result = 0.0;

mutex result_mutex;

void calculatePartialTerms(int start, int end) {
    long double partial_result = 0.0L;
    for (int i = start; i < end; ++i) {
        partial_result += calculateTerm(x, i);
    }

    lock_guard<mutex> lock(result_mutex);
    result += partial_result;
}

void taylorConHilos() {
    vector<thread> threads;
    int terms_per_thread = num_terms / num_threads;

    for (int i = 0; i < num_threads; ++i) {
        int start_term = i * terms_per_thread;
        int end_term = (i == num_threads - 1) ? num_terms : (i + 1) * terms_per_thread;
        threads.emplace_back(calculatePartialTerms, start_term, end_term);
    }

    for (thread& t : threads) {
        t.join();
    }

    // Imprimir el resultado con 15 dígitos de precisión
    cout << fixed << setprecision(15);
    cout << "El logaritmo natural de " << x << " es: " << result * 2 << endl;
}

void taylorSinHilos() {
    long double x = 1500000;

    long double result = 0.0;
    int termsAmount = 10000000;

    for (int i = 0; i <= termsAmount; ++i) {
        result += 2*calculateTerm(x, i);
    }

    std::cout << std::setprecision(15) << "El logaritmo natural de " << x << " es: " << result << std::endl;
}

int main() {
    timeval time1{},time2{};

    gettimeofday(&time1,nullptr);
    taylorSinHilos();
    gettimeofday(&time2,nullptr);
    double sinHilos = double(time2.tv_sec - time1.tv_sec) + double(time2.tv_usec-time1.tv_usec)/1000000;
    cout << "Tiempo de ejecucion sin hilos: " << sinHilos << endl;

    gettimeofday(&time1,nullptr);
    taylorConHilos();
    gettimeofday(&time2,nullptr);
    double conHilos = double(time2.tv_sec - time1.tv_sec) + double(time2.tv_usec-time1.tv_usec)/1000000;
    cout << "Tiempo de ejecucion con hilos: " << conHilos << endl;

    cout << "Speedup: " << (sinHilos / conHilos) << endl;

    return 0;
}