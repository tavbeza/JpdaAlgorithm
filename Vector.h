#ifndef TRACKER_VECTOR_H_
#define TRACKER_VECTOR_H_

#include <fstream>

//#include "ServiceLayer\SrvDspMath.h"

//#include "InfraLayer/MONAGENT/Pack1Start.h"

template<class _T, unsigned int _Rows>
/// <summary>
/// A genaric vector class
/// </summary>
class Vector
{
public:
	/// <summary>
	/// Default Constructor for vector
	/// </summary>
	Vector()
	{
		SetZero();
	}

	/// <summary>
	/// 4d construcotr
	/// </summary>
	Vector(_T x, _T y, _T z, _T k)
	{
		m_Data[0] = x;
		m_Data[1] = y;
		m_Data[2] = z;
		m_Data[3] = k;
	}

	/// <summary>
	/// 3d construcotr
	/// </summary>
	Vector(_T x, _T y, _T z)
	{
		m_Data[0] = x;
		m_Data[1] = y;
		m_Data[2] = z;
	}

	/// <summary>
	/// 2d construcotr
	/// </summary>
	Vector(_T x, _T y)
	{
		m_Data[0] = x;
		m_Data[1] = y;
	}

	/// <summary>
	/// 5d construcotr
	/// </summary>
	Vector(_T x0, _T x1, _T x2, _T x3, _T x4)
	{
		m_Data[0] = x0;
		m_Data[1] = x1;
		m_Data[2] = x2;
		m_Data[3] = x3;
		m_Data[4] = x4;

	}

	/// <summary>
	/// Prints the vector information to a file.
	/// </summary>
	void PrintToFile(std::ofstream& fileStream) const
	{

		for (int i = 0; i < _Rows; i++)
		{
			fileStream << m_Data[i];
			if (i < _Rows - 1)
				fileStream << ",";
		}

		fileStream << endl; //std::endl ;

	}

	/// <summary>
	/// Set the array with 0 values
	/// </summary>
	void SetZero()
	{
		for (int i = 0; i < _Rows; i++)
			m_Data[i] = 0;
	}

	/// <summary>
	/// addition vector on the right
	/// </summary>
	Vector<_T, _Rows>  operator +(const Vector<_T, _Rows> &other) const
	{
		Vector<_T, _Rows> temp;
		for (int i = 0; i < _Rows; i++)
			temp.m_Data[i] = m_Data[i] + other.m_Data[i];
		return temp;
	}

	/// <summary>
	/// addition vector on the right
	/// </summary>
	Vector<_T, _Rows>  operator +=(const Vector<_T, _Rows> &other) const
	{
		for (int i = 0; i < _Rows; i++)
			this.m_Data[i] += other.m_Data[i];
		return *this;
	}

	/// <summary>
	/// substract vector on the right
	/// </summary>
	Vector<_T, _Rows>  operator -(const Vector<_T, _Rows> &other) const
	{
		Vector<_T, _Rows> temp;
		for (int i = 0; i < _Rows; i++)
			temp.m_Data[i] = m_Data[i] - other.m_Data[i];
		return temp;
	}

	/// <summary>
	/// substract vector on the right
	/// </summary>
	Vector<_T, _Rows>  operator -=(const Vector<_T, _Rows> &other) const
	{
		for (int i = 0; i < _Rows; i++)
			this.m_Data[i] -= other.m_Data[i];
		return *this;
	}

	/// <summary>
	/// Multiply vector on the right
	/// </summary>
	Vector<_T, _Rows>  operator *(const _T &value) const
	{
		Vector<_T, _Rows> temp;
		for (int i = 0; i < _Rows; i++)
			temp.m_Data[i] = m_Data[i] * value;
		return temp;
	}

	/// <summary>
	/// Multiply vector with constant
	/// </summary>
	Vector<_T, _Rows>  operator *=(const _T &value) const
	{
		for (int i = 0; i < _Rows; i++)
			this.m_Data[i] *= value;
		return *this;
	}

	/// <summary>
	/// Multiply vector on the right
	/// </summary>
	_T  operator *(const Vector<_T, _Rows> &value) const
	{
		_T temp = 0;
		for (int i = 0; i < _Rows; i++)
			temp += m_Data[i] * value.m_Data[i];
		return temp;
	}

	/// <summary>
	/// Overload function call
	/// </summary>
	_T operator()(unsigned int a)
	{
		return m_Data[a];
	}

