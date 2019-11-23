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
DataPlot::DataPlot(float range, float azimuth, float elevation, float velocity,
	float rangeAccuracy, float azimuthAccuracy, float elevationAccuracy,
	float velocityAccuracy, float SNR, float magnitude, unsigned int seqNumber)
{
	m_range = range;
	m_azimuth = azimuth;
	m_elevation = elevation;
	m_velocity = velocity;
	m_rangeAccuracy = rangeAccuracy;
	m_azimuthAccuracy = azimuthAccuracy;
	m_elevationAccuracy = elevationAccuracy;
	m_velocityAccuracy = velocityAccuracy;
	m_SNR = SNR;
	m_magnitude = magnitude;
	m_seqNumber = seqNumber;
}

/// <summary>
/// Destructor
/// </summary>
DataPlot::~DataPlot()
{
}

/// <summary>
/// Get range of the plot
/// </summary>
float DataPlot::GetRange() const 
{
	return m_range;
}

/// <summary>
/// Get	azimuth angle of the plot
/// </summary>
float DataPlot::GetAzimuthAngle() const 
{
	return m_azimuth;
}

/// <summary>
/// Get	elevation angle of the plot
/// </summary>
float DataPlot::GetElevationAngle()  const 
{
	return m_elevation;
}

/// <summary>
/// Get	range accuracy of the plot
/// </summary>
float DataPlot::GetRangeAccuracy() const 
{
	return m_rangeAccuracy;
}

/// <summary>
/// Get	velocity accuracy of the plot
/// </summary>
float DataPlot::GetVelocityAccuracy() const 
{
	return m_velocityAccuracy;
}

/// <summary>
/// Get	azimuth	accuracy of the plot
/// </summary>
float DataPlot::GetAzimuthAccuracy() const 
{
	return m_azimuthAccuracy;
}

/// <summary>
/// Get	elevation accuracy of the plot
/// </summary>
float DataPlot::GetElevationAccuracy() const 
{
	return m_elevationAccuracy;
}
