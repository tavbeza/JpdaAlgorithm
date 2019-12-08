#include "TreeNode.h"
#include<iostream>
#include <math.h>
#include <stdlib.h>

#include "SrvTimeStamp.h"
#define NUMONE 8
#define MATRIX_SIZE 7
#define SIZE 1000
#define FA 9
using namespace std;
bool matrix[MATRIX_SIZE][MATRIX_SIZE + 1];
TreeNode pool[60000];
int currentPoolSize = 0;

// Generic function to display the matrix. We use it to display 
// both adjoin and inverse. adjoin is integer matrix and inverse 
// is a float. 
template<class T>

/// <summary>
/// Print A
/// </summary>
void display(T** A, int n)
{
	for (int i = 0; i<n; i++)
	{
		for (int j = 0; j<n; j++)
			cout << A[i][j] << " ";
		cout << endl;
	}
}

/// <summary>
/// Get Cofactor
/// </summary>
void getCofactor(int** A, int** temp, int p, int q, int n)
{
	int i = 0, j = 0;

	// Looping for each element of the matrix 
	for (int row = 0; row < n; row++)
	{
		for (int col = 0; col < n; col++)
		{
			// Copying into temporary matrix only those element 
			// which are not in given row and column 
			if (row != p && col != q)
			{
				temp[i][j++] = A[row][col];

				// Row is filled, so increase row index and 
				// reset col index 
				if (j == n - 1)
				{
					j = 0;
					i++;
				}
			}
		}
	}
}

/// <summary>
/// Recursive function for finding determinant of matrix.
///n is current dimension of A[][].
/// </summary>
int determinant(int** A, int n)
{
	int D = 0; // Initialize result 

			   // Base case : if matrix contains single element 
	if (n == 1)
		return A[0][0];

	int **temp = (int**)calloc(n*n, sizeof(int*));
	for (int i = 0; i < n; i++) {
		temp[i] = (int*)calloc(n, sizeof(int));
	}

	int sign = 1; // To store sign multiplier 

				  // Iterate for each element of first row 
	for (int f = 0; f < n; f++)
	{
		// Getting Cofactor of A[0][f] 
		getCofactor(A, temp, 0, f, n);
		D += sign * A[0][f] * determinant(temp, n - 1);

		// terms are to be added with alternate sign 
		sign = -sign;
	}

	return D;
}

/// <summary>
/// Function to get adjoint of A[N][N] in adj[N][N]. 
/// </summary>
void adjoint(int** A, float** adj, int n)
{
	if (n == 1)
	{
		adj[0][0] = 1;
		return;
	}

	// temp is used to store cofactors of A[][] 
	int sign = 1;

	int **temp = (int**)calloc(n*n, sizeof(int*));
	for (int i = 0; i < n; i++) {
		temp[i] = (int*)calloc(n, sizeof(int));
	}

	for (int i = 0; i<n; i++)
	{
		for (int j = 0; j<n; j++)
		{
			// Get cofactor of A[i][j] 
			getCofactor(A, temp, i, j, n);

			// sign of adj[j][i] positive if sum of row 
			// and column indexes is even. 
			sign = ((i + j) % 2 == 0) ? 1 : -1;

			// Interchanging rows and columns to get the 
			// transpose of the cofactor matrix 
			adj[j][i] = (sign)*(determinant(temp, n - 1));
		}
	}
}

/// <summary>
/// Function to calculate and store inverse, returns false if matrix is singular 
/// </summary>
bool inverse(int** A, float** inverse, int n)
{
	// Find determinant of A[][] 
	int det = determinant(A, n);
	if (det == 0)
	{
		cout << "Singular matrix, can't find its inverse";
		return false;
	}

	// Find adjoint 
	float **adj = (float**)calloc(n*n, sizeof(float*));
	for (int i = 0; i < n; i++) {
		adj[i] = (float*)calloc(n, sizeof(float));
	}
	adjoint(A, adj, n);

	// Find Inverse using formula "inverse(A) = adj(A)/det(A)" 
	for (int i = 0; i<n; i++)
		for (int j = 0; j<n; j++)
			inverse[i][j] = adj[i][j] / float(det);

	return true;
}

/// <summary>
/// This function multiplication the original matrix and the inverse matrix
/// </summary>
int** mult(int **A, float **B, int n) {
	int i, j, k;
	float **C = (float**)calloc(n, sizeof(float*));
	for (i = 0; i < n; i++) {
		C[i] = (float*)calloc(n, sizeof(float));
	}
	int **CK = (int**)calloc(n*n, sizeof(int*));
	for (int i = 0; i < n; i++) {
		CK[i] = (int*)calloc(n, sizeof(int));
	}
	for (i = 0; i<n; i++) {
		for (j = 0; j<n; j++) {
			C[i][j] = 0;
			for (k = 0; k<n; k++) {
				C[i][j] += A[i][k] * B[k][j];
				//	*(*(C + i) + j) += (*(*(A + i) + k)) * (*(*(B + k) + j));
			}
			CK[i][j] = static_cast<int>(std::round(C[i][j]));
		}
	}
	return CK;
}


