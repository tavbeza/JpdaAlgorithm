#include "pch.h"
#include "EquationOfMotion.h"

EquationOfMotion::EquationOfMotion(float dt, float x0, float y0, float z0, float vx0, float vy0, float vz0) :
															m_x(0), m_y(0), m_z(0), m_vx(0), m_vy(0), m_vz(0)
{

	m_x0 = x0;
	m_y0 = y0;
	m_z0 = z0;

	m_vx = m_vx0 = vx0; //right now the movment doesn't support acceleration so this is fine,
						//will need to be changed if acceleration support is added.
	m_vy = m_vy0 = vy0;
	m_vz = m_vz0 = vz0;

	m_dt = dt;

}
