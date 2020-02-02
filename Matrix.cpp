#include "stdafx.h"
#include "Matrix.h"

/// <summary>
/// multiply matrix on the right 44 = 49*94
/// </summary>
Matrix4d operator *(Matrix49d &rhMatrix, const Matrix94d &other)
{
	Matrix4d temp;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 9; k++)
			{
				temp.m_Data[i][j] += rhMatrix.m_Data[i][k] * other.m_Data[k][j];
			}
		}
	}
	return temp;
}

/// <summary>
/// multiply matrix on the right 33 = 39*93
/// </summary>
Matrix3d operator *(Matrix39d &rhMatrix, const Matrix93d &other)
{
	Matrix3d temp;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			for (int k = 0; k < 9; k++)
			{
				temp.m_Data[i][j] += rhMatrix.m_Data[i][k] * other.m_Data[k][j];
			}
		}
	}
	return temp;
}

/// <summary>
/// multiply matrix on the right 49 = 44*49
/// </summary>
Matrix49d operator *(Matrix4d &rhMatrix, const Matrix49d &other)
{
	Matrix49d temp;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				temp.m_Data[i][j] += rhMatrix.m_Data[i][k] * other.m_Data[k][j];
			}
		}
	}
	return temp;
}

/// <summary>
/// multiply matrix on the right 39 = 33*39
/// </summary>
Matrix39d operator *(Matrix3d &rhMatrix, const Matrix39d &other)
{
	Matrix39d temp;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			for (int k = 0; k < 3; k++)
			{
				temp.m_Data[i][j] += rhMatrix.m_Data[i][k] * other.m_Data[k][j];
			}
		}
	}
	return temp;
}

/// <summary>
/// multiply matrix on the right 99 = 94*49
/// </summary>
Matrix9d operator *(Matrix94d &rhMatrix, const Matrix49d &other)
{
	Matrix9d temp;
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				temp.m_Data[i][j] += rhMatrix.m_Data[i][k] * other.m_Data[k][j];
			}
		}
	}
	return temp;
}

/// <summary>
/// multiply matrix on the right 99 = 93*39
/// </summary>
Matrix9d operator *(Matrix93d &rhMatrix, const Matrix39d &other)
{
	Matrix9d temp;
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			for (int k = 0; k < 3; k++)
			{
				temp.m_Data[i][j] += rhMatrix.m_Data[i][k] * other.m_Data[k][j];
			}
		}
	}
	return temp;
}

/// <summary>
/// multiply matrix on the right 94 = 99*94
/// </summary>
Matrix94d operator *(Matrix9d &rhMatrix, const Matrix94d &other)
{
	Matrix94d temp;
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 9; k++)
			{
				temp.m_Data[i][j] += rhMatrix.m_Data[i][k] * other.m_Data[k][j];
			}
		}
	}
	return temp;
}

/// <summary>
/// multiply matrix on the right 94 = 99*93
/// </summary>
Matrix93d operator *(Matrix9d &rhMatrix, const Matrix93d &other)
{
	Matrix93d temp;
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			for (int k = 0; k < 9; k++)
			{
				temp.m_Data[i][j] += rhMatrix.m_Data[i][k] * other.m_Data[k][j];
			}
		}
	}
	return temp;
}

/// <summary>
/// multiply matrix on the right 94 = 94*44
/// </summary>
Matrix94d operator *(Matrix94d &rhMatrix, const Matrix4d &other)
{
	Matrix94d temp;
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				temp.m_Data[i][j] += rhMatrix.m_Data[i][k] * other.m_Data[k][j];
			}
		}
	}
	return temp;
}

/// <summary>
/// multiply matrix on the right 93 = 93*33
/// </summary>
Matrix93d operator *(Matrix93d &rhMatrix, const Matrix3d &other)
{
	Matrix93d temp;
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			for (int k = 0; k < 3; k++)
			{
				temp.m_Data[i][j] += rhMatrix.m_Data[i][k] * other.m_Data[k][j];
			}
		}
	}
	return temp;
}

/// <summary>
/// multiply matrix on the right 44 = 42*24
/// </summary>
Matrix4f operator *(Matrix42f &rhMatrix, const Matrix24f &other)
{
	Matrix4f temp;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 2; k++)
			{
				temp.m_Data[i][j] += rhMatrix.m_Data[i][k] * other.m_Data[k][j];
			}
		}
	}
	return temp;
}

