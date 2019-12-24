
#include "DataTrack.h"
#define MAX_TRACKS 200
class DataTrackList
{
public:
	DataTrackList();
	~DataTrackList();

	int GetCount();

	DataTrack* CreateTrack();

private:
	DataTrack m_dataTracksList[MAX_TRACKS];
	int m_nTracks;
};

