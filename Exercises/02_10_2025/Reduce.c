/*
Implement a “reduce” by using send and receive + sum over the set of data collected
*/

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

int main() {

    int world_size;
    int world_rank;
    int num, buff;

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    MPI_Status status;

    // Each process generates a random number between 1 and 10;
    srand(time(NULL)+world_rank);
    num = rand()%10+1;
    printf("[%d] Generated: '%d'\n", world_rank, num);

    // All processes send their number to 0
    if (world_rank != 0) {
        MPI_Send(&num, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    } else {
        // 0 receives all messages and adds them to its number
        for (int i=1; i<world_size; i++) {
            MPI_Recv(&buff, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            num += buff;
        }
        sleep(1);
        printf("[%d] The reduced value is: %d\n", world_rank, num);
    }

    MPI_Finalize();

    return 0;
}