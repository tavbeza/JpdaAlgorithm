#ifndef _TRACKING_HYPOTHESIS_H_
#define _TRACKING_HYPOTHESIS_H_

#include "TreeNode.h"
#include<iostream>
#include <math.h>
#include <stdlib.h>
#include "DataTrack.h"
#include "SrvTimeStamp.h"
#include "Matrix.h"
#include "Vector.h"
#include "TrackerParams.h"
#include "SrvDspMath.h"
#include "ExtendedKalman.h"
#include "DataPlot.h"
#include <algorithm>
#define NUMONE 8
#define MATRIX_SIZE 7
#define SIZE 1000
#define FA 9
#define TRACK_MERGE_SIMILIAR_PLOT_COUNT 4

using namespace std;
bool matrix[MATRIX_SIZE][MATRIX_SIZE + 1];
TreeNode pool[60000];
int currentPoolSize = 0;

// Generic function to display the matrix. We use it to display 
// both adjoin and inverse. adjoin is integer matrix and inverse 
// is a float. 
template<class T>
class Tracking_Hypothesis
{
public:
	DataPlot* m_List_of_Assigned_Plots;
	ExtendedKalman m_Kalman;

	/// <summary>
	/// Default Constructor
	/// </summary>
	Tracking_Hypothesis() noexcept;

	/// <summary>
	/// Hypothesis constructor
	/// </summary>
	Tracking_Hypothesis(DataPlot* plot_list, ExtendedKalman kalman);

	/// <summary>
	/// Virtual Hypothesis destructor
	/// </summary>
	virtual ~Tracking_Hypothesis();

	/// <summary>
	/// Returns the list of assigned plots.
	/// </summary>
	DataPlot* Get_List_of_Assigned_Plots()
	{
		return m_List_of_Assigned_Plots;
	}

	/// <summary>
	/// Returns the kalman member.
	/// </summary>
	ExtendedKalman Get_Kalman()
	{
		return m_Kalman;
	}

	/// <summary>
	/// Print A
	/// </summary>
	void display(T** A, int n);

	/// <summary>
	/// Get Cofactor --------- Creates a new matrix without the row and col of matrix item (p,q)
	/// </summary>
	void getCofactor(int** A, int** temp, int p, int q, int n);

	/// <summary>
	/// Recursive function for finding determinant of matrix.
	///n is current dimension of A[][].
	/// </summary>
	int determinant(int** A, int n);

	/// <summary>
	/// Function to get adjoint of A[N][N] in adj[N][N]. 
	///The adjoint of a matrix A is the transpose of the cofactor matrix of A
	/// </summary>
	void adjoint(int** A, float** adj, int n);

	/// <summary>
	/// Function to calculate and store inverse, returns false if matrix is singular 
	/// </summary>
	bool inverse(int** A, float** inverse, int n);

	/// <summary>
	/// This function multiplication the original matrix and the inverse matrix
	/// </summary>
	int** mult(int** A, float** B, int n);

	/// <summary>
	/// Create hypothesis function
	/// </summary>
	void createHypothesis();

	/// <summary>
	/// Generate the hypothesis tree
	/// </summary>
	TreeNode* GenerateHypothesis(int nCluster);

	void secondMain();

	/// <summary>
	/// Calculates and returns the value of the Gate.
	/// </summary>
	double  Gate(DataTrack track);

	/// <summary>
	/// Calculates the propabilities of each hypothesis and sets them.
	/// </summary>
	void Hypotheses_Probability(TreeNode* leaf_list, int num_of_leafs, int num_of_plots, DataTrack* list_of_tracks);

	/// <summary>
	/// Helper function that returns a list of only the NDTs out of all the leafs
	/// </summary>
	DataTrack* NDT_Track_List(int NDT, TreeNode* leaf_list, int num_of_leafs, DataTrack* list_of_tracks);

	/// <summary>
	/// Function that reduces the amount of hypothesises by removing the irrelevant children of the hypothesis tree.
	/// </summary>
	TreeNode* Hypotheses_Reduction(TreeNode* leaf_list, int num_of_leafs);


	/// <summary>
	/// Function that merges tracks with similiar plots in their history.
	/// </summary>
	DataTrack* Track_Merge(DataTrack* track_list, int num_of_tracks);

	/// <summary>
	/// Helper function that helps to figure out if two tracks share the same history of plots.
	/// </summary>
	bool Check_Tracks_For_Merge(DataTrack track1, DataTrack track2);




private:
	/// <summary>
	/// Sets the list of assigned plots.
	/// </summary>
	void Set_List_of_Assigned_Plots(DataPlot* plot_list);

	/// <summary>
	/// Sets the kalman member.
	/// </summary>
	void Set_Kalman(ExtendedKalman kalman);

};
#endif //_TRACKING_HYPOTHESIS_H_