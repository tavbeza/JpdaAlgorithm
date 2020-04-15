#ifndef _EQUATION_OF_MOTION_H_
#define _EQUATION_OF_MOTION_H_

#include "Vector.h"
#include <fstream>

class EquationOfMotion
{

public:

	/*
	EquationOfMotion() : 
		m_x0(0), m_y0(0), m_z0(0),
		m_x(0) , m_y(0) , m_z(0) ,
		m_vx0(0), m_vy0(0), m_vz0(0),
		m_vx(0) , m_vy(0) , m_vz(0),
		m_dt(0)
	{};
	*/

	EquationOfMotion(float dt = 0, float x0 = 0, float y0 = 0, float z0 = 0,
		float vx0 = 0, float vy0 = 0, float vz0 = 0) ;
	
	virtual void CalculateMotion(int dwellCount, Vector3f& locVec, Vector3f& velVec) = 0;


	friend std::ofstream& operator<<(std::ofstream& out, const EquationOfMotion& eom)
	{
		out << eom.getMx() << "," << eom.getMy() << eom.getMz() << ","
			<< eom.getMvx() << "," << eom.getMvy() << "," << eom.getMvz();

		return out;
	}

	float getMx() const { return m_x; }

	void setMx(float mx) { m_x = mx; }

	float getMy() const { return m_y; }

	void setMy(float my) { m_y = my; }

	float getMz() const { return m_z; }

	void setMz(float mz) { m_z = mz; }

	float getMvx() const { return m_vx; }

	void setMvx(float mvx) { m_vx = mvx; }

	float getMvy() const { return m_vy; }

	void setMvy(float mvy) { m_vy = mvy; }

	float getMvz() const { return m_vz; }

	void setMvz(float mvz) { m_vz = mvz; }

	float getMvx0() const { return m_vx0; }

	float getMvy0() const { return m_vy0; }

	float getMvz0() const { return m_vz0; }

	float getMdt() const { return m_dt; }


protected:

	float m_x0;
	float m_y0;
	float m_z0;
	float m_x;
	float m_y;
	float m_z;
	float m_vx;
	float m_vy;
	float m_vz;
	float m_vx0;
	float m_vy0;
	float m_vz0;
	float m_dt;


    

};




#endif //_EQUATION_OF_MOTION_H_