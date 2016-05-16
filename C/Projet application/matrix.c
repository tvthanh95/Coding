#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#define SIZE 2048
int A[SIZE][SIZE], B[SIZE][SIZE], C[SIZE][SIZE];
int main(int argc, char **argv)
{
	int rank, number_process, from, to, i, j, k;
	int tag = 1234;
	MPI_Status status;
	//starting MPI
	MPI_Init(&argc, &argv);
	//determine rank of current process
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	//determine current number of process, because of limitation of this program, I suggest using a even number of process, ex:4 or 8
	MPI_Comm_size(MPI_COMM_WORLD, &number_process);
	//we divide thw work per process:
	//we need SIZE % number of process == 0
	from = rank * SIZE / number_process;
	to = (rank + 1) * SIZE / number_process;
	if(rank == 0)
	{
		for(i = 0; i < SIZE; i++)
		{
			for(j = 0; j < SIZE; j++)
			{
				A[i][j] = rand() % 10;
				B[i][j] = rand() % 10;
			}
		}
	}
	//Now for easier calculating, we will broadcast(send to all process) matrix B
	MPI_Bcast(B, SIZE * SIZE, MPI_INT, 0, MPI_COMM_WORLD);
	//Now we send the work to process:
	MPI_Scatter(A, SIZE * SIZE / number_process, MPI_INT, A[from], SIZE * SIZE / number_process, MPI_INT, 0, MPI_COMM_WORLD);
	//Do work:
	for(i = from; i < to; i++)
	{
		for(j = 0; j < SIZE; j++)
		{
			C[i][j] = 0;
			for(k = 0; k < SIZE; k++)
			{
				C[i][j] += A[i][k] + B[k][j];
			}

		}
	}
	//Now, we gather all result:
	MPI_Gather(C[from], SIZE * SIZE /number_process, MPI_INT, C, SIZE * SIZE / number_process, MPI_INT, 0, MPI_COMM_WORLD);
	if(rank == 0)
	{
		printf("finish calculate\n");
	}
	//Finist function
	MPI_Finalize();
	return 0;
}