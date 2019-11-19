#ifndef _DATA_PLOT_H_
#define _DATA_PLOT_H_
#include "Vector.h"
class DataPlot
{


public:
	float GetRange() const ;

	float GetAzimuthAngle() const ;

	float GetElevationAngle() const ;

	float GetRangeAccuracy() const ;

	float GetVelocityAccuracy() const ;

	float GetAzimuthAccuracy() const ;

	float GetElevationAccuracy() const ;

private:
	float m_range;
	float m_azimuth;
	float m_elevation;
	float m_velocity;
	float m_rangeAccuricy;
	float m_azimuthAccuricy;
	float m_elevationAccuricy;
	float m_velocityAccuricy;
	float m_SNR;
	float m_magnitot;
	unsigned int m_ID;


public:
	/// <summary>
	/// empty constructor
	/// </summary>
	DataPlot();

	/// <summary>
	/// empty disconstructor
	/// </summary>
	~DataPlot();

	/// <summary>
	/// constructor with elements
	/// </summary>
	DataPlot::DataPlot(float m_range, float m_azimuth, float m_elevation, float m_velocity,
		float m_rangeAccuricy, float m_azimuthAccuricy, float m_elevationAccuricy,
		float m_velocityAccuricy, float m_SNR, float m_magnitot, unsigned int m_ID)
	{
	}
	Vector3d m_CartEnu0;
};

#endif //_DATA_PLOT_H_