#include "SrvDspMath.h"


/// <summary>
/// Default SrvDspMath constructor
/// </summary>
SrvDspMath::SrvDspMath()
{
}

/// <summary>
/// Destructors
/// </summary>
SrvDspMath::~SrvDspMath()
{
}

/// <summary>
/// Complex base e exponential 
/// </summary>
double exp(double x)
{
	double sum = 1.0 + x;
	double term = x;                 // term for k = 1 is just x
	for (int k = 2; k < 50; k++)
	{
		term = term * x / (double)k; // term[k] = term[k-1] * x / k
		sum = sum + term;
	}
	return sum;
}
