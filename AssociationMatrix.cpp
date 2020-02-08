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
			if (m_TrkIds[i] == trkID)
			{
				idxTrk = i;
				break;
			}
		}
		if (-1 == idxTrk)
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
	const NavPlatStatusStruct &platData,
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

	//Z=[Meas.enu_0.R; Meas.enu_0.Az; Meas.enu_0.El;  Meas.enu_0.RR];
	Vector4d z;
	z.m_Data[0] = r1;
	z.m_Data[1] = az1;
	z.m_Data[2] = el1;
	z.m_Data[3] = v1;
		
	// GateType='Rect'
	// case 'nonlin'
	// Rectangular gating coefficient
	TrackerParams *pTrackerParams = new TrackerParams();
	double kgl = pTrackerParams->m_Kgl; // m_Kgl = 1
	Vector4d gateR;
	gateR.m_Data[0] = SrvDspMath::sqrt(track.m_pKalman->m_S.m_Data[0][0]);
	gateR.m_Data[1] = SrvDspMath::sqrt(track.m_pKalman->m_S.m_Data[1][1]);
	gateR.m_Data[2] = SrvDspMath::sqrt(track.m_pKalman->m_S.m_Data[2][2]);
	gateR.m_Data[3] = SrvDspMath::sqrt(track.m_pKalman->m_S.m_Data[3][3]);
		
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

		//Vector4d y = z - zPredict;
		Vector4d y = z;

		// Calculate S
		// m_S = m_H*m_P*m_H' + m_R
		//track.m_pKalman.
		Matrix94d temp1 = track.m_pKalman->m_P_Predict * Transpose(track.m_pKalman->m_H);
		track.m_pKalman->m_S = track.m_pKalman->m_H * temp1 + track.m_pKalman->m_R;
		//track.m_KF.m_S.Print();

		Vector4d tempV = track.m_pKalman->m_S * y;
		double d2 = (y * tempV) / y.Norm();
		double m = 4;//length(Z);
		double sqrtDetSi = SrvDspMath::sqrt(track.m_pKalman->m_S.Determinant());
		g = SrvDspMath::exp(-d2 / 2) / (pow(2 * PI_VALUE, (m / 2)) * sqrtDetSi);
	}
}

