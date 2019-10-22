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


/// 
/// <param name="startTime"></param>
long long SrvTimeStamp::getTimeDelta(long long startTime) {

	return (Stop() - startTime);
}


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
long long SrvTimeStamp::Start()
{
	if (SrvTimeStamp::m_Frequency == 0)
		Init();

	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);

	return li.QuadPart;
}

long long SrvTimeStamp::GetCurrent()
{
	return Stop();
}

long long SrvTimeStamp::Stop()
{
	return Start();
}

double SrvTimeStamp::Stop(long long startTime)
{
	if (SrvTimeStamp::m_Frequency == 0)
		Init();
	long long stopTime = Stop(); 
	return (stopTime - startTime) / (SrvTimeStamp::m_Frequency);
}
