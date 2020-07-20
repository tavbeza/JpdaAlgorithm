#include "stdafx.h"

#include "AssociationMatrix.h"
#include "SrvDspMath.h"
#include "TrackerParams.h"
#include "GeodeticConverter.h"

#define PI_VALUE 3.14159

AssociationMatrix   gAssociationMatrix;

/// <summary>
/// </summary>
AssociationMatrix::AssociationMatrix()
{
	Clean();
}

/// <summary>
/// </summary>
AssociationMatrix::~AssociationMatrix()
{
}

/// <summary>
/// Returns the global objects
/// </summary>
AssociationMatrix *AssociationMatrix::GetInstance()
{
	return &gAssociationMatrix;
}

/// <summary>
/// Clean the plots and tracks
/// </summary>
void AssociationMatrix::Clean()
{
	m_nPlots = 0;
	m_nTracks = 0;
	memset(&m_MatTracksPlots, 0, sizeof(m_MatTracksPlots));
	memset(&m_TrkIds, 0, sizeof(m_TrkIds));
	memset(&m_nPlots, 0, sizeof(m_nPlots));
	memset(&m_Gain, 0, sizeof(m_Gain));
}

/// <summary>
/// Associate track and plot and set the gain
/// </summary>
void AssociationMatrix::Associate(unsigned int trkID,
	unsigned int pltID,
	bool assocFlg,
	double g)
{
	int idxTrk = -1;
	int idxPlt = -1;
	if (0 == m_nTracks)
	{
		idxTrk = 0;
		m_TrkIds[idxTrk] = trkID;
		m_nTracks++;
	}
	else
	{
		for (unsigned int i = 0; i < m_nTracks; i++)
		{
			if (m_TrkIds[i] == trkID) //cheak if the track exsist in the array
			{
				idxTrk = i;
				break;
			}
		}
		if (-1 == idxTrk) // the track is not exsist and the array is not empty
		{
			m_TrkIds[m_nTracks] = trkID;
			idxTrk = m_nTracks;
			m_nTracks++;
		}
	}

	if (0 == m_nPlots)
	{
		idxPlt = 0;
		m_PlotIds[idxPlt] = pltID;
		m_nPlots++;
	}
	else
	{
		for (unsigned int i = 0; i < m_nPlots; i++)
		{
			if (m_PlotIds[i] == pltID)
			{
				idxPlt = i;
				break;
			}
		}
		if (-1 == idxPlt)
		{
			m_PlotIds[m_nPlots] = pltID;
			idxPlt = m_nPlots;
			m_nPlots++;
		}
	}


	m_Gain[idxTrk][idxPlt] = g; 
	if (g != 0)
	{
		int test = 1;
	}
	m_MatTracksPlots[idxTrk][idxPlt] = assocFlg;
}

/// <summary>
/// Set the angle between -pi:+pi
/// </summary>
double AssociationMatrix::Mod7(double angle)
{
	double x = 1;
	double retAngle = fmod(angle, 2 * PI_VALUE);
	// While angle is outside the range of -pi:+pi
	while (x > 0)
	{
		double b1 = (retAngle > PI_VALUE);
		retAngle = retAngle - 2 * PI_VALUE*b1;
		double b2 = (retAngle < -1 * PI_VALUE);
		retAngle = retAngle + 2 * PI_VALUE*b2;
		x = b1 + b2;
	}

	return retAngle;

}


