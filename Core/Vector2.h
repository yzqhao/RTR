
#ifndef _Core_VECTOR2_H_
#define _Core_VECTOR2_H_

#include "type.h"
#include <iostream>

namespace RTR
{
	template<class T>
	class Vector2
	{
	public:
		Vector2() :_x(0), _y(0) {};
		Vector2(T x, T y) :_x(x), _y(y){};
		Vector2(const T& vec) : _x(vec._x), _y(vec._y) {};
		
		Vector2<T> operator-() const { return Vector2<T>(-_x, -_y); }

		Vector2<T>& operator=(const Vector2<T>& other)	{ _x = other._x; _y = other._y; return *this; }

		Vector2<T> operator+(const Vector2<T>& other) const { return Vector2<T>(_x + other._x, _y + other._y); }
		Vector2<T>& operator+=(const Vector2<T>& other)	{ _x += other._x; _y += other._y; return *this; }

		Vector2<T> operator-(const Vector2<T>& other) const { return Vector2<T>(_x - other._x, _y - other._y); }
		Vector2<T>& operator-=(const Vector2<T>& other)	{ _x -= other._x; _y -= other._y; return *this; }

		Vector2<T> operator*(const Vector2<T>& other) const { return Vector2<T>(_x * other._x, _y * other._y); }
		Vector2<T>& operator*=(const Vector2<T>& other)	{ _x *= other._x; _y *= other._y; return *this; }
		Vector2<T> operator*(const T v) const { return Vector2<T>(_x * v, _y * v); }
		Vector2<T>& operator*=(const T v) { _x *= v; _y *= v; return *this; }

		bool operator==(const Vector2<T>& other) const { return other._x == _x && other._y == _y; }
		bool operator!=(const Vector2<T>& other) const { return other._x != _x || other.Y != _y; }
		

	public:
		void print()
		{
			std::cout << "x : " << _x << ", _y : " << _y << std::endl;
		}
	
		T _x;
		T _y;
	};

	typedef Vector2<s32> vector2di;
	typedef Vector2<f32> vector2df;
}

#endif

