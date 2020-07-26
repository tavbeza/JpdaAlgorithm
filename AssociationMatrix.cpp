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
void AssociationMatrix::CheckAssociation(DataTrack &track, const DataPlot &plot, bool &isAsocFlagVec, double &g)
{
	//case 'ENU'
	//case 'nonlin'
	g = 0;
	double r, az, el, rr;
	
	// pos without Enu0 (without converts)
	double r1 = plot.GetRange();
	double az1 = plot.GetAzimuthAngle();
	double el1 = plot.GetElevationAngle();
	double v1 = plot.GetVelocity();

	double xPredict = track.m_pKalman->m_X_Predict.m_Data[0];
	double yPredict = track.m_pKalman->m_X_Predict.m_Data[1];
	double zPredict = track.m_pKalman->m_X_Predict.m_Data[2];
	double VxPredict= track.m_pKalman->m_X_Predict.m_Data[3];
	double VyPredict= track.m_pKalman->m_X_Predict.m_Data[4];
	double VzPredict= track.m_pKalman->m_X_Predict.m_Data[5];
	double rPredict = SrvDspMath::sqrt(SrvDspMath::pow(xPredict,2)+
										SrvDspMath::pow(yPredict, 2)+ 
										SrvDspMath::pow(zPredict, 2));
	double azPredict = SrvDspMath::atan(yPredict/xPredict);
	double elPredict = SrvDspMath::acos(zPredict/rPredict);
	double rrPredict = (xPredict*VxPredict + yPredict * VyPredict + zPredict * VzPredict) / rPredict;
	Vector4d z_Predict;
	z_Predict.m_Data[0] = rPredict;
	z_Predict.m_Data[1] = azPredict;
	z_Predict.m_Data[2] = elPredict;
	z_Predict.m_Data[3] = rrPredict;

	Vector9d plot_pos;
	plot_pos.m_Data[0] = r1 * SrvDspMath::sin(el1)*SrvDspMath::cos(az1);	// x
	plot_pos.m_Data[1] = r1 * SrvDspMath::sin(el1)*SrvDspMath::sin(az1);	// y
	plot_pos.m_Data[2] = r1 * SrvDspMath::cos(el1);							// z
	plot_pos.m_Data[3] = 0; // cartVelocity.m_Data[0];							// Vx
	plot_pos.m_Data[4] = 0; // cartVelocity.m_Data[1];							// Vy
	plot_pos.m_Data[5] = 0; // cartVelocity.m_Data[2];							// Vz
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

	Vector4d z;
	z.m_Data[0] = plot.GetRange();
	z.m_Data[1] = plot.GetAzimuthAngle();
	z.m_Data[2] = plot.GetElevationAngle();
	z.m_Data[3] = plot.GetVelocity();

	Vector4d resZ;
	resZ = z - z_Predict;
	resZ.m_Data[1] = Mod7(resZ.m_Data[1]);

	bool flagInGate = true;
	flagInGate &= (abs(resZ.m_Data[0] <= kgl * gateR.m_Data[0]));
	flagInGate &= (abs(resZ.m_Data[1] <= kgl * gateR.m_Data[1]));
	flagInGate &= (abs(resZ.m_Data[2] <= kgl * gateR.m_Data[2]));
	flagInGate &= (abs(resZ.m_Data[3] <= kgl * gateR.m_Data[3]));

	if (flagInGate)
	{
		// Update Track
		// case 'nonlin'
		isAsocFlagVec = true;
		new_S.m_Data[0][0] += sigma_r_plot;
		new_S.m_Data[1][1] += sigma_az_plot;
		new_S.m_Data[2][2] += sigma_el_plot;
		new_S.m_Data[3][3] += sigma_v_plot;

		Vector4d tempV = new_S.Inverse() * resZ;
		double d2 = (resZ * tempV);
		double m = 4;	// length(Z);
		double sqrtDetSi = SrvDspMath::sqrt(new_S.Determinant());
		double x1 = SrvDspMath::exp(-d2 / 2);
		double x2 = (pow(2 * PI_VALUE, (m / 2)) * sqrtDetSi);
		if (sqrtDetSi != 0)
		{
			g = x1 / x2;
		}
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
	double ro = x * x + y * y;		// x^2 + y^2
	double sqrtro = SrvDspMath::sqrt(ro);								// sqrt(x^2 + y^2)
	double rv = x * vx + y * vy + z * vz;

	temp_h.Zero();

	if (r != 0)
	{
		temp_h.m_Data[0][0] = x / r;   //	= dR / dX
		temp_h.m_Data[0][1] = y / r;	//	= dR / dY
		temp_h.m_Data[0][2] = z / r;	//	= dR / dZ
	}
	if (ro != 0)
	{
		temp_h.m_Data[1][0] = -(y / ro);  //	dAz / dX	// ?
		temp_h.m_Data[1][1] = x / ro;  //	dAz / dY
		temp_h.m_Data[2][0] = (z * x) / (rr * sqrtro);  //  dEl / dX
		temp_h.m_Data[2][1] = (z * y) / (rr * sqrtro);  //  dEl / dY
		temp_h.m_Data[2][2] = -(sqrtro / rr); //  dEl / dZ
	}
	

	//TODO:
	// maybe lines 384 385 386 need to be in // 
	temp_h.m_Data[3][0] = (vx*(y*y + z * z) - x * (y*vy + z * vz)) / rrr;
	temp_h.m_Data[3][1] = (vy*(x*x + z * z) - y * (x*vx + z * vz)) / rrr;
	temp_h.m_Data[3][2] = (vz*(y*y + x * x) - z * (y*vy + x * vx)) / rrr;
	temp_h.m_Data[3][3] = x / r;
	temp_h.m_Data[3][4] = y / r;
	temp_h.m_Data[3][5] = z / r;
}
