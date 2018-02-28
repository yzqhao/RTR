
#ifndef _ENGINE_CAMERA_H_
#define _ENGINE_CAMERA_H_

#include "Frustum.h"

namespace RTR
{
	enum RenderMode
	{
		RENDER_WIRE,	// �߿���Ⱦ
		RENDER_SOILD,	// ʵ����Ⱦ
	};

	class Camera
	{
	public:
		~Camera();

		void update();

		// ���ý��ü������
		void setZNear(f32 znear);
		// ����Զ�ü������
		void setZFar(f32 zfar);

		// �����������������
		void setPosition(const vector3df &pos);
		// ����������۲��, ��lockTargetΪtrueʱ, �����۲��
		void setTarget(const vector3df &target, bool lockTarget = false);
		void releaseTarget() { mLockTarget = false; }

		// ������������ϵ�ƶ�
		void move(const vector3df &mov);
		// �����������������ϵ�ƶ�
		void moveRelative(const vector3df &mov);

		// ��Y����ת
		void yaw(f32 yDegree);
		// ��X����ת
		void pitch(f32 pDegree);

		// ������Ⱦģʽ, �߿����ʵ��
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