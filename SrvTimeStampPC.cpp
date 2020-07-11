#include "stdafx.h"
#include "SrvTimeStamp.h"
#include <Windows.h>
#include <iostream>

double SrvTimeStamp::m_Frequency = 0;

/// <summary>
/// Constructor
/// </summary>
SrvTimeStamp::SrvTimeStamp()
{
	initService();
	// TODO Auto-generated constructor stub
}



/// <summary>
/// Destructor
/// </summary>
SrvTimeStamp::~SrvTimeStamp()
{

}

/// <summary>
/// Initialize the frequency of the timer
/// </summary>
void SrvTimeStamp::Init()
{
	if (SrvTimeStamp::m_Frequency == 0)
	{
		LARGE_INTEGER li;
		if (!QueryPerformanceFrequency(&li))
		{
			std::cout << "QueryPerformanceFrequency failed!\n";
		}

		m_Frequency = double(li.QuadPart);
	}
}


/// <summary>
/// <param name="startTime"></param>
/// </summary>
long long SrvTimeStamp::getTimeDelta(long long startTime) {

	return (Stop() - startTime);
}

/// <summary>
/// Get the TimeStamp
/// </summary>
long long SrvTimeStamp::getTimeStamp() {

	return Start();
}


/// <summary>
/// this method is used to initialize the service parameters
/// </summary>
void SrvTimeStamp::initService()
{
	Init();
}

/// <summary>
/// Start the timer
/// </summary>
long long SrvTimeStamp::Start()
{
	if (SrvTimeStamp::m_Frequency == 0)
		Init();

	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);

	return li.QuadPart;
}

/// <summary>
/// Get Current Time of timer
/// </summary>
long long SrvTimeStamp::GetCurrent()
{
	return Stop();
}

/// <summary>
/// Stop the timer
/// </summary>
long long SrvTimeStamp::Stop()
{
	return Start();
}

/// <summary>
/// Stop the timer and return the time difference in sec
/// </summary>
double SrvTimeStamp::Stop(long long startTime)
{
	if (SrvTimeStamp::m_Frequency == 0)
		Init();
	long long stopTime = Stop(); 
	return (stopTime - startTime) / (SrvTimeStamp::m_Frequency);
}
