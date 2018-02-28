
#ifndef _Core_VECTOR3_H_
#define _Core_VECTOR3_H_

#include "type.h"
#include <iostream>
#include <cmath>

namespace RTR
{
	template<class T>
	class Vector3
	{
	public:
		Vector3() :_x(0), _y(0), _z(0) {};
		Vector3(T x, T y, T z) :_x(x), _y(y), _z(z){};
		Vector3(const T& vec) : _x(vec._x), _y(vec._y), _z(vec._z) {};
		
		Vector3<T> operator-() const { return Vector3<T>(-_x, -_y, -_z); }

		Vector3<T>& operator=(const Vector3<T>& other)	{ _x = other._x; _y = other._y; _z = other._z; return *this; }

		Vector3<T> operator+(const Vector3<T>& other) const { return Vector3<T>(_x + other._x, _y + other._y, _z + other._z); }
		Vector3<T>& operator+=(const Vector3<T>& other)	{ _x += other._x; _y += other._y; _z += other._z; return *this; }

		Vector3<T> operator-(const Vector3<T>& other) const { return Vector3<T>(_x - other._x, _y - other._y, _z - other._z); }
		Vector3<T>& operator-=(const Vector3<T>& other)	{ _x -= other._x; _y -= other._y; _z -= other._z; return *this; }

		Vector3<T> operator*(const Vector3<T>& other) const { return Vector3<T>(_x * other._x, _y * other._y, _z * other._z); }
		Vector3<T>& operator*=(const Vector3<T>& other)	{ _x *= other._x; _y *= other._y; _z *= other._z; return *this; }
		Vector3<T> operator*(const T v) const { return Vector3<T>(_x * v, _y * v, _z * v); }
		Vector3<T>& operator*=(const T v) { _x *= v; _y *= v; _z *= v; return *this; }

		bool operator==(const Vector3<T>& other) const { return other._x == _x && other._y == _y && other._z == _z;}
		bool operator!=(const Vector3<T>& other) const { return other._x != _x || other._y != _y || other._z != _z;}
		
		//! Returns length of the vector.
		f64 getLength() const { return sqrt(_x*_x + _y*_y + _z*_z); }

		//! Returns squared length of the vector.
		/** This is useful because it is much faster then getLength(). */
		f64 getLengthSQ() const { return _x*_x + _y*_y + _z*_z; }

		//! Calculates the cross product with another vector
		Vector3<T> crossProduct(const Vector3<T>& p) const
		{
			return Vector3<T>(_y * p._z - _z * p._y, _z * p._x - _x * p._z, _x * p._y - _y * p._x);
		}

		//! Returns the dot product with another vector.
		T dotProduct(const Vector3<T>& other) const
		{
			return _x*other._x + _y*other._y + _z*other._z;
		}

		inline f32 angleBetween(const Vector3<T>&right) const
		{
			f32 lenProduct = getLength() * right.getLength();

			if (lenProduct < (f32)(1E-6))
				lenProduct = (f32)(1E-6);

			f32 f = dotProduct(right) / lenProduct;
			return acos(f);
		}
		
		//! Normalizes the vector.
		Vector3<T>& normalize();

	public:
		void print()
		{
			std::cout << "x : " << _x << ", _y : " << _y << ", _z : " << _z << std::endl;
		}
	
		T _x;
		T _y;
        T _z;
	};

	//! Normalizes the vector.
	template<class T>
	Vector3<T>& Vector3<T>::normalize()
	{
		T l = (T)getLength();
		if (l == 0)
			return *this;

		l = (T)1.0 / l;
		_x *= l;
		_y *= l;
		_z *= l;
		return *this;
	}

	typedef Vector3<s32> vector3di;
	typedef Vector3<f32> vector3df;
}

#endif

