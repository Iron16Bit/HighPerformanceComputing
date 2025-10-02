/*
Implement a Broadcast in a Tree-like fashion using send & receive
*/

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <string.h>
#include <unistd.h>

int main() {

    int world_size;
    int world_rank;
    int num;

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    MPI_Status status;

    // Process 0 will do the first send
    if (world_rank == 0) {
        num = 329;
        for (int i=1; i<world_size; i*=2) {
            MPI_Send(&num, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
    } else {
        MPI_Recv(&num, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
        printf("[%d] Received: '%d' from %d\n", world_rank, num, status.MPI_SOURCE);

        if (world_rank%2 == 0) {
            if (world_rank+1 < world_size) {
                MPI_Send(&num, 1, MPI_INT, world_rank+1, 0, MPI_COMM_WORLD);
            }

            for (int i=world_rank+2; i<world_size; i*=2) {
                MPI_Send(&num, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            }
        }
    }

    MPI_Finalize();

    return 0;
}