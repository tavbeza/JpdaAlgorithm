#include "ExtendedKalman.h"

// test push
//test push 2
// test push 3


float ExtendedKalman::m_k = 5.0620f; // n/2 * log(4*PI) where n is the state dimention (x, y, x', y')

/// <summary>
/// Default kalman Constructor
/// </summary>
ExtendedKalman::ExtendedKalman() noexcept
{
	m_IsFirst = true;
	m_Entropy = 0;
}

/// <summary>
/// Kalman 3D constructor
/// </summary>
ExtendedKalman::ExtendedKalman(
	const float& dt,
	const float& x,
	const float& y,
	const float& z,
	const float& vx,
	const float& vy,
	const float& vz,
	const Matrix4d& R) noexcept
{
	m_Entropy = 0;
	m_last_prediction = Vector3d(x, y, z);
	m_last_speed = Vector3d(vx, vy, vz);
	//meas.ecef
	Vector9d x_predict;
	x_predict.m_Data[0] = x;
	x_predict.m_Data[1] = 0;
	x_predict.m_Data[2] = 0;
	x_predict.m_Data[3] = y;
	x_predict.m_Data[4] = 0;
	x_predict.m_Data[5] = 0;
	x_predict.m_Data[6] = z;
	x_predict.m_Data[7] = 0;
	x_predict.m_Data[8] = 0;
	
	TrakerParams *pTrakerParams;
	//Xsensor=[Plat.PosEcef(1) Plat.VelEcef(1) 0 Plat.PosEcef(2) Plat.VelEcef(2) 0 Plat.PosEcef(3) Plat.VelEcef(3) 0]';
	//Plat.PosEcef
	//Plat.VelEcef
	Vector9d x_sensor;
	x_sensor.m_Data[0] = 0;
	x_sensor.m_Data[1] = 0;
	x_sensor.m_Data[2] = 0;
	x_sensor.m_Data[3] = 0;
	x_sensor.m_Data[4] = 0;
	x_sensor.m_Data[5] = 0;
	x_sensor.m_Data[6] = 0;
	x_sensor.m_Data[7] = 0;
	x_sensor.m_Data[8] = 0;
	double r_dot = 0;
	SetH_Ecef(x_predict, x_sensor, m_H, r_dot);
	Vector3d cartesian;
	cartesian.m_Data[0] = x;
	cartesian.m_Data[1] = y;
	cartesian.m_Data[2] = z;
	Vector3d spherical;
	spherical.ToSpherical(cartesian);
	
	// convert x y z to r el az
	//double r = spherical.m_Data[0];
	//double el = spherical.m_Data[1];
	//double az = spherical.m_Data[2];
	
	/*
	Vector3d vs;
	vs.m_Data[0] = pTrakerParams->m_SigmaVxs;
	vs.m_Data[1] = pTrakerParams->m_SigmaVys;
	vs.m_Data[2] = pTrakerParams->m_SigmaVzs;

	Vector3d velEnu;
	//Plat.VelEnu
	//TODO: 50114 Initialize the platform velocity
	SetR_Ecef(r, az, el, vs,
				pTrakerParams->m_Sigma_r,
				pTrakerParams->m_Sigma_rdot,
				pTrakerParams->m_Sigma_az,
				pTrakerParams->m_Sigma_el,
				velEnu,
				m_R);
	SetR_Enu(0, r, az, el, vs,
				pTrakerParams->m_Sigma_r,
				pTrakerParams->m_Sigma_rdot,
				pTrakerParams->m_Sigma_az,
				pTrakerParams->m_Sigma_el,
				velEnu,
				m_R);
	*/
	//TODO: 50114 set Z
	Vector4d meas;
	//SetZ_Ecef(meas, m_Z);
	//TODO: 50114 called twice in algorithm code
	//SetR_Ecef();
	//INITIAL COVARIANCE MATRIX
	//P_vel=[Params.SigmaVxs^2         0                     0  ;
	//    0             Params.SigmaVys^2             0         ;
	//    0                     0             Params.SigmaVzs^2];
	//P_acc=[Params.SigmaAxs^2         0                     0  ;
	//    0             Params.SigmaAys^2             0         ;
	//    0                     0             Params.SigmaAzs^2];
	m_P.Zero();
	m_P.m_Data[0][0] = m_R.m_Data[0][0];
	m_P.m_Data[1][1] = pow(pTrakerParams->m_SigmaVxs, 2);
	m_P.m_Data[2][2] = pow(pTrakerParams->m_SigmaAxs, 2);
	m_P.m_Data[3][3] = m_R.m_Data[1][1];
	m_P.m_Data[4][4] = pow(pTrakerParams->m_SigmaVys, 2);
	m_P.m_Data[5][5] = pow(pTrakerParams->m_SigmaAys, 2);
	m_P.m_Data[6][6] = m_R.m_Data[2][2];
	m_P.m_Data[7][7] = pow(pTrakerParams->m_SigmaVzs, 2);
	m_P.m_Data[8][8] = pow(pTrakerParams->m_SigmaAzs, 2);
	//m_P.Print();
	//SET TRANSITION MATRIX
	//SetF(dt, pTrakerParams->m_TauAcc, m_F);
	//SetQ(dt, pTrakerParams->m_TauAcc, pTrakerParams->m_Amax, pTrakerParams->m_Pmax, pTrakerParams->m_P0, pTrakerParams->m_SigmaManeuver2, pTrakerParams->m_SigmaMnvrCalcMtd, m_Q);

	//process noise covariance matrix
	//m_Q = R,
	//NOISE EVOLUTION
	//m_G = MatrixXf(4, 2);

	//m_G << std::pow(dt, 2) / 2, 0,
	//    dt,     0,
	//    0,     std::pow(dt, 2) / 2,
	//    0,     dt;

	//STATE OBSERVATION MATRIX
	//m_H = MatrixXf(2, 4);
	//m_H << 1, 0, 0, 0,
	   //     0, 0, 1, 0;

	//GAIN    
	//m_K = MatrixXf(4, 2);

	//measurement noise covariance matrix
	m_R = R;

	m_IsFirst = true;
}


