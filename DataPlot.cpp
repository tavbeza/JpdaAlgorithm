#include "DataPlot.h"


/// <summary>
/// empty constructor
/// </summary>
DataPlot::DataPlot()
{
}


/// <summary>
/// constructor with elements
/// </summary>
DataPlot::DataPlot(float m_range, float m_azimuth, float m_elevation, float m_velocity,
					float m_rangeAccuricy, float m_azimuthAccuricy, float m_elevationAccuricy, 
					float m_velocityAccuricy, float m_SNR, float m_magnitot, unsigned int m_ID)
{
}

/// <summary>
/// empty disconstructor
/// </summary>
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
