#ifndef TRACKER_MATRIX_H_
#define TRACKER_MATRIX_H_

//#include "ServiceLayer\SrvDspMath.h"
#include "Vector.h"
#include <iostream>
#include <iomanip>


template<class _T, unsigned int _Rows, unsigned int _Cols>
/// <summary>
/// A genaric matrix class
/// </summary>
class Matrix
{
public:
	/// <summary>
	/// Default matrix constructor
	/// </summary>
	Matrix()
	{
		for (int i = 0; i<_Rows; i++)
		{
			for (int j = 0; j<_Cols; j++)
			{
				m_Data[i][j] = 0;
			}
		}
	}

	/// <summary>
	/// Copy constructor
	/// </summary>
	Matrix(const Matrix<_T, _Rows, _Cols> &other)
	{
		for (int i = 0; i<_Rows; i++)
			for (int j = 0; j<_Cols; j++)
				m_Data[i][j] = other.m_Data[i][j];
	}

	/// <summary>
	/// Constructor that accept 2x2 array
	/// </summary>
	Matrix(_T *pData)
	{
		for (int i = 0; i<_Rows; i++)
		{
			for (int j = 0; j<_Cols; j++)
			{
				m_Data[i][j] = pData[i*_Rows + j];
			}
		}
	}

	/// <summary>
	/// Initialize 2x2 matrix
	/// </summary>
	//#if (_Rows == 2 && _Cols == 2)
	void Init(_T a11, _T a12, _T a21, _T a22)
	{
		if (_Rows < 2 && _Cols << 2)
			return;
		m_Data[0][0] = a11;
		m_Data[0][1] = a12;

		m_Data[1][0] = a21;
		m_Data[1][1] = a22;
	}
	//#endif

	/// <summary>
	/// Initialize 3x3 matrix
	/// </summary>
#if (_Rows == 3 && _Cols == 3)
	void Init(_T a11, _T a12, _T a13,
		_T a21, _T a22, _T a23,
		_T a31, _T a32, _T a33)
	{
		if (_Rows < 3 && _Cols << 3)
			return;
		m_Data[0][0] = a11;
		m_Data[0][1] = a12;
		m_Data[0][2] = a13;

		m_Data[1][0] = a21;
		m_Data[1][1] = a22;
		m_Data[1][2] = a23;

		m_Data[2][0] = a31;
		m_Data[2][1] = a32;
		m_Data[2][2] = a33;
	}
#endif



	/// <summary>
	/// Initialize 4x4 matrix
	/// </summary>
	void Init(_T a11, _T a12, _T a13, _T a14,
		_T a21, _T a22, _T a23, _T a24,
		_T a31, _T a32, _T a33, _T a34,
		_T a41, _T a42, _T a43, _T a44)
	{
		if (_Rows < 4 && _Cols << 4)
			return;
		m_Data[0][0] = a11;
		m_Data[0][1] = a12;
		m_Data[0][2] = a13;
		m_Data[0][3] = a14;

		m_Data[1][0] = a21;
		m_Data[1][1] = a22;
		m_Data[1][2] = a23;
		m_Data[1][3] = a24;

		m_Data[2][0] = a31;
		m_Data[2][1] = a32;
		m_Data[2][2] = a33;
		m_Data[2][3] = a34;

		m_Data[3][0] = a41;
		m_Data[3][1] = a42;
		m_Data[3][2] = a43;
		m_Data[3][3] = a44;
	}

	/// <summary>
	/// Return Identity matrix
	/// </summary>
	Matrix<_T, _Rows, _Cols> Identity()
	{
		_T tempData[_Rows][_Cols];
		for (int i = 0; i<_Rows; i++)
		{
			for (int j = 0; j<_Cols; j++)
			{
				if (i == j)
					tempData[i][j] = 1;
				else
					tempData[i][j] = 0;
			}
		}
		return Matrix((_T *)&tempData);
	}



