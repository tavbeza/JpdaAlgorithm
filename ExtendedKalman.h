#ifndef _EXTENDED_KALMAN_H_
#define _EXTENDED_KALMAN_H_

#include <iostream>
#include "Vector.h"
#include "Matrix.h"
#include "DataPlot.h"
#include "SrvDspMath.h"
#include "TrackerParams.h"

/// <summary>
/// The Kalman class encapsulates the extended kalman filter algorithm. It contains 2D and 3D implementation.
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
	void Init(Vector3d cartesian) noexcept;

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
	/// Returns the last predictions
	/// </summary>
	const Vector3d GetLastPrediction() const
	{
		return m_last_prediction;
	}

	/// <summary>
	/// Returns the last location
	/// </summary>
	const Vector3d GetLastLocation() const;

	/// <summary>
	/// Sets the delta time in the matrix A (Evolution state matrix)
	/// </summary>
	void SetDt(float dt);

	/// <summary>
	/// Sets m_S
	/// </summary>
	void SetS();

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
	///                                        accelaeration with ability
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
	/// Convert cartesian velocity to R dot 
	/// </summary>
	void SetRdot();

	/// <summary>
	/// Convert cartesian velocity to R dot 
	/// </summary>
	void SetPredictRdot();


	/// <summary>
	/// Set covariance matrix P
	/// </summary>
	void SetP(Vector3d spherical);
	
	/// <summary>
	/// Set measurement covariance matrix
	/// </summary>
	void ExtendedKalman::SetR(double error_x,
		double error_y,
		double error_z,
		double error_v);

public:
	/// <summary>
	/// The kalman state estimate vector of current track in enu_0 coordinate
	/// axes: [x,Vx,Ax,y,Vy,Ay,z,Vz,Az]
	/// </summary>
	Vector9d m_X;
	
	/// <summary>
	///The kalman gain matrix
	/// </summary>
	Matrix94d m_K;
	
	/// <summary>
	/// The kalman error covariance matrix in ENU_0 coordinate axes
	/// </summary>
	Matrix9d m_P;
	
	/// <summary>
	/// The kalman residual covariance matrix
	/// </summary>
	Matrix4d m_S;
	
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
	/// The kalman process noise covariance matrix
	/// </summary>
	Matrix9d m_Q;
		
	/// <summary>
	/// The kalman transition matrix for state vector
	/// </summary>
	Matrix9d m_F;

	/// <summary>
	/// Identity matrix 9x9
	/// </summary>
	Matrix9d m_I;

	/// <summary>
	/// Vector of the last predicted position in cartesian coordinates (x,y,z)
	/// </summary>
	Vector3d m_last_prediction;
	
	/// <summary>
	/// Vector of the velocity in cartesian coordinates (Vx,Vy,Vz)
	/// </summary>
	Vector3d m_last_speed_cartesian;
	
	/// <summary>
	/// Delta time of dwell
	/// </summary>
	float	 m_Dt;
	
	/// <summary>
	/// The velocity in spherical coordinates
	/// </summary>
	float	 m_Rdot;
	
	/// <summary>
	/// The predicted velocity in spherical coordinates
	/// </summary>
	float m_Rdot_Predict;

};
#endif //_EXTENDED_KALMAN_H_