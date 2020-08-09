#include "pch.h"
#include "Tracking_Hypothesis.h"


/// <summary>
/// Print A
/// </summary>
template<class T>
void Tracking_Hypothesis<T>::display(T** A, int n)
{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
			cout << A[i][j] << " ";
		cout << endl;
	}
}

/// <summary>
/// Get Cofactor --------- Creates a new matrix without the row and col of matrix item (p,q)
/// </summary>
template<class T>
void Tracking_Hypothesis<T>::getCofactor(int** A, int** temp, int p, int q, int n)
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
template<class T>
int Tracking_Hypothesis<T>::determinant(int** A, int n)
{
	int D = 0; // Initialize result 

			   // Base case : if matrix contains single element 
	if (n == 1)
		return A[0][0];

	int** temp = (int**)calloc(n * n, sizeof(int*));
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
///The adjoint of a matrix A is the transpose of the cofactor matrix of A
/// </summary>
template<class T>
void Tracking_Hypothesis<T>::adjoint(int** A, float** adj, int n)
{
	if (n == 1)
	{
		adj[0][0] = 1;
		return;
	}

	// temp is used to store cofactors of A[][] 
	int sign = 1;

	int** temp = (int**)calloc(n * n, sizeof(int*));
	for (int i = 0; i < n; i++) {
		temp[i] = (int*)calloc(n, sizeof(int));
	}

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			// Get cofactor of A[i][j] 
			getCofactor(A, temp, i, j, n);

			// sign of adj[j][i] positive if sum of row 
			// and column indexes is even. 
			sign = ((i + j) % 2 == 0) ? 1 : -1;

			// Interchanging rows and columns to get the 
			// transpose of the cofactor matrix 
			adj[j][i] = (sign) * (determinant(temp, n - 1));
		}
	}
}

/// <summary>
/// Function to calculate and store inverse, returns false if matrix is singular 
/// </summary>
template<class T>
bool Tracking_Hypothesis<T>::inverse(int** A, float** inverse, int n)
{
	// Find determinant of A[][] 
	int det = determinant(A, n);
	if (det == 0)
	{
		cout << "Singular matrix, can't find its inverse";
		return false;
	}

	// Find adjoint 
	float** adj = (float**)calloc(n * n, sizeof(float*));
	for (int i = 0; i < n; i++) {
		adj[i] = (float*)calloc(n, sizeof(float));
	}
	adjoint(A, adj, n);

	// Find Inverse using formula "inverse(A) = adj(A)/det(A)" 
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			inverse[i][j] = adj[i][j] / float(det);

	return true;
}

/// <summary>
/// This function multiplication the original matrix and the inverse matrix
/// </summary>
template<class T>
int** Tracking_Hypothesis<T>::mult(int** A, float** B, int n) {
	int i, j, k;
	float** C = (float**)calloc(n, sizeof(float*));
	for (i = 0; i < n; i++) {
		C[i] = (float*)calloc(n, sizeof(float));
	}
	int** CK = (int**)calloc(n * n, sizeof(int*));
	for (int i = 0; i < n; i++) {
		CK[i] = (int*)calloc(n, sizeof(int));
	}
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			C[i][j] = 0;
			for (k = 0; k < n; k++) {
				C[i][j] += A[i][k] * B[k][j];
				//	*(*(C + i) + j) += (*(*(A + i) + k)) * (*(*(B + k) + j));
			}
			CK[i][j] = static_cast<int>(std::round(C[i][j]));
		}
	}
	return CK;
}

/// <summary>
/// Sets the list of assigned plots.
/// </summary>
template<class T>
void Tracking_Hypothesis<T>::Set_List_of_Assigned_Plots(DataPlot* plot_list)
{
	this->m_List_of_Assigned_Plots = plot_list;
}

/// <summary>
/// Sets the kalman member.
/// </summary>
template<class T>
void Tracking_Hypothesis<T>::Set_Kalman(ExtendedKalman kalman)
{
	this->m_Kalman = kalman;
}

/// <summary>
/// Virtual Hypothesis destructor
/// </summary>
template<class T>
Tracking_Hypothesis<T>::~Tracking_Hypothesis()
{

}

