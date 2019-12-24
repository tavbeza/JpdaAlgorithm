#include "Tracker.h"



Tracker::Tracker()
{
}


Tracker::~Tracker()
{
}

/// <summary>
/// Returns all tracks in the system.
/// </summary>
DataTrackList* Tracker::GetTrack()
{
	return &m_dataTrackList;
}
