#include "DataPlot.h"



DataPlot::DataPlot()
{
}


DataPlot::~DataPlot()
{
}

float DataPlot::GetRange() const 
{
	return m_range;
}

float DataPlot::GetAzimuthAngle() const 
{
	return m_azimuth;
}

float DataPlot::GetElevationAngle()  const 
{
	return m_elevation;
}

float DataPlot::GetRangeAccuracy() const 
{
	return m_rangeAccuricy;
}

float DataPlot::GetVelocityAccuracy() const 
{
	return m_velocityAccuricy;
}

float DataPlot::GetAzimuthAccuracy() const 
{
	return m_azimuthAccuricy;
}

float DataPlot::GetElevationAccuracy() const 
{
	return m_elevationAccuricy;
}
