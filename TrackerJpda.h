#ifndef _TRACKERJPDA_H_
#define _TRACKERJPDA_H_

#include "Tracker.h"
#include "ExtendedKalman.h"
#include "AssociationMatrix.h"


/// <summary>
/// This tracker class inherits from the class Tracker
/// </summary>
class TrackerJpda :  public Tracker
{

public:
	/// <summary>
	/// Empty constructor
	/// </summary>
	TrackerJpda();

	/// <summary>
	/// Destructor 
	/// </summary>
	virtual ~TrackerJpda();

	/// <summary>
	// The function should receive plots  
	/// </summary>
	virtual void DoTrack(const DataPlotList &dataPlotList);

private:
	
	int trackListCluster[MAX_NUM_TRACKS];
	int plotListCluster[MAX_NUM_PLOTS];
};

#endif // _TRACKERJPDA_H_

