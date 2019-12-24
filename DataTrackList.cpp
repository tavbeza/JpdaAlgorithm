#include "DataTrackList.h"

DataTrackList::DataTrackList()
{
	m_nTracks = 0;
}


DataTrackList::~DataTrackList()
{
}

int DataTrackList::GetCount()
{
	return m_nTracks;
}

DataTrack* DataTrackList::CreateTrack()
{
	DataTrack* pDataTrack = &(m_dataTracksList[m_nTracks]);
	m_nTracks++;
}