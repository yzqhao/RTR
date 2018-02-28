
#ifndef _GAME_BULLET_H_
#define _GAME_BULLET_H_

#include "Object.h"
#include "GameManager.h"
#include "SceneManager.h"

namespace RTR
{
	// �ӵ�����
	class Tank;
	class Bullet : public Object
	{
	public:
		virtual ~Bullet();

		void setScale(const vector3df &scale) { mMesh->setScale(scale); }
		// ���ó�ʼ����λ��
		void setPosition(const vector3df &pos);
		vector3df getPosition() const { return mMesh->getPosition(); }
		// ���ó�ʼ���䷽��
		void setDirection(const vector3df &dir);
		// ��Y����ӵ�������ת
		void yaw(f32 degree);

		void update();

		bool isVisible() const { return mMesh->isVisible(); }
		void setVisible(bool visible) { mMesh->setVisible(visible); }

	protected:
		friend class GameManager;
		Bullet(Tank *owner, const std::string &meshName, GameManager *scene);

	protected:
		GameManager	*mGameMgr;
		Mesh			*mMesh;

		vector3df		mDirection;		// ��ʼ�ƶ�����
		f32			mSpeed;			// �ƶ��ٶ�

		Tank			*mOwner;

		static s32		mID;			// ����ID
	};
}

#endif