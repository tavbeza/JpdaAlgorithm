#ifndef _DATA_TRACK_LIST_H
#define _DATA_TRACK_LIST_H

#include "DataTrack.h"
#define MAX_TRACKS 200

/// <summary>
/// Class with a list of DataTrack
/// </summary>
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

	/// <summary>
	/// New operator return the DataTrack in the DataTrackList with index
	/// </summary>
	DataTrack* DataTrackList::operator[](const int index) const;

private:
	DataTrack* m_dataTracksList[MAX_TRACKS];
	int m_nTracks;
};

#endif // _DATA_TRACK_LIST_H