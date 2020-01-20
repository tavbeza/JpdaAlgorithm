#ifndef _KALMAN_H_
#define _KALMAN_H_

#include <iostream>
#include <fstream>
#include "Vector.h"
#include "Matrix.h"
#include "DataPlot.h"
#include <vector>

//#include "Eigen/Dense"
//#include <opencv2/opencv.hpp>

/// <summary>
/// The Kalman class encapsulate the basic kalman filter algorithm. It contains 2D and 3D implementation.
/// </summary>
class Kalman
{
public:
	/// <summary>
	/// Empty constructor
	/// </summary>
	Kalman();

	/// <summary>
	/// 2D constructor
	/// </summary>
	void InitKalman(const float& dt, const Vector2f &target_delta,
		const float& x, const float& y,
		const float& vx, const float& vy,
		const Matrix2f &R);

	/// <summary>
	/// 2D constructor that get pointer of DataPlot
	/// </summary>
	Kalman(DataPlot* pPlot);

	/// <summary>
	/// The algorithm works in a two-step process. 
	/// In the prediction step, the Kalman filter produces estimates of the current state variables, along with their uncertainties. 
	/// Once the outcome of the next measurement (necessarily corrupted with some amount of error, including random noise) is observed, 
	/// these estimates are updated using a weighted average, with more weight being given to estimates with higher certainty.
	/// </summary>
	virtual Vector2f Predict();

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
	
	/// <summary>
	/// The algorithm works in a two-step process. 
	/// In the prediction step, the Kalman filter produces estimates of the current state variables, along with their uncertainties. 
	/// Once the outcome of the next measurement (necessarily corrupted with some amount of error, including random noise) is observed, 
	// these estimates are updated using a weighted average, with more weight being given to estimates with higher certainty.
	/// </summary>
	virtual Vector4f Update(const std::vector< Vector2f >& selected_detections, const Vector5f& beta, const float& last_beta);

public:
	/// <summary>
	/// Returns the matrix S
	/// <\summary>
	inline const Matrix2f GetS() const
	{
		return m_S;
	}

	/// <summary>
	/// Returns the last predictions
	/// <\summary>
	const Vector2f GetLastPrediction() const
	{
		return m_last_prediction;
	}

	/// <summary>
	/// Returns the last prediction eigen 
	/// <\summary>
	inline const Vector2f GetLastPredictionEigen() const
	{
		return m_last_prediction_eigen;
	}

	/// <summary>
	/// Returns the entropy 
	/// <\summary>
	inline const float GetEntropy() const
	{
		return m_entropy;
	}

	/// <summary>
	/// Sets the delta time in the matrix A (Evolution state matrix)
	/// <\summary>
	void SetDt(const double& dt)
	{
		//TODO:change it
		//m_F(4) = m_F(14) = (float)dt;
	}

	/// <summary>
	/// Returns the updated vector
	/// <\summary>
	const Vector4f GetUpdate()
	{
		return m_x_filter;
	}

protected:
	Matrix4f m_I;			// Identity Matrix
	Vector4f m_x_filter;	// State vector
	Matrix4f m_P;			// Prediction Covariance Matrix
	Matrix2f m_Q;			// Covariance Matrix associated to the evolution process
	Matrix2f m_R;			// Proces measurement Covariance matrix
	Matrix42f m_G;			// Evolution Noise Matrix

	Matrix4f m_F;			// Jacobian of process model
	Matrix24f m_H;			// Jacobian of measurement model

	Matrix2f m_S;
	Matrix42f m_K;			// Gain

	Matrix4f m_P_predict;	// Covariance Matrix predicted error
	Vector4f m_x_predict;
	Vector2f m_z_predict;

	Vector2f m_last_prediction;
	Vector2f m_last_prediction_eigen;
	Vector2f m_last_speed;

	bool m_first;
	float m_entropy;
protected:
	static constexpr float m_k = 5.0620f; // n/2 * log(4*PI) where n is the state dimention (x, y, x', y')
	//static constexpr float k = 2.1984;
	std::ofstream	m_kalmanFile;
	int				m_nTrack;
};

#endif _KALMAN_H_