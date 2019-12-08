#ifndef _NAV_PLAT_STATUS_STRUCT_H_
#define _NAV_PLAT_STATUS_STRUCT_H_
#include "Vector.h"

/// <summary>
/// This class encapsulate the navigation parameters accepted by the platform navigation system
/// </summary>
class NavPlatStatusStruct
{
public:
	/// <summary>
	/// Empty constructor
	/// </summary>
	NavPlatStatusStruct();

	/// <summary>
	/// Destructor 
	/// </summary>
	~NavPlatStatusStruct();


	unsigned long long eslUtcTime;
	double lati;
	double longi;
	double alti;
	double horizontalPositionAccutacy;
	double verticaltalPositionAccutacy;
	double rollAngle;
	double pitchAnlge;
	double yawAngle;
	double rollRate;
	double pitchRate;
	double yawRate;
	double inertialVelocityNorth;
	double inertialVelocityEast;
	double inertialVelocityDown;
	double calibratedAirspeed;
	Vector3d velENU;
};

#endif //_NAV_PLAT_STATUS_STRUCT_H_