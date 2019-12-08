#ifndef TRACKERJPDA_H_
#define TRACKERJPDA_H_

/// <summary>
/// Tracker Class
/// </summary>
class TrackerJpda
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

