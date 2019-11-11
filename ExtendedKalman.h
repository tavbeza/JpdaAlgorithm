#ifndef _EXTENDED_KALMAN_H_

#include <iostream>
#include "Vector.h"
#include "Matrix.h"

struct TrakerParams {
	double m_SigmaVxs=0.1;
	double m_SigmaVys = 0.1;
	double m_SigmaVzs = 0.1;
	double m_Sigma_r = 0.1;
	double m_Sigma_rdot = 0.1;
	double m_Sigma_az = 0.1;
	double m_Sigma_el = 0.1;
	double m_SigmaAxs = 0.1;
	double m_SigmaAys = 0.1;
	double m_SigmaAzs = 0.1;
	Vector3d m_TauAcc;
	Vector3d m_Amax;
	Vector3d m_Pmax;
	Vector3d m_P0;
	Vector3d m_SigmaManeuver2 ;
	double m_SigmaMnvrCalcMtd = 0.1;


};

/// <summary>
/// The Kalman class encapsulat the basic kalman filter algorithm. It contains 2D and 3D implementation.
/// </summary>
class ExtendedKalman
{
public:
	/// <summary>
	/// Default Constructor
	/// </summary>
	ExtendedKalman() noexcept;

	/// <summary>
	/// Virtual kalman destructor
	/// </summary>
	virtual ~ExtendedKalman();

	/// <summary>
	/// Kalman 3D constructor
	/// </summary>
	ExtendedKalman(
		const float& dt,
		const float& x,
		const float& y,
		const float& z,
		const float& vx,
		const float& vy,
		const float& vz,
		const Matrix4d& R)
		noexcept;

	/// <summary>
	/// Initialize the kalman
	/// </summary>
	void Init(
		const float& dt,
		const float& x,
		const float& y,
		const float& z,
		const float& vx,
		const float& vy,
		const float& vz,
		const Matrix4d& R);

	/// <summary>
	/// The algorithm works in a two-step process. 
	/// In the prediction step, the Kalman filter produces estimates of the current state variables, along with their uncertainties. 
	/// Once the outcome of the next measurement (necessarily corrupted with some amount of error, including random noise) is observed, 
	/// these estimates are updated using a weighted average, with more weight being given to estimates with higher certainty.
	/// </summary>
	Vector3d Predict(const double& dt);
	
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
	virtual void GainUpdate(const float& beta);

	void SetQ(double Dt, Matrix9d &Q);

public:
	/// <summary>
	/// Returns the matrix S
	/// </summary>
	inline const Matrix4d GetS() const
	{
		return m_S;
	}

	/// <summary>
	/// Returns the last predictions
	/// </summary>
	const Vector3d GetLastPrediction() const
	{
		return m_last_prediction;
	}

	/// <summary>
	/// Returns the last prediction eigen
	/// </summary>
	inline const Vector9d GetLastPredictionEigen() const
	{
		return m_last_prediction_eigen;
	}

	/// <summary>
	/// Returns the entropy
	/// </summary>
	inline double GetEntropy() const
	{
		return m_Entropy;
	}

	/// <summary>
	/// Sets the delta time in the matrix A (Evolution state matrix)
	/// </summary>
	void SetDt(const double& dt);

	/// <summary>
	/// Returns the updated vector
	/// </summary>
	const Vector9d GetUpdate()
	{
		return m_X;
	}

	/// <summary>
	/// Set Singer Model Process Noise Covariance matrix for state vector:[x Vx Ax]';
	/// </summary>
	void SetQ_Singer(double dt, double tau, double sigmaManeuver2, Matrix3d &q);

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
	///		q{i}=SetQ_Singer(Dt,tau(i),SigMan2(i));
	/// end
	/// z3=zeros(3);
	/// Q = [ q{1} z3 z3;
	///      z3 q{2} z3;
	///      z3 z3 q{3} ];
	/// </summary>
	void SetQ(double Dt, Matrix9d &Q);

	/// <summary>
	/// Set the matrix F
	/// </summary>
	void SetF_Singer(double dt, double tau, Matrix3d &f);
	
	/// <summary>
	/// Set the matrix F
	/// </summary>
	void SetF(double dt, Matrix9d &F);
	
	/// <summary>
	/// Set measurement matrix H in ECEF coordinate axes
	/// </summary>
	void SetH_Ecef(Vector9d X_predict, Vector9d X_sensor, Matrix49d &H, double &R_dot);
	