/// <summary>
/// multiply matrix on the right 49 = 49*99
/// </summary>
Matrix49d operator *(Matrix49d &rhMatrix, const Matrix9d &other)
{
	Matrix49d temp;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			for (int k = 0; k < 9; k++)
			{
				temp.m_Data[i][j] += rhMatrix.m_Data[i][k] * other.m_Data[k][j];
			}
		}
	}
	return temp;
}

/// <summary>
/// multiply matrix on the right 24 = 22*24
/// </summary>
Matrix24f operator *(Matrix2f &rhMatrix, const Matrix24f &other)
{
	Matrix24f temp;
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 2; k++)
			{
				temp.m_Data[i][j] += rhMatrix.m_Data[i][k] * other.m_Data[k][j];
			}
		}
	}
	return temp;
}

/// <summary>
/// multiply matrix on the right 24 = 24*44
/// </summary>
Matrix24f operator *(Matrix24f &rhMatrix, const Matrix4f &other)
{
	Matrix24f temp;
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				temp.m_Data[i][j] += rhMatrix.m_Data[i][k] * other.m_Data[k][j];
			}
		}
	}
	return temp;
}

/// <summary>
/// multiply matrix on the right 42 = 44*42
/// </summary>
Matrix42f operator *(Matrix4f &rhMatrix, const Matrix42f &other)
{
	Matrix42f temp;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				temp.m_Data[i][j] += rhMatrix.m_Data[i][k] * other.m_Data[k][j];
			}
		}
	}
	return temp;
}

/// <summary>
/// multiply matrix on the right 22 = 24*42
/// </summary>
Matrix2f operator *(Matrix24f &rhMatrix, const Matrix42f &other)
{
	Matrix2f temp;
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				temp.m_Data[i][j] += rhMatrix.m_Data[i][k] * other.m_Data[k][j];
			}
		}
	}
	return temp;
}

/// <summary>
/// multiply matrix on the right 44 = 41*14
/// </summary>
Matrix4f operator *(Vector4f &rhMatrix, const Matrix14f &other)
{
	Matrix4f temp;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 1; k++)
			{
				temp.m_Data[i][j] += rhMatrix.m_Data[i] * other.m_Data[k][j];
			}
		}
	}
	return temp;
}

/// <summary>
/// multiply matrix on the right 42 = 42*22
/// </summary>
Matrix42f operator *(Matrix42f &rhMatrix, const Matrix2f &other)
{
	Matrix42f temp;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			for (int k = 0; k < 2; k++)
			{
				temp.m_Data[i][j] += rhMatrix.m_Data[i][k] * other.m_Data[k][j];
			}
		}
	}
	return temp;
}

/// <summary>
/// multiply matrix on the right 21 = 24*41
/// </summary>
Vector2f operator *(Matrix24f &rhMatrix, const Vector4f &other)
{
	Vector2f temp;
	for (int i = 0; i < 2; i++)
	{
		for (int k = 0; k < 4; k++)
		{
			temp.m_Data[i] += rhMatrix.m_Data[i][k] * other.m_Data[k];
		}
	}
	return temp;
}

/// <summary>
/// multiply matrix on the right 91 = 94*41
/// </summary>
Vector9d operator *(Matrix94d &rhMatrix, const Vector4d &other)
{
	Vector9d temp;
	for (int i = 0; i < 9; i++)
	{
		for (int k = 0; k < 4; k++)
		{
			temp.m_Data[i] += rhMatrix.m_Data[i][k] * other.m_Data[k];
		}
	}
	return temp;
}

/// <summary>
/// multiply matrix on the right 91 = 93*31
/// </summary>
Vector9d operator *(Matrix93d &rhMatrix, const Vector3d &other)
{
	Vector9d temp;
	for (int i = 0; i < 9; i++)
	{
		for (int k = 0; k < 3; k++)
		{
			temp.m_Data[i] += rhMatrix.m_Data[i][k] * other.m_Data[k];
		}
	}
	return temp;
}

