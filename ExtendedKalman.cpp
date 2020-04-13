#include "ExtendedKalman.h"

/// <summary>
/// Default kalman Constructor
/// </summary>
ExtendedKalman::ExtendedKalman() noexcept
{
}

/// <summary>
/// Kalman 3D constructor
/// </summary>
ExtendedKalman::ExtendedKalman(const DataPlot &plot)
{
	Vector3d spherical;
	spherical.m_Data[0] = plot.GetRange();
	spherical.m_Data[1] = plot.GetAzimuthAngle();
	spherical.m_Data[2] = plot.GetElevationAngle();

	Vector3d errorSpherical;
	errorSpherical.m_Data[0] = plot.GetRangeAccuracy();
	errorSpherical.m_Data[1] = plot.GetAzimuthAccuracy();
	errorSpherical.m_Data[2] = plot.GetElevationAccuracy();

	//Vector3d errorCartesian;
	//errorCartesian.ErrorSphericalToCart(spherical, errorSpherical);

	SetR(errorSpherical.m_Data[0], errorSpherical.m_Data[1], errorSpherical.m_Data[2], plot.GetVelocityAccuracy());

	float dt = 93.0 / 1000;
	SetDt(dt);

	Vector3d cartesian;
	cartesian.SphericalToCart(spherical);

	SetP(spherical);
	m_I.Identity();

	Init(
		cartesian		// (x, y, z)
	);
}

/// <summary>
/// Init Extended Kalman Filter
/// </summary>
void ExtendedKalman::Init(Vector3d cartesian) noexcept
{
	m_last_prediction = cartesian;
	m_last_speed = 0;

	m_X.m_Data[0] = cartesian.m_Data[0];
	m_X.m_Data[1] = cartesian.m_Data[1];
	m_X.m_Data[2] = cartesian.m_Data[2];
	m_X.m_Data[3] = 0;
	m_X.m_Data[4] = 0;
	m_X.m_Data[5] = 0;
	m_X.m_Data[6] = 0;
	m_X.m_Data[7] = 0;
	m_X.m_Data[8] = 0;

	m_X_Predict = m_X;

	SetH();
	SetF();
	SetQ();
}

/// <summary>
/// Virtual kalman destructor
/// </summary>
ExtendedKalman::~ExtendedKalman()
{

}

/// <summary>
/// The algorithm works in a two-step process. 
/// In the prediction step, the Kalman filter produces estimates of the current state variables, along with their uncertainties. 
/// Once the outcome of the next measurement (necessarily corrupted with some amount of error, including random noise) is observed, 
/// these estimates are updated using a weighted average, with more weight being given to estimates with higher certainty.
/// </summary>
Vector3d ExtendedKalman::Predict()
{

	m_X_Predict = m_F * m_X;
	
	m_P_Predict = m_F * m_P * m_F.Transpose() + m_Q;

	//Predicted Measurement
	//Vector4d zPredict = m_H * m_X_Predict;	// H * F * X

	Vector3d cartesian;
	cartesian.m_Data[0] = m_X_Predict.m_Data[0];
	cartesian.m_Data[1] = m_X_Predict.m_Data[1];
	cartesian.m_Data[2] = m_X_Predict.m_Data[2];

	Vector3d spherical;
	spherical.CartToSpherical(cartesian);

	m_last_speed = (m_X_Predict.m_Data[0] * m_X_Predict.m_Data[3]	// r_dot_predict
					+ m_X_Predict.m_Data[1] * m_X_Predict.m_Data[4]
					+ m_X_Predict.m_Data[2] * m_X_Predict.m_Data[5]) 
					/ spherical.m_Data[0];

	m_last_prediction = Vector3d(spherical.m_Data[0], spherical.m_Data[1], spherical.m_Data[2]);
	return m_last_prediction;
}

