#pragma once
#ifndef _CORE_FRUSTUM_H_
#define _CORE_FRUSTUM_H_

#include "type.h"
#include "Vector3.h"
#include "Matrix.h"
#include "Plane3d.h"
#include "Util.h"

namespace RTR
{
	enum CAMERA_TYPE
	{
		CAMERA_MODEL_ELUA = 0x0001,	// ŷ�������ģ��
		CAMERA_MODEL_UVN = 0x0002,	// UVN�����ģ��
	};

	struct Frustum
	{
		s32		state;		// �����״̬
		s32		attribute;	// ���������
		CAMERA_TYPE	camMode;

		vector3df	position;	// ���������������ϵ��λ��
		vector3df	direction;	// �����ע�ӵķ���--����

		// UVNģ��ʹ��, ���ڶ���UVN�����
		vector3df	camUp;		// UVN�����ģ�͵ĳ���������
		vector3df	camRight;	// UVN�����ģ�͵ĳ���������
		vector3df	camLook;	// UVN�����ģ�͵ĳ���ע�ӷ���
		vector3df	camTarget;		// UVN�����ģ�͵�ע�ӵ�

		f32		view_dist;	// �ӵ㵽ͶӰ��ľ���

		f32		fov;			// ˮƽ����ʹ�ֱ�������Ұ

		f32		clip_z_near;	// ���ü���
		f32		clip_z_far;		// Զ�ü���

		Plane3D	clip_plane_R;	// �Ҳü���
		Plane3D	clip_plane_L;	// ��ü���
		Plane3D	clip_plane_T;	// �ϲü���
		Plane3D	clip_plane_B;	// �²ü���

		f32		viewplane_width;		// ͶӰ����
		f32		viewplane_height;		// ���ڹ�һ��ͶӰ, Ϊ2X2�� �����С����Ļ���ڴ�С��ͬ

		f32		viewport_width;			// ��Ļ/�ӿڵĴ�С
		f32		viewport_height;
		f32		viewport_center_X;		// ��Ļ��������
		f32		viewprot_center_Y;

		f32		aspect_ratio;			// ��Ļ�ĸ߿��

		Matrix	mWorldToCamera;				// �������굽�������任����
		Matrix	mCameraToPerspective;	// ������굽͸������ϵ�任����
		Matrix	mPerspectiveToScreen;	// ͸������ϵ����Ļ����ϵ�任����

		Frustum(CAMERA_TYPE mode, const vector3df &pos, const vector3df &dir,
			const vector3df& target, f32 nearZ, f32 farZ, f32 ffov,
			f32 viewportWidth, f32 viewportHeight);
	};
}

#endif