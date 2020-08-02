#ifndef _TRACKER_PARAMS_H_
#define _TRACKER_PARAMS_H_

#include "Vector.h"

/// <summary>
/// Class of tracker parameters 
/// </summary>
class TrackerParams
{
public:
	/// <summary>
	/// Empty constructor
	/// </summary>
	TrackerParams();
	
	/// <summary>
	/// Destructor 
	/// </summary>
	~TrackerParams();

public:
	double m_SigmaVxs = 10;	// 10 m/sec
	double m_SigmaVys = 10;
	double m_SigmaVzs = 10;
	double m_SigmaAxs = 9;	// 9 m/sec*sec
	double m_SigmaAys = 9;
	double m_SigmaAzs = 9;
	double m_Sigma_r = 0.1;
	double m_Sigma_rdot = 0.1;
	double m_Sigma_az = 0.1;
	double m_Sigma_el = 0.1;
	double m_SigmaVxt = 0.1;
	double m_SigmaVyt = 0.1;
	double m_SigmaVzt = 0.1;
	double m_SigmaAxt = 0.1;
	double m_SigmaAyt = 0.1;
	double m_SigmaAzt = 0.1;
	double m_Sensor_Theta = 0.1;
	double m_Sensor_Psi = 0.1;
	double  m_Sensor_Phi = 0.1;
	Vector3d m_TauAcc = Vector3d(0.1, 0.1, 0.1);
	Vector3d m_Amax = Vector3d(0.1, 0.1, 0.1);
	Vector3d m_Pmax = Vector3d(0.1, 0.1, 0.1);
	Vector3d m_P0 = Vector3d(0.1, 0.1, 0.1);
	Vector3d m_SigmaManeuver2 = Vector3d(0.1, 0.1, 0.1);
	double m_SigmaMnvrCalcMtd = 0.1;
	double m_Kgl = 1;
	double m_Eliptic_Gate = 3;
};

#endif // _TRACKER_PARAMS_H_