	/// <summary>
	/// Return Identity matrix
	/// </summary>
	Matrix<_T, _Rows, _Cols> Inverse()
	{
		Matrix<_T, _Rows, _Cols> temp;
		_T determinant = Determinant();
		if(_Rows == 2 && _Cols == 2)
		{
			temp.m_Data[0][0] = m_Data[1][1] / determinant;
			temp.m_Data[0][1] = -m_Data[0][1] / determinant;
			temp.m_Data[1][0] = -m_Data[1][0] / determinant;
			temp.m_Data[1][1] = m_Data[0][0] / determinant;

		}
		else if(_Rows == 3 && _Cols == 3)
		{
			for (int i = 0; i < _Rows; i++)
			{
				for (int j = 0; j < _Cols; j++)
				{
					temp.m_Data[i][j] = (
						(m_Data[(j + 1) % 3][(i + 1) % 3] * m_Data[(j + 2) % 3][(i + 2) % 3]) -
						(m_Data[(j + 1) % 3][(i + 2) % 3] * m_Data[(j + 2) % 3][(i + 1) % 3])
						) / determinant;
				}
			}
		}
		else if(_Rows == 4 && _Cols == 4)

		return temp;
	}
	
	/// <summary>
	/// Return the determinant of the matrix
	/// </summary>
	float Determinant() const
	{
		float deter = 0;
		
		//if (_Rows != _Cols)
		//	return -1;

		switch (_Rows)
		{
		case 2: {
			deter = m_Data[0][0] * m_Data[1][1] - m_Data[0][1] * m_Data[1][0];
			break;
		}
		case 3: {
			_T determinant1 = (m_Data[1][1] * m_Data[2][2]) - (m_Data[2][1] * m_Data[1][2]);
			_T determinant2 = (m_Data[1][0] * m_Data[2][2]) - (m_Data[2][0] * m_Data[1][2]);
			_T determinant3 = (m_Data[1][0] * m_Data[2][1]) - (m_Data[2][0] * m_Data[1][1]);
			deter = (m_Data[0][0] * determinant1) - (m_Data[0][1] * determinant2);
			break;
		}
		case 4: {
			deter = m_Data[0][3] * m_Data[1][2] * m_Data[2][1] * m_Data[3][0] - m_Data[0][2] * m_Data[1][3] * m_Data[2][1] * m_Data[3][0] -
				m_Data[0][3] * m_Data[1][1] * m_Data[2][2] * m_Data[3][0] + m_Data[0][1] * m_Data[1][3] * m_Data[2][2] * m_Data[3][0] +
				m_Data[0][2] * m_Data[1][1] * m_Data[2][3] * m_Data[3][0] - m_Data[0][1] * m_Data[1][2] * m_Data[2][3] * m_Data[3][0] -
				m_Data[0][3] * m_Data[1][2] * m_Data[2][0] * m_Data[3][1] + m_Data[0][2] * m_Data[1][3] * m_Data[2][0] * m_Data[3][1] +
				m_Data[0][3] * m_Data[1][0] * m_Data[2][2] * m_Data[3][1] - m_Data[0][0] * m_Data[1][3] * m_Data[2][2] * m_Data[3][1] -
				m_Data[0][2] * m_Data[1][0] * m_Data[2][3] * m_Data[3][1] + m_Data[0][0] * m_Data[1][2] * m_Data[2][3] * m_Data[3][1] +
				m_Data[0][3] * m_Data[1][1] * m_Data[2][0] * m_Data[3][2] - m_Data[0][1] * m_Data[1][3] * m_Data[2][0] * m_Data[3][2] -
				m_Data[0][3] * m_Data[1][0] * m_Data[2][1] * m_Data[3][2] + m_Data[0][0] * m_Data[1][3] * m_Data[2][1] * m_Data[3][2] +
				m_Data[0][1] * m_Data[1][0] * m_Data[2][3] * m_Data[3][2] - m_Data[0][0] * m_Data[1][1] * m_Data[2][3] * m_Data[3][2] -
				m_Data[0][2] * m_Data[1][1] * m_Data[2][0] * m_Data[3][3] + m_Data[0][1] * m_Data[1][2] * m_Data[2][0] * m_Data[3][3] +
				m_Data[0][2] * m_Data[1][0] * m_Data[2][1] * m_Data[3][3] - m_Data[0][0] * m_Data[1][2] * m_Data[2][1] * m_Data[3][3] -
				m_Data[0][1] * m_Data[1][0] * m_Data[2][2] * m_Data[3][3] + m_Data[0][0] * m_Data[1][1] * m_Data[2][2] * m_Data[3][3];
			break;
		}
		default: {
			std::cout << "Problem in: Matrix.h --> float Determinant() const" << std::endl;
			exit(-1);
			break;
		}
		}

		return deter;

		/*
#if (_Rows == 2 && _Cols == 2)
		deter = m_Data[0][0] * m_Data[1][1] - m_Data[0][1] * m_Data[1][0];
#elif (_Rows == 3 && _Cols == 3)
		_T determinant1 = (m_Data[1][1] * m_Data[2][2]) - (m_Data[2][1] * m_Data[1][2]);
		_T determinant2 = (m_Data[1][0] * m_Data[2][2]) - (m_Data[2][0] * m_Data[1][2]);
		_T determinant3 = (m_Data[1][0] * m_Data[2][1]) - (m_Data[2][0] * m_Data[1][1]);
		deter = (m_Data[0][0] * determinant1) - (m_Data[0][1] * determinant2);
#elif (_Rows == 4 && _Cols == 4)
		deter = m_Data[0][3] * m_Data[1][2] * m_Data[2][1] * m_Data[3][0] - m_Data[0][2] * m_Data[1][3] * m_Data[2][1] * m_Data[3][0] -
			m_Data[0][3] * m_Data[1][1] * m_Data[2][2] * m_Data[3][0] + m_Data[0][1] * m_Data[1][3] * m_Data[2][2] * m_Data[3][0] +
			m_Data[0][2] * m_Data[1][1] * m_Data[2][3] * m_Data[3][0] - m_Data[0][1] * m_Data[1][2] * m_Data[2][3] * m_Data[3][0] -
			m_Data[0][3] * m_Data[1][2] * m_Data[2][0] * m_Data[3][1] + m_Data[0][2] * m_Data[1][3] * m_Data[2][0] * m_Data[3][1] +
			m_Data[0][3] * m_Data[1][0] * m_Data[2][2] * m_Data[3][1] - m_Data[0][0] * m_Data[1][3] * m_Data[2][2] * m_Data[3][1] -
			m_Data[0][2] * m_Data[1][0] * m_Data[2][3] * m_Data[3][1] + m_Data[0][0] * m_Data[1][2] * m_Data[2][3] * m_Data[3][1] +
			m_Data[0][3] * m_Data[1][1] * m_Data[2][0] * m_Data[3][2] - m_Data[0][1] * m_Data[1][3] * m_Data[2][0] * m_Data[3][2] -
			m_Data[0][3] * m_Data[1][0] * m_Data[2][1] * m_Data[3][2] + m_Data[0][0] * m_Data[1][3] * m_Data[2][1] * m_Data[3][2] +
			m_Data[0][1] * m_Data[1][0] * m_Data[2][3] * m_Data[3][2] - m_Data[0][0] * m_Data[1][1] * m_Data[2][3] * m_Data[3][2] -
			m_Data[0][2] * m_Data[1][1] * m_Data[2][0] * m_Data[3][3] + m_Data[0][1] * m_Data[1][2] * m_Data[2][0] * m_Data[3][3] +
			m_Data[0][2] * m_Data[1][0] * m_Data[2][1] * m_Data[3][3] - m_Data[0][0] * m_Data[1][2] * m_Data[2][1] * m_Data[3][3] -
			m_Data[0][1] * m_Data[1][0] * m_Data[2][2] * m_Data[3][3] + m_Data[0][0] * m_Data[1][1] * m_Data[2][2] * m_Data[3][3];
#endif
*/
		//return deter;

	}

