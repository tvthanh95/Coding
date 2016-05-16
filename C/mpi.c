#include <stdio.h>
#include <mpi.h>
int main(int argc, char **argv)
{
	int ierr, myid, num_proc;
	ierr = MPI_Init(&argc, &argv);
	ierr = MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	ierr = MPI_Comm_size(MPI_COMM_WORLD, &num_proc);
//	printf("An example using  MPI from process %d\n", myid);
	if(myid == 0)
	{
		printf("Example using MPI\n");
	}
	else if(myid == 1)
	{
		printf("Another process task\n");
	}
	else
		printf("End\n");
	ierr = MPI_Finalize();
}
