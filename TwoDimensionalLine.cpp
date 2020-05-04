#include "pch.h"
#include "TwoDimensionalLine.h"

/*
TwoDimensionalLine::TwoDimensionalLine(float x0, float y0, float z0,
									float vx0, float vy0, float vz0, float dt)
{
	m_x0 = x0;
	m_y0 = y0;
	m_z0 = z0;

	m_vx = m_vx0 = vx0; //right now the movment doesn't support acceleration so this is fine,
						//will need to be changed if acceleration support is added.
	m_vy = m_vy0 = vy0;
	m_vz = m_vz0 = vz0;

	m_dt = dt;

	
	//m_z = 0;
	//m_vz = 0;
	//m_dt = 93.0 / 1000;
	//m_vx = 5;
	//m_vy = 5;
	
}
*/

void TwoDimensionalLine::CalculateMotion(int dwell, Vector3f& locVec, Vector3f& velVec)
{

	m_x = m_x0 + m_vx * m_dt * dwell;
	m_y = m_y0 + m_vy * m_dt * dwell;

	locVec.m_Data[0] = m_x;
	locVec.m_Data[1] = m_y;
	locVec.m_Data[2] = m_z;

	velVec.m_Data[0] = m_vx;
	velVec.m_Data[1] = m_vy;
	velVec.m_Data[2] = m_vz;

}