	/// <summary>
	/// Return Zero matrix
	/// </summary>
	static Matrix<_T, _Rows, _Cols> Zero()
	{
		_T tempData[_Rows][_Cols];
		for (int i = 0; i<_Rows; i++)
		{
			for (int j = 0; j<_Cols; j++)
			{
				tempData[i][j] = 0;
			}
		}
		return Matrix((_T *)&tempData);
	}

	/// <summary>
	/// Print matrix
	/// </summary>
	void Print()
	{
		for (int i = 0; i<_Rows; i++)
		{
			for (int j = 0; j<_Cols; j++)
			{
				std::cout << m_Data[i][j] << " ";
			}
			std::cout << std::endl;
		}
	}


	/// <summary>
	/// Returns element i,j
	/// </summary>
	_T get(int i, int j)
	{
		return m_Data[i][j];
	}

	/// <summary>
	/// Sets element i,j
	/// </summary>
	void set(int i, int j, _T e)
	{
		m_Data[i][j] = e;
	}

	/// <summary>
	/// overloading operator +
	/// </summary>
	Matrix<_T, _Rows, _Cols>  operator+(const Matrix<_T, _Rows, _Cols> &other)const
	{
		Matrix<_T, _Rows, _Cols> temp;
		for (int i = 0; i<_Rows; i++)
			for (int j = 0; j<_Cols; j++)
				temp.m_Data[i][j] = m_Data[i][j] + other.m_Data[i][j];
		return temp;
	}

