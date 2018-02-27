
#include "Camera.h"
#include "CoreMath.h"

namespace RTR
{
	Camera::Camera() : mLockTarget(false), mNeedUpdate(true), mRenderMode(RENDER_SOILD)
	{
		mFrustum = new Frustum(CAMERA_MODEL_UVN, vector3df(0, 0, 0), vector3df(0, 0, 0), vector3df(0, 0, 0),
			10, 1000, 75, SCREEN_WIDTH, SCREEN_HEIGHT);

		update();
	}

	Camera::~Camera() { SafeDelete(mFrustum); }

	void Camera::update()
	{
		if (mNeedUpdate)
		{
			Matrix mt_inv;
			GetTranslateMatrix44(mt_inv, -mFrustum->position._x, -mFrustum->position._y, -mFrustum->position._z);
			mFrustum->camLook = mFrustum->camTarget - mFrustum->position;

			mFrustum->camUp = vector3df(0, 1, 0);
			mFrustum->camRight = mFrustum->camUp.crossProduct(mFrustum->camLook);
			mFrustum->camUp = mFrustum->camLook.crossProduct(mFrustum->camRight);

			mFrustum->camLook.normalize();
			mFrustum->camRight.normalize();
			mFrustum->camUp.normalize();

			Matrix mt_uvn(mFrustum->camRight._x, mFrustum->camUp._x, mFrustum->camLook._x, 0,
				mFrustum->camRight._y, mFrustum->camUp._y, mFrustum->camLook._y, 0,
				mFrustum->camRight._z, mFrustum->camUp._z, mFrustum->camLook._z, 0,
				0, 0, 0, 1);
			mFrustum->mWorldToCamera = mt_inv * mt_uvn;

			mNeedUpdate = false;
		}
	}

	void Camera::setZNear(f32 znear)
	{
		mFrustum->clip_z_near = znear;
		mNeedUpdate = true;
	}

	void Camera::setZFar(f32 zfar)
	{
		mFrustum->clip_z_far = zfar;
		mNeedUpdate = true;
	}

	void Camera::setPosition(const vector3df &pos)
	{
		mFrustum->position._x = pos._x;
		mFrustum->position._y = pos._y;
		mFrustum->position._z = pos._z;

		mNeedUpdate = true;
	}

	void Camera::setTarget(const vector3df &target, bool lockTarget)
	{
		mFrustum->camTarget._x = target._x;
		mFrustum->camTarget._y = target._y;
		mFrustum->camTarget._z = target._z;

		mLockTarget = lockTarget;
		mNeedUpdate = true;
	}

	// 基于世界坐标系移动
	void Camera::move(const vector3df &mov)
	{
		mFrustum->position._x += mov._x;
		mFrustum->position._y += mov._y;
		mFrustum->position._z += mov._z;

		if (!mLockTarget)
		{
			mFrustum->camTarget._x += mov._x;
			mFrustum->camTarget._y += mov._y;
			mFrustum->camTarget._z += mov._z;
		}

		mNeedUpdate = true;
	}

	// 基于摄像机自身坐标系移动
	void Camera::moveRelative(const vector3df &mov)
	{
		vector3df offset = mFrustum->camRight * mov._x + mFrustum->camUp * mov._y + mFrustum->camLook * mov._z;
		mFrustum->position = mFrustum->position + offset;

		if (!mLockTarget)
			mFrustum->camTarget = mFrustum->camTarget + offset;

		mNeedUpdate = true;
	}

	// 绕Y轴旋转
	void Camera::yaw(f32 yDegree)
	{
		Matrix rotMat;
		vector3df distence = mFrustum->camTarget - mFrustum->position;
		GetRotateMatrix44(rotMat, mFrustum->camUp, yDegree);
		GetVector3DMulMatrix44(distence, rotMat, distence);
		// 旋转后的目标点
		mFrustum->camTarget = distence + mFrustum->position;
		mNeedUpdate = true;
	}

	// 绕X轴旋转
	void Camera::pitch(f32 pDegree)
	{
		Matrix rotMat;
		vector3df distence = mFrustum->camTarget - mFrustum->position;
		GetRotateMatrix44(rotMat, mFrustum->camRight, pDegree);
		GetVector3DMulMatrix44(distence, rotMat, distence);
		// 旋转后的目标点
		mFrustum->camTarget = distence + mFrustum->position;
		mNeedUpdate = true;
	}

}