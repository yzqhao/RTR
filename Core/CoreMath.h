
#ifndef _CORE_COREMATH_H_
#define _CORE_COREMATH_H_

#include <iostream>

#include "type.h"
#include "Vector3.h"
#include "Matrix.h"
#include "Vertex4D.h"

namespace RTR
{
	// 获取平移变换矩阵
	void GetTranslateMatrix44(Matrix &mat, f32 x, f32 y, f32 z);

	// 获取缩放变换矩阵
	void GetScaleMatrix44(Matrix &mat, f32 x, f32 y, f32 z);

	// 获取延X轴旋转变换矩阵
	// 旋转参数为角度, 非弧度
	void GetRotateMatrix44X(Matrix &mat, f32 degree);
	// 获取延Y轴旋转变换矩阵
	// 旋转参数为角度, 非弧度
	void GetRotateMatrix44Y(Matrix &mat, f32 degree);
	// 获取延Z轴旋转变换矩阵
	// 旋转参数为角度, 非弧度
	void GetRotateMatrix44Z(Matrix &mat, f32 degree);
	// 延任意轴旋转
	// 旋转参数为角度, 非弧度
	void GetRotateMatrix44(Matrix &mat, const vector3df &normal, f32 degree);

	void GetVertex4DAddVector4D(const Vertex4D &v, const vector3df& p, Vertex4D &result);

	// 3D向量X44矩阵
	void GetVector3DMulMatrix44(const vector3df& vec, const Matrix&mat, vector3df &result);

	void GetVertex4DMulMatrix44(const Vertex4D& vec, const Matrix&mat, Vertex4D &result);
}

#endif