	/// <summary>
	/// overloading operator +=
	/// </summary>
	Matrix<_T, _Rows, _Cols>  operator+=(const Matrix<_T, _Rows, _Cols> &other)
	{
		for (int i = 0; i<_Rows; i++)
			for (int j = 0; j<_Cols; j++)
				m_Data[i][j] = m_Data[i][j] + other.m_Data[i][j];
		return *this;
	}

	/// <summary>
	/// overloading operator -
	/// </summary>
	Matrix<_T, _Rows, _Cols>  operator-(const Matrix<_T, _Rows, _Cols> &other)const
	{
		Matrix<_T, _Rows, _Cols> temp;
		for (int i = 0; i<_Rows; i++)
			for (int j = 0; j<_Cols; j++)
				temp.m_Data[i][j] = m_Data[i][j] - other.m_Data[i][j];
		return temp;
	}

	/// <summary>
	/// overloading operator -=
	/// </summary>
	Matrix<_T, _Rows, _Cols>  operator-=(const Matrix<_T, _Rows, _Cols> &other)const
	{
		for (int i = 0; i<_Rows; i++)
			for (int j = 0; j<_Cols; j++)
				this.m_Data[i][j] -= other.m_Data[i][j];
		return *this;
	}

	/// <summary>
	/// overloading operator =
	/// </summary>
	void operator=(const Matrix<_T, _Rows, _Cols> &other)
	{
		for (int i = 0; i<_Rows; i++)
			for (int j = 0; j<_Cols; j++)
				m_Data[i][j] = other.m_Data[i][j];
	}

	/// <summary>
	/// overloading operator -
	/// </summary>
	Matrix<_T, _Rows, _Cols>  operator-()const
	{
		Matrix<_T, _Rows, _Cols> temp();
		for (int i = 0; i<_Rows; i++)
			for (int j = 0; j<_Cols; j++)
				temp.m_Data[i][j] = m_Data[i][j] * -1;
		return temp;
	}

	/// <summary>
	/// overloading operator *
	/// </summary>
	Matrix<_T, _Rows, _Cols>  operator*(const _T value) const
	{
		Matrix<_T, _Rows, _Cols> temp;
		for (int i = 0; i<_Rows; i++)
			for (int j = 0; j<_Cols; j++)
				temp.m_Data[i][j] = m_Data[i][j] * value;
		return temp;
	}

	/// <summary>
	/// multiplay matrix on the right
	/// </summary>
	Matrix<_T, _Rows, _Cols>  operator *(const Matrix<_T, _Rows, _Cols> &other) const
	{
		Matrix<_T, _Rows, _Cols> temp;
		for (int i = 0; i < _Rows; i++)
			for (int j = 0; j < _Cols; j++)
				for (int k = 0; k < _Cols; k++)
					temp.m_Data[i][j] += m_Data[i][k] * other.m_Data[k][j];

		return temp;
	}


