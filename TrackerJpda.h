#ifndef TRACKERJPDA_H_
#define TRACKERJPDA_H_

#include "Tracker.h"

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
	void Track();
};

#endif // TRACKERJPDA_H_

