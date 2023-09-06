#include <iostream>
#include <cmath>
#include <iomanip>
#include <sys/time.h>

long double calculateTerm(long double x, long double n){
    long double div = (x-1.0)/(x+1.0);
    long double div_pow = pow(div, (2.0*n+1.0));
    return (1.0/(2.0*n+1.0))*div_pow;
}

void calculateNaturalLog(){
    long double x = 1500000;
    /*
    std::cout << "Ingrese un número mayor a 1500000: ";
    std::cin >> x;

    while (x < 1500000.0) {
        std::cout << "El número debe ser mayor a 1500000." << std::endl << std::endl;
        
        std::cout << "Ingrese un número mayor a 1500000: ";
        std::cin >> x;
    }
  */  
    long double result = 0.0;
    int termsAmount = 10000000;

    for (int i = 0; i <= termsAmount; ++i) {
        result += 2*calculateTerm(x, i);
    }

    std::cout << std::setprecision(15) << "El logaritmo natural de " << x << " es: " << result << std::endl;
}


int main() {
    timeval time1,time2;
    gettimeofday(&time1,NULL);
    calculateNaturalLog();
    gettimeofday(&time2,NULL);
    std::cout << "Tiempo ejecución: " << double(time2.tv_sec - time1.tv_sec) +
    + double(time2.tv_usec-time1.tv_usec)/1000000 << std::endl;
    return 0;
}

