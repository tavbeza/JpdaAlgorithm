#ifndef _DATA_TRACK_LIST_H
#define _DATA_TRACK_LIST_H

#include "DataTrack.h"
#define MAX_TRACKS 200
class DataTrackList
{
public:
	DataTrackList();
	~DataTrackList();

	/// <summary>
	/// Returns the number of Tracks 
	/// </summary>
	int GetCount();

	/// <summary>
	/// Creat new Track and upward the m_nTracks 
	/// </summary>
	DataTrack* CreateTrack();

private:
	DataTrack m_dataTracksList[MAX_TRACKS];
	int m_nTracks;
};

#endif // _DATA_TRACK_LIST_H