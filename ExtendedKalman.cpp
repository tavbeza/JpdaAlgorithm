#include "ExtendedKalman.h"
//#include "ns.h"

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
ExtendedKalman::ExtendedKalman(const DataPlot &plot)
{
	TrackerParams *pTrakerParams = new TrackerParams;
	Vector3d vs;
	vs.m_Data[0] = pTrakerParams->m_SigmaVxs;
	vs.m_Data[1] = pTrakerParams->m_SigmaVys;
	vs.m_Data[2] = pTrakerParams->m_SigmaVzs;
	
	SetR_Enu(2,
		plot.GetRange(),
		plot.GetAzimuthAngle(),
		plot.GetElevationAngle(),
		vs,
		plot.GetRangeAccuracy(),
		plot.GetVelocityAccuracy(),
		plot.GetAzimuthAccuracy(),
		plot.GetElevationAccuracy(),
		/*platData.velENU,*/
		m_R);

	float dt = 93.0 / 1000;

	Init(
		dt,
		cos(plot.GetAzimuthAngle())*plot.GetRange(),		// x
		sin(plot.GetAzimuthAngle())*plot.GetRange(),		// y
		0,													// z
		cos(plot.GetAzimuthAngle())*plot.GetVelocity(),		// vx
		sin(plot.GetAzimuthAngle())*plot.GetVelocity(),		// vy
		0
	);
}

/// <summary>
/// Init Extended Kalman Filter
/// </summary>
void ExtendedKalman::Init(
	const float& dt,
	const float& x,
	const float& y,
	const float& z,
	const float& vx,
	const float& vy,
	const float& vz) noexcept
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
	
	TrackerParams *pTrakerParams = new TrackerParams;
	
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
	
	Vector4d meas;
	
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
	//m_R = R;

	m_IsFirst = true;
	SetF(dt, m_F);
	SetQ(dt, m_Q);
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
	//m_Entropy = m_k + 0.5 * log10(m_P.Determinant());

	m_last_prediction_eigen = m_Z_Predict;
	m_last_prediction = Vector3d(m_Z_Predict.m_Data[0], m_Z_Predict.m_Data[1], m_Z_Predict.m_Data[2]);
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
	m_S = (m_H * (m_P_Predict * Transpose(m_H))) + m_R;
	// Near-optimal Kalman gain
	// Sets the optimal kalman gain
	// 94 = 99 * (94*44)
	// m_K = m_P *m_S'*m_H'
	Matrix94d temp1 = Transpose(m_H);
	m_K = m_P * (temp1 * m_S.Inverse());
	// Updated state estimate 
	Vector4d Zk;
	Zk.m_Data[0] = pPlot->GetRange();
	Zk.m_Data[1] = pPlot->GetAzimuthAngle();
	Zk.m_Data[2] = pPlot->GetElevationAngle();
	Zk.m_Data[3] = pPlot->GetVelocity();
	Vector4d zPredict = m_H * m_X_Predict; // To check the result zPredict = 0
	
	// (Zk - zPredict) = Yk
	// m_Z_Predict = X(k,k-1)

	// TODO: we changed m_Z_Predict to m_X_Predict because the velocity
	m_X = m_X_Predict + (m_K * (Zk)); //m_X = m_Z_Predict + (m_K * (Zk - zPredict));
	
	// 99 = 94 * (44 * 49)
	//m_K * m_S * m_K.Transpose()
	Matrix49d temp2 = Transpose(m_K);
	Matrix49d temp3 = m_S * temp2;
	Matrix9d kskt = m_K * temp3; // kskt =  m_K * m_S * Transpose(m_K)
	
	// m_P_Predict = P(k,k-1)
	m_P = m_P_Predict - kskt * (1 - 0); // m_P = P(k,k-1) - ( m_K * m_S * Transpose(m_K) ) * ( 1-0 )
}

