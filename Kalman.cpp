#include "stdafx.h"
#include "kalman.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

static int numOfPrints = 0;

Kalman::Kalman(const float& dt, const Vector2f &target_delta, const float& x, const float& y, const float& vx, const float& vy, const Matrix2f& R)
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



