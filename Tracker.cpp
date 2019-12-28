#include "Tracker.h"


/// <summary>
/// Tracker constractor.
/// </summary>
Tracker::Tracker()
{
}

/// <summary>
/// Tracker destractor.
/// </summary>
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
