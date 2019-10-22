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
	SrvTimeStamp();
	virtual ~SrvTimeStamp();

	/// 
	/// <param name="startTime"></param>
	long long getTimeDelta(long long startTime);
	long long  getTimeStamp();
	/// <summary>
	/// this method is used to initialize the service parameters
	/// </summary>
	void initService();

	///Initialize the frequency of the timer
	static void Init();

	///Start the timer
	static long long Start();

	///Get Current Time of timer
	static long long GetCurrent();

	///Stop the timer
	static long long Stop();

	///Stop the timer and return the time difference
	static double Stop(long long startTime);
private:
	static double m_Frequency;
};
#endif // !defined(TIMESTAMP_H_)
