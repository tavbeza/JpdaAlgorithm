#include "SrvDspMath.h"


/// <summary>
/// Empty constructor
/// </summary>
SrvDspMath::SrvDspMath()
{
}

/// <summary>
/// Destructor 
/// </summary>
SrvDspMath::~SrvDspMath()
{
}

/// <summary>
/// Compute exponential function
/// </summary>
double SrvDspMath::exp(double x)
{
	return ::exp(x);
}

/// <summary>
/// Compute arc tangent
/// </summary>
float SrvDspMath::atan(float a)
{
	return ::atan(a);
}

/// <summary>
/// Compute arc tangent with two parameters
/// </summary>
float SrvDspMath::atan2(float a, float b)
{
	return ::atan2(a, b);
}

/// <summary>
/// Compute arc sine 
/// </summary>
float SrvDspMath::asin(float number)
{
	return ::asin(number);
}

/// <summary>
/// Compute arc cos
/// </summary>
float SrvDspMath::acos(float number)
{
	return ::acos(number);
}

/// <summary>
/// Compute cos 
/// </summary>
float SrvDspMath::cos(float number)
{
	return ::cos(number);
}

/// <summary>
/// Compute sin 
/// </summary>
float SrvDspMath::sin(float number)
{
	return ::sin(number);
}

/// <summary>
/// Raise to power 
/// </summary>
double SrvDspMath::pow(double x, int y)
{
	double temp = ::pow(x, y);
	return temp;

	/*double temp;
	if (y == 0)
		return 1;
	temp = pow(x, y / 2);
	if ((y % 2) == 0) {
		return temp * temp;
	}
	else {
		if (y > 0)
			return x * temp * temp;
		else
			return (temp * temp) / x;*/
	//}
}

/// <summary>
/// Compute square root
/// </summary>
double SrvDspMath::sqrt(double x) {
	double temp = ::sqrt(x);
	return temp;
	/*const double EPS = 1e-6;
	double low = 0, high = x; // should be double not int here
	while (low + EPS < high) {  // gist, note this predicate! 
		double mid = (low + high) / 2;
		if (mid * mid > x) high = mid;
		else low = mid;
	}
	return low;            // it seems that returning'low' or 'high' will have the same results*/
}