/// <summary>
/// The algorithm works in a two-step process. 
/// In the prediction step, the Kalman filter produces estimates of the current state variables, along with their uncertainties. 
/// Once the outcome of the next measurement (necessarily corrupted with some amount of error, including random noise) is observed, 
// these estimates are updated using a weighted average, with more weight being given to estimates with higher certainty.
/// </summary>
void ExtendedKalman::Update(DataPlot* pPlot)
{
	// Innovation (or pre-fit residual) covariance
	// Error Measurement Covariance Matrix
	SetH();

	SetR(pPlot->GetRangeAccuracy(), pPlot->GetAzimuthAccuracy(), pPlot->GetElevationAccuracy(), pPlot->GetVelocityAccuracy());

	Matrix94d H_Transpose;
	H_Transpose = Transpose(m_H);

	m_S = (m_H * (m_P_Predict * H_Transpose)) + m_R;
	// Near-optimal Kalman gain
	// Sets the optimal kalman gain
	// 93 = 99 * (93*33)
	// m_K = m_P *m_S'*m_H'

	m_K = m_P_Predict * (H_Transpose * m_S.Inverse());

	Vector4d z_predict;
	z_predict.m_Data[0] = m_last_prediction.m_Data[0];
	z_predict.m_Data[1] = m_last_prediction.m_Data[1];
	z_predict.m_Data[2] = m_last_prediction.m_Data[2];
	z_predict.m_Data[3] = m_last_speed;


	// Updated state estimate 
	Vector4d Zk;
	Zk.m_Data[0] = pPlot->GetRange();
	Zk.m_Data[1] = pPlot->GetAzimuthAngle();
	Zk.m_Data[2] = pPlot->GetElevationAngle();
	Zk.m_Data[3] = pPlot->GetVelocity();

	Vector4d y = Zk - z_predict;

	// TODO: Should be zPredict instead m_X_Predict ?
	m_X = m_X_Predict + (m_K * y);
		
	// m_P_Predict = P(k,k-1)
	// TODO: Ask Israel

	m_P = (m_I - m_K * m_H) * m_P_Predict;

	//m_P = m_P_Predict - (m_K * (m_S * Transpose(m_K)));
	//m_P = I * m_P_Predict - m_K * m_H * m_P_Predict;
}

/// <summary>
/// Purpose:          Set process noise covariance matrix Q
/// algorithm:        Based on Singer model
/// Input:                                 (1) Dt - time interval [sec]
///                                        (2) tau - Assumed maneuver time constant [1X3] for each dimension [sec]
///                                        (3) SigmaM - Assumed maneuver time
///                                        standard deviation [1X3] for each
///                                        dimension [sec]
///                                        (4) Amax - Maximal target
///                                        accelaeration with probability
///                                        Pmax [1X3] for each dimension
///                                        (5) Pmax - Acceleration
///                                        probability [1X3] for each dimension
///                                        (6) P0 - Probability that the
///                                        target has no acceleration [1X3] for each dimension
///
/// for i=1:3
///     switch SigmaMnvrCalcMtd
///         case 'Direct'
///             SigMan2(i)=SigmaManeuver2(i);               /// Maneuver variance
///         case 'AccProb'
///             SigMan2(i)=Amax(i)^2/3*(1+4*Pmax(i)-P0(i)); /// Maneuver variance
///     end
/// q{i}=SetQ_Singer(Dt,tau(i),SigMan2(i));
/// end
/// z3=zeros(3);
/// Q=[q{1} z3 z3;
///     z3 q{2} z3;
///     z3 z3 q{3}];
/// </summary>
void ExtendedKalman::SetQ()
{
	m_Q.Zero();
	/*TrackerParams *pTrakerParams = new TrackerParams;
	Matrix3d temp[3];
	Vector3d sigMan2;
	for (int i = 0; i < 3; i++)
	{
		if (0 == pTrakerParams->m_SigmaMnvrCalcMtd)
		{
			//Direct
			//Maneuver variance
			sigMan2.m_Data[i] = pTrakerParams->m_SigmaManeuver2.m_Data[i];
		}
		else
		{
			//AccProb
			//Maneuver variance
			sigMan2.m_Data[i] = pow(pTrakerParams->m_Amax.m_Data[i], 2) / 3 * (1 + 4 * pTrakerParams->m_Pmax.m_Data[i] - pTrakerParams->m_P0.m_Data[i]);
		}
		SetQ_Singer(pTrakerParams->m_TauAcc.m_Data[i], sigMan2.m_Data[i], temp[i]);
	}
	m_Q.Zero();
	m_Q.m_Data[0][0] = temp[0].m_Data[0][0];
	m_Q.m_Data[0][1] = temp[0].m_Data[0][1];
	m_Q.m_Data[0][2] = temp[0].m_Data[0][2];
	m_Q.m_Data[1][0] = temp[0].m_Data[1][0];
	m_Q.m_Data[1][1] = temp[0].m_Data[1][1];
	m_Q.m_Data[1][2] = temp[0].m_Data[1][2];
	m_Q.m_Data[2][0] = temp[0].m_Data[2][0];
	m_Q.m_Data[2][1] = temp[0].m_Data[2][1];
	m_Q.m_Data[2][2] = temp[0].m_Data[2][2];
	m_Q.m_Data[3][3] = temp[1].m_Data[0][0];
	m_Q.m_Data[3][4] = temp[1].m_Data[0][1];
	m_Q.m_Data[3][5] = temp[1].m_Data[0][2];
	m_Q.m_Data[4][3] = temp[1].m_Data[1][0];
	m_Q.m_Data[4][4] = temp[1].m_Data[1][1];
	m_Q.m_Data[4][5] = temp[1].m_Data[1][2];
	m_Q.m_Data[5][3] = temp[1].m_Data[2][0];
	m_Q.m_Data[5][4] = temp[1].m_Data[2][1];
	m_Q.m_Data[5][5] = temp[1].m_Data[2][2];
	m_Q.m_Data[6][6] = temp[2].m_Data[0][0];
	m_Q.m_Data[6][7] = temp[2].m_Data[0][1];
	m_Q.m_Data[6][8] = temp[2].m_Data[0][2];
	m_Q.m_Data[7][6] = temp[2].m_Data[1][0];
	m_Q.m_Data[7][7] = temp[2].m_Data[1][1];
	m_Q.m_Data[7][8] = temp[2].m_Data[1][2];
	m_Q.m_Data[8][6] = temp[2].m_Data[2][0];
	m_Q.m_Data[8][7] = temp[2].m_Data[2][1];
	m_Q.m_Data[8][8] = temp[2].m_Data[2][2];
	*/
}