/// <summary>
/// The relative certainty of the measurements and current state estimate is an important consideration, 
/// and it is common to discuss the response of the filter in terms of the Kalman filter's gain. 
/// The Kalman gain is the relative weight given to the measurements and current state estimate, 
/// and can be "tuned" to achieve particular performance. 
/// With a high gain, the filter places more weight on the most recent measurements, and thus follows them more responsively. 
/// With a low gain, the filter follows the model predictions more closely. 
/// At the extremes, a high gain close to one will result in a more jumpy estimated trajectory, 
/// while low gain close to zero will smooth out noise but decrease the responsiveness.
/// </summary>
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
void ExtendedKalman::SetQ(double Dt,
	Matrix9d &Q)
{
	TrackerParams *pTrakerParams;
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
		SetQ_Singer(Dt, pTrakerParams->m_TauAcc.m_Data[i], sigMan2.m_Data[i], temp[i]);
	}
	Q.Zero();
	Q.m_Data[0][0] = temp[0].m_Data[0][0];
	Q.m_Data[0][1] = temp[0].m_Data[0][1];
	Q.m_Data[0][2] = temp[0].m_Data[0][2];
	Q.m_Data[1][0] = temp[0].m_Data[1][0];
	Q.m_Data[1][1] = temp[0].m_Data[1][1];
	Q.m_Data[1][2] = temp[0].m_Data[1][2];
	Q.m_Data[2][0] = temp[0].m_Data[2][0];
	Q.m_Data[2][1] = temp[0].m_Data[2][1];
	Q.m_Data[2][2] = temp[0].m_Data[2][2];
	Q.m_Data[3][3] = temp[1].m_Data[0][0];
	Q.m_Data[3][4] = temp[1].m_Data[0][1];
	Q.m_Data[3][5] = temp[1].m_Data[0][2];
	Q.m_Data[4][3] = temp[1].m_Data[1][0];
	Q.m_Data[4][4] = temp[1].m_Data[1][1];
	Q.m_Data[4][5] = temp[1].m_Data[1][2];
	Q.m_Data[5][3] = temp[1].m_Data[2][0];
	Q.m_Data[5][4] = temp[1].m_Data[2][1];
	Q.m_Data[5][5] = temp[1].m_Data[2][2];
	Q.m_Data[6][6] = temp[2].m_Data[0][0];
	Q.m_Data[6][7] = temp[2].m_Data[0][1];
	Q.m_Data[6][8] = temp[2].m_Data[0][2];
	Q.m_Data[7][6] = temp[2].m_Data[1][0];
	Q.m_Data[7][7] = temp[2].m_Data[1][1];
	Q.m_Data[7][8] = temp[2].m_Data[1][2];
	Q.m_Data[8][6] = temp[2].m_Data[2][0];
	Q.m_Data[8][7] = temp[2].m_Data[2][1];
	Q.m_Data[8][8] = temp[2].m_Data[2][2];
}

