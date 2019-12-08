///////////////////////////////////////////////////////////
//  SrvTimeStamp.h
//  Implementation of the Class SrvTimeStamp
//  Created on:      22-Jul-2018 10:19:45
//  Original author: sb50451
///////////////////////////////////////////////////////////


#ifndef TIMESTAMP_H_
#define TIMESTAMP_H_
//#include "Service.h"

/// <summary>
/// this class is supporting Time Stamp service.
///The time stamp measure the activities in the system
///The time stamp shall contain init start and stop functionality
///The initialization shall accrue once.start shall return the start time and stop shall return the stop time
/// </summary>
class SrvTimeStamp// : public Service
{

public:
	/// <summary>
	/// Empty constructor
	/// </summary>
	SrvTimeStamp();

	/// <summary>
	/// Virtual destructor 
	/// </summary>
	virtual ~SrvTimeStamp();

	/// <summary>
	/// <param name="startTime"></param>
	/// </summary>
	long long getTimeDelta(long long startTime);

	/// <summary>
	/// Get the Time Stamp
	/// </summary>
	long long  getTimeStamp();

	/// <summary>
	/// this method is used to initialize the service parameters
	/// </summary>
	void initService();

	/// <summary>
	/// Initialize the frequency of the timer
	/// </summary>
	static void Init();

	/// <summary>
	/// Start the timer
	/// </summary>
	static long long Start();

	/// <summary>
	/// Get Current Time of timer
	/// </summary>
	static long long GetCurrent();

	/// <summary>
	/// Stop the timer
	/// </summary>
	static long long Stop();

	/// <summary>
	/// Stop the timer and return the time difference
	/// </summary>
	static double Stop(long long startTime);

private:
	static double m_Frequency;
};
#endif // !defined(TIMESTAMP_H_)