/// <summary>
/// Check the track association to plot and calculate the gain
/// </summary>
void AssociationMatrix::CheckAssociation(DataTrack &track,
	const DataPlot &plot,
	/*const NavPlatStatusStruct &platData,*/
	bool &isAsocFlagVec,
	double &g)
{
	g = 0;

	//case 'ENU'
	//case 'nonlin'
	double r, az, el, rr;
	//platData.posENU.Print();
	//platData.velENU.Print();
	//track.m_Hyp[iHyp].m_XPredict.Print();

	/*Vector3d cartesian;
	cartesian.m_Data[0] = platData.posENU.m_Data[0];	// X
	cartesian.m_Data[1] = platData.posENU.m_Data[1];	// Y
	cartesian.m_Data[2] = platData.posENU.m_Data[2];	// Z

	Vector3d spherical;
	spherical.ToSpherical(cartesian);
	r = spherical.m_Data[0];
	az = spherical.m_Data[1];
	el = spherical.m_Data[2];

	cartesian.m_Data[0] = platData.velENU.m_Data[0];	// Vx
	cartesian.m_Data[1] = platData.velENU.m_Data[1];	// Vy
	cartesian.m_Data[2] = platData.velENU.m_Data[2];	// Vz
	spherical.ToSpherical(cartesian);*/


	/*GeodeticConverter::Cart2Sph(platData.posENU,
		platData.velENU,
		track.m_pKalman->m_X_Predict,
		r,
		az,
		el,
		rr);*/


	/*
	//plot.enu_0.Az;
	double r1 = plot.m_PolarEnu0.m_Data[0];
	double az1 = plot.m_PolarEnu0.m_Data[1];
	double el1 = plot.m_PolarEnu0.m_Data[2];
	double v1 = plot.GetVelocity();

	m_PolarEnu0 - Converted pos to Enu0 (Our static location)
	*/
	
	// pos without Enu0 (without converts)
	double r1 = plot.GetRange();
	double az1 = plot.GetAzimuthAngle();
	double el1 = plot.GetElevationAngle();
	double v1 = plot.GetVelocity();

	/*
	// diff between the plot and the platform
	double dAz = Mod7(az1 - az);
	double dEl = el1 - el;
	double dR = r1 - r;		
	double dRR = rr1 - rr; */


	Vector3d cartVelocity;
	Vector3d spherical(r1, az1, el1);
	cartVelocity.SphericalToCartVelocity(spherical);

	Vector9d plot_pos;
	plot_pos.m_Data[0] = r1 * SrvDspMath::sin(az1)*SrvDspMath::cos(el1);	// x
	plot_pos.m_Data[1] = r1 * SrvDspMath::sin(az1)*SrvDspMath::sin(el1);	// y
	plot_pos.m_Data[2] = r1 * SrvDspMath::cos(el1);							// z
	plot_pos.m_Data[3] = cartVelocity.m_Data[0];							// Vx
	plot_pos.m_Data[4] = cartVelocity.m_Data[1];							// Vy
	plot_pos.m_Data[5] = cartVelocity.m_Data[2];							// Vz
	plot_pos.m_Data[6] = 0;													// Ax
	plot_pos.m_Data[7] = 0;
	plot_pos.m_Data[8] = 0;
	
	Matrix49d temp_h;

	SetTempH(track.m_pKalman->m_X_Predict, plot_pos, temp_h);

	Matrix94d temp1 = track.m_pKalman->m_P_Predict * Transpose(temp_h);
	//track.m_pKalman->m_S = track.m_pKalman->m_H * temp1 + track.m_pKalman->m_R;
	Matrix4d new_S;

	//new_S = track.m_pKalman->m_H * temp1 + R;
	new_S = temp_h * temp1; // +track.m_pKalman->m_R;

	double sigma_r_plot = plot.GetRangeAccuracy();
	double sigma_az_plot = plot.GetAzimuthAccuracy();
	double sigma_el_plot = plot.GetElevationAccuracy();
	double sigma_v_plot = plot.GetVelocityAccuracy();

	double sigma_r = SrvDspMath::pow(sigma_r_plot, 2) + SrvDspMath::pow(new_S.m_Data[0][0], 2);
	double sigma_az = SrvDspMath::pow(sigma_az_plot, 2) + SrvDspMath::pow(new_S.m_Data[1][1], 2);
	double sigma_el = SrvDspMath::pow(sigma_el_plot, 2) + SrvDspMath::pow(new_S.m_Data[2][2], 2);
	double sigma_v = SrvDspMath::pow(sigma_v_plot, 2) + SrvDspMath::pow(new_S.m_Data[3][3], 2);
		
	// GateType='Rect'
	// case 'nonlin'
	// Rectangular gating coefficient
	TrackerParams *pTrackerParams = new TrackerParams();
	double kgl = 4;	//pTrackerParams->m_Kgl;
	Vector4d gateR;
	gateR.m_Data[0] = SrvDspMath::sqrt(sigma_r);
	gateR.m_Data[1] = SrvDspMath::sqrt(sigma_az);
	gateR.m_Data[2] = SrvDspMath::sqrt(sigma_el);
	gateR.m_Data[3] = SrvDspMath::sqrt(sigma_v);

	bool flagInGate = true;
	flagInGate &= r1 <= kgl * gateR.m_Data[0];
	flagInGate &= az1 < kgl * gateR.m_Data[1];
	flagInGate &= el1 < kgl * gateR.m_Data[2];
	flagInGate &= v1 < kgl * gateR.m_Data[3];

	if (flagInGate)
	{
		// Update Track
		// case 'nonlin'
		isAsocFlagVec = true;
		//double rPredict, azPredict, elPredict, vPredict;

		//TODO: 50114 this is the same previous call to Cart2Sph
		/*GeodeticConverter::Cart2Sph(platData.posENU,
			platData.velENU,
			track.m_Hyp[iHyp].m_XPredict,
			rPredict,
			azPredict,
			elPredict,
			vPredict);*/

		//Z_predict=[R_predict; Az_predict; El_predict;RR_predict];
		/*Vector4d zPredict;
		zPredict.m_Data[0] = rPredict;
		zPredict.m_Data[1] = azPredict;
		zPredict.m_Data[2] = elPredict;
		zPredict.m_Data[3] = rrPredict; */

		//Z=[Meas.enu_0.R; Meas.enu_0.Az; Meas.enu_0.El;  Meas.enu_0.RR];
		Vector4d z;
		z.m_Data[0] = r1;
		z.m_Data[1] = az1;
		z.m_Data[2] = el1;
		z.m_Data[3] = v1;

		// xPredict	cartesian position
		Vector4d xPredict;
		xPredict.m_Data[0] = track.m_pKalman->m_X_Predict.m_Data[0];	// x
		xPredict.m_Data[1] = track.m_pKalman->m_X_Predict.m_Data[1];	// y
		xPredict.m_Data[2] = track.m_pKalman->m_X_Predict.m_Data[2];	// z

		double r_dot = track.m_pKalman->m_Rdot_Predict;

		// zPredict spherical position
		Vector4d zPredict;
		zPredict.CartToSpherical(xPredict);
		zPredict.m_Data[3] = r_dot;

		//Vector4d y = z - zPredict;
		Vector4d y = z -zPredict;

		Matrix4d R;
		R.m_Data[0][0] = plot.GetRangeAccuracy();
		R.m_Data[1][1] = plot.GetAzimuthAccuracy();
		R.m_Data[2][2] = plot.GetElevationAccuracy();
		R.m_Data[3][3] = plot.GetVelocityAccuracy();

		// Calculate S
		// m_S = m_H*m_P*m_H' + m_R
		//track.m_pKalman.
		//Matrix94d temp1 = track.m_pKalman->m_P_Predict * Transpose(track.m_pKalman->m_H);
		//track.m_pKalman->m_S = track.m_pKalman->m_H * temp1 + track.m_pKalman->m_R;
		//Matrix4d new_S;
		
		//new_S = track.m_pKalman->m_H * temp1 + R;
		//new_S = track.m_pKalman->m_H * temp1 + track.m_pKalman->m_R;
		//new_S = track.m_pKalman->m_H * temp1 + track.m_pKalman->m_R;
		//לשנות, לא לגעת במטריצת אס של הקלמן זה אמור להיות משהו זמני וגם האמ אר הוא של הפלוט ולא של הטרק
		//track.m_KF.m_S.Print();

		Vector4d tempV = new_S * y;
		double d2 = (y * tempV) / y.Norm();
		double m = 4;	// length(Z);
		double sqrtDetSi = SrvDspMath::sqrt(new_S.Determinant());
		double x1 = SrvDspMath::exp(-d2 / 2);
		double x2 = (pow(2 * PI_VALUE, (m / 2)) * sqrtDetSi);
		g = x1 / x2;

		//g = SrvDspMath::exp(-d2 / 2) / (pow(2 * PI_VALUE, (m / 2)) * sqrtDetSi);
	}
}


