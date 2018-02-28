
#ifndef _ENGINE_CAMERA_H_
#define _ENGINE_CAMERA_H_

#include "Frustum.h"

namespace RTR
{
	enum RenderMode
	{
		RENDER_WIRE,	// 线框渲染
		RENDER_SOILD,	// 实体渲染
	};

	class Camera
	{
	public:
		~Camera();

		void update();

		// 设置近裁剪面距离
		void setZNear(f32 znear);
		// 设置远裁剪面距离
		void setZFar(f32 zfar);

		// 设置摄像机世界坐标
		void setPosition(const vector3df &pos);
		// 设置摄像机观察点, 当lockTarget为true时, 锁定观察点
		void setTarget(const vector3df &target, bool lockTarget = false);
		void releaseTarget() { mLockTarget = false; }

		// 基于世界坐标系移动
		void move(const vector3df &mov);
		// 基于摄像机自身坐标系移动
		void moveRelative(const vector3df &mov);

		// 绕Y轴旋转
		void yaw(f32 yDegree);
		// 绕X轴旋转
		void pitch(f32 pDegree);

		// 设置渲染模式, 线框或者实体
		void setRenderMode(RenderMode mode) { mRenderMode = mode; }
		RenderMode getRenderMode() const { return mRenderMode; }

		Frustum *getFrustum() const { return mFrustum; }

	protected:
		friend class SceneManager;
		Camera();

		Frustum	*mFrustum;

		bool		mLockTarget;
		bool		mNeedUpdate;

		RenderMode	mRenderMode;
	};
}

#endif