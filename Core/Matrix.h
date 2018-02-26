
#ifndef _CORE_MATRIX_H_
#define _CORE_MATRIX_H_

#include "type.h"
#include <cstdio>

namespace RTR
{
	class Matrix
	{
	public:
		Matrix();
		Matrix(f32 m0_0, f32 m0_1, f32 m0_2, f32 m0_3,
			f32 m1_0, f32 m1_1, f32 m1_2, f32 m1_3,
			f32 m2_0, f32 m2_1, f32 m2_2, f32 m2_3,
			f32 m3_0, f32 m3_1, f32 m3_2, f32 m3_3);
		
		void makeIdentity();
		f32& operator()(s32 row, s32 col) { return _m[col * 4 + row]; }
		const f32& operator()(s32 row, s32 col) const { return _m[col * 4 + row]; }

		Matrix& operator=(const Matrix &other);
		bool operator==(const Matrix &other) const;
		bool operator!=(const Matrix &other) const;
		Matrix& operator*=(const Matrix& other);
		Matrix operator*(const Matrix& other) const;

		Matrix Matrix::getTransposed();

	public:
		void print()
		{
			printf("matrix : \n");
			for (s32 i = 0; i < 16; ++i)
			{
				printf("%10.6f", _m[i]);
				if (i % 4 == 3)
				{
					printf("\n");
				}
				else
				{
					printf(", ");
				}
			}
		}
	public:
		f32 _m[16];
	};

	inline Matrix::Matrix()
	{
		makeIdentity();
	}

	inline Matrix::Matrix(f32 m0_0, f32 m0_1, f32 m0_2, f32 m0_3,
		f32 m1_0, f32 m1_1, f32 m1_2, f32 m1_3,
		f32 m2_0, f32 m2_1, f32 m2_2, f32 m2_3,
		f32 m3_0, f32 m3_1, f32 m3_2, f32 m3_3)
	{
		_m[0] = m0_0, _m[1] = m0_1, _m[2] = m0_2, _m[3] = m0_3;
		_m[4] = m1_0, _m[5] = m1_1, _m[6] = m1_2, _m[7] = m1_3;
		_m[8] = m2_0, _m[9] = m2_1, _m[10] = m2_2, _m[11] = m2_3;
		_m[12] = m3_0, _m[13] = m3_1, _m[14] = m3_2, _m[15] = m3_3;
	}

	inline void Matrix::makeIdentity()
	{
		for (s32 i = 0; i < 16; ++i)
			_m[i] = 0.0f;
		_m[0] = _m[5] = _m[10] = _m[15] = 1;
	}

	inline Matrix& Matrix::operator = (const Matrix &other)
	{
		for (s32 i = 0; i < 16; ++i)
		{
			_m[i] = other._m[i];
		}
		return *this;
	}

	inline bool Matrix::operator==(const Matrix &other) const
	{
		for (s32 i = 0; i < 16; ++i)
		{
			if (_m[i] != other._m[i])
			{
				return false;
			}
		}
		return true;
	}

	inline bool Matrix::operator!=(const Matrix &other) const
	{
		return !(*this == other);
	}

