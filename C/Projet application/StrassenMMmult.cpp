
#include <stdio.h>   
#include <stdlib.h>  
#include <time.h>
#include "2DArray.h"

#define THRESHOLD  32768 /* product size below which matmultleaf is used */  

void seqMatMult(int m, int n, int p, double** A, double** B, double** C)   
{    
  for (int i = 0; i < m; i++)   
		for (int j = 0; j < n; j++)    
		{          
			C[i][j] = 0.0;   
			for (int k = 0; k < p; k++)   
				C[i][j] += A[i][k]*B[k][j];   
		}   
}  
  
void matmultleaf(int mf, int ml, int nf, int nl, int pf, int pl, double **A, double **B, double **C)    
/*  
  subroutine that uses the simple triple loop to multiply  
  a submatrix from A with a submatrix from B and store the  
  result in a submatrix of C.   
*/  
// mf, ml; /* first and last+1 i index */  
// nf, nl; /* first and last+1 j index */  
// pf, pl; /* first and last+1 k index */  
{      
	for (int i = mf; i < ml; i++)   
		for (int j = nf; j < nl; j++) {
			C[i][j] = 0.0;  
			for (int k = pf; k < pl; k++)   
				C[i][j] += A[i][k]*B[k][j];  
		} 
}   
  

void copyQtrMatrix(double **X, int m, double **Y, int mf, int nf)
{
	for (int i = 0; i < m; i++) 
		X[i] = &Y[mf+i][nf];
}

void AddMatBlocks(double **T, int m, int n, double **X, double **Y)
{
	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j++)
			T[i][j] = X[i][j] + Y[i][j];
}

void SubMatBlocks(double **T, int m, int n, double **X, double **Y)
{
	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j++)
			T[i][j] = X[i][j] - Y[i][j];
}


void strassenMMult(int ml, int nl, int pl, double **A, double **B, double **C)
{
	if (((float)ml)*((float)nl)*((float)pl) < THRESHOLD)   
		matmultleaf(0, ml, 0, nl, 0, pl, A, B, C); 

	else {
		int m2 = ml/2;
		int n2 = nl/2;
		int p2 = pl/2;

		double **M1 = Allocate2DArray< double >(m2, n2);
		double **M2 = Allocate2DArray< double >(m2, n2);
		double **M3 = Allocate2DArray< double >(m2, n2);
		double **M4 = Allocate2DArray< double >(m2, n2);
		double **M5 = Allocate2DArray< double >(m2, n2);
		double **M6 = Allocate2DArray< double >(m2, n2);
		double **M7 = Allocate2DArray< double >(m2, n2);

		double **wAM1 = Allocate2DArray< double >(m2, p2);
		double **wBM1 = Allocate2DArray< double >(p2, n2);
		double **wAM2 = Allocate2DArray< double >(m2, p2);
		double **wBM3 = Allocate2DArray< double >(p2, n2);
		double **wBM4 = Allocate2DArray< double >(p2, n2);
		double **wAM5 = Allocate2DArray< double >(m2, p2);
		double **wAM6 = Allocate2DArray< double >(m2, p2);
		double **wBM6 = Allocate2DArray< double >(p2, n2);
		double **wAM7 = Allocate2DArray< double >(m2, p2);
		double **wBM7 = Allocate2DArray< double >(p2, n2);

		double **A11 = new double*[m2];
		double **A12 = new double*[m2];
		double **A21 = new double*[m2];
		double **A22 = new double*[m2];

		double **B11 = new double*[p2];
		double **B12 = new double*[p2];
		double **B21 = new double*[p2];
		double **B22 = new double*[p2];

		double **C11 = new double*[m2];
		double **C12 = new double*[m2];
		double **C21 = new double*[m2];
		double **C22 = new double*[m2];

		copyQtrMatrix(A11, m2, A,  0,  0);
		copyQtrMatrix(A12, m2, A,  0, p2);
		copyQtrMatrix(A21, m2, A, m2,  0);
		copyQtrMatrix(A22, m2, A, m2, p2);

		copyQtrMatrix(B11, p2, B,  0,  0);
		copyQtrMatrix(B12, p2, B,  0, n2);
		copyQtrMatrix(B21, p2, B, p2,  0);
		copyQtrMatrix(B22, p2, B, p2, n2);

		copyQtrMatrix(C11, m2, C,  0,  0);
		copyQtrMatrix(C12, m2, C,  0, n2);
		copyQtrMatrix(C21, m2, C, m2,  0);
		copyQtrMatrix(C22, m2, C, m2, n2);

#pragma omp task
		{
	// M1 = (A11 + A22)*(B11 + B22)
		AddMatBlocks(wAM1, m2, p2, A11, A22);
		AddMatBlocks(wBM1, p2, n2, B11, B22);
		strassenMMult(m2, n2, p2, wAM1, wBM1, M1);
		}

#pragma omp task
		{
	//M2 = (A21 + A22)*B11
		AddMatBlocks(wAM2, m2, p2, A21, A22);
		strassenMMult(m2, n2, p2, wAM2, B11, M2);
		}

#pragma omp task
		{
	//M3 = A11*(B12 - B22)
		SubMatBlocks(wBM3, p2, n2, B12, B22);
		strassenMMult(m2, n2, p2, A11, wBM3, M3);
		}

#pragma omp task
		{
	//M4 = A22*(B21 - B11)
		SubMatBlocks(wBM4, p2, n2, B21, B11);
		strassenMMult(m2, n2, p2, A22, wBM4, M4);
		}

#pragma omp task
		{
	//M5 = (A11 + A12)*B22
		AddMatBlocks(wAM5, m2, p2, A11, A12);
		strassenMMult(m2, n2, p2, wAM5, B22, M5);
		}

#pragma omp task
		{
	//M6 = (A21 - A11)*(B11 + B12)
		SubMatBlocks(wAM6, m2, p2, A21, A11);
		AddMatBlocks(wBM6, p2, n2, B11, B12);
		strassenMMult(m2, n2, p2, wAM6, wBM6, M6);
		}

#pragma omp task
		{
	//M7 = (A12 - A22)*(B21 + B22)
		SubMatBlocks(wAM7, m2, p2, A12, A22);
		AddMatBlocks(wBM7, p2, n2, B21, B22);
		strassenMMult(m2, n2, p2, wAM7, wBM7, M7);
		}
#pragma omp taskwait

		for (int i = 0; i < m2; i++)
			for (int j = 0; j < n2; j++) {
				C11[i][j] = M1[i][j] + M4[i][j] - M5[i][j] + M7[i][j];
				C12[i][j] = M3[i][j] + M5[i][j];
				C21[i][j] = M2[i][j] + M4[i][j];
				C22[i][j] = M1[i][j] - M2[i][j] + M3[i][j] + M6[i][j];
			}

		Free2DArray< double >(M1);
		Free2DArray< double >(M2);
		Free2DArray< double >(M3);
		Free2DArray< double >(M4);
		Free2DArray< double >(M5);
		Free2DArray< double >(M6);
		Free2DArray< double >(M7);

		Free2DArray< double >(wAM1);
		Free2DArray< double >(wBM1);
		Free2DArray< double >(wAM2);
		Free2DArray< double >(wBM3);
		Free2DArray< double >(wBM4);
		Free2DArray< double >(wAM5);
		Free2DArray< double >(wAM6);
		Free2DArray< double >(wBM6);
		Free2DArray< double >(wAM7);
		Free2DArray< double >(wBM7);

		delete[] A11; delete[] A12; delete[] A21; delete[] A22;
		delete[] B11; delete[] B12; delete[] B21; delete[] B22;
		delete[] C11; delete[] C12; delete[] C21; delete[] C22;
	}
}
              