	/// <summary>
	/// Set measurement matrix H in ENU coordinate axes
	/// </summary>
	void SetH_Enu(	int type,
					double az,
					double el,
					Vector3d pos_t,
					Vector3d pos_s,
					Vector3d vel_t,
					Vector3d vel_s,
					Matrix49d &H);
	
	/// <summary>
	/// Set measurement covariance matrix in ECEF tracking axes
	/// </summary>
	void SetR_Ecef(	double r,
					double az,
					double el,
					Vector3d Vs,
					double Sigma_r,
					double Sigma_rdot,
					double Sigma_az,
					double Sigma_el,
					Vector3d SigmaV,
					Matrix4d &R);
	
	/// <summary>
	/// Set measurement covariance matrix in ENU tracking axes
	/// </summary>
	void SetR_Enu(	int type,
					double r,
					double az,
					double el,
					Vector3d Vs,
					double Sigma_r,
					double Sigma_rdot,
					double Sigma_az,
					double Sigma_el,
					Vector3d SigmaV,
					Matrix4d &R);

	/// <summary>
	/// Set measurement vector Z in ECEF coordinates
	/// </summary>
	void SetZ_Ecef(const Vector4d &meas, Vector4d &z);
	
	/// <summary>
	/// Set measurment vector Z in ENU coordinates
	/// </summary>
	void SetZ_Enu(const Vector3d &meas, double velocity, const Vector3d &velENU, Vector4d &z);
	
	/// <summary>
	/// New function that calculates H matrix between Cartesian state vector to nonlinear measurement Z
	/// </summary>
	void Calc_H_Matrix_Cart2Pol(Vector9d X, Matrix49d &H);
	
	/// <summary>
	/// Initialize State Vector X and Covariance Matrix P
	/// </summary>
	/*void InitXP(
		const DataPlot &plot,
		const NavPlatStatusStruct &platData,
		Vector9d &xInit,
		Matrix9d &pInit
	);*/

public:
	/// <summary>
	/// The kalman state estimate vector of current track in enu_0 coordinate
	/// axes: [x,Vx,Ax,y,Vy,Ay,z,Vz,Az]
	/// </summary>
	Vector9d m_X;
	
	/// <summary>
	/// The kalman state estimate vector of current track translated into polar
    /// coordinates (relative to enu_p)
	/// axes: [R,Az,El,RR]
	/// </summary>
	Vector4d m_X_Polar;
	
	/// <summary>
	///The kalman gain matrix
	/// </summary>
	Matrix94d m_K;
	
	/// <summary>
	/// The kalman error covariance matrix in ENU_0 coordinate axes
	/// </summary>
	Matrix9d m_P;
	
	/// <summary>
	/// The kalman error covariance matrix translated into polar relative to ENU_p coordinate axes
	/// </summary>
	Matrix4d m_P_Polar;
	
	/// <summary>
	/// The kalman residual covariance matrix
	/// </summary>
	Matrix4d m_S;
	
	/// <summary>
	/// The kalman measurement vector [x,y,z,rDot]
	/// [m;m;m;m/sec]
	/// </summary>
	Vector4d m_Z;
	
	/// <summary>
	/// The kalman observation model
	/// </summary>
	Matrix49d m_H;
	
	/// <summary>
	/// The kalman covariance matrix of the observation noise
	/// </summary>
	Matrix4d m_R;
	
	/// <summary>
	/// The kalman predicted (a prior) state estimate
	/// </summary>
	Vector9d m_X_Predict;
	
	/// <summary>
	/// The kalman predicted (a prior) error covariance matrix
	/// </summary>
	Matrix9d m_P_Predict;
	
	/// <summary>
	/// The measurement of the true state
	/// </summary>
	Vector9d m_Z_Predict;
	
	/// <summary>
	/// The kalman process noise covariance matrix
	/// </summary>
	Matrix9d m_Q;
		
	/// <summary>
	/// The kalman transition matrix for state vector
	/// </summary>
	Matrix9d m_F;

	Vector3d m_last_prediction;
	Vector3d m_last_speed;
	Vector9d m_last_prediction_eigen;
	bool     m_IsFirst;
	double   m_Entropy;

protected:
	static float m_k;

};
#endif _EXTENDED_KALMAN_H_