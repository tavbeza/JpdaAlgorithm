#include "pch.h"
#include "TwoDimensionalSquigglyLine.h"


TwoDimensionalSquigglyLine::TwoDimensionalSquigglyLine(float dt, float x0, float y0, float z0,
														float vx0, float vy0, float vz0, int seed) :
														EquationOfMotion(dt, x0, y0, z0, vx0, vy0, vz0)
{

	if(seed == NULL)
		seed = time(NULL);

	// m_seed = seed;

	// srand(m_seed);

	m_z = 0;
	m_vz = 0;
	m_dt = 93.0 / 1000;
	m_vx = 5;
	m_vy = 5;

}


void TwoDimensionalSquigglyLine::CalculateMotion(int dwellCount, Vector3f& locVec, Vector3f& velVec)
{
	static bool isUp = true;
	//float range = 5;
	//m_x = m_vx * m_dt * dwellCount + (static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) * range - range/2;
	//m_y = m_vx * m_dt * dwellCount + (static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) * range - range/2;

	/*
	if (isUp)
	{
		m_x = m_vx * m_dt * dwellCount * 0.9;
		m_y = m_vx * m_dt * dwellCount * 1.1;
	}
	else
	{
		m_x = m_vx * m_dt * dwellCount * 1.1;
		m_y = m_vx * m_dt * dwellCount * 0.9;
	}
	*/

	if (isUp)
	{
		m_x = m_vx * m_dt * dwellCount * 1.1;
		m_y = m_vy * m_dt * dwellCount * 1.1;
	}
	else
	{
		m_x = m_vx * m_dt * dwellCount * 0.9;
		m_y = m_vy * m_dt * dwellCount * 0.9;
	}

	isUp = !isUp;
	locVec.m_Data[0] = m_x;
	locVec.m_Data[1] = m_y;
	locVec.m_Data[2] = m_z;

	velVec.m_Data[0] = m_vx;
	velVec.m_Data[1] = m_vy;
	velVec.m_Data[2] = m_vz;

}

int TwoDimensionalSquigglyLine::GetSeed()
{
	return m_seed;
}