	inline Matrix& Matrix::operator*=(const Matrix& other)
	{
		f32 newMatrix[16];
		const f32* m1 = _m;
		const f32* m2 = other._m;

		// 0   1   2   3
		// 4   5   6   7
		// 8   9   10  11
		// 12  13  14  15
		newMatrix[0] = m1[0] * m2[0] + m1[4] * m2[1] + m1[8] * m2[2] + m1[12] * m2[3];
		newMatrix[1] = m1[1] * m2[0] + m1[5] * m2[1] + m1[9] * m2[2] + m1[13] * m2[3];
		newMatrix[2] = m1[2] * m2[0] + m1[6] * m2[1] + m1[10] * m2[2] + m1[14] * m2[3];
		newMatrix[3] = m1[3] * m2[0] + m1[7] * m2[1] + m1[11] * m2[2] + m1[15] * m2[3];

		newMatrix[4] = m1[0] * m2[4] + m1[4] * m2[5] + m1[8] * m2[6] + m1[12] * m2[7];
		newMatrix[5] = m1[1] * m2[4] + m1[5] * m2[5] + m1[9] * m2[6] + m1[13] * m2[7];
		newMatrix[6] = m1[2] * m2[4] + m1[6] * m2[5] + m1[10] * m2[6] + m1[14] * m2[7];
		newMatrix[7] = m1[3] * m2[4] + m1[7] * m2[5] + m1[11] * m2[6] + m1[15] * m2[7];

		newMatrix[8] = m1[0] * m2[8] + m1[4] * m2[9] + m1[8] * m2[10] + m1[12] * m2[11];
		newMatrix[9] = m1[1] * m2[8] + m1[5] * m2[9] + m1[9] * m2[10] + m1[13] * m2[11];
		newMatrix[10] = m1[2] * m2[8] + m1[6] * m2[9] + m1[10] * m2[10] + m1[14] * m2[11];
		newMatrix[11] = m1[3] * m2[8] + m1[7] * m2[9] + m1[11] * m2[10] + m1[15] * m2[11];

		newMatrix[12] = m1[0] * m2[12] + m1[4] * m2[13] + m1[8] * m2[14] + m1[12] * m2[15];
		newMatrix[13] = m1[1] * m2[12] + m1[5] * m2[13] + m1[9] * m2[14] + m1[13] * m2[15];
		newMatrix[14] = m1[2] * m2[12] + m1[6] * m2[13] + m1[10] * m2[14] + m1[14] * m2[15];
		newMatrix[15] = m1[3] * m2[12] + m1[7] * m2[13] + m1[11] * m2[14] + m1[15] * m2[15];

		for (s32 i = 0; i < 16; ++i)
		{
			_m[i] = newMatrix[i];
		}

		return *this;
	}

	inline Matrix Matrix::operator*(const Matrix& other) const
	{
		Matrix tmtrx;
		const f32* m1 = _m;
		const f32* m2 = other._m;
		f32* m3 = tmtrx._m;

		// 0   1   2   3
		// 4   5   6   7
		// 8   9   10  11
		// 12  13  14  15
		m3[0] = m1[0] * m2[0] + m1[4] * m2[1] + m1[8] * m2[2] + m1[12] * m2[3];
		m3[1] = m1[1] * m2[0] + m1[5] * m2[1] + m1[9] * m2[2] + m1[13] * m2[3];
		m3[2] = m1[2] * m2[0] + m1[6] * m2[1] + m1[10] * m2[2] + m1[14] * m2[3];
		m3[3] = m1[3] * m2[0] + m1[7] * m2[1] + m1[11] * m2[2] + m1[15] * m2[3];

		m3[4] = m1[0] * m2[4] + m1[4] * m2[5] + m1[8] * m2[6] + m1[12] * m2[7];
		m3[5] = m1[1] * m2[4] + m1[5] * m2[5] + m1[9] * m2[6] + m1[13] * m2[7];
		m3[6] = m1[2] * m2[4] + m1[6] * m2[5] + m1[10] * m2[6] + m1[14] * m2[7];
		m3[7] = m1[3] * m2[4] + m1[7] * m2[5] + m1[11] * m2[6] + m1[15] * m2[7];

		m3[8] = m1[0] * m2[8] + m1[4] * m2[9] + m1[8] * m2[10] + m1[12] * m2[11];
		m3[9] = m1[1] * m2[8] + m1[5] * m2[9] + m1[9] * m2[10] + m1[13] * m2[11];
		m3[10] = m1[2] * m2[8] + m1[6] * m2[9] + m1[10] * m2[10] + m1[14] * m2[11];
		m3[11] = m1[3] * m2[8] + m1[7] * m2[9] + m1[11] * m2[10] + m1[15] * m2[11];

		m3[12] = m1[0] * m2[12] + m1[4] * m2[13] + m1[8] * m2[14] + m1[12] * m2[15];
		m3[13] = m1[1] * m2[12] + m1[5] * m2[13] + m1[9] * m2[14] + m1[13] * m2[15];
		m3[14] = m1[2] * m2[12] + m1[6] * m2[13] + m1[10] * m2[14] + m1[14] * m2[15];
		m3[15] = m1[3] * m2[12] + m1[7] * m2[13] + m1[11] * m2[14] + m1[15] * m2[15];

		return tmtrx;
	}

	//! returns transposed matrix
	inline Matrix Matrix::getTransposed()
	{
		Matrix t;
		for (s32 r = 0; r < 4; ++r)
		for (s32 c = 0; c < 4; ++c)
			t(r, c) = (*this)(c, r);
		return t;
	}
}

#endif

