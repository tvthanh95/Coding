#include <stdio.h>
#include <stdlib.h>
#define N 2048
#define LEAF 32
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
	struct Matrix A11, A12, B11, B12, A22, A21, B21, B22, P1, P2, P3, P4, P5, P6, P7, C11, C12, C21, C22;
	struct Matrix tmpA1, tmpA2, tmpA5, tmpA6, tmpA7, tmpB1, tmpB3, tmpB4, tmpB6, tmpB7;
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
	C11 = Create_Matrix(A.size/2);
	C12 = Create_Matrix(A.size/2);
	C21 = Create_Matrix(A.size/2);
	C22 = Create_Matrix(A.size/2);

	//WTF is this????????????
	tmpA1 = Create_Matrix(A.size/2);
	tmpA2 = Create_Matrix(A.size/2);
	tmpA5 = Create_Matrix(A.size/2);
	tmpA6 = Create_Matrix(A.size/2);
	tmpA7 = Create_Matrix(A.size/2);


	tmpB1 = Create_Matrix(A.size/2);
	tmpB3 = Create_Matrix(A.size/2);
	tmpB4 = Create_Matrix(A.size/2);
	tmpB6 = Create_Matrix(A.size/2);
	tmpB7 = Create_Matrix(A.size/2);	


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
		Sum_Matrix(A11, A22, &tmpA1);
		Sum_Matrix(B11, B22, &tmpB1);
		Strassen_Mul(tmpA1, tmpB1, &P1);
	}

	#pragma omp task
	{
		Sum_Matrix(A21, A22, &tmpA2);
		Strassen_Mul(tmpA2, B11, &P2);
	}
	#pragma omp task
	{
		Sub_Matrix(B12, B22, &tmpB3);
		Strassen_Mul(A11, tmpB3, &P3);
	}
	#pragma omp task
	{
		Sub_Matrix(B21, B11, &tmpB4);
		Strassen_Mul(A22, tmpB4, &P4);
	}
	#pragma omp task
	{
		Sum_Matrix(A11, A12, &tmpA5);
		Strassen_Mul(tmpA5, B22, &P5);
	}
	#pragma omp task
	{
		Sub_Matrix(A21, A11, &tmpA6);
		Sum_Matrix(B11, B12, &tmpB6);
		Strassen_Mul(tmpA6, tmpB6, &P6);
	}
	#pragma omp task
	{
		Sub_Matrix(A12, A22, &tmpA7);
		Sum_Matrix(B21, B22, &tmpB7);
		Strassen_Mul(tmpA7, tmpB7, &P7);
	}
	#pragma omp taskwait
	{
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

		Free_Matrix(&tmpA2);
		Free_Matrix(&tmpA5);
		Free_Matrix(&tmpA6);
		Free_Matrix(&tmpA7);

		Free_Matrix(&tmpB3);
		Free_Matrix(&tmpB4);
		Free_Matrix(&tmpB6);
		Free_Matrix(&tmpB7);

		Sum_Matrix(P3, P5, &C12);
		Sum_Matrix(P2, P4, &C21);

		Sum_Matrix(P1, P4, &tmpA1);
		Sum_Matrix(tmpA1, P7, &tmpB1);
		Sub_Matrix(tmpB1, P5, &C11);

		Sum_Matrix(P1, P3, &tmpA1);
		Sum_Matrix(tmpA1, P6, &tmpB1);
		Sub_Matrix(tmpB1, P2, &C22);
	
		Free_Matrix(&tmpA1);
		Free_Matrix(&tmpB1);

		for(i = 0; i < C11.size; i++)
			for(j = 0; j < C11.size; j++)
			{
				C->matrix[i][j] = C11.matrix[i][j];
				C->matrix[i][j + C11.size] = C12.matrix[i][j];
				C->matrix[i + C11.size][j] = C21.matrix[i][j];
				C->matrix[i + C11.size][j + C11.size] = C22.matrix[i][j];
			}
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