/// <summary>
/// Set Singer Model Process Noise Covariance matrix for state vector:[x Vx Ax]';
/// </summary>
void ExtendedKalman::SetQ_Singer(double tau, double sigmaManeuver2, Matrix3d &q)
{
	q.Zero();
	double beta = 1 / tau;
	double rho = SrvDspMath::exp(-beta * m_Dt);
	double gama = beta * m_Dt;
	q.m_Data[0][0] = (1 - SrvDspMath::exp(-2 * gama) + 2 * gama + 2 * pow(gama, 3) / 3 - 2 * pow(gama, 2) - 4 * gama*SrvDspMath::exp(-gama)) / (2 * pow(beta, 5));
	q.m_Data[0][1] = (SrvDspMath::exp(-2 * gama) + 1 - 2 * SrvDspMath::exp(-gama) + 2 * gama*SrvDspMath::exp(-gama) - 2 * gama + pow(gama, 2)) / (2 * pow(beta, 4));
	q.m_Data[0][2] = (1 - SrvDspMath::exp(-2 * gama) - 2 * gama*SrvDspMath::exp(-gama)) / (2 * pow(beta, 3));
	q.m_Data[1][0] = q.m_Data[0][1];
	q.m_Data[1][1] = (4 * SrvDspMath::exp(-gama) - 3 - SrvDspMath::exp(-2 * gama) + 2 * gama) / (2 * pow(beta, 3));
	q.m_Data[1][2] = (1 + SrvDspMath::exp(-2 * gama) - 2 * SrvDspMath::exp(-gama)) / (2 * pow(beta, 2));
	q.m_Data[2][0] = q.m_Data[0][2];
	q.m_Data[2][1] = q.m_Data[1][2];
	q.m_Data[2][2] = (1 - SrvDspMath::exp(-2 * gama)) / (2 * beta);
	q = q * 2 * sigmaManeuver2*beta;
}

/// <summary>
/// Set the matrix F
/// </summary>
void ExtendedKalman::SetF_Singer(double tau,
	Matrix3d &f)
{
	f.Zero();
	double beta = 1 / tau;
	double rho = SrvDspMath::exp(-beta * m_Dt);
	f.m_Data[0][0] = 1;
	f.m_Data[0][1] = (double) m_Dt;
	f.m_Data[0][2] = 1 / SrvDspMath::pow(beta, 2)*(-1 + beta * m_Dt + rho);
	f.m_Data[1][1] = 1;
	f.m_Data[1][2] = 1 / beta * (1 - rho);
	f.m_Data[2][2] = rho;
}