/// <summary>
/// Default Hypothesis Constructor
/// </summary>
template<class T>
Tracking_Hypothesis<T>::Tracking_Hypothesis() noexcept
{
}

/// <summary>
/// Hypothesis constructor
/// </summary>
template<class T>
Tracking_Hypothesis<T>::Tracking_Hypothesis(DataPlot* plot_list, ExtendedKalman kalman)
{
	this->m_Kalman = kalman;
	this->m_List_of_Assigned_Plots = plot_list;
}

TreeNode* trees[SIZE];
TreeNode* fathers[SIZE * SIZE];
TreeNode* sons[SIZE * SIZE];

/// <summary>
/// Create hypothesis function
/// </summary>
template<class T>
void Tracking_Hypothesis<T>::createHypothesis()
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


	cout << "\n\nThe calculate hypothesis time: " << timeDiff * 1000000 << "\tcount: " << lastSons << endl;

}

/// <summary>
/// Generate the hypothesis tree
/// </summary>
template<class T>
TreeNode* Tracking_Hypothesis<T>::GenerateHypothesis(int nCluster)
{
	AssociationMatrixCluster* pAssociationMatrix = AssociationMatrixCluster::GetInstance(nCluster);
	int nTracks = pAssociationMatrix->m_nTracks;
	int nPlots = pAssociationMatrix->m_nPlots;

	TreeNode* pTree = new TreeNode();
	pTree->m_i = -1;
	pTree->m_j = -1;

	list<TreeNode*>* pChildrens = new list<TreeNode*>[nPlots + 1];
	pChildrens[0].push_back(pTree);
	for (int i = 0; i < nPlots; i++)
	{
		for (int j = 0; j < nTracks + 1; j++)
		{
			if (pAssociationMatrix->m_MatTracksPlots[i][j])
			{

				for (list<TreeNode*>::iterator it = pChildrens[i].begin(); it != pChildrens[i].end(); it++)
				{
					TreeNode* pNode = new TreeNode();
					pNode->m_i = i;
					pNode->m_j = j;
					TreeNode* pParent = *it;
					pNode->SetParent(pParent);
					pParent->AddChild(pNode);
					pChildrens[i + 1].push_back(pNode);

					TreeNode* pNodeFT = new TreeNode();
					pNodeFT->m_i = i;
					pNodeFT->m_j = -2;
					pNodeFT->SetParent(pParent);
					pParent->AddChild(pNodeFT);
					pChildrens[i + 1].push_back(pNodeFT);
				}
			}
		}
	}

	DeleteDuplicatedHypothesis(pTree, nPlots);
	pTree->PrintPathToRoot();
	return pTree;
}

