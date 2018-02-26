
#ifndef _CORE_VERTEX4D_H_
#define _CORE_VERTEX4D_H_

#include "type.h"
#include <iostream>

#include "Vector3.h"

namespace RTR
{
	// 顶点对象,包含顶点,法线, uv
	struct Vertex4D
	{
		f32 _x, _y, _z, w;

		f32 u, v;

		Vertex4D();
		Vertex4D(const vector3df &v);
		Vertex4D(f32 xf, f32 yf, f32 zf);
		Vertex4D operator *(const vector3df &right);
		Vertex4D operator -(const Vertex4D &right);
		Vertex4D& operator =(const Vertex4D &right);
		Vertex4D operator +(const vector3df &right);
		Vertex4D operator +(const Vertex4D &right);
		Vertex4D operator /(f32 factor);

		vector3df toVector4D() const;
	};

	inline Vertex4D::Vertex4D() : _x(0.0f), _y(0.0f), _z(0.0f), w(1.0f),
		u(0.0f), v(0.0f){}


	inline Vertex4D::Vertex4D(const vector3df &v) : _x(v._x), _y(v._y), _z(v._z), w(1),

		u(0.0f), v(0.0f){}

	inline Vertex4D::Vertex4D(f32 xf, f32 yf, f32 zf) : _x(xf), _y(yf), _z(zf), w(1),
		u(0.0f), v(0.0f){}

	inline Vertex4D Vertex4D::operator *(const vector3df &right)
	{
		Vertex4D temp = *this;
		temp._x *= right._x, temp._y *= right._y, temp._z *= right._z;
		return temp;
	}

	inline Vertex4D Vertex4D::operator -(const Vertex4D &right)
	{
		Vertex4D temp = *this;
		temp._x -= right._x, temp._y -= right._y, temp._z -= right._z;
		return temp;
	}

	inline Vertex4D Vertex4D::operator +(const vector3df &right)
	{
		Vertex4D temp = *this;
		temp._x += right._x, temp._y += right._y, temp._z += right._z;
		return temp;
	}

	inline Vertex4D Vertex4D::operator +(const Vertex4D &right)
	{
		Vertex4D temp = *this;
		temp._x += right._x, temp._y += right._y, temp._z += right._z;
		return temp;
	}

	inline Vertex4D Vertex4D::operator /(f32 factor)
	{
		Vertex4D temp = *this;
		temp._x /= factor, temp._y /= factor, temp._z /= factor;
		return temp;
	}

	inline Vertex4D& Vertex4D::operator =(const Vertex4D &right)
	{
		_x = right._x;	_y = right._y;	_z = right._z;	w = right.w;
		u = right.u;	v = right.v;
		return *this;
	}

	inline vector3df Vertex4D::toVector4D() const
	{
		return vector3df(_x, _y, _z);
	}

}

#endif