/// <summary>
/// Set Singer Model Process Noise Covariance matrix for state vector:[x Vx Ax]';
/// </summary>
void ExtendedKalman::SetQ_Singer(double dt, double tau, double sigmaManeuver2, Matrix3d &q)
{
	q.Zero();
	double beta = 1 / tau;
	double rho = SrvDspMath::exp(-beta * dt);
	double gama = beta * dt;
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
void ExtendedKalman::SetF_Singer(double dt,
	double tau,
	Matrix3d &f)
{
	f.Zero();
	double beta = 1 / tau;
	double rho = SrvDspMath::exp(-beta * dt);
	f.m_Data[0][0] = 1;
	f.m_Data[0][1] = (double)dt;
	f.m_Data[0][2] = 1 / SrvDspMath::pow(beta, 2)*(-1 + beta * dt + rho);
	f.m_Data[1][1] = 1;
	f.m_Data[1][2] = 1 / beta * (1 - rho);
	f.m_Data[2][2] = rho;
}

/// <summary>
/// Set the matrix F
/// </summary>
void ExtendedKalman::SetF(double dt,
	Matrix9d &F)
{
	TrackerParams *pTrakerParams = new TrackerParams;
	F.Zero();
	Matrix3d fx, fy, fz;
	SetF_Singer(dt, pTrakerParams->m_TauAcc.m_Data[0], fx);
	SetF_Singer(dt, pTrakerParams->m_TauAcc.m_Data[1], fy);
	SetF_Singer(dt, pTrakerParams->m_TauAcc.m_Data[2], fz);

	F.m_Data[0][0] = fx.m_Data[0][0];
	F.m_Data[0][1] = fx.m_Data[0][1];
	F.m_Data[0][2] = fx.m_Data[0][2];
	F.m_Data[1][0] = fx.m_Data[1][0];
	F.m_Data[1][1] = fx.m_Data[1][1];
	F.m_Data[1][2] = fx.m_Data[1][2];
	F.m_Data[2][0] = fx.m_Data[2][0];
	F.m_Data[2][1] = fx.m_Data[2][1];
	F.m_Data[2][2] = fx.m_Data[2][2];
	F.m_Data[3][3] = fy.m_Data[0][0];
	F.m_Data[3][4] = fy.m_Data[0][1];
	F.m_Data[3][5] = fy.m_Data[0][2];
	F.m_Data[4][3] = fy.m_Data[1][0];
	F.m_Data[4][4] = fy.m_Data[1][1];
	F.m_Data[4][5] = fy.m_Data[1][2];
	F.m_Data[5][3] = fy.m_Data[2][0];
	F.m_Data[5][4] = fy.m_Data[2][1];
	F.m_Data[5][5] = fy.m_Data[2][2];
	F.m_Data[6][6] = fz.m_Data[0][0];
	F.m_Data[6][7] = fz.m_Data[0][1];
	F.m_Data[6][8] = fz.m_Data[0][2];
	F.m_Data[7][6] = fz.m_Data[1][0];
	F.m_Data[7][7] = fz.m_Data[1][1];
	F.m_Data[7][8] = fz.m_Data[1][2];
	F.m_Data[8][6] = fz.m_Data[2][0];
	F.m_Data[8][7] = fz.m_Data[2][1];
	F.m_Data[8][8] = fz.m_Data[2][2];
}

/// <summary>
/// Set measurement matrix H in ECEF coordinate axes
/// </summary>
void ExtendedKalman::SetH_Ecef(Vector9d X_predict,
	Vector9d X_sensor,
	Matrix49d &H,
	double &R_dot)
{
	Vector9d DX = X_predict - X_sensor;
	double Dx = DX.m_Data[0];
	double Dy = DX.m_Data[3];
	double Dz = DX.m_Data[6];
	double Dvx = DX.m_Data[1];
	double Dvy = DX.m_Data[4];
	double Dvz = DX.m_Data[7];
	// Difference between state vector of target prediction to self
	double D = Dx * Dvx + Dy * Dvy + Dz * Dvz;
	double R = SrvDspMath::sqrt(Dx*Dx + Dy * Dy + Dz * Dz);
	R_dot = D / R;
	H.Zero();
	H.m_Data[0][0] = 1;
	H.m_Data[1][3] = 1;
	H.m_Data[2][6] = 1;
	H.m_Data[3][0] = (Dvx*R - D * Dx / R) / (SrvDspMath::pow(R, 2));
	H.m_Data[3][1] = Dx / R;
	H.m_Data[3][2] = 0;
	H.m_Data[3][3] = Dvy / R - Dy * D / (SrvDspMath::pow(R, 3));
	H.m_Data[3][4] = Dy / R;
	H.m_Data[3][5] = 0;
	H.m_Data[3][6] = Dvz / R - Dz * D / (SrvDspMath::pow(R, 3));
	H.m_Data[3][7] = Dz / R;
	H.m_Data[3][8] = 0;
}

/// <summary>
/// Set measurement matrix H in ENU coordinate axes
/// </summary>
void ExtendedKalman::SetH_Enu(int type,
	double az,
	double el,
	Vector3d pos_t,
	Vector3d pos_s,
	Vector3d vel_t,
	Vector3d vel_s,
	Matrix49d &H)
{
	H.Zero();
	H.m_Data[0][0] = 1;
	H.m_Data[1][3] = 1;
	H.m_Data[2][6] = 1;
	/*
		switch (type)
		{
			case 0://lin
			{
				H.m_Data[3][1] = SrvDspMath::sin(az)*SrvDspMath::cos(el);
				H.m_Data[3][4] = SrvDspMath::cos(az)*SrvDspMath::cos(el);
				H.m_Data[3][7] = SrvDspMath::sin(el);
				break;
			}
			case 1: //ext
			{
				double xs = pos_s.m_Data[0];
				double ys = pos_s.m_Data[1];
				double zs = pos_s.m_Data[2];
				double xt = pos_t.m_Data[0];
				double yt = pos_t.m_Data[1];
				double zt = pos_t.m_Data[2];
				double vxs = vel_s.m_Data[0];
				double vys = vel_s.m_Data[1];
				double vzs = vel_s.m_Data[2];
				double vxt = vel_t.m_Data[0];
				double vyt = vel_t.m_Data[1];
				double vzt = vel_t.m_Data[2];
				double Dx = xt-xs;
				double Dy = yt-ys;
				double Dz = zt-zs;
				double Dvx = vxt-vxs;
				double Dvy = vyt-vys;
				double Dvz = vzt-vzs;
				double R = SrvDspMath::sqrt(Dx*Dx+Dy*Dy+Dz*Dz);
				double dRdx = Dx/R;
				double dRdy = Dy/R;
				double dRdz = Dz/R;
				double D = Dx*Dvx + Dy*Dvy + Dz*Dvz;
				H.m_Data[3][0] = (Dvx*R-D*dRdx)/pow(R,2);
				H.m_Data[3][1] = Dx/R;
				H.m_Data[3][3] = (Dvy*R-D*dRdy)/pow(R,2);
				H.m_Data[3][4] = Dy/R;
				H.m_Data[3][6] = (Dvz*R-D*dRdz)/pow(R,2);
				H.m_Data[3][7] = Dz/R;
				break;
			}
			case 2://nonlin
			{
	*/
	double xs = pos_s.m_Data[0];
	double ys = pos_s.m_Data[1];
	double zs = pos_s.m_Data[2];
	double xt = pos_t.m_Data[0];
	double yt = pos_t.m_Data[1];
	double zt = pos_t.m_Data[2];
	double vxs = vel_s.m_Data[0];
	double vys = vel_s.m_Data[1];
	double vzs = vel_s.m_Data[2];
	double vxt = vel_t.m_Data[0];
	double vyt = vel_t.m_Data[1];
	double vzt = vel_t.m_Data[2];
	double dx = xt - xs;
	double dy = yt - ys;
	double dz = zt - zs;
	double dvx = vxt - vxs;
	double dvy = vyt - vys;
	double dvz = vzt - vzs;
	Vector9d X;
	X.m_Data[0] = dx;
	X.m_Data[1] = dvx;
	X.m_Data[2] = 0;
	X.m_Data[3] = dy;
	X.m_Data[4] = dvy;
	X.m_Data[5] = 0;
	X.m_Data[6] = dz;
	X.m_Data[7] = dvz;
	X.m_Data[8] = 0;
	Calc_H_Matrix_Cart2Pol(X, H);
	/*            break;
			}
		}
	*/
	m_H = H;
}

/// <summary>
/// New function that calculates H matrix between Cartesian state vector to nonlinear measurement Z
/// </summary>
void ExtendedKalman::Calc_H_Matrix_Cart2Pol(Vector9d X, Matrix49d &H)
{
	H.Zero();
	double x = X.m_Data[0];
	double vx = X.m_Data[1];
	double y = X.m_Data[3];
	double vy = X.m_Data[4];
	double z = X.m_Data[6];
	double vz = X.m_Data[7];

	Vector3d vec1(x, y, z);
	double R = vec1.Norm();;
	double R2 = R * R;
	double R3 = R2 * R;
	double x2y2 = x * x + y * y;
	H.m_Data[0][0] = x / R;
	H.m_Data[0][3] = y / R;
	H.m_Data[0][6] = z / R;
	H.m_Data[1][0] = y / x2y2;
	H.m_Data[1][3] = -x / x2y2;
	H.m_Data[2][0] = -(x*z) / (SrvDspMath::sqrt(x2y2)*R2);
	H.m_Data[2][3] = -(y*z) / (SrvDspMath::sqrt(x2y2)*R2);
	H.m_Data[2][6] = SrvDspMath::sqrt(x2y2) / R2;
	H.m_Data[3][0] = vx / R - (x*(vx*x + vy * y + vz * z)) / R3;
	H.m_Data[3][1] = x / R;
	H.m_Data[3][3] = vy / R - (y*(vx*x + vy * y + vz * z)) / R3;
	H.m_Data[3][4] = y / R;
	H.m_Data[3][6] = vz / R - (z*(vx*x + vy * y + vz * z)) / R3;
	H.m_Data[3][7] = z / R;
}

/// <summary>
/// Set measurement covariance matrix in ECEF tracking axes
/// </summary>
void ExtendedKalman::SetR_Ecef(double r,
	double az,
	double el,
	Vector3d Vs,
	double Sigma_r,
	double Sigma_rdot,
	double Sigma_az,
	double Sigma_el,
	Vector3d SigmaV,
	Matrix4d &R)
{
	TrackerParams *pTrakerParams = new TrackerParams;
	Vector3d eulerSens;
	eulerSens.m_Data[0] = pTrakerParams->m_Sensor_Theta;
	eulerSens.m_Data[1] = pTrakerParams->m_Sensor_Psi;
	eulerSens.m_Data[2] = pTrakerParams->m_Sensor_Phi;
	double theta_sens = eulerSens.m_Data[0];
	double psi_sens = eulerSens.m_Data[1];
	double phi_sens = eulerSens.m_Data[2];
	Matrix3d Js;
	//TODO:
	//GeodeticConverter::InitJs1(r, az, el, Js);
	Matrix3d r_r;
	r_r.Zero();
	r_r.m_Data[0][0] = pow(Sigma_r, 2);
	r_r.m_Data[1][1] = pow(Sigma_az, 2);
	r_r.m_Data[2][2] = pow(Sigma_el, 2);
	Matrix3d Cs2b;
	//TODO:
	//GeodeticConverter::InitTCb2p1(Cs2b, psi_sens, theta_sens, phi_sens, 1);
	Matrix3d Cb2p;
	/*
		l_b=P.lever_b;
		Xb=l_b+Cs2b*Xs;
		Meas.b.x=Xb(1);
		Meas.b.y=Xb(2);
		Meas.b.z=Xb(3);
		Meas.b.R=norm(Xb);
		Meas.b.Az=atan2(Xb(2),Xb(1));
		Meas.b.El=asin(-Xb(3)/Meas.b.R);
		Meas.b.RR=Meas.s.RR;
	*/
	// From Body to P - use Euler Angles
	double theta = 0;//Plat.ImuEuler(1);
	double psi = 0;//Plat.ImuEuler(2);
	double phi = 0;//Plat.ImuEuler(3);
	//TODO:
	//GeodeticConverter::InitTCb2p1(Cb2p, theta, psi, phi, 1);

	Matrix3d Cp2enu_p;//=C.p2enu_p;
	Matrix3d Cenu_p2ecef;//=C.enu_p2ecef;
	Matrix3d Cs2ecef = Cenu_p2ecef * Cp2enu_p*Cb2p*Cs2b;
	Matrix3d r_ecef = Cs2ecef * Js*r_r*Js.Transpose()*Cs2ecef.Transpose();
	R.Zero();
	R.m_Data[0][0] = r_ecef.m_Data[0][0];
	R.m_Data[0][1] = r_ecef.m_Data[0][1];
	R.m_Data[0][2] = r_ecef.m_Data[0][2];
	R.m_Data[1][0] = r_ecef.m_Data[1][0];
	R.m_Data[1][1] = r_ecef.m_Data[1][1];
	R.m_Data[1][2] = r_ecef.m_Data[1][2];
	R.m_Data[2][0] = r_ecef.m_Data[2][0];
	R.m_Data[2][1] = r_ecef.m_Data[2][1];
	R.m_Data[2][2] = r_ecef.m_Data[2][2];
	R.m_Data[3][3] = pow(Sigma_rdot, 2);
}


/// <summary>
/// Set measurement covariance matrix in ENU tracking axes
/// </summary>
void ExtendedKalman::SetR_Enu(int type,
	double r,
	double az,
	double el,
	Vector3d Vs,
	double Sigma_r,
	double Sigma_rdot,
	double Sigma_az,
	double Sigma_el,
	/*Vector3d SigmaV,*/
	Matrix4d &R)
{
	switch (type)
	{
	case 0://{lin,ext}
	case 1:
	{
		/*
		double Vxs = Vs.m_Data[0];
		double Vys = Vs.m_Data[1];
		double Vzs = Vs.m_Data[2];
		double Sigma_Vxs = SigmaV.m_Data[0];
		double Sigma_Vys = SigmaV.m_Data[1];
		double Sigma_Vzs = SigmaV.m_Data[2];
		*/
		Matrix3d J;
		//TODO:
		//GeodeticConverter::InitJ(r, az, el, J);

		Matrix3d r_r;
		r_r.Zero();
		r_r.m_Data[0][0] = pow(Sigma_r, 2);
		r_r.m_Data[1][1] = pow(Sigma_az, 2);
		r_r.m_Data[2][2] = pow(Sigma_el, 2);
		Matrix3d r_enu = J * r_r*J.Transpose();
		//Matrix3d A;
		//A(1)=cos(az)*cos(el)*Vxs-sin(az)*cos(el)*Vys;
		//A(2)=-sin(az)*sin(el)*Vxs-cos(az)*sin(el)*Vys+cos(el)*Vzs;
		//A=A';
		Matrix3d Q;
		Q.Zero();
		Q.m_Data[0][0] = pow(Sigma_az, 2);
		Q.m_Data[1][1] = pow(Sigma_el, 2);
		double Sigma2_ref = pow(Sigma_rdot, 2);
		R.Zero();
		R.m_Data[0][0] = r_enu.m_Data[0][0];
		R.m_Data[0][1] = r_enu.m_Data[0][1];
		R.m_Data[0][2] = r_enu.m_Data[0][2];
		R.m_Data[1][0] = r_enu.m_Data[1][0];
		R.m_Data[1][1] = r_enu.m_Data[1][1];
		R.m_Data[1][2] = r_enu.m_Data[1][2];
		R.m_Data[2][0] = r_enu.m_Data[2][0];
		R.m_Data[2][1] = r_enu.m_Data[2][1];
		R.m_Data[2][2] = r_enu.m_Data[2][2];
		R.m_Data[3][3] = Sigma2_ref;
		break;
	}
	case 2://nonlin
	{
		R.Zero();
		R.m_Data[0][0] = pow(Sigma_r, 2);
		R.m_Data[1][1] = pow(Sigma_az, 2);
		R.m_Data[2][2] = pow(Sigma_el, 2);
		R.m_Data[3][3] = pow(Sigma_rdot, 2);
		break;
	}
	}
}


/// <summary>
/// Set measurement vector Z in ECEF coordinates
/// </summary>
void ExtendedKalman::SetZ_Ecef(const Vector4d &meas, Vector4d &z)
{
	z.m_Data[0] = meas.m_Data[0];
	z.m_Data[1] = meas.m_Data[1];
	z.m_Data[2] = meas.m_Data[2];
	z.m_Data[3] = meas.m_Data[3];
}

/// <summary>
/// Set measurment vector Z in ENU coordinates
/// </summary>
void ExtendedKalman::SetZ_Enu(const Vector3d &meas,
	double velocity,
	const Vector3d &velENU,
	Vector4d &z)
{
	//case 'nonlin'
	// Z=[Meas.enu_0.R; Meas.enu_0.Az; Meas.enu_0.El; Meas.enu_0.RR];
	z.m_Data[0] = meas.m_Data[0];
	z.m_Data[1] = meas.m_Data[1];
	z.m_Data[2] = meas.m_Data[2];
	z.m_Data[3] = velocity;
}

/// <summary>
/// Initialize State Vector X and Covariance Matrix P
/// </summary>
void ExtendedKalman::InitXP(const DataPlot &plot,
	/*const NavPlatStatusStruct &platData,*/
	Vector9d &xInit,
	Matrix9d &pInit)
{
	TrackerParams *pTrakerParams = new TrackerParams;

	//case 'ENU'
	//CovMat=SetR_enu(Params.H_Type,
	//                Meas.enu_0.R,
	//                Meas.enu_0.Az,
	//                Meas.enu_0.El,
	//                Plat.VelEnu,
	//                Meas.s.RangeSigma,
	//                Meas.s.RangeRateSigma,
	//                Meas.s.AzSigma,
	//                Meas.s.ElSigma,
	//                [Params.SigmaVxs Params.SigmaVys Params.SigmaVzs]);
	Vector3d vs;
	vs.m_Data[0] = pTrakerParams->m_SigmaVxs;
	vs.m_Data[1] = pTrakerParams->m_SigmaVys;
	vs.m_Data[2] = pTrakerParams->m_SigmaVzs;
	SetR_Enu(2,
		plot.GetRange(),
		plot.GetAzimuthAngle(),
		plot.GetElevationAngle(),
		vs,
		plot.GetRangeAccuracy(),
		plot.GetVelocityAccuracy(),
		plot.GetAzimuthAccuracy(),
		plot.GetElevationAccuracy(),
		/*platData.velENU,*/
		m_R);
	//m_R.Print();
		  //P_enu=CovMat([1:3],[1:3]);
		  //P_init1=zeros(9,9);
	Matrix9d pInit1;
	pInit1.Zero();
	//P_init1(1:3,1:3) =P_enu;
	pInit1.m_Data[0][0] = m_R.m_Data[0][0];
	pInit1.m_Data[0][1] = m_R.m_Data[0][1];
	pInit1.m_Data[0][2] = m_R.m_Data[0][2];
	pInit1.m_Data[1][0] = m_R.m_Data[1][0];
	pInit1.m_Data[1][1] = m_R.m_Data[1][1];
	pInit1.m_Data[1][2] = m_R.m_Data[1][2];
	pInit1.m_Data[2][0] = m_R.m_Data[2][0];
	pInit1.m_Data[2][1] = m_R.m_Data[2][1];
	pInit1.m_Data[2][2] = m_R.m_Data[2][2];
	//P_init1(4:6,4:6) =P_vel;
	//P_vel=[Params.SigmaVxt^2         0                     0         ;
	//    0             Params.SigmaVyt^2             0         ;
	//    0                     0             Params.SigmaVzt^2];
	pInit1.m_Data[3][3] = pow(pTrakerParams->m_SigmaVxt, 2);
	pInit1.m_Data[4][4] = pow(pTrakerParams->m_SigmaVyt, 2);
	pInit1.m_Data[5][5] = pow(pTrakerParams->m_SigmaVzt, 2);
	//P_init1(7:9,7:9) =P_acc;
	//P_acc=[Params.SigmaAxt^2         0                     0         ;
	//    0             Params.SigmaAyt^2             0         ;
	//    0                     0             Params.SigmaAzt^2];
	pInit1.m_Data[6][6] = pow(pTrakerParams->m_SigmaAxt, 2);
	pInit1.m_Data[7][7] = pow(pTrakerParams->m_SigmaAyt, 2);
	pInit1.m_Data[8][8] = pow(pTrakerParams->m_SigmaAzt, 2);
	//pInit1.Print();
	Matrix9d kk;
	kk.Zero();
	kk.m_Data[0][0] = 1;
	kk.m_Data[1][3] = 1;
	kk.m_Data[2][6] = 1;
	kk.m_Data[3][1] = 1;
	kk.m_Data[4][4] = 1;
	kk.m_Data[5][7] = 1;
	kk.m_Data[6][2] = 1;
	kk.m_Data[7][5] = 1;
	kk.m_Data[8][8] = 1;
	//kk.Print();
	pInit = kk * pInit1*kk.Transpose();
	//pInit.Print();
		  // x Vx y Vy z Vz
	//x_init=[Meas.enu_0.x;0;0;Meas.enu_0.y;0;0;Meas.enu_0.z;0;0];
	xInit.SetZero();
	xInit.m_Data[0] = plot.m_CartEnu0.m_Data[0];
	xInit.m_Data[3] = plot.m_CartEnu0.m_Data[1];
	xInit.m_Data[6] = plot.m_CartEnu0.m_Data[2];
}