	/// <summary>
	/// Convert spherical coordinate R, Az, El to cartesian  coordinate X,Y,Z
	/// </summary>
	void SphericalToCart(const Vector<_T, _Rows> &spherical)
	{
		m_Data[0] = spherical.m_Data[0] * SrvDspMath::sin(spherical.m_Data[2])*SrvDspMath::cos(spherical.m_Data[1]);
		m_Data[1] = spherical.m_Data[0] * SrvDspMath::sin(spherical.m_Data[2])*SrvDspMath::sin(spherical.m_Data[1]);
		if (spherical.m_Data[2] < 1.6 || spherical.m_Data[2] > 1.5)
		{
			m_Data[2] = 0;
		}
		else
		{
			m_Data[2] = spherical.m_Data[0] * SrvDspMath::cos(spherical.m_Data[2]);
		}
	}

	/// <summary>
	/// Convert spherical coordinate (velocity) R, Az, El to cartesian  coordinate VX,VY,VZ
	/// </summary>
	void SphericalToCartVelocity(const Vector<_T, _Rows> &spherical)
	{
		Vector3d cartesian;
		cartesian.SphericalToCart(spherical);

		double x = cartesian.m_Data[0];
		double y = cartesian.m_Data[1];
		double z = cartesian.m_Data[2];

		double xx = x*x;
		double yy = y*y;
		double xxyysq = SrvDspMath::sqrt(xx + yy);

		double r = spherical.m_Data[0];
		double rr = r*r;
		double sinTetha = SrvDspMath::sin(spherical.m_Data[1]);
		double sinPhi = SrvDspMath::sin(spherical.m_Data[2]);
		double cosTetha = SrvDspMath::cos(spherical.m_Data[1]);
		double cosPhi = SrvDspMath::cos(spherical.m_Data[2]);

		m_Data[0] = cosTetha * cosPhi * (x / r) + r * cosPhi * sinTetha * (-y / (xx + yy)) 
							+ r * sinPhi * cosTetha * ( (x*z) / (rr * xxyysq) );		// Vx
		m_Data[1] = cosPhi * sinTetha * (y / r) - r * cosPhi * cosTetha * (x / (xx + yy))
							+ r * sinPhi * sinTetha * ((y*z) / (rr * xxyysq));			// Vy
		m_Data[2] = sinPhi * (z / r) - r * cosPhi * (-xxyysq / rr);						// Vz
	}

	/// <summary>
	/// Convert cartesian coordinate X,Y,Z to spherical coordinate R, Az, El 
	/// </summary>
	void CartToSpherical(const Vector<_T, _Rows> &cartesian)
	{
		m_Data[0] = SrvDspMath::sqrt(SrvDspMath::pow(cartesian.m_Data[0], 2) +
			SrvDspMath::pow(cartesian.m_Data[1], 2) +
			SrvDspMath::pow(cartesian.m_Data[2], 2));

		// TODO: Unit Test //if(cartesian.m_Data[0] != 0)
		m_Data[1] = SrvDspMath::atan(cartesian.m_Data[1] / cartesian.m_Data[0]);
		m_Data[2] = SrvDspMath::acos(cartesian.m_Data[2] / m_Data[0]);
	}

	/// <summary>
	/// Convert cartesian velocity to R dot 
	/// </summary>
	void CartToSphericalVelocity(const Vector<_T, _Rows> &cartLocation, const Vector<_T, _Rows> &cartVelocity)
	{
		double x = cartLocation.m_Data[0];
		double y = cartLocation.m_Data[1];
		double z = cartLocation.m_Data[2];
		z = 0;
		double Vx = cartVelocity.m_Data[0];
		double Vy = cartVelocity.m_Data[1];
		double Vz = cartVelocity.m_Data[2];
		double r = SrvDspMath::sqrt(x*x + y * y + z * z);
		if (r == 0)
		{
			m_Data[0] = 0;
		}
		else
		{
			m_Data[0] = (x*Vx + y * Vy + z * Vz) / r;
		}
	}