/// <summary>
/// Set the matrix F
/// </summary>
void ExtendedKalman::SetF()
{
	m_F.Zero();
	m_F.m_Data[0][0] = 1;
	m_F.m_Data[1][1] = 1;
	m_F.m_Data[2][2] = 1;
	m_F.m_Data[3][3] = 1;
	m_F.m_Data[4][4] = 1;
	m_F.m_Data[5][5] = 1;

	m_F.m_Data[0][3] = m_Dt;
	m_F.m_Data[1][4] = m_Dt;
	m_F.m_Data[2][5] = m_Dt;

	/*
	TrackerParams *pTrakerParams = new TrackerParams;
	Matrix3d fx, fy, fz;
	SetF_Singer(pTrakerParams->m_TauAcc.m_Data[0], fx);
	SetF_Singer(pTrakerParams->m_TauAcc.m_Data[1], fy);
	SetF_Singer(pTrakerParams->m_TauAcc.m_Data[2], fz);
	// TODO:
	m_F.m_Data[0][0] = fx.m_Data[0][0];
	m_F.m_Data[0][1] = fx.m_Data[0][1];
	m_F.m_Data[0][2] = 0;// fx.m_Data[0][2];
	m_F.m_Data[1][0] = fx.m_Data[1][0];
	m_F.m_Data[1][1] = fx.m_Data[1][1];
	m_F.m_Data[1][2] = 0;//fx.m_Data[1][2];
	m_F.m_Data[2][0] = fx.m_Data[2][0];
	m_F.m_Data[2][1] = fx.m_Data[2][1];
	m_F.m_Data[2][2] = 0;//fx.m_Data[2][2];
	m_F.m_Data[3][3] = fy.m_Data[0][0];
	m_F.m_Data[3][4] = fy.m_Data[0][1];
	m_F.m_Data[3][5] = 0;//fy.m_Data[0][2];
	m_F.m_Data[4][3] = fy.m_Data[1][0];
	m_F.m_Data[4][4] = fy.m_Data[1][1];
	m_F.m_Data[4][5] = 0;//fy.m_Data[1][2];
	m_F.m_Data[5][3] = fy.m_Data[2][0];
	m_F.m_Data[5][4] = fy.m_Data[2][1];
	m_F.m_Data[5][5] = 0;//fy.m_Data[2][2];
	m_F.m_Data[6][6] = fz.m_Data[0][0];
	m_F.m_Data[6][7] = fz.m_Data[0][1];
	m_F.m_Data[6][8] = 0;//fz.m_Data[0][2];
	m_F.m_Data[7][6] = fz.m_Data[1][0];
	m_F.m_Data[7][7] = fz.m_Data[1][1];
	m_F.m_Data[7][8] = 0;//fz.m_Data[1][2];
	m_F.m_Data[8][6] = fz.m_Data[2][0];
	m_F.m_Data[8][7] = fz.m_Data[2][1];
	m_F.m_Data[8][8] = 0;//fz.m_Data[2][2];
	*/
}

/// <summary>
/// Set measurement matrix H
/// </summary>
void ExtendedKalman::SetH()
{
	double x = m_X_Predict.m_Data[0];	// x
	double y = m_X_Predict.m_Data[1];	// y
	double z = m_X_Predict.m_Data[2];	// z
	double vx = m_X_Predict.m_Data[3];	// vx
	double vy = m_X_Predict.m_Data[4];	// vy
	double vz = m_X_Predict.m_Data[5];	// vz
	// Difference between state vector of target prediction to self
	double R = SrvDspMath::sqrt(x*x + y*y + z*z);
	m_H.Zero();
	m_H.m_Data[0][0] = x / R;  //	= dR / dX
	m_H.m_Data[0][1] = y / R;	//	= dR / dY
	m_H.m_Data[0][2] = z / R;	//	= dR / dZ
	m_H.m_Data[1][0] = - ( y / (SrvDspMath::pow(y, 2) + SrvDspMath::pow(x, 2)));  //	dAz / dX
	m_H.m_Data[1][1] = x / (SrvDspMath::pow(y, 2) + SrvDspMath::pow(x, 2));  //	dAz / dY
	m_H.m_Data[2][0] = (z * x) / (SrvDspMath::pow(R, 2) * SrvDspMath::sqrt(x*x + y*y));  //  dEl / dX
	m_H.m_Data[2][1] = (z * y) / (SrvDspMath::pow(R, 2) * SrvDspMath::sqrt(x*x + y*y));  //  dEl / dY
	m_H.m_Data[2][2] = -SrvDspMath::sqrt(x*x + y*y) / SrvDspMath::pow(R, 2); //  dEl / dZ
	m_H.m_Data[4][0] = (vx * (SrvDspMath::pow(y, 2) + SrvDspMath::pow(z, 2)) - x * (y*vy + z * vz)) / SrvDspMath::pow(R, 3);
	m_H.m_Data[4][1] = (vy * (SrvDspMath::pow(x, 2) + SrvDspMath::pow(z, 2)) - y * (x*vx + z * vz)) / SrvDspMath::pow(R, 3);
	m_H.m_Data[4][2] = (vz * (SrvDspMath::pow(y, 2) + SrvDspMath::pow(x, 2)) - z * (y*vy + x * vx)) / SrvDspMath::pow(R, 3);
	m_H.m_Data[4][3] = x / R;
	m_H.m_Data[4][4] = y / R;
	m_H.m_Data[4][5] = z / R;
}

