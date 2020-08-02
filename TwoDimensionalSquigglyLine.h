#ifndef _TWO_DIMENSIONAL_SQUIGGLY_LINE_H_
#define _TWO_DIMENSIONAL_SQUIGGLY_LINE_H_


#include "EquationOfMotion.h"
#include <time.h>


/// <summary>
/// Class that inherits from EquationOfMotion class and hold motion equation of two dimensional squiggly line 
/// and method of motion calculate for two dimensional squiggly line test cases
/// </summary>
class TwoDimensionalSquigglyLine :
	public EquationOfMotion
{

public:

	TwoDimensionalSquigglyLine(float dt = 0, float x0 = 0, float y0 = 0, float z0 = 0,
		float vx0 = 0, float vy0 = 0, float vz0 = 0, int seed = NULL);

	virtual void CalculateMotion(int dwellCount, Vector3f& locVec, Vector3f& velVec);
	int GetSeed();

private:

	int m_seed;

};


#endif //_TWO_DIMENSIONAL_SQUIGGLY_LINE_H_