#ifndef _SRV_DSP_MATH_H
#define _SRV_DSP_MATH_H

#include <math.h>

/// <summary>
/// This class shall support enhanced Mathematical services for<span dir="ltr">DSP.
/// </summary>

class SrvDspMath
{
public:
	/// <summary>
	/// Empty constructor
	/// </summary>
	SrvDspMath();
	
	/// <summary>
	/// Destructor 
	/// </summary>
	~SrvDspMath();

	/// <summary>
	/// Compute exponential function
	/// </summary>
	static double exp(double x);

	/// <summary>
	/// Compute arc tangent with two parameters
	/// </summary>
	static float atan2(float a, float b);

	/// <summary>
	/// Compute arc sine 
	/// </summary>
	static float asin(float number);

	/// <summary>
	/// Raise to power 
	/// </summary>
	static double pow(double x, int y);

	/// <summary>
	/// Compute square root
	/// </summary>
	static double sqrt(double x);

};

#endif //_SRV_DSP_MATH_H