template<class T>
void Tracking_Hypothesis<T>::secondMain() {

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

/// <summary>
/// Calculates and returns the value of the Gate.
/// </summary>
template<class T>
double  Tracking_Hypothesis<T>::Gate(DataTrack track)
{
	//where B =  HpiH + R and R is the measurement noise covariance matrix. 
	Vector4d Zm = track.GetKalmanFilter().GetZ();
	Matrix49d H = track.GetKalmanFilter().GetH();
	Matrix9d P = track.GetKalmanFilter().GetP();
	Matrix4d R = track.GetKalmanFilter().GetR();
	Matrix4d B = H * P * H.Transpose() + R;
	Vector9d X = track.GetKalmanFilter().GetUpdate();

	Vector4d H_X = H * X;
	Vector4d Z_H_X = Zm - (H_X);
	Vector4d B_Inverse_Z_H_X = B.Inverse() * Z_H_X;
	double G = Z_H_X * B_Inverse_Z_H_X;
	return G;
}

/// <summary>
/// Calculates the propabilities of each hypothesis and sets them.
/// </summary>
template<class T>
void Tracking_Hypothesis<T>::Hypotheses_Probability(TreeNode* leaf_list, int num_of_leafs, int num_of_plots, DataTrack* list_of_tracks)
{
	TrackingParam params;
	double Pd = params.m_Pd; //Probability of detection
	double Bft = params.m_Bft; // Denisty of false targets
	double Bnt = params.m_Bnt; //  Density of previously unknown targets that have been detected 
	int NDT = 0; // Number of measurements associated with prior targets
	int NFT = 0; // Number of measurements associated with false targets
	int N = num_of_plots; // Num of plots
	DataTrack* list_of_assosiated_tracks;


	double first, second; // Devided the probability calculation to two parts.
	double* Pik = new double[num_of_leafs]; // List of probabilities
	double* Pik_devided_by_c = new double[num_of_leafs]; // List of probabilities devided by c
	int track_data = 0;

	for (int i = 0; i < num_of_leafs; i++)
	{
		TreeNode temp_hypo = leaf_list[i];
		while (temp_hypo.getParent() != 0)
		{
			if (temp_hypo.getAssociationData() == 1)
			{
				track_data = temp_hypo.getM_J();
				if (list_of_tracks[track_data].GetID() != -2)
				{
					NDT += 1;
				}
				else
				{
					NFT += 1;
				}
			}
			temp_hypo = *temp_hypo.getParent();
		}

		track_data = temp_hypo.getM_J();
		if (list_of_tracks[track_data].GetID() != -2)
		{
			NDT += 1;
		}
		else
		{
			NFT += 1;
		}
	}


	first = ::SrvDspMath::pow(Pd, NDT) * ::SrvDspMath::pow(1 - Pd, NFT) * ::SrvDspMath::pow(Bft, NFT) * ::SrvDspMath::pow(Bnt, N);
	list_of_assosiated_tracks = NDT_Track_List(NDT, leaf_list, num_of_leafs, list_of_tracks);

	Vector4d Z;
	Matrix49d H;
	Vector9d X;
	Matrix4d B;
	Vector4d HXB;
	double c = 0;
	for (int i = 0; i < num_of_leafs; i++)
	{
		second = 1;
		for (int j = 0; j < NDT; j++)
		{
			Z = list_of_assosiated_tracks[j].GetKalmanFilter().GetZ();
			H = list_of_assosiated_tracks[j].GetKalmanFilter().GetH();
			X = list_of_assosiated_tracks[j].GetKalmanFilter().GetUpdate();
			B = list_of_assosiated_tracks[j].GetKalmanFilter().GetB();
			//	HXB = H * X  * B;

			//second = second * ((Z-HXB)* N)
		}
		Pik[i] = first * second;
		c = c + (first * second);
	}

	for (int i = 0; i < num_of_leafs; i++)
	{
		Pik_devided_by_c[i] = Pik[i] / c;
	}
}

/// <summary>
/// Helper function that returns a list of only the NDTs out of all the leafs
/// </summary>
template<class T>
DataTrack* Tracking_Hypothesis<T>::NDT_Track_List(int NDT, TreeNode* leaf_list, int num_of_leafs, DataTrack* list_of_tracks)
{
	DataTrack* NDT_List = (DataTrack*)malloc(NDT * sizeof(DataTrack));
	int track_data = 0;
	int index = 0;

	for (int i = 0; i < num_of_leafs; i++)
	{
		TreeNode temp_hypo = leaf_list[i];
		while (temp_hypo.getParent() != 0)
		{
			if (temp_hypo.getAssociationData() == 1)
			{
				track_data = temp_hypo.getM_J();
				if (list_of_tracks[track_data].GetID() != -2)
				{

				}
			}
			temp_hypo = *temp_hypo.getParent();
		}

		track_data = temp_hypo.getM_J();
		if (list_of_tracks[track_data].GetID() != -2)
		{
			NDT_List[index] = list_of_tracks[track_data];
			index++;
		}
	}
	return NDT_List;
}

/// <summary>
/// Function that reduces the amount of hypothesises by removing the irrelevant children of the hypothesis tree.
/// </summary>
template<class T>
TreeNode* Tracking_Hypothesis<T>::Hypotheses_Reduction(TreeNode* leaf_list, int num_of_leafs)
{
	vector<int> temp_track_ids;
	TreeNode* reduced_tree;
	int temp_list_size = 0;
	int temp_unique_list_size = 0;

	for (int i = 0; i < num_of_leafs; i++)
	{
		//Resets the values and vector before working on each leaf.
		temp_track_ids.clear();
		temp_list_size = 0;
		temp_unique_list_size = 0;
		TreeNode temp_hypo = leaf_list[i];

		//Addsd all track ids from leaf to root(Without FA) to a vector
		while (temp_hypo.getParent() != 0)
		{
			if (temp_hypo.getM_I() != -1) // Assuming FA id is -1 , can change the value if FA id changes 
			{
				temp_track_ids.push_back(temp_hypo.getM_I());
			}

			temp_hypo = *temp_hypo.getParent();
		}

		//Holds the amount of track ids we added above.
		temp_list_size = temp_track_ids.size();
		//Sorts the vector to help us use the std::unique function better.
		std::sort(temp_track_ids.begin(), temp_track_ids.end());

		// Using std::unique and std::distance to count unique elements in a container 
		temp_unique_list_size = std::distance(v.begin(), std::unique(v.begin(), v.end()));


		//If the amount of unique track ids is the same as the total number of track ids it means there are no duplicates the hypothesis is okay.
		//Adds the child to the reduced tree
		if (temp_list_size == temp_unique_list_size)
		{
			reduced_tree->AddChild(temp_hypo);
		}

	}
	//Returns the new tree that now holds only leafs that were not reduced in this function.
	return reduced_tree;
}

/// <summary>
/// Function that merges tracks with similiar plots in their history.
/// Should recieve a track list with tracks from the same cluster.
/// </summary>
template<class T>
DataTrack* Tracking_Hypothesis<T>::Track_Merge(DataTrack* track_list, int num_of_tracks)
{
	//The list of tracks that will be returned after the merging
	DataTrack* merged_Track_List;
	bool has_Changed = true;

	//Loops over every single track and checks if it should be merged with any other track (should be in same cluster to avoid too many tracks being checked)
	//If two tracks should be merged, change the first tracks id to defined value and move on to the next track.
	//Should look like this for example.
	// 1 2 1 2 2 1 3 1 
	// x 2 1 2 2 1 3 1
	// x x 1 2 2 1 3 1 etc.
	//after the run will be : x x x x 2 x 3 1 = 2 3 1 the unique merged tracks
	for (int i = 0; i < num_of_tracks - 1; i++)
	{
		for (int j = i + 1; j < num_of_tracks - 1 - i; j++)
		{
			if (Check_Tracks_For_Merge(track_list[i], track_list[j]))
			{
				track_list[i].SetIdToMerge();
				break;
			}
		}
	}
	for (int i = 0; i < num_of_tracks; i++)
	{
		if (track_list[i].GetID() != TRACK_ID_TO_BE_REMOVED_MERGE)
		{
			merged_Track_List->AddTrack(track_list[i]);
		}
	}

	return merged_Track_List;
}


/// <summary>
/// Helper function that helps to figure out if two tracks share the same history of plots.
/// </summary>
template<class T>
bool Tracking_Hypothesis<T>::Check_Tracks_For_Merge(DataTrack track1, DataTrack track2)
{
	//Gets the associated plot list of each Track.
	DataPlot* track1_List = track1.GetPlot_List();
	DataPlot* track2_List = track1.GetPlot_List();

	//Checks how many plots have been associated with each track
	int track1_Num_Of_Plots = track1.GetNDwells();
	int track2_Num_Of_Plots = track2.GetNDwells();

	////////int num_of_Plots = std::min(track1_Num_Of_Plots, track2_Num_Of_Plots);

	//If one of the tracks (Or both) have less than 5 plots there is not enough information, return false.
	if (track1_Num_Of_Plots < 5 || track2_Num_Of_Plots < 5)
	{
		return false;
	}

	//Helps in the for loop
	int location1 = track1_Num_Of_Plots - 1;
	int location2 = track2_Num_Of_Plots - 1;

	int num_of_Matched_Plots = 0;

	//Checks the X+1 amount (parameter) of last plots of both tracks, if they share at least X amount of same plots they are good to be merged.
	for (int i = 0; i < TRACK_MERGE_SIMILIAR_PLOT_COUNT + 1; i++)
	{
		num_of_Matched_Plots = 0;
		if (track1_List[track1_Num_Of_Plots - 1].GetSequenceNumber() == track2_List[track2_Num_Of_Plots - 1].GetSequenceNumber())
		{
			num_of_Matched_Plots++;
		}
		location1--;
		location2--;
	}

	if (num_of_Matched_Plots >= TRACK_MERGE_SIMILIAR_PLOT_COUNT)
	{
		return true;
	}
	else
	{
		return false;
	}
}