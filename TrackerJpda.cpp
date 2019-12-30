#include "TrackerJpda.h"


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
		for (int i = 0; i < nTracks; i++)
		{
			float dt = 93.0 / 1000.0;
			m_dataTrackList[i]->m_pKalman->Predict(dt);
			m_dataTrackList[i]->m_pKalman->Update()
		}
	}
}