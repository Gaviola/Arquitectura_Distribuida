#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <mpi.h>

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
  MPI_Init(&argc, &argv);

  int rank, total_procesos;
  char direccion_ip[40];

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &total_procesos);

  obtener_IP(direccion_ip);

  cout << "Hola Mundo! Soy el proceso " << rank << " de " << total_procesos << " corriendo en la máquina " << direccion_ip << " IP=" << direccion_ip << endl;

  MPI_Finalize();
  return 0;
}