/// <summary>
/// multiply matrix on the right 41 = 49*91
/// </summary>
Vector4d operator *(Matrix49d &rhMatrix, const Vector9d &other)
{
	Vector4d temp;
	for (int i = 0; i < 4; i++)
	{
		for (int k = 0; k < 9; k++)
		{
			temp.m_Data[i] += rhMatrix.m_Data[i][k] * other.m_Data[k];
		}
	}
	return temp;
}

/// <summary>
/// multiply matrix on the right 41 = 42*21
/// </summary>
Vector4f operator *(Matrix42f &rhMatrix, const Vector2f &other)
{
	Vector4f temp;
	for (int i = 0; i < 4; i++)
	{
		for (int k = 0; k < 2; k++)
		{
			temp.m_Data[i] += rhMatrix.m_Data[i][k] * other.m_Data[k];
		}
	}
	return temp;
}

/// <summary>
/// Inverse Matrix2f
/// </summary>
Matrix2f Inverse2f(Matrix2f &matrix2f)
{
	Matrix2f temp;
	float determinant = matrix2f.Determinant();
	temp.m_Data[0][0] = matrix2f.m_Data[1][1] / determinant;
	temp.m_Data[0][1] = -matrix2f.m_Data[0][1] / determinant;
	temp.m_Data[1][0] = -matrix2f.m_Data[1][0] / determinant;
	temp.m_Data[1][1] = matrix2f.m_Data[0][0] / determinant;
	return temp;
}

/// <summary>
/// Transpose Matrix49f
/// </summary>
Matrix94f Transpose(Matrix49f &matrix49f)
{
	Matrix94f temp;
	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 4; j++)
			temp.m_Data[i][j] = matrix49f.m_Data[j][i];

	return temp;
}

/// <summary>
/// Transpose Matrix39d
/// </summary>
Matrix93d Transpose(Matrix39d &matrix39d)
{
	Matrix93d temp;
	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 3; j++)
			temp.m_Data[i][j] = matrix39d.m_Data[j][i];

	return temp;
}

/// <summary>
/// Transpose Matrix49d
/// </summary>
Matrix94d Transpose(Matrix49d &matrix49d)
{
	Matrix94d temp;
	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 4; j++)
			temp.m_Data[i][j] = matrix49d.m_Data[j][i];

	return temp;
}

/// <summary>
/// Transpose Matrix94f
/// </summary>
Matrix49f Transpose(Matrix94f &matrix94f)
{
	Matrix49f temp;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 9; j++)
			temp.m_Data[i][j] = matrix94f.m_Data[j][i];

	return temp;
}

/// <summary>
/// Transpose Matrix93d
/// </summary>
Matrix39d Transpose(Matrix93d &matrix93d)
{
	Matrix39d temp;
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 9; j++)
			temp.m_Data[i][j] = matrix93d.m_Data[j][i];

	return temp;
}

/// <summary>
/// Transpose Matrix94d
/// </summary>
Matrix49d Transpose(Matrix94d &matrix94f)
{
	Matrix49d temp;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 9; j++)
			temp.m_Data[i][j] = matrix94f.m_Data[j][i];

	return temp;
}

/// <summary>
/// Transpose Matrix42f
/// </summary>
Matrix24f Transpose(Matrix42f &matrix42f)
{
	Matrix24f temp;
	for (int i = 0; i < 2; i++)
		for (int j = 0; j < 4; j++)
			temp.m_Data[i][j] = matrix42f.m_Data[j][i];

	return temp;
}

/// <summary>
/// Transpose Matrix24f
/// </summary>
Matrix42f Transpose(Matrix24f &matrix24f)
{
	Matrix42f temp;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 2; j++)
			temp.m_Data[i][j] = matrix24f.m_Data[j][i];

	return temp;
}

/// <summary>
/// Transpose Vector4f
/// </summary>
Matrix14f Transpose(Vector4f &vector4f)
{
	Matrix14f temp;
	for (int i = 0; i < 1; i++)
		for (int j = 0; j < 4; j++)
			temp.m_Data[i][j] = vector4f.m_Data[j];

	return temp;
}

/// <summary>
/// Print matrix 4x2 
/// </summary>
void print42f(Matrix42f rhMatrix)
{
	for (int i = -0; i < 4; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			std::cout << rhMatrix.m_Data[i][j] << " ";
		}
		std::cout << std::endl;
	}
}
