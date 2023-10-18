#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <mpi.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>

using namespace std;

void obtener_IP(char *respuesta){  
    int mi_socket=socket(AF_INET,SOCK_STREAM,0);
    if(mi_socket==0){
        cout << "Error creando el socket" << endl;
    }
    struct sockaddr_in servidor_addr, my_addr;
        servidor_addr.sin_family = AF_INET;
        servidor_addr.sin_addr.s_addr = inet_addr("179.0.132.58"); 
        servidor_addr.sin_port = htons(80);
    if(connect(mi_socket, (struct sockaddr *)&servidor_addr, (socklen_t)sizeof(servidor_addr))<0){
        cout << "\nError funcion Connect" << endl;
        exit(1);
    }
    socklen_t len = sizeof(my_addr);
    getsockname(mi_socket, (struct sockaddr *) &my_addr, &len);
    inet_ntop(AF_INET, &my_addr.sin_addr, respuesta, 40); 
    close(mi_socket);  
    return;
}

int main(int argc, char *argv[]) {

    timeval startTime{},endTime{};
    gettimeofday(&startTime,nullptr);

    MPI_Init(&argc, &argv);

    int num_processes, process_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
    MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);

    vector<string> ips;

    // Leer los patrones desde el archivo "patrones.txt"
    vector<string> patrones;
    ifstream patrones_file("/home/bauti/Documents/Facultad/3°/Arquitectura_Distribuida/TP3/Ejercicios/patrones.txt");
    ifstream texto_file("/home/bauti/Documents/Facultad/3°/Arquitectura_Distribuida/TP3/Ejercicios/texto.txt");

    //ifstream patrones_file("/home/mpiuser/facu_bauti/patrones.txt");
    //ifstream texto_file("/home/mpiuser/facu_bauti/texto.txt");

    string patron;
    while (getline(patrones_file, patron)) {
        patrones.push_back(patron);
    }
    patrones_file.close();

    if (patrones.size() < num_processes) {
        cerr << "Error: No hay suficientes patrones para todos los procesos." << endl;
        MPI_Finalize();
        return 1;
    }

    // Leer el archivo de texto completo
    string texto;
    getline(texto_file, texto);
    texto_file.close();

    // Obtener la IP del proceso
    char ip[40];
    obtener_IP(ip);
    //ips.emplace_back(ip);

    // Cada proceso busca su patrón en el texto
    string patron_local = patrones[process_rank];
    int conteo_local = 0;
    size_t pos = texto.find(patron_local);
    while (pos != string::npos) {
        conteo_local++;
        pos = texto.find(patron_local, pos + 1);
    }

    // Recopilar los resultados locales en el proceso 0
    vector<int> conteos_totales(num_processes);
    char ip_buffer[40 * num_processes];
    MPI_Gather(&conteo_local, 1, MPI_INT, &conteos_totales[0], 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Gather(ip, 40, MPI_CHAR, ip_buffer, 40, MPI_CHAR, 0, MPI_COMM_WORLD);


    // Imprimir los resultados
    if (process_rank == 0) {
        for (int i = 0; i < num_processes; i++) {
            cout << "El patrón " << i << " aparece " << conteos_totales[i] << " veces. Buscado por " << ip_buffer + i * 40 << endl;
        }

        gettimeofday(&endTime,nullptr);
        double executionTime = double(endTime.tv_sec - startTime.tv_sec) + double(endTime.tv_usec-startTime.tv_usec)/1000000;
        cout << "Tiempo de ejecucion: " << executionTime << endl;
    }

    MPI_Finalize();
    return 0;
}