/// <summary>
/// Set measurement covariance matrix
/// </summary>
void ExtendedKalman::SetR(double error_x,
	double error_y,
	double error_z,
	double error_v)
{
	// TODO: Change to 3x3 without v and change like the research
		m_R.Zero();
		m_R.m_Data[0][0] = pow(error_x, 2);
		m_R.m_Data[1][1] = pow(error_y, 2);
		m_R.m_Data[2][2] = pow(error_z, 2);
		m_R.m_Data[3][3] = pow(error_v, 2);
}

/// <summary>
/// Set covariance matrix P
/// </summary>
void ExtendedKalman::SetP(Vector3d spherical)
{
	Matrix4d jacobian;
	jacobian.Zero();
	jacobian.m_Data[0][0] = SrvDspMath::sin(spherical.m_Data[2]) * SrvDspMath::cos(spherical.m_Data[1]);
	jacobian.m_Data[0][1] = -spherical.m_Data[0] * SrvDspMath::sin(spherical.m_Data[2]) * SrvDspMath::sin(spherical.m_Data[1]);
	jacobian.m_Data[0][2] = spherical.m_Data[0] * SrvDspMath::cos(spherical.m_Data[2]) * SrvDspMath::cos(spherical.m_Data[1]);
	jacobian.m_Data[1][0] = SrvDspMath::sin(spherical.m_Data[2]) * SrvDspMath::sin(spherical.m_Data[1]);
	jacobian.m_Data[1][1] = spherical.m_Data[0] * SrvDspMath::sin(spherical.m_Data[2]) * SrvDspMath::cos(spherical.m_Data[1]);
	jacobian.m_Data[1][2] = spherical.m_Data[0] * SrvDspMath::cos(spherical.m_Data[2]) * SrvDspMath::sin(spherical.m_Data[1]);
	jacobian.m_Data[2][0] = SrvDspMath::cos(spherical.m_Data[2]);
	//jacobian.m_Data[2][1] = 0;
	jacobian.m_Data[2][2] = -spherical.m_Data[0] * SrvDspMath::sin(spherical.m_Data[2]);

	Matrix4d jacobianT = Transpose(jacobian);

	Matrix4d p_location = jacobian * m_R * jacobianT;

	TrackerParams *pTrakerParams = new TrackerParams;
	m_P.Zero();
	m_P.m_Data[0][0] = p_location.m_Data[0][0];
	m_P.m_Data[0][1] = p_location.m_Data[0][1];
	m_P.m_Data[0][2] = p_location.m_Data[0][2];
	m_P.m_Data[1][0] = p_location.m_Data[1][0];
	m_P.m_Data[1][1] = p_location.m_Data[1][1];
	m_P.m_Data[1][2] = p_location.m_Data[1][2];
	m_P.m_Data[2][0] = p_location.m_Data[2][0];
	m_P.m_Data[2][1] = p_location.m_Data[2][1];
	m_P.m_Data[2][2] = p_location.m_Data[2][2];
	// v
	m_P.m_Data[3][3] = pow(pTrakerParams->m_SigmaVxs, 2);
	m_P.m_Data[4][4] = pow(pTrakerParams->m_SigmaVys, 2);
	m_P.m_Data[5][5] = pow(pTrakerParams->m_SigmaVzs, 2);
	// a
	m_P.m_Data[6][6] = pow(pTrakerParams->m_SigmaAxs, 2);
	m_P.m_Data[7][7] = pow(pTrakerParams->m_SigmaAys, 2);
	m_P.m_Data[8][8] = pow(pTrakerParams->m_SigmaAzs, 2);
}

/// <summary>
/// Sets the delta time in the matrix A (Evolution state matrix)
/// </summary>
void ExtendedKalman::SetDt(float dt)
{
	m_Dt = dt;
}