
#ifndef _CORE_COREMATH_H_
#define _CORE_COREMATH_H_

#include <iostream>

#include "type.h"
#include "Vector3.h"
#include "Matrix.h"
#include "Vertex4D.h"

namespace RTR
{
	// ��ȡƽ�Ʊ任����
	void GetTranslateMatrix44(Matrix &mat, f32 x, f32 y, f32 z);

	// ��ȡ���ű任����
	void GetScaleMatrix44(Matrix &mat, f32 x, f32 y, f32 z);

	// ��ȡ��X����ת�任����
	// ��ת����Ϊ�Ƕ�, �ǻ���
	void GetRotateMatrix44X(Matrix &mat, f32 degree);
	// ��ȡ��Y����ת�任����
	// ��ת����Ϊ�Ƕ�, �ǻ���
	void GetRotateMatrix44Y(Matrix &mat, f32 degree);
	// ��ȡ��Z����ת�任����
	// ��ת����Ϊ�Ƕ�, �ǻ���
	void GetRotateMatrix44Z(Matrix &mat, f32 degree);
	// ����������ת
	// ��ת����Ϊ�Ƕ�, �ǻ���
	void GetRotateMatrix44(Matrix &mat, const vector3df &normal, f32 degree);

	void GetVertex4DAddVector4D(const Vertex4D &v, const vector3df& p, Vertex4D &result);

	// 3D����X44����
	void GetVector3DMulMatrix44(const vector3df& vec, const Matrix&mat, vector3df &result);

	void GetVertex4DMulMatrix44(const Vertex4D& vec, const Matrix&mat, Vertex4D &result);
}

#endif

