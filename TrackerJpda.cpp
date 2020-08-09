#include "TrackerJpda.h"
#include "SrvTimeStamp.h"
#include "Graph.h"
#include "KalmanTests.h"


/// <summary>
/// Empty constructor
/// </summary>
TrackerJpda::TrackerJpda()
{
}

/// <summary>
/// Destructor 
/// </summary>
TrackerJpda::~TrackerJpda()
{
}

/// <summary>
/// A gnn algorithm impementation
/// The gnn update tracks and plots according to their gain in the association matrix
/// </summary>
void TrackerJpda::GnnAssociate(const DataPlotList &dataPlotList)
{
	int nTracks = m_dataTrackList.GetCount();
	int nPlots = dataPlotList.GetCount();

	AssociationMatrix *gAssociationMatrix = AssociationMatrix::GetInstance();

	//printf("nTracks = %d\nPlots = %d\n", nTracks, nPlots);

	int maxGain = -1, rowIndex = -1, colIndex = -1;

	for (int i = 0; i < nTracks; i++)
	{
		for (int j = 0; j < nPlots; j++)
		{
			if (gAssociationMatrix->m_MatTracksPlots[i][j])
			{
				if (gAssociationMatrix->m_Gain[i][j] > maxGain)		// find and generate the maximum position
				{
					maxGain = gAssociationMatrix->m_Gain[i][j];
					rowIndex = i;
					colIndex = j;
				}
			}
		}
		if (maxGain != -1)
		{
			m_dataTrackList[i]->m_pKalman->m_S = 
				gAssociationMatrix->m_MatS[m_dataTrackList[i]->m_Id][dataPlotList[colIndex]->m_seqNumber];
			m_dataTrackList[i]->m_pKalman->Update(dataPlotList[colIndex]);
			dataPlotList[colIndex]->m_isAssociate = true;
		}
		maxGain = -1;
		rowIndex = -1;
		colIndex = -1;
	}

	for (int j = 0; j < nPlots; j++)
	{
		if (!dataPlotList[j]->m_isAssociate)
		{
			DataTrack* pDataTrack = m_dataTrackList.CreateTrack();
			pDataTrack->InitTrack(*dataPlotList[j]);
		}
	}
}


/// <summary>
/// The function should receive plots  
/// </summary>
Vector3d TrackerJpda::DoTrack(const DataPlotList &dataPlotList)
{
	// file pointer 
	fstream fout;
	//ofstream outfile("temp-KalmanPredictions.csv");

	// Create CSV file for Extended Kalman 3D test
	string fileName = "dataPlots.csv";

	// opens an existing csv file or creates a new file. 
	fout.open(fileName, ios::out | ios::app);

	int nTracks = m_dataTrackList.GetCount();
	int nPlots = dataPlotList.GetCount();
	
	if (nTracks == 0)
	{
		for (int i = 0; i < nPlots; i++)
		{
			DataTrack* pDataTrack = m_dataTrackList.CreateTrack();
			pDataTrack->InitTrack(*dataPlotList[i]);
		}
	}
	else
	{
		float dt = 93.0 / 1000.0;
		AssociationMatrix *gAssociationMatrix = AssociationMatrix::GetInstance();
		gAssociationMatrix->Clean();
		
		Graph graph(nTracks + nPlots); //creat graph-list in nTracks size ?
		
		bool isAsocFlagVec;
		double g;
		int i, j, t;
		int counter;

		for (t = 0; t < nTracks; t++) //Predict all the tracks
		{ 
			m_dataTrackList[t]->m_pKalman->Predict();
		}

		for (i = 0; i < nPlots; i++)
		{	
			counter = 0;
			for (j = 0; j < nTracks; j++)
			{
				isAsocFlagVec = false;
				g = 0;
				gAssociationMatrix->CheckAssociation(*m_dataTrackList[j], *dataPlotList[i], isAsocFlagVec, g);
				gAssociationMatrix->Associate(m_dataTrackList[j]->m_Id, dataPlotList[i]->m_seqNumber, isAsocFlagVec, g);
			}

			/*if(!dataPlotList[i]->m_isAssociate)
			{
				DataTrack* pDataTrack = m_dataTrackList.CreateTrack();
				pDataTrack->InitTrack(*dataPlotList[i]);
			}*/
		}

		//GnnAssociate(dataPlotList);
		
		for (i = 0; i < nTracks; i++)
		{
			m_dataTrackList[i]->m_pKalman->m_S = gAssociationMatrix->m_MatS[m_dataTrackList[i]->m_Id][dataPlotList[i]->m_seqNumber];
			m_dataTrackList[i]->m_pKalman->Update(dataPlotList[i]);
			//m_dataTrackList[i]->m_pKalman->GetLastLocation();
			
			// Choose which track to print
			if (i == 0)
			{
				//m_dataTrackList[i]->m_pKalman->GetLastLocation().PrintToFile(outfile);// << ",";
				fout << m_dataTrackList[i]->m_pKalman->m_X.m_Data[0];
				fout << ",";
				fout << m_dataTrackList[i]->m_pKalman->m_X.m_Data[1];
				fout << endl;
			}
			
		}

		//if (isAsocFlagVec && i == j)
		//{
			//long long time = SrvTimeStamp::Start();
			//m_dataTrackList[j]->m_pKalman->Predict();
			//double time1 = SrvTimeStamp::Stop(time)*1000*1000;

			//time = SrvTimeStamp::Start();
			//m_dataTrackList[j]->m_pKalman->Update(dataPlotList[i]);
			//double time2 = SrvTimeStamp::Stop(time)*1000*1000;
		//}
		//else
		//{
			//counter++;
		//}

		/*
		for (i = 0; i < nTracks; i++) 
		{
			for (j = j; j < nPlots; j++)
			{
				if (gAssociationMatrix->GetInstance[i][j] == 1)
				{			
					trackListCluster[i] = i;
				}
			}
		}
		*/
	}
	Vector3d q;
	return q;
}