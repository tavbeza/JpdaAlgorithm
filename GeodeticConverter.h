#ifndef _Geodetic_Converter_H
#define _Geodetic_Converter_H

#include "Vector.h"
#include "Matrix.h"


/// <summary>
/// Converts the class to do the geodetic transformation
/// </summary>
class GeodeticConverter
{
public:

	/// <summary>
	/// Default Constructor
	/// </summary>
	GeodeticConverter();

	/// <summary>
	/// GeodeticConverter Destructor.
	/// </summary>
	~GeodeticConverter();


	/// <summary>
	/// Performs the velocity (rotation only) transformation from ECEF to ENU coordinates
	///for a given position vector and ENU frame location.
	/// </summary>
	static void VEcef2Enu(const Vector3d &vel_ecef, const Vector3d &pos_enu_0, Vector3d& vel_enu);


	/// <summary>
	/// Performs the velocity (rotation only) transformation from ENU to ECEF coordinates
	///for a given position vector and ENU frame location.
	/// </summary>
	static void VEnu2Ecef(const Vector3d &vel_enu, const Vector3d &pos_enu_0, Vector3d& vel_ecef);



	/// <summary>
	/// Rotate vector from ENU to local ECEF
	/// </summary>
	static void Enu2Ecef(const Vector3d &pos_enu, const Vector3d &pos_enu_0, Vector3d &pos_ecef);

	/// <summary>
	/// Rotate vector from ECEF to local ENU
	/// </summary>
	static void Ecef2Enu(const Vector3d& pos_ecef, const Vector3d& pos_enu_0, Vector3d& pos_enu);

	/// <summary>
	/// Initialize rotation matrix Rpsi (Rotation around z axis)
	/// </summary>
	static void InitRpsi(Matrix3d& C, double psi);

	/// <summary>
	/// Initialize rotation matrix Rphi (Rotation around x axis)
	/// </summary>
	static void InitRphi(Matrix3d& C, double phi);

	/// <summary>
	/// Initialize rotation matrix Rtheta (Rotation around y axis)
	/// </summary>
	static void InitRtheta(Matrix3d& C, double theta);

	/// <summary>
	/// Initialize rotation matrix psi_theta_phi cb2p - Rphi*Rtheta*Rpsi
	/// </summary>
	static void InitTcb2p(Matrix3d& C, double psi, double theta, double phi);

};

#endif // _Geodetic_Converter_H