#include <mpi.h>
#include "/TP3/direccion_ip.h"

if(MPI_Init(NULL, NULL)!=MPI_SUCCESS) {
    cout<<"Error iniciando MPI"<<endl;
    exit(1);
}
int rank;
int size;

MPI_Comm_rank(MPI_COMM_WORLD,&rank);
MPI_Comm_size(MPI_COMM_WORLD,&size);
//……Código del programa…….



if(MPI_Finalize()!=MPI_SUCCESS)
{
cout<<"Error finalizando MPI"<<endl;
exit(1);
}