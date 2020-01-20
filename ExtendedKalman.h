#ifndef _EXTENDED_KALMAN_H_
#define _EXTENDED_KALMAN_H_

#include <iostream>
#include "Vector.h"
#include "Matrix.h"
#include "DataPlot.h"
#include "NavPlatStatusStruct.h"
#include "SrvDspMath.h"
#include "TrackerParams.h"

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
	/// Kalman 3D constructor
	/// </summary>
	ExtendedKalman(const DataPlot &plot);

	/// <summary>
	/// Virtual kalman destructor
	/// </summary>
	virtual ~ExtendedKalman();

	/// <summary>
	/// Initialize the kalman
	/// </summary>
	void Init(
		Vector3d cartesian,
		float vx,
		float vy,
		float vz)
		noexcept;

	/// <summary>
	/// The algorithm works in a two-step process. 
	/// In the prediction step, the Kalman filter produces estimates of the current state variables, along with their uncertainties. 
	/// Once the outcome of the next measurement (necessarily corrupted with some amount of error, including random noise) is observed, 
	/// these estimates are updated using a weighted average, with more weight being given to estimates with higher certainty.
	/// </summary>
	Vector3d Predict();

	/// <summary>
	/// The algorithm works in a two-step process. 
	/// In the prediction step, the Kalman filter produces estimates of the current state variables, along with their uncertainties. 
	/// Once the outcome of the next measurement (necessarily corrupted with some amount of error, including random noise) is observed, 
	// these estimates are updated using a weighted average, with more weight being given to estimates with higher certainty.
	/// </summary>
	virtual void Update(DataPlot* pPlot);


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
	void SetDt(float dt);

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
	void SetQ_Singer(double tau, double sigmaManeuver2, Matrix3d &q);

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
	void SetQ();

	/// <summary>
	/// Set the matrix F
	/// </summary>
	void SetF_Singer(double tau, Matrix3d &f);
	
	/// <summary>
	/// Set the matrix F
	/// </summary>
	void SetF();
	
	/// <summary>
	/// Set measurement matrix H
	/// </summary>
	void SetH();
	
	/// <summary>
	/// Set measurement covariance matrix
	/// </summary>
	void SetR(double error_r,
		double error_az,
		double error_el,
		double error_v);

	/// <summary>
	/// Set measurement vector Z in ECEF coordinates
	/// </summary>
	void SetZ_Ecef(const Vector4d &meas, Vector4d &z);
	
	/// <summary>
	/// Set measurment vector Z in ENU coordinates
	/// </summary>
	void SetZ_Enu(const Vector3d &meas, double velocity, const Vector3d &velENU, Vector4d &z);

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
	float	 m_Dt;

protected:
	static float m_k;

};
#endif //_EXTENDED_KALMAN_H_