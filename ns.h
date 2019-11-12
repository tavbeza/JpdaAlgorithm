#ifndef _NS_H
#define _NS_H
#include "stdafx.h"


namespace SrvDspMath {
	
	double exp(double x)
	{
		return ::exp(x);
	}

	float atan2(float a, float b)
	{
		return ::atan2(a,b);
	}

	float asin(float number)
	{
		return ::asin(number);
	}

	double pow(double x, int y)
	{
		double temp;
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
				return (temp * temp) / x;
		}
	}

	double sqrt(double x) {
		const double EPS = 1e-6;
		double low = 0, high = x; // should be double not int here
		while (low + EPS < high) {  // gist, note this predicate! 
			double mid = (low + high) / 2;
			if (mid * mid > x) high = mid;
			else low = mid;
		}
		return low;            // it seems that returning'low' or 'high' will have the same results
	}

}

#endif //_NS_H
