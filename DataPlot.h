#ifndef _DATA_PLOT_H_
#define _DATA_PLOT_H_

#include "Vector.h"
class DataPlot
{
public:
	/// <summary>
	/// empty constructor
	/// </summary>
	DataPlot();

	/// <summary>
	/// Destructor 
	/// </summary>
	~DataPlot();

	/// <summary>
	/// Constructor with elements
	/// </summary>
	DataPlot::DataPlot(float m_range, float m_azimuth, float m_elevation, float m_velocity,
		float m_rangeAccuracy, float m_azimuthAccuracy, float m_elevationAccuracy,
		float m_velocityAccuracy, float m_SNR, float m_magnitude, unsigned int m_ID);

	/// <summary>
	/// Get range of the plot
	/// </summary>
	float GetRange() const;

	/// <summary>
	/// Get	azimuth angle of the plot
	/// </summary>
	float GetAzimuthAngle() const;

	/// <summary>
	/// Get	elevation angle of the plot
	/// </summary>
	float GetElevationAngle() const;

	/// <summary>
	/// Get	range accuracy of the plot
	/// </summary>
	float GetRangeAccuracy() const;

	/// <summary>
	/// Get	velocity accuracy of the plot
	/// </summary>
	float GetVelocityAccuracy() const;

	/// <summary>
	/// Get	azimuth	accuracy of the plot
	/// </summary>
	float GetAzimuthAccuracy() const;
	
	/// <summary>
	/// Get	elevation accuracy of the plot
	/// </summary>
	float GetElevationAccuracy() const;



public:
	float m_range;
	float m_azimuth;
	float m_elevation;
	float m_velocity;
	float m_rangeAccuracy;
	float m_azimuthAccuracy;
	float m_elevationAccuracy;
	float m_velocityAccuracy;
	float m_SNR;
	float m_magnitude;
	unsigned int m_seqNumber;
	Vector3d m_CartEnu0;
};

#endif //_DATA_PLOT_H_