	/// <summary>
	/// multiplay matrix on the right
	/// </summary>
	Vector3d  operator *(const Vector3d &v) const
	{
		Vector3d temp;
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
				temp.m_Data[i] += m_Data[i][j] * v.m_Data[j];

		}
		return temp;
	}

	/// <summary>
	/// multiplay matrix on the right
	/// </summary>
	Vector4d  operator *(const Vector4d &v) const
	{
		Vector4d temp;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
				temp.m_Data[i] += m_Data[i][j] * v.m_Data[j];

		}

		return temp;
	}

	/// <summary>
	/// multiplay matrix on the right
	/// </summary>
	Vector4f  operator *(const Vector4f &v) const
	{
		Vector4f temp;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
				temp.m_Data[i] += m_Data[i][j] * v.m_Data[j];

		}

		return temp;
	}

	/// <summary>
	/// multiplay matrix on the right
	/// </summary>
	Vector9d  operator *(const Vector9d &v) const
	{
		Vector9d temp;
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
				temp.m_Data[i] += m_Data[i][j] * v.m_Data[j];

		}
		return temp;
	}

	/// <summary>
	/// Return the transpose matrix
	/// </summary>
#if (_Rows == _Cols)
	Matrix<_T, _Rows, _Cols> Transpose()
	{
		Matrix<_T, _Rows, _Cols> temp;
		for (int i = 0; i < _Rows; i++)
			for (int j = 0; j < _Cols; j++)
				temp.m_Data[j][i] = m_Data[i][j];
		return temp;
	}
#endif

	/// <summary>
	/// Print matrix to file
	/// </summary>
	void PrintToFile(std::ofstream& kalmanFile) const
	{
		
		for (int i = 0; i < _Rows; i++)
		{
			for (int j = 0; j < _Cols; j++)
			{
				kalmanFile << std::fixed << std::setprecision(8) << m_Data[i][j] << " ";
			}
			kalmanFile << std::endl;
		}
		
	}

	/*
	// overload function call
	_T& operator()(_T a)
	{
	for(int i=0; i < _Rows; i++)
	for(int j = 0; j < _Cols; j++)
	m_Data[j][i] = a;
	return a;
	}
	*/
	_T m_Data[_Rows][_Cols];
	//private:
	//unsigned int m_nRows;
	//unsigned int m_nCols;
};


//Changing the value of Dynamic breaks the ABI, as Dynamic is often used as a template parameter for Matrix.
//extern const int Dynamic = -1;
//typedef Matrix<float, Dynamic, Dynamic> MatrixXf;
//typedef Matrix<float, Dynamic, 1> VectorXf;

typedef Matrix<float, 2, 2> Matrix2f;
typedef Matrix<float, 3, 3> Matrix3f;
typedef Matrix<float, 4, 4> Matrix4f;
typedef Matrix<float, 4, 2> Matrix42f;
typedef Matrix<float, 1, 4> Matrix14f;
typedef Matrix<float, 2, 4> Matrix24f;
typedef Matrix<float, 9, 9> Matrix9f;
typedef Matrix<float, 9, 4> Matrix94f;
typedef Matrix<float, 4, 9> Matrix49f;
typedef Matrix<double, 2, 2> Matrix2d;
typedef Matrix<double, 3, 3> Matrix3d;
typedef Matrix<double, 4, 4> Matrix4d;
typedef Matrix<double, 9, 9> Matrix9d;
typedef Matrix<double, 9, 4> Matrix94d;
typedef Matrix<double, 4, 9> Matrix49d;

/// <summary>
/// Inverse Matrix2f
/// </summary>
Matrix2f Inverse2f(Matrix2f &matrix2f);

/// <summary>
/// Transpose Matrix49f
/// </summary>
Matrix94f Transpose(Matrix49f &matrix49f);

/// <summary>
/// Transpose Matrix94f
/// </summary>
Matrix49f Transpose(Matrix94f &Matrix94f);

