
#ifndef _CORE_PLANE3d_H_
#define _CORE_PLANE3d_H_

#include "type.h"
#include "Vector3.h"

namespace RTR
{
	// 面
	struct Plane3D
	{
		vector3df	point;	// 面上一点
		vector3df	normal;	// 面的法线

		Plane3D(){}
		Plane3D(const vector3df &p, const vector3df &nor);
		Plane3D& operator=(const Plane3D &right);
	};

	inline Plane3D::Plane3D(const vector3df &p, const vector3df &nor) : point(p)
	{
		normal = nor;
		normal.normalize();
	}

	inline Plane3D& Plane3D::operator=(const Plane3D &right)
	{
		point = right.point;
		normal = right.normal;
		return *this;
	}
}

#endif