TreeNode *trees[SIZE];
TreeNode *fathers[SIZE*SIZE];
TreeNode *sons[SIZE*SIZE];

/// <summary>
/// Create hypothesis function
/// </summary>
void createHypothesis()
{
	TreeNode lastChild;
	int numOfTrees = 0, fathersSize = 0, sonsSize = 0, lastSons;
	long long start1 = SrvTimeStamp::Start();

	for (int i = 0; i < MATRIX_SIZE; i++)
	{
		for (int j = 0; j < MATRIX_SIZE; j++)
		{
			if (i == 0 && matrix[i][j] == 1)	// if root
			{
				trees[numOfTrees] = &(pool[currentPoolSize++]);
				trees[numOfTrees]->m_i = i;
				trees[numOfTrees]->m_j = j;
				numOfTrees++;
			}
			else if (i != 0 && matrix[i][j] == 1)
			{
				if (fathersSize == 0) // second plot (the father is only the first plot (root))
				{
					for (int k = 0; k < numOfTrees; k++)
					{
						if (j != 0 /*&& trees[k]->m_j != j*/)
						{
							sons[sonsSize] = &(pool[currentPoolSize++]);
							sons[sonsSize]->m_i = i;
							sons[sonsSize]->m_j = j;
							sons[sonsSize]->SetParent(trees[k]);
							trees[k]->AddChild(sons[sonsSize++]);
						}
						else if (j == 0)
						{
							sons[sonsSize] = &(pool[currentPoolSize++]);
							sons[sonsSize]->m_i = i;
							sons[sonsSize]->m_j = j;
							sons[sonsSize]->SetParent(trees[k]);
							trees[k]->AddChild(sons[sonsSize++]);
						}
					}
				}
				else {
					for (int k = 0; k < fathersSize; k++)
					{
						sons[sonsSize] = &(pool[currentPoolSize++]);
						sons[sonsSize]->m_i = i;
						sons[sonsSize]->m_j = j;
						sons[sonsSize]->SetParent(fathers[k]);
						fathers[k]->AddChild(sons[sonsSize++]);
					}
				}
			}
		} // done row i
		fathersSize = 0;
		//all the sons now fathers
		for (int k = 0; k < sonsSize; k++) {
			fathers[k] = &(pool[currentPoolSize++]);
			fathers[k] = sons[k];
			fathersSize++;
		}
		// clean son array
		if (sonsSize != 0)
			lastSons = sonsSize;
		sonsSize = 0;
	}

//	TreeNode *s[SIZE*SIZE];
	int sLength = 0;
	/*
	// Insert to s[] the legal sons[k] without duplicates
	for (int k = 0; k < lastSons; k++)
	{
		if (sons[k]->m_j != 0 && !sons[k]->hasTrackParent(sons[k]->getParent(), sons[k]->m_j))
		{
			s[sLength++] = sons[k];
		}
		else if (sons[k]->m_j == 0)
		{
			s[sLength++] = sons[k];
		}
	}
	*/
	double timeDiff = SrvTimeStamp::Stop(start1);

	// Print the hipothesis
	/*for (int k = 0; k < sLength; k++)
	{

		s[k]->PrintPathToRoot();
	}*/

	
	cout <<"\n\nThe calculate hypothesis time: " << timeDiff*1000000 << "\tcount: " << lastSons << endl;
	
}


void secondMain() {

	/*matrix[0][0] = 1;
	matrix[0][1] = 1;
	matrix[1][0] = 1;
	matrix[1][2] = 1;
	matrix[2][0] = 1;
	matrix[2][2] = 1;*/

	// Fill the matrix
	for (int i = 0; i < MATRIX_SIZE; i++) {
		for (int j = 0; j < NUMONE; j++) {
			matrix[i][j] = 1;
		}
	}
	
	// print the matrix
	for (int i = 0; i < MATRIX_SIZE; i++) {
		for (int p = 0; p < MATRIX_SIZE + 1; p++) {
			cout << matrix[i][p];
		}
		cout << endl;
	}
	
	createHypothesis();
	cout << endl;

	/*int **m = (int**)malloc(sizeof(int*) * 4*4);
	float **inv = (float**)malloc(sizeof(float*) * 4*4);
	
	for (int i = 0; i < 4; i++) {
		m[i] = (int*)malloc(sizeof(int) * 4);
		inv[i] = (float*)malloc(sizeof(float) * 4);
	}

	m[0][0] = 7;
	m[0][1] = 8;
	m[0][2] = 9;
	m[0][3] = 23;

	m[1][0] = 10;
	m[1][1] = 11;
	m[1][2] = 12;
	m[1][3] = 81;

	m[2][0] = 13;
	m[2][1] = 14;
	m[2][2] = 15;
	m[2][3] = 11;

	m[3][0] = 1;
	m[3][1] = 73;
	m[3][2] = 42;
	m[3][3] = 65;

	long long start1 = SrvTimeStamp::Start();
	inverse(m, inv, 4);
	double timeDiff = SrvTimeStamp::Stop(start1);

	display(inv, 4);
	
	cout << "\nThe inverse time in matrix 4X4: " << timeDiff * 1000000 << endl;
	*/
	system("pause");
}