ExtendedKalman::~ExtendedKalman()
{
}

/// <summary>
/// Initialize the kalman
/// </summary>
void ExtendedKalman::Init(
	const float& dt,
	const float& x,
	const float& y,
	const float& z,
	const float& vx,
	const float& vy,
	const float& vz,
	const Matrix4d& R)
{
}

Vector3d ExtendedKalman::Predict(const double& dt)
{
	if (m_IsFirst)
	{
		m_X_Predict.m_Data[0] = m_last_prediction.m_Data[0];
		m_X_Predict.m_Data[1] = m_last_speed.m_Data[0];
		m_X_Predict.m_Data[2] = 0;
		m_X_Predict.m_Data[3] = m_last_prediction.m_Data[1];
		m_X_Predict.m_Data[4] = m_last_speed.m_Data[1];
		m_X_Predict.m_Data[5] = 0;
		m_X_Predict.m_Data[6] = m_last_prediction.m_Data[2];
		m_X_Predict.m_Data[7] = m_last_speed.m_Data[2];
		m_X_Predict.m_Data[8] = 0;
		m_IsFirst = false;
	}
	else
	{
		m_X_Predict = m_F * m_X;
	}
	//m_X.Print();
	//m_X_Predict.Print();
	//m_Q.Print();
	//m_P.Print();
	//Predicted (a priori) error covariance
	   //Covariance Matrix predicted error
	m_P_Predict = m_F * m_P * m_F.Transpose() + m_Q;
	//m_P_Predict.Print();

	//Predicted Measurement
	Vector4d zPredict = m_H * m_X_Predict;
	m_Z_Predict.m_Data[0] = zPredict.m_Data[0];
	m_Z_Predict.m_Data[3] = zPredict.m_Data[1];
	m_Z_Predict.m_Data[6] = zPredict.m_Data[2];


	//Compute Entropy
	m_Entropy = m_k + 0.5 * log10(m_P.Determinant());

	m_last_prediction_eigen = m_Z_Predict;
	m_last_prediction = Vector3d(m_Z_Predict.m_Data[0], m_Z_Predict.m_Data[1], m_Z_Predict.m_Data[2]);
	return m_last_prediction;
}

void ExtendedKalman::GainUpdate(const float& beta)
{
	// Innovation (or pre-fit residual) covariance
	// Error Measurement Covariance Matrix
	m_S = (m_H * (m_P_Predict * Transpose(m_H))) + m_R;
	// Near-optimal Kalman gain
	// Sets the optimal kalman gain
	// 94 = 99 * (94*44)
	// m_K = m_P *m_S'*m_H'
	Matrix94d temp1 = Transpose(m_H);
	m_K = m_P * (temp1 * m_S.Inverse());
	// 99 = 94 * (44 * 49)
	//m_K * m_S * m_K.Transpose()
	Matrix49d temp2 = Transpose(m_K);
	Matrix49d temp3 = m_S * temp2;
	Matrix9d kskt = m_K * temp3;
	m_P = m_P_Predict - kskt * (1 - beta);
}
