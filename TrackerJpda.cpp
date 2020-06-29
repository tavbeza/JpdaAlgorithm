#include "TrackerJpda.h"
#include "SrvTimeStamp.h"
#include "Graph.h"


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
/// The function should receive plots  
/// </summary>
void TrackerJpda::DoTrack(const DataPlotList &dataPlotList)
{
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
		Graph graph(nTracks); //creat graph-list in nTracks size ?
		bool isAsocFlagVec;
		double g;
		int i, j, t;
		//int counter;

		for (t = 0; t < nTracks; t++) //Predict all the tracks
		{ 
			m_dataTrackList[t]->m_pKalman->Predict();
		}

		for (i = 0; i < nPlots; i++)
		{	
			//counter = 0;
			for (j = 0; j < nTracks; j++)
			{
				isAsocFlagVec = false;
				g = 0;
				gAssociationMatrix->CheckAssociation(*m_dataTrackList[j], *dataPlotList[i], isAsocFlagVec, g);
				gAssociationMatrix->Associate(m_dataTrackList[j]->m_Id, dataPlotList[i]->m_seqNumber, isAsocFlagVec, g);
				
				//if (isAsocFlagVec)
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
				//	counter++;
				//}
			}

			//if(counter == nTracks)
			//{
			//	DataTrack* pDataTrack = m_dataTrackList.CreateTrack();
			//	pDataTrack->InitTrack(*dataPlotList[i]);
			//}
		}
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
	}
}