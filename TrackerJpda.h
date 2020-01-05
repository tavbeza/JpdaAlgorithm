#ifndef _TRACKERJPDA_H_
#define _TRACKERJPDA_H_

#include "Tracker.h"
#include "ExtendedKalman.h"
#include "AssociationMatrix.h"


/// <summary>
/// This tracker class inherits from the class Tracker
/// </summary>
class TrackerJpda : Tracker
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
	

};

#endif // _TRACKERJPDA_H_

