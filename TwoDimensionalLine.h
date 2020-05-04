#ifndef _TWO_DIMENSIONAL_LINE_H_
#define _TWO_DIMENSIONAL_LINE_H_

#include "EquationOfMotion.h"

class TwoDimensionalLine : public EquationOfMotion
{

public:
	//TwoDimensionalLine(float x0 = 0, float y0 = 0, float z0 = 0,
	//			float vx0 = 5, float vy0 = 5, float vz0 = 0, float dt = 93.0 / 1000);
	TwoDimensionalLine(float dt = 0, float x0 = 0, float y0 = 0, float z0 = 0,
		float vx0 = 0, float vy0 = 0, float vz0 = 0) : EquationOfMotion(dt, x0,  y0,  z0, vx0,  vy0,  vz0) {};

	virtual void CalculateMotion(int dwell, Vector3f& locVec, Vector3f& velVec);

private:



};


#endif //_TWO_DIMENSIONAL_LINE_H_