/// <summary>
/// Transpose Matrix42f
/// </summary>
Matrix24f Transpose(Matrix42f &matrix42f);

/// <summary>
/// Transpose Matrix24f
/// </summary>
Matrix42f Transpose(Matrix24f &matrix24f);

/// <summary>
/// Transpose Matrix49d
/// </summary>
Matrix94d Transpose(Matrix49d &matrix49d);

/// <summary>
/// Transpose Matrix94d
/// </summary>
Matrix49d Transpose(Matrix94d &matrix94f);

/// <summary>
/// Transpose Vector4f
/// </summary>
Matrix14f Transpose(Vector4f &vector4f);

/// <summary>
/// multiply matrix on the right 44 = 49*94
/// </summary>
Matrix4d operator *(Matrix49d &rhMatrix, const Matrix94d &other);

/// <summary>
/// multiply matrix on the right 24 = 22*24
/// </summary>
Matrix24f operator *(Matrix2f &rhMatrix, const Matrix24f &other);

/// <summary>
/// multiply matrix on the right 24 = 24*44
/// </summary>
Matrix24f operator *(Matrix24f &rhMatrix, const Matrix4f &other);

/// <summary>
/// multiply matrix on the right 44 = 42*24
/// </summary>
Matrix4f operator *(Matrix42f &rhMatrix, const Matrix24f &other);

/// <summary>
/// multiply matrix on the right 42 = 44*42
/// </summary>
Matrix42f operator *(Matrix4f &rhMatrix, const Matrix42f &other);

/// <summary>
/// multiply matrix on the right 22 = 24*42
/// </summary>
Matrix2f operator *(Matrix24f &rhMatrix, const Matrix42f &other);

/// <summary>
/// multiply matrix on the right 44 = 41*14
/// </summary>
Matrix4f operator *(Vector4f &rhMatrix, const Matrix14f &other);

/// <summary>
/// multiply matrix on the right 42 = 42*22
/// </summary>
Matrix42f operator *(Matrix42f &rhMatrix, const Matrix2f &other);

/// <summary>
/// multiply matrix on the right 49 = 44*49
/// </summary>
Matrix49d operator *(Matrix4d &rhMatrix, const Matrix49d &other);

/// <summary>
/// multiply matrix on the right 99 = 94*49
/// </summary>
Matrix9d operator *(Matrix94d &rhMatrix, const Matrix49d &other);

/// <summary>
/// multiply matrix on the right 94 = 99*94
/// </summary>
Matrix94d operator *(Matrix9d &rhMatrix, const Matrix94d &other);

/// <summary>
/// multiply matrix on the right 94 = 94*44
/// </summary>
Matrix94d operator *(Matrix94d &rhMatrix, const Matrix4d &other);

/// <summary>
/// multiply matrix on the right 21 = 24*41
/// </summary>
Vector2f operator *(Matrix24f &rhMatrix, const Vector4f &other);

/// <summary>
/// multiply matrix on the right 91 = 94*41
/// </summary>
Vector9d operator *(Matrix94d &rhMatrix, const Vector4d &other);

/// <summary>
/// multiply matrix on the right 41 = 49*91
/// </summary>
Vector4d operator *(Matrix49d &rhMatrix, const Vector9d &other);

/// <summary>
/// multiply matrix on the right 41 = 42*21
/// </summary>
Vector4f operator *(Matrix42f &rhMatrix, const Vector2f &other);

/// <summary>
/// multiply matrix on the right 49 = 49*99
/// </summary>
Matrix49d operator *(Matrix49d &rhMatrix, const Matrix9d &other);

/// <summary>
/// print float matrix 42
/// </summary>
void print42f(Matrix42f rhMatrix);

/*
/// <summary>
/// multiply float with matrix 42 = f*42
/// </summary>
Matrix42f operator *(float f, const Matrix42f &other);

/// <summary>
/// multiply float with vector 42 = f*42
/// </summary>
Matrix42f operator *(float f, const Matrix42f &other);*/

#endif // TRACKER_MATRIX_H_
