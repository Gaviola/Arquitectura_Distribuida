#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <mpi.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;

void obtener_IP(char *respuesta) {
    int mi_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (mi_socket == 0) {
        cout << "Error creando el socket" << endl;
    }
    struct sockaddr_in servidor_addr, my_addr;
    servidor_addr.sin_family = AF_INET;
    servidor_addr.sin_addr.s_addr = inet_addr("179.0.132.58"); // Reemplaza con el servidor de referencia
    servidor_addr.sin_port = htons(80);
    if (connect(mi_socket, (struct sockaddr *)&servidor_addr, (socklen_t)sizeof(servidor_addr)) < 0) {
        cout << "\nError en la función Connect" << endl;
        exit(1);
    }
    socklen_t len = sizeof(my_addr);
    getsockname(mi_socket, (struct sockaddr *) &my_addr, &len);
    inet_ntop(AF_INET, &my_addr.sin_addr, respuesta, 40);
    close(mi_socket);
}

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int process_rank, num_processes;
    MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_processes);

    ifstream patronesFile("/home/bauti/Documents/Facultad/3°/Arquitectura_Distribuida/TP3/Ejercicios/patrones.txt");
    ifstream textoFile("/home/bauti/Documents/Facultad/3°/Arquitectura_Distribuida/TP3/Ejercicios/texto.txt");

    if (!patronesFile.is_open() || !textoFile.is_open()) {
        cerr << "No se pudo abrir uno o ambos archivos." << endl;
        MPI_Finalize();
        return 1;
    }

    // Leer patrones y almacenarlos en un vector
    vector<string> patrones;
    string patron;

    while (getline(patronesFile, patron)) {
        patrones.push_back(patron);
    }

    // Contador de patrones
    map<string, int> contadorPatrones;

    // Leer el contenido del archivo de texto y dividir el trabajo
    string linea;
    int line_count = 0;
    while (getline(textoFile, linea)) {
        if (line_count % num_processes == process_rank) {
            for (const string& patron : patrones) {
                size_t pos = 0;
                while ((pos = linea.find(patron, pos)) != string::npos) {
                    contadorPatrones[patron]++;
                    pos += patron.length();
                }
            }
        }
        line_count++;
    }

    // Comunicación para recopilar resultados
    for (int i = 0; i < num_processes; i++) {
        if (i != process_rank) {
            map<string, int> receivedData;
            MPI_Recv(&receivedData, patrones.size(), MPI_LONG, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            for (const string& patron : patrones) {
                contadorPatrones[patron] += receivedData[patron];
            }
        }
    }

    // Enviar resultados al proceso maestro (rank 0)
    if (process_rank != 0) {
        MPI_Send(&contadorPatrones, patrones.size(), MPI_LONG, 0, 0, MPI_COMM_WORLD);
    } else {
        for (int i = 1; i < num_processes; i++) {
            map<string, int> receivedData;
            MPI_Recv(&receivedData, patrones.size(), MPI_LONG, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            for (const string& patron : patrones) {
                contadorPatrones[patron] += receivedData[patron];
            }
        }

        // Imprimir resultados
        for (const string& patron : patrones) {
            char ip[40];
            obtener_IP(ip);
            cout << "El patrón " << patron << " aparece " << contadorPatrones[patron] << " veces. Buscado por " << ip << endl;
        }
    }

    MPI_Finalize();
    return 0;
}
