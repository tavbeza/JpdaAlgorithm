#include "stdafx.h"
#include "kalman.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

static int numOfPrints = 0;

/// <summary>
/// Empty constructor
/// </summary>
Kalman::Kalman()
{
}

/// <summary>
/// 2D constructor
/// </summary>
void Kalman::InitKalman(const float& dt, const Vector2f &target_delta, const float& x, const float& y, const float& vx, const float& vy, const Matrix2f& R)
{
	static int counter = 0;
	m_nTrack = counter++;

	m_I.Identity();

	if (m_nTrack == 0)
	{
		std::stringstream path;
		path << "C:\\Users\\tavbe\\Desktop\\parameters\\newPar" << m_nTrack << ".txt";

		m_kalmanFile.open(path.str());
		m_kalmanFile << "//dt" << std::endl;
		m_kalmanFile << dt << std::endl;

		m_kalmanFile << "//target_delta" << std::endl;
		m_kalmanFile << target_delta.m_Data[0] << std::endl;
		m_kalmanFile << target_delta.m_Data[1] << std::endl;

		m_kalmanFile << "//x" << std::endl;
		m_kalmanFile << x << std::endl;

		m_kalmanFile << "//y" << std::endl;
		m_kalmanFile << y << std::endl;

		m_kalmanFile << "//vx" << std::endl;
		m_kalmanFile << vx << std::endl;

		m_kalmanFile << "//vy" << std::endl;
		m_kalmanFile << vy << std::endl;

		m_kalmanFile << "//R" << std::endl;
		R.PrintToFile(m_kalmanFile);

		m_kalmanFile << std::endl;

		//m_kalmanFile.close();
	}

	//TRANSITION MATRIX
	m_F.set(0, 0, 1);
	m_F.set(0, 1, dt);
	m_F.set(0, 2, 0);
	m_F.set(0, 3, 0);
	m_F.set(1, 0, 0);
	m_F.set(1, 1, 1);
	m_F.set(1, 2, 0);
	m_F.set(1, 3, 0);
	m_F.set(2, 0, 0);
	m_F.set(2, 1, 0);
	m_F.set(2, 2, 1);
	m_F.set(2, 3, dt);
	m_F.set(3, 0, 0);
	m_F.set(3, 1, 0);
	m_F.set(3, 2, 0);
	m_F.set(3, 3, 1);

	//process noise covariance matrix 
	m_Q = R;

	//NOISE EVOLUTION
	m_G.m_Data[0][0] = std::pow(dt, 2) / 2;
	m_G.m_Data[0][1] = 0;
	m_G.m_Data[1][0] = dt;
	m_G.m_Data[1][1] = 0;
	m_G.m_Data[2][0] = 0;
	m_G.m_Data[2][1] = std::pow(dt, 2) / 2;
	m_G.m_Data[3][0] = 0;
	m_G.m_Data[3][1] = dt;

	//STATE OBSERVATION MATRIX
	m_H.m_Data[0][0] = 1;
	m_H.m_Data[0][1] = 0;
	m_H.m_Data[0][2] = 0;
	m_H.m_Data[0][3] = 0;
	m_H.m_Data[1][0] = 0;
	m_H.m_Data[1][1] = 0;
	m_H.m_Data[1][2] = 1;
	m_H.m_Data[1][3] = 0;

	//INITIAL COVARIANCE MATRIX
	m_P.m_Data[0][0] = std::pow(target_delta.m_Data[0], 2);
	m_P.m_Data[0][1] = 0;
	m_P.m_Data[0][2] = 0;
	m_P.m_Data[0][3] = 0;
	m_P.m_Data[1][0] = 0;
	m_P.m_Data[1][1] = (float)0.1;
	m_P.m_Data[1][2] = 0;
	m_P.m_Data[1][3] = 0;
	m_P.m_Data[2][0] = 0;
	m_P.m_Data[2][1] = 0;
	m_P.m_Data[2][2] = std::pow(target_delta.m_Data[1], 2);
	m_P.m_Data[2][3] = 0;
	m_P.m_Data[3][0] = 0;
	m_P.m_Data[3][1] = 0;
	m_P.m_Data[3][2] = 0;
	m_P.m_Data[3][3] = (float)0.1;
  

	//measurement noise covariance matrix
	m_R = R;

	m_last_prediction.m_Data[0] = x;
	m_last_prediction.m_Data[1] = y;

	m_last_speed.m_Data[0] = vx;
	m_last_speed.m_Data[1] = vy;

	m_first = true;
}

