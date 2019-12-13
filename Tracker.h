#ifndef _TRACKER_H_
#define _TRACKER_H_

#include <list>
#include "DataTrack.h"


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
	virtual void DoTrack() = 0;

	/// <summary>
	/// Returns all tracks in the system.
	/// </summary>
	virtual std::list<DataTrack*> GetTrack() = 0;
};

#endif //_TRACKER_H_

