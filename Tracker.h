#ifndef _TRACKER_H_
#define _TRACKER_H_

#include <list>
#include "DataTrackList.h"
#include "DataPlotList.h"

/// <summary>
/// This class encpsulate the tracker entity. 
/// This is the base class for all trackers.
/// </summary>
class Tracker
{
public:
	/// <summary>
	/// Tracker constractor.
	/// </summary>
	Tracker();

	/// <summary>
	/// Tracker destractor.
	/// </summary>
	virtual ~Tracker();

	/// <summary>
	/// Preform the tracking of the current duell.
	/// </summary>
	virtual Vector3d DoTrack(const DataPlotList &dataPlotList) = 0;

	/// <summary>
	/// Returns all tracks in the system.
	/// </summary>
	DataTrackList* GetTrack();

protected:
	DataTrackList m_dataTrackList;
};

#endif //_TRACKER_H_

