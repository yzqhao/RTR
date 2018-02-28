
#ifndef _GAME_BULLET_H_
#define _GAME_BULLET_H_

#include "Object.h"
#include "GameManager.h"
#include "SceneManager.h"

namespace RTR
{
	// 子弹类型
	class Tank;
	class Bullet : public Object
	{
	public:
		virtual ~Bullet();

		void setScale(const vector3df &scale) { mMesh->setScale(scale); }
		// 设置初始发射位置
		void setPosition(const vector3df &pos);
		vector3df getPosition() const { return mMesh->getPosition(); }
		// 设置初始发射方向
		void setDirection(const vector3df &dir);
		// 延Y轴对子弹进行旋转
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

		vector3df		mDirection;		// 初始移动方向
		f32			mSpeed;			// 移动速度

		Tank			*mOwner;

		static s32		mID;			// 计数ID
	};
}

#endif