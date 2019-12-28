#include "DataTrackList.h"

DataTrackList::DataTrackList()
{
	m_nTracks = 0;
}


DataTrackList::~DataTrackList()
{
}

/// <summary>
/// Returns the number of Tracks 
/// </summary>
int DataTrackList::GetCount()
{
	return m_nTracks;
}

/// <summary>
/// Creat new Track and upward the m_nTracks 
/// </summary>
DataTrack* DataTrackList::CreateTrack()
{
	DataTrack* pDataTrack = &(m_dataTracksList[m_nTracks]);
	m_nTracks++;
}