/// <summary>
/// 2D constructor that get pointer of DataPlot
/// </summary>
Kalman::Kalman(DataPlot* pPlot)
{
	float dt = 93.0 / 1000.0;
	Vector2f target_delta((float)10, (float)10);
	Matrix2f R;
	R.Init((float)100, (float)0, (float)0, (float)100);

	InitKalman(dt,												// dt
		target_delta,
		cos(pPlot->GetAzimuthAngle())*pPlot->GetRange(),		// x
		sin(pPlot->GetAzimuthAngle())*pPlot->GetRange(),		// y
		cos(pPlot->GetAzimuthAngle())*pPlot->GetVelocity(),		// vx
		sin(pPlot->GetAzimuthAngle())*pPlot->GetVelocity(),		// vy
		R);
}

/// <summary>
/// The algorithm works in a two-step process. 
/// In the prediction step, the Kalman filter produces estimates of the current state variables, along with their uncertainties. 
/// Once the outcome of the next measurement (necessarily corrupted with some amount of error, including random noise) is observed, 
/// these estimates are updated using a weighted average, with more weight being given to estimates with higher certainty.
/// </summary>
Vector2f Kalman::Predict()
{
	if (m_first)
	{
		m_x_predict.m_Data[0] = m_last_prediction.m_Data[0];
		m_x_predict.m_Data[1] = m_last_speed.m_Data[0];
		m_x_predict.m_Data[2] = m_last_prediction.m_Data[1];
		m_x_predict.m_Data[3] = m_last_speed.m_Data[1];
		m_first = false;
	}
	else
	{
		m_x_predict = m_F * m_x_filter;
	}

	//Covariance Matrix predicted error
	m_P_predict = m_F * m_P * m_F.Transpose() + m_G *( m_Q * Transpose(m_G));


	//Predicted Measurement
	m_z_predict = m_H * m_x_predict;

	//Error Measurement Covariance Matrix
	//Transpose();
	m_S = m_H * m_P_predict * Transpose(m_H) + m_R;

	//Compute Entropy
	m_entropy = m_k + (float)0.5 * log10(m_P.Determinant());

	m_last_prediction_eigen = m_z_predict;
	m_last_prediction.m_Data[0] = m_z_predict.m_Data[0];
	m_last_prediction.m_Data[1] = m_z_predict.m_Data[1];

	if (m_nTrack == 0 && numOfPrints < 10)
	{
		numOfPrints++;

		m_kalmanFile << "//m_x_predict" << std::endl;
		m_kalmanFile << m_x_predict.m_Data[0] << std::endl;
		m_kalmanFile << m_x_predict.m_Data[1] << std::endl;
		m_kalmanFile << m_x_predict.m_Data[2] << std::endl;
		m_kalmanFile << m_x_predict.m_Data[3] << std::endl;


		m_kalmanFile << "//m_P_predict" << std::endl;
		m_P_predict.PrintToFile(m_kalmanFile);

		m_kalmanFile << "//m_z_predict" << std::endl;
		m_kalmanFile << m_z_predict.m_Data[0] << std::endl;
		m_kalmanFile << m_z_predict.m_Data[1] << std::endl;

		m_kalmanFile << "//m_S" << std::endl;
		m_S.PrintToFile(m_kalmanFile);

		m_kalmanFile << "//m_entropy" << std::endl;
		m_kalmanFile << m_entropy << std::endl;

		m_kalmanFile << "//m_last_prediction_eigen" << std::endl;
		m_kalmanFile << m_last_prediction_eigen.m_Data[0] << std::endl;
		m_kalmanFile << m_last_prediction_eigen.m_Data[1] << std::endl;

		m_kalmanFile << "//m_last_prediction" << std::endl;
		m_kalmanFile << m_last_prediction.m_Data[0] << std::endl;
		m_kalmanFile << m_last_prediction.m_Data[1] << std::endl;

		m_kalmanFile << std::endl;
	}

	return m_last_prediction;
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
void Kalman::GainUpdate(const float& beta)
{
	//Matrix24f m_H_T;
	//std::cout << "m_S.Inverse() = ";
	//std::cout << std::endl;
	//Matrix2f m_S_Inv;
	//m_S_Inv = m_S.Inverse();// = m_S.Inverse();
	/*for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			m_S_Inv.m_Data[i][j] = m_S.Inverse().m_Data[i][j];
		}
	}*/
	m_K = m_P_predict * Transpose(m_H) * Inverse2f(m_S);
	m_P = m_P_predict - (m_K * (1 - beta)) * m_S * Transpose(m_K);

	if (m_nTrack == 0 && numOfPrints < 10)
	{
		numOfPrints++;

		m_kalmanFile << "//m_K" << std::endl;
		m_K.PrintToFile(m_kalmanFile);

		m_kalmanFile << "//m_P" << std::endl;
		m_P.PrintToFile(m_kalmanFile);

		m_kalmanFile << std::endl;
	}
}

/// <summary>
/// The algorithm works in a two-step process. 
/// In the prediction step, the Kalman filter produces estimates of the current state variables, along with their uncertainties. 
/// Once the outcome of the next measurement (necessarily corrupted with some amount of error, including random noise) is observed, 
// these estimates are updated using a weighted average, with more weight being given to estimates with higher certainty.
/// </summary>
Vector4f Kalman::Update(const std::vector< Vector2f >& selected_detections, const Vector5f& beta, const float& last_beta)
{
	//static int numOfPrints = 0;
	Vector4f a;
	a.SetZero();

	m_x_filter.SetZero();
	unsigned int i = 0;

	for (const auto& det : selected_detections)
	{
		a = m_x_predict + m_K * (det - m_z_predict);
		m_x_filter = m_x_filter + a * beta.m_Data[i];
		++i;
	}

	m_x_filter = m_x_predict * last_beta + m_x_filter;

	a.SetZero();
	Matrix4f Pk = Matrix4f::Zero();
	Vector2f det;

	for (i = 0; i < selected_detections.size() + 1; ++i)
	{
		if (i == selected_detections.size())
		{
			a = m_x_predict;
		}
		else
		{
			a = m_x_predict + m_K * (selected_detections.at(i) - m_z_predict);
		}
		
		Pk = Pk + (a * Transpose(a) - m_x_filter * Transpose(m_x_filter)) * beta.m_Data[i];

	}

	m_P += Pk;

	if (m_nTrack == 0 && numOfPrints < 10)
	{
		numOfPrints++;

		m_kalmanFile << "//m_x_filter" << std::endl;
		m_kalmanFile << m_x_filter.m_Data[0] << std::endl;
		m_kalmanFile << m_x_filter.m_Data[1] << std::endl;
		m_kalmanFile << m_x_filter.m_Data[2] << std::endl;
		m_kalmanFile << m_x_filter.m_Data[3] << std::endl;

		m_kalmanFile << "//m_P" << std::endl;
		m_P.PrintToFile(m_kalmanFile);

		m_kalmanFile << std::endl;
	}

	return m_x_filter;
}

/// <sumary>
/// Test
/// <\summary>
void Kalman::kalmanTest()
{
	const float dt = 0.4;
	const Vector2f target_delta((float)10, (float)10);
	const float& x = 511;
	const float& y = 194;
	const float& vx = -3;
	const float& vy = 0;
	Matrix2f R;
	R.Init((float)100, (float)0, (float)0, (float)100);

	Kalman kalman(dt, target_delta, x, y, vx, vy, R);

	kalman.Predict();

	const float beta = 0.240925908;

	kalman.GainUpdate(beta);

	const std::vector< Vector2f > selected_detections = { {508.000000, 194.000000}, {278.000000, 262.000000}, {516.000000, 205.000000}, {644.000000, 281.000000} };
	const Vector5f beta2(0.497215271, 0, 0.261858821, 0, 0.240925908);
	const float& last_beta = 0.240925908;

	kalman.Update(selected_detections, beta2, last_beta);

}





