/*
Implement a “ping pong” using send & receive:
Two processes one send and one receive each.
Run it using: mpirun –n 2
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

    // We only want to do the ping-pong between 2 processes. If there are more, just ignore them.
    if (world_rank == 0) {
        // Process 0 will do the first send
        sprintf(buffer, "Hello, world!");
        MPI_Send(&buffer, strlen(buffer), MPI_CHAR, 1, 0, MPI_COMM_WORLD);
        // Receive-then-Send loop
        while(1) {
            MPI_Recv(&buffer, 256, MPI_CHAR, 1, 0, MPI_COMM_WORLD, &status);
            printf("[%d] Received: '%s' from %d\n", world_rank, buffer, status.MPI_SOURCE);
            sleep(1);
            MPI_Send(&buffer, strlen(buffer)+1, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
        }
    } else if (world_rank == 1) {
        // Receive-then-send loop
        while(1) {
            MPI_Recv(&buffer, 256, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
            printf("[%d] Received: '%s' from %d\n", world_rank, buffer, status.MPI_SOURCE);
            sleep(1);
            MPI_Send(&buffer, strlen(buffer)+1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
        }
    }
    

    MPI_Finalize();

    return 0;
}