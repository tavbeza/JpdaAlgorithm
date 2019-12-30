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
	DataTrack* pDataTrack = m_dataTracksList[m_nTracks];
	m_nTracks++;

	return pDataTrack;
}

/// <summary>
/// New operator return the DataTrack in the DataTrackList with index
/// </summary>
DataTrack* DataTrackList::operator[](const int index) const
{
	if (index > m_nTracks)
		return 0;

	return m_dataTracksList[index];
}