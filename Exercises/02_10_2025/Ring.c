/*
Implement an open ring using send & receive
*/

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <string.h>
#include <unistd.h>

int main() {

    int world_size;
    int world_rank;
    char buffer[256];

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    MPI_Status status;

    // Process 0 will do the first send
    if (world_rank == 0) {
        sprintf(buffer, "Hello, world!");
        MPI_Send(&buffer, strlen(buffer)+1, MPI_CHAR, world_rank+1, 0, MPI_COMM_WORLD);
    } else {
        // Receive from previous process
        MPI_Recv(&buffer, 256, MPI_CHAR, world_rank-1, 0, MPI_COMM_WORLD, &status);
        printf("[%d] Received: '%s' from %d\n", world_rank, buffer, status.MPI_SOURCE);
        sleep(1);
        // If we are not the last process, we also forward it
        if (world_rank != world_size-1) {
            MPI_Send(&buffer, strlen(buffer)+1, MPI_CHAR, world_rank+1, 0, MPI_COMM_WORLD);
        }
    }

    MPI_Finalize();

    return 0;
}