/// <summary>
/// Set matrix H with difference between track and plot
/// </summary>
void AssociationMatrix::SetTempH(Vector9d track_pos, Vector9d plot_pos, Matrix49d &temp_h)
{
	double x = track_pos.m_Data[0] - plot_pos.m_Data[0];		// x
	double y = track_pos.m_Data[1] - plot_pos.m_Data[1];		// y
	double z = track_pos.m_Data[2] - plot_pos.m_Data[2];		// z
	double vx = track_pos.m_Data[3] - plot_pos.m_Data[3];		// vx
	double vy = track_pos.m_Data[4] - plot_pos.m_Data[4];		// vy
	double vz = track_pos.m_Data[5] - plot_pos.m_Data[5];		// vz
	double ax = track_pos.m_Data[6] - plot_pos.m_Data[6];		// ax
	double ay = track_pos.m_Data[7] - plot_pos.m_Data[7];		// ay
	double az = track_pos.m_Data[8] - plot_pos.m_Data[8];		// az

	double r = SrvDspMath::sqrt(x*x + y * y + z * z);
	double rr = r * r;
	double rrr = r * r*r;
	double temp1 = x * x + y * y;		// x^2 + y^2
	double temp2 = SrvDspMath::sqrt(temp1);								// sqrt(x^2 + y^2)

	temp_h.Zero();
	temp_h.m_Data[0][0] = x / r;   //	= dR / dX
	temp_h.m_Data[0][1] = y / r;	//	= dR / dY
	temp_h.m_Data[0][2] = z / r;	//	= dR / dZ
	temp_h.m_Data[1][0] = -(y / temp1);  //	dAz / dX
	temp_h.m_Data[1][1] = x / temp1;  //	dAz / dY
	temp_h.m_Data[2][0] = (z * x) / (rr * temp2);  //  dEl / dX
	temp_h.m_Data[2][1] = (z * y) / (rr * temp2);  //  dEl / dY
	temp_h.m_Data[2][2] = -(temp2 / rr); //  dEl / dZ

	//TODO:
	// maybe lines 384 385 386 need to be in // 
	temp_h.m_Data[3][0] = (vx*(y*y + z * z) - x * (y*vy + z * vz)) / rrr;
	temp_h.m_Data[3][1] = (vy*(x*x + z * z) - y * (x*vx + z * vz)) / rrr;
	temp_h.m_Data[3][2] = (vz*(y*y + x * x) - z * (y*vy + x * vx)) / rrr;
	temp_h.m_Data[3][3] = x / r;
	temp_h.m_Data[3][4] = y / r;
	temp_h.m_Data[3][5] = z / r;
}
