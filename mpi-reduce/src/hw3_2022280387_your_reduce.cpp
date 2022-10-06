#include "your_reduce.h"

// You may add your functions and variables here

void YOUR_Reduce(const int* sendbuf, int* recvbuf, int count) {
    /*
        Modify the code here.
        Your implementation should have the same result as this MPI_Reduce
        call. However, you MUST NOT use MPI_Reduce (or like) for your hand-in
        version. Instead, you should use MPI_Send and MPI_Recv (or like). See
        the homework instructions for more information.
    */

    // Original implementation
    // MPI_Reduce(sendbuf, recvbuf, count, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    /*
        You may assume:
        - Data type is always `int` (MPI_INT).
        - Operation is always MPI_SUM.
        - Process to hold final results is always process 0.
        - Number of processes is 2, 4, or 8.
        - Number of elements (`count`) is 1, 16, 256, 4096, 65536, 1048576,
          16777216, or 268435456.
        For other cases, your code is allowed to produce wrong results or even
        crash. It is totally fine.
    */

    int size;
    int orig_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &orig_rank);

    int* buf = (int*) sendbuf;

    for (int i = 0; i < count; ++i) {
        recvbuf[i] = sendbuf[i];
    }

    int rank = orig_rank;

    for (int step = 1; step < size; step <<= 1) {
        if (rank % 2 == 0) {
            int src = (rank + 1) * step;
            MPI_Recv(buf, count, MPI_INT, src, 0, MPI_COMM_WORLD, nullptr);
            for (int i = 0; i < count; ++i) {
                recvbuf[i] += buf[i];
            }
        } else {
            int dst = (rank - 1) * step;
            MPI_Send(recvbuf, count, MPI_INT, dst, 0, MPI_COMM_WORLD);
            break;
        }
        rank /= 2;
    }
}
