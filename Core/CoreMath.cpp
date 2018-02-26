
#include "CoreMath.h"
#include "Util.h"

// 0   1   2   3
// 4   5   6   7
// 8   9   10  11
// 12  13  14  15

namespace RTR
{
	void GetTranslateMatrix44(Matrix &mat, f32 _x, f32 _y, f32 _z)
	{
		mat._m[0] = 1, mat._m[1] = 0, mat._m[2] = 0, mat._m[3] = 0;
		mat._m[4] = 0, mat._m[5] = 1, mat._m[6] = 0, mat._m[7] = 0;
		mat._m[8] = 0, mat._m[9] = 0, mat._m[10] = 1, mat._m[11] = 0;
		mat._m[12] = _x, mat._m[13] = _y, mat._m[14] = _z, mat._m[15] = 1;
	}

	void GetScaleMatrix44(Matrix &mat, f32 _x, f32 _y, f32 _z)
	{
		mat._m[0] = _x, mat._m[1] = 0, mat._m[2] = 0, mat._m[3] = 0;
		mat._m[4] = 0, mat._m[5] = _y, mat._m[6] = 0, mat._m[7] = 0;
		mat._m[8] = 0, mat._m[9] = 0, mat._m[10] = _z, mat._m[11] = 0;
		mat._m[12] = 0, mat._m[13] = 0, mat._m[14] = 0, mat._m[15] = 1;
	}

	void GetRotateMatrix44X(Matrix &mat, f32 degree)
	{
		f32 sinA = sin(Degree_TO_Radian(degree));
		f32 cosA = cos(Degree_TO_Radian(degree));

		mat._m[0] = 1, mat._m[1] = 0, mat._m[2] = 0, mat._m[3] = 0;
		mat._m[4] = 0, mat._m[5] = cosA, mat._m[6] = sinA, mat._m[7] = 0;
		mat._m[8] = 0, mat._m[9] = -sinA, mat._m[10] = cosA, mat._m[11] = 0;
		mat._m[12] = 0, mat._m[13] = 0, mat._m[14] = 0, mat._m[15] = 1;
	}

	void GetRotateMatrix44Y(Matrix &mat, f32 degree)
	{
		f32 sinA = sin(Degree_TO_Radian(degree));
		f32 cosA = cos(Degree_TO_Radian(degree));

		mat._m[0] = cosA, mat._m[1] = 0, mat._m[2] = -sinA, mat._m[3] = 0;
		mat._m[4] = 0, mat._m[5] = 1, mat._m[6] = 0, mat._m[7] = 0;
		mat._m[8] = sinA, mat._m[9] = 0, mat._m[10] = cosA, mat._m[11] = 0;
		mat._m[12] = 0, mat._m[13] = 0, mat._m[14] = 0, mat._m[15] = 1;
	}
	
	void GetRotateMatrix44Z(Matrix &mat, f32 degree)
	{
		f32 sinA = sin(Degree_TO_Radian(degree));
		f32 cosA = cos(Degree_TO_Radian(degree));

		mat._m[0] = cosA, mat._m[1] = sinA, mat._m[2] = 0, mat._m[3] = 0;
		mat._m[4] = -sinA, mat._m[5] = cosA, mat._m[6] = 0, mat._m[7] = 0;
		mat._m[8] = 0, mat._m[9] = 0, mat._m[10] = 1, mat._m[11] = 0;
		mat._m[12] = 0, mat._m[13] = 0, mat._m[14] = 0, mat._m[15] = 1;
	}
	
	void GetRotateMatrix44(Matrix &mat, const vector3df &n, f32 degree)
	{
		f32 sinA = sin(Degree_TO_Radian(degree));
		f32 cosA = cos(Degree_TO_Radian(degree));

		f32 oneCosA = 1 - cosA;

		mat._m[0] = n._x * n._x * oneCosA + cosA, mat._m[1] = n._x * n._y * oneCosA + n._x * sinA, mat._m[2] = n._x * n._z * oneCosA - n._y * sinA, mat._m[3] = 0;
		mat._m[4] = n._x * n._y * oneCosA - n._z * sinA, mat._m[5] = n._y * n._y * oneCosA + cosA, mat._m[6] = n._y * n._z * oneCosA + n._x * sinA, mat._m[7] = 0;
		mat._m[8] = n._x * n._z * oneCosA + n._y * sinA, mat._m[9] = n._y * n._z * oneCosA - n._x * sinA, mat._m[10] = n._z * n._z * oneCosA + cosA, mat._m[11] = 0;
		mat._m[12] = 0, mat._m[13] = 0, mat._m[14] = 0, mat._m[15] = 1;
	}

	void GetVertex4DAddVector4D(const Vertex4D &v, const vector3df& p, Vertex4D &result)
	{
		f32 x = v._x + p._x;
		f32 y = v._y + p._y;
		f32 z = v._z + p._z;

		result._x = x, result._y = y, result._z = z;
	}

	void GetVector3DMulMatrix44(const vector3df& vec, const Matrix&mat, vector3df &result)
	{
		f32 x = vec._x * mat._m[0] + vec._y * mat._m[4] + vec._z * mat._m[8] + mat._m[12];
		f32 y = vec._x * mat._m[1] + vec._y * mat._m[5] + vec._z * mat._m[9] + mat._m[13];
		f32 z = vec._x * mat._m[2] + vec._y * mat._m[6] + vec._z * mat._m[10] + mat._m[14];

		result._x = x, result._y = y, result._z = z;
	}

	void GetVertex4DMulMatrix44(const Vertex4D& vec, const Matrix&mat, Vertex4D &result)
	{
		f32 x = vec._x * mat._m[0] + vec._y * mat._m[4] + vec._z * mat._m[8] + mat._m[12];
		f32 y = vec._x * mat._m[1] + vec._y * mat._m[5] + vec._z * mat._m[9] + mat._m[13];
		f32 z = vec._x * mat._m[2] + vec._y * mat._m[6] + vec._z * mat._m[10] + mat._m[14];
		f32 w = 1;												  

		result._x = x, result._y = y, result._z = z, result.w = 1;
	}
}

