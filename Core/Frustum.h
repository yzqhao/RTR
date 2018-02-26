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
		CAMERA_MODEL_ELUA = 0x0001,	// 欧拉摄像机模型
		CAMERA_MODEL_UVN = 0x0002,	// UVN摄像机模型
	};

	struct Frustum
	{
		s32		state;		// 摄像机状态
		s32		attribute;	// 摄像机属性
		CAMERA_TYPE	camMode;

		vector3df	position;	// 摄像机在世界坐标系的位置
		vector3df	direction;	// 摄像机注视的方向--朝向

		// UVN模型使用, 用于定义UVN摄像机
		vector3df	camUp;		// UVN摄像机模型的朝向上向量
		vector3df	camRight;	// UVN摄像机模型的朝向右向量
		vector3df	camLook;	// UVN摄像机模型的朝向注视方向
		vector3df	camTarget;		// UVN摄像机模型的注视点

		f32		view_dist;	// 视点到投影面的距离

		f32		fov;			// 水平方向和垂直方向的视野

		f32		clip_z_near;	// 近裁减面
		f32		clip_z_far;		// 远裁减面

		Plane3D	clip_plane_R;	// 右裁减面
		Plane3D	clip_plane_L;	// 左裁减面
		Plane3D	clip_plane_T;	// 上裁减面
		Plane3D	clip_plane_B;	// 下裁减面

		f32		viewplane_width;		// 投影面宽高
		f32		viewplane_height;		// 对于归一化投影, 为2X2， 否则大小与屏幕窗口大小相同

		f32		viewport_width;			// 屏幕/视口的大小
		f32		viewport_height;
		f32		viewport_center_X;		// 屏幕中心坐标
		f32		viewprot_center_Y;

		f32		aspect_ratio;			// 屏幕的高宽比

		Matrix	mWorldToCamera;				// 世界坐标到相机坐标变换矩阵
		Matrix	mCameraToPerspective;	// 相机坐标到透视坐标系变换矩阵
		Matrix	mPerspectiveToScreen;	// 透视坐标系到屏幕坐标系变换矩阵

		Frustum(CAMERA_TYPE mode, const vector3df &pos, const vector3df &dir,
			const vector3df& target, f32 nearZ, f32 farZ, f32 ffov,
			f32 viewportWidth, f32 viewportHeight);
	};
}

#endif