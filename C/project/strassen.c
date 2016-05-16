#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#define N 2048
#define LEAF 16
struct Matrix
{
	int size;
	int **matrix;
};
struct Matrix Create_Matrix(int size)
{
	struct Matrix temp;
	temp.size = size;
	temp.matrix = malloc(sizeof(int*)*size);
	if(temp.matrix == NULL)
		return temp;
	int i;
	for(i = 0; i < size; i++)
	{
		temp.matrix[i] = malloc(sizeof(int)*size);
		if(temp.matrix[i] == NULL)
		{
			temp.matrix = NULL;
			return temp;
		}
	}
	return temp;
}
void Fill_Matrix(struct Matrix *m)
{
	int i,j;
	srand(time(NULL));
	for(i = 0; i < m->size; i++)
	{
		for(j = 0; j < m->size; j++)
		{
			m->matrix[i][j] = rand() % 10;
		}
	}
}
void Normal_Mul(struct Matrix a, struct Matrix b, struct Matrix *c)
{
	int i, j, k;
	for(i = 0; i < a.size; i++)
		for(j = 0; j < a.size; j++)
		{
			c->matrix[i][j] = 0;
			for(k = 0; k < a.size; k++)
			{
				c->matrix[i][j] += a.matrix[i][k] * b.matrix[k][j]; 
			}			
		}

}
void Print_Matrix(struct Matrix m)
{
	int i,j;
	for(i = 0; i < m.size; i++)
	{
		for(j = 0; j < m.size; j++)
		{
			printf("%d ",m.matrix[i][j]);
		}
		printf("\n");
	}
}
void Sum_Matrix(struct Matrix A, struct Matrix B, struct Matrix *C)
{
	int i,j;
	for(i = 0; i < A.size; i++)
	{
		for(j = 0; j < A.size; j++)
		{
			C->matrix[i][j] = A.matrix[i][j] + B.matrix[i][j];
		}
	}
}
void Sub_Matrix(struct Matrix A, struct Matrix B, struct Matrix *C)
{
	int i,j;
	for(i = 0; i < A.size; i++)
	{
		for(j = 0; j < A.size; j++)
		{
			C->matrix[i][j] = A.matrix[i][j] - B.matrix[i][j];
		}
	}

}
void Free_Matrix(struct Matrix *A)
{
	int i;
	for(i = 0; i < A->size; i++)
	{
		free(A->matrix[i]);
	}
	free(A->matrix);
}
void Strassen_Mul(struct Matrix A, struct Matrix B, struct Matrix *C)
{
	if(A.size <= LEAF)
	{
		Normal_Mul(A, B, C);
		return;
	}
	int i,j;
	struct Matrix A11, A12, B11, B12, A22, A21, B21, B22, P1, P2, P3, P4, P5, P6, P7, A2res, B4res, C11, C12, C21, C22, B1res, A1res, A5res, A6res, A7res, B3res, B6res, B7res, Ares, Bres;
	A11 = Create_Matrix(A.size/2);
	A12 = Create_Matrix(A.size/2);
	A21 = Create_Matrix(A.size/2);
	A22 = Create_Matrix(A.size/2);
	B11 = Create_Matrix(A.size/2);
	B12 = Create_Matrix(A.size/2);
	B21 = Create_Matrix(A.size/2);
	B22 = Create_Matrix(A.size/2);
	P1 = Create_Matrix(A.size/2);
	P2 = Create_Matrix(A.size/2);
	P3 = Create_Matrix(A.size/2);
	P4 = Create_Matrix(A.size/2);
	P5 = Create_Matrix(A.size/2);
	P6 = Create_Matrix(A.size/2);
	P7 = Create_Matrix(A.size/2);

	Ares = Create_Matrix(A.size/2);
	Bres = Create_Matrix(A.size/2);

	A2res = Create_Matrix(A.size/2);
	A1res = Create_Matrix(A.size/2);
	A5res = Create_Matrix(A.size/2);
	A6res = Create_Matrix(A.size/2);
	A7res = Create_Matrix(A.size/2);
	B4res = Create_Matrix(A.size/2);
	B1res = Create_Matrix(A.size/2);
	B3res = Create_Matrix(A.size/2);
	B6res = Create_Matrix(A.size/2);
	B7res = Create_Matrix(A.size/2);

	C11 = Create_Matrix(A.size/2);
	C12 = Create_Matrix(A.size/2);
	C21 = Create_Matrix(A.size/2);
	C22 = Create_Matrix(A.size/2);
	for(i = 0; i < A11.size; i++)
	{
		for(j = 0; j < A11.size; j++)
		{
			A11.matrix[i][j] = A.matrix[i][j];
			A12.matrix[i][j] = A.matrix[i][j + A11.size];
			A21.matrix[i][j] = A.matrix[i + A11.size][j];
			A22.matrix[i][j] = A.matrix[i + A11.size][j + A11.size];
			B11.matrix[i][j] = B.matrix[i][j];
			B12.matrix[i][j] = B.matrix[i][j + A11.size];
			B21.matrix[i][j] = B.matrix[i + A11.size][j];
			B22.matrix[i][j] = B.matrix[i + A11.size][j + A11.size];
		}	
	}

	#pragma omp task
	{
		Sum_Matrix(A11, A22, &A1res);
		Sum_Matrix(B11, B22, &B1res);
		Strassen_Mul(A1res, B1res, &P1);
	}
	#pragma omp task
	{
		Sum_Matrix(A21, A22, &A2res);
		Strassen_Mul(A2res,B11, &P2);
	}
	#pragma omp task
	{
		Sub_Matrix(B12, B22, &B3res);
		Strassen_Mul(A11, B3res, &P3);
	}

	#pragma omp task
	{
		Sub_Matrix(B21, B11, &B4res);
		Strassen_Mul(A22, B4res, &P4);
	}
	#pragma omp task
	{
		Sum_Matrix(A11, A12, &A5res);
		Strassen_Mul(A5res, B22, &P5);
	}
	#pragma omp task
	{
		Sub_Matrix(A21, A11, &A6res);
		Sum_Matrix(B11, B12, &B6res);
		Strassen_Mul(A6res, B6res, &P6);
	}
	#pragma omp task
	{
		Sub_Matrix(A12, A22, &A7res);
		Sum_Matrix(B21, B22, &B7res);
		Strassen_Mul(A7res, B7res, &P7);
	}


	#pragma omp taskwait
	{



		Sum_Matrix(P3, P5, &C12);
		Sum_Matrix(P2, P4, &C21);

		Sum_Matrix(P1, P4, &Ares);
		Sum_Matrix(Ares, P7, &Bres);
		Sub_Matrix(Bres, P5, &C11);

		Sum_Matrix(P1, P3, &Ares);
		Sum_Matrix(Ares, P6, &Bres);
		Sub_Matrix(Bres, P2, &C22);
	




		for(i = 0; i < C11.size; i++)
			for(j = 0; j < C11.size; j++)
			{
				C->matrix[i][j] = C11.matrix[i][j];
				C->matrix[i][j + C11.size] = C12.matrix[i][j];
				C->matrix[i + C11.size][j] = C21.matrix[i][j];
				C->matrix[i + C11.size][j + C11.size] = C22.matrix[i][j];
			}
		Free_Matrix(&A11);
		Free_Matrix(&A12);
		Free_Matrix(&A21);
		Free_Matrix(&A22);

		Free_Matrix(&B11);
		Free_Matrix(&B12);
		Free_Matrix(&B21);
		Free_Matrix(&B22);

		Free_Matrix(&P1);
		Free_Matrix(&P2);
		Free_Matrix(&P3);
		Free_Matrix(&P4);
		Free_Matrix(&P5);
		Free_Matrix(&P6);
		Free_Matrix(&P7);

		Free_Matrix(&Ares);
		Free_Matrix(&A1res);
		Free_Matrix(&A2res);
		Free_Matrix(&A5res);
		Free_Matrix(&A6res);
		Free_Matrix(&A7res);

		Free_Matrix(&Bres);
		Free_Matrix(&B1res);
		Free_Matrix(&B3res);
		Free_Matrix(&B4res);
		Free_Matrix(&B6res);
		Free_Matrix(&B7res);

		Free_Matrix(&C11);
		Free_Matrix(&C12);
		Free_Matrix(&C21);
		Free_Matrix(&C22);
	}

}
int main(int argc, char **argv)
{
	struct Matrix A, B, C;
	A = Create_Matrix(N);
	B = Create_Matrix(N);
	C = Create_Matrix(N);
	Fill_Matrix(&A);
	Fill_Matrix(&B);
	#pragma omp parallel
	{
		#pragma omp single
		{
			Strassen_Mul(A,B, &C);
		}
	}
	//Normal_Mul(A, B, &C);
	//Print_Matrix(A);
	//Print_Matrix(B);
	//Print_Matrix(C);
	return 0;
}