	/// <summary>
	/// Error Convert spherical coordinate R, Az, El to cartesian  coordinate X,Y,Z
	/// </summary>
	void ErrorSphericalToCart(const Vector<_T, _Rows> &spherical, const Vector<_T, _Rows> &errorSpherical)
	{
		m_Data[0] = SrvDspMath::cos(spherical.m_Data[1])*SrvDspMath::sin(spherical.m_Data[2])
			*errorSpherical.m_Data[0] - spherical.m_Data[0]
			* SrvDspMath::sin(spherical.m_Data[1])
			*SrvDspMath::sin(spherical.m_Data[2])*errorSpherical.m_Data[1] + spherical.m_Data[0]
			* SrvDspMath::cos(spherical.m_Data[1])*SrvDspMath::cos(spherical.m_Data[2])
			*errorSpherical.m_Data[2];

		m_Data[1] = SrvDspMath::sin(spherical.m_Data[1])*SrvDspMath::sin(spherical.m_Data[2])*errorSpherical.m_Data[0]
			+ spherical.m_Data[0] * SrvDspMath::sin(spherical.m_Data[2])* SrvDspMath::cos(spherical.m_Data[1])
			* errorSpherical.m_Data[1] + spherical.m_Data[0] * SrvDspMath::sin(spherical.m_Data[1])
			* SrvDspMath::cos(spherical.m_Data[2])*errorSpherical.m_Data[2];

		if (spherical.m_Data[2] < 1.6 || spherical.m_Data[2] > 1.5)
		{
			m_Data[2] = 0;
		}
		else
		{
			m_Data[2] = SrvDspMath::cos(spherical.m_Data[2])*errorSpherical.m_Data[0] - spherical.m_Data[0]
				* SrvDspMath::sin(spherical.m_Data[2])*errorSpherical.m_Data[2];
		}
	}

	/// <summary>
	/// Error Convert cartesian  coordinate X,Y,Z to spherical coordinate R, Az, El
	/// </summary>
	void ErrorCartToSpherical(const Vector<_T, _Rows> &spherical, const Vector<_T, _Rows> &errorCartesian)
	{
		m_Data[0] = SrvDspMath::cos(spherical.m_Data[1])*SrvDspMath::sin(spherical.m_Data[2])
						*errorCartesian.m_Data[0] + SrvDspMath::sin(spherical.m_Data[1]) *
						SrvDspMath::sin(spherical.m_Data[2])*errorCartesian.m_Data[1] + SrvDspMath::cos(spherical.m_Data[2])
						*errorCartesian.m_Data[2];
			
		m_Data[1] = (-1 * SrvDspMath::sin(spherical.m_Data[1]) / (spherical.m_Data[0] * SrvDspMath::sin(spherical.m_Data[2])))
						* errorCartesian.m_Data[0]
						+ (SrvDspMath::cos(spherical.m_Data[1]) / (spherical.m_Data[0] * SrvDspMath::sin(spherical.m_Data[2])))
						* errorCartesian.m_Data[1];
			
		m_Data[2] = ((SrvDspMath::cos(spherical.m_Data[1]) * SrvDspMath::cos(spherical.m_Data[2])) / spherical.m_Data[0])
						* errorCartesian.m_Data[0]
						+ ((SrvDspMath::sin(spherical.m_Data[1]) * SrvDspMath::cos(spherical.m_Data[2])) / spherical.m_Data[0])
						* errorCartesian.m_Data[1]
						- ((SrvDspMath::sin(spherical.m_Data[2])) / spherical.m_Data[0])
						* errorCartesian.m_Data[2];

		if (spherical.m_Data[2] >= 1.5 && spherical.m_Data[2] <= 1.6)
		{
			m_Data[2] = 0;
		}
	}

	/// <summary>
	/// Return the size of the vector
	/// </summary>
	_T Norm() const
	{
		_T value = 0;
		_T sum = 0;
		for (int i = 0; i < _Rows; i++)
			sum += m_Data[i] * m_Data[i];

		value = SrvDspMath::sqrt(sum);
		return value;
	}

	_T m_Data[_Rows];
};
typedef Vector<float, 2> Vector2f;
typedef Vector<float, 3> Vector3f;
typedef Vector<float, 4> Vector4f;
typedef Vector<float, 5> Vector5f;
typedef Vector<float, 6> Vector6f;
typedef Vector<float, 9> Vector9f;

typedef Vector<double, 2> Vector2d;
typedef Vector<double, 3> Vector3d;
typedef Vector<double, 4> Vector4d;
typedef Vector<double, 5> Vector5d;
typedef Vector<double, 6> Vector6d;
typedef Vector<double, 9> Vector9d;

//#include "InfraLayer/MONAGENT/Pack1End.h"

#endif // TRACKER_VECTOR_H_