void matmultS(int m, int n, int p, double **A, double **B, double **C)
{   
#pragma omp parallel 
  {
#pragma omp single
	  {
    strassenMMult(m, n, p, A, B, C);
	  }
  }
}  


int CheckResults(int m, int n, double **C, double **C1)
{
#define ERR_THRESHOLD 0.001
//
//  May need to take into consideration the floating point roundoff error
//    due to parallel execution
//
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      if (abs(C[i][j] - C1[i][j]) > ERR_THRESHOLD ) {
        printf("%f  %f\n", C[i][j], C1[i][j]);
        return 1;
      }
    }
  }
  return 0;
}


  
int main(int argc, char* argv[])   
{      
  clock_t before, after;

  if (argc != 4) {
	  printf("Usage: %s <M> <N> <P> \n", argv[0]);
	  exit(1);
  }
  int M = atoi(argv[1]);
  int N = atoi(argv[2]);
  int P = atoi(argv[3]);

  double **A = Allocate2DArray< double >(M, P);
  double **B = Allocate2DArray< double >(P, N);
  double **C = Allocate2DArray< double >(M, N);
  double **C4 = Allocate2DArray< double >(M, N);

  int i, j;   

  for (i = 0; i < M; i++) {   
    for (j = 0; j < P; j++) {   
		A[i][j] = 5.0 - ((double)(rand()%100) / 10.0);  
    }      
  }   

  for (i = 0; i < P; i++) {   
    for (j = 0; j < N; j++) {   
      B[i][j] = 5.0 - ((double)(rand()%100) / 10.0);   
    }      
  }   

  printf("Execute Standard matmult  M = %d  N = %d  P = %d\n\n", M, N, P);
  before = clock();
  seqMatMult(M, N, P, A, B, C);
  after = clock();
  printf("Standard matrix function done in %7.2f secs\n\n\n",(float)(after - before)/ CLOCKS_PER_SEC);

  before = clock();
  matmultS(M, N, P, A, B, C4);
  after = clock();
  printf("Strassen matrix function done in %7.2f secs\n\n\n",(float)(after - before)/ CLOCKS_PER_SEC);

  if (CheckResults(M, N, C, C4)) 
    printf("Error in matmultS\n\n");
  else
    printf("OKAY\n\n");

  Free2DArray< double >(A);
  Free2DArray< double >(B);
  Free2DArray< double >(C);
  Free2DArray< double >(C4);

  return 0;   
}  