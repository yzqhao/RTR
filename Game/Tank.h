
#ifndef _GAME_TANK_H_
#define _GAME_TANK_H_

#include "Object.h"
#include "SceneManager.h"

namespace RTR
{
	// 子弹类型
	enum BulletType
	{
		BULLET_BALL,	// 球形子弹
		BULLET_ROCKET,	// 导弹型子弹
	};

	enum TankType
	{
		TANK_AI,
		TANK_PLAYER,
	};

	// tank的基类, tank分两部分, 一部分是脑袋, 一部分是车身, 脑袋可以旋转
	// 脑袋是基于车身移动的
	class Bullet;
	class Camera;
	class GameManager;
	class Tank : public Object
	{
		typedef std::list<Bullet*>::iterator BulletIter;

	public:
		virtual ~Tank();

		// 移动tank
		void move(const vector3df &mov);
		// 向前移动tank, 指按照当前tank的朝向为前
		bool moveUp();
		// 向后移动tank, 指按照当前tank的朝向的反方向
		bool moveDown();

		// 向左转tank车体
		void turnLeft();
		// 向右转tank车体
		void turnRight();

		// 瞄准器向左转
		void fireAimLeft();
		// 瞄准器向右转
		void fireAimRight();

		// 获得当前tank的世界坐标
		vector3df getPosition() const  { return mBody->getPosition(); }
		void setPosition(const vector3df &pos);

		// tank延Y轴旋转, 参数为角度
		void yaw(f32 degree);

		// 设置子弹类型BULLET_BALL, BULLET_ROCKET
		void setBulletType(BulletType type){ mBulletType = type; }
		BulletType getBulletTye() const { return mBulletType; }
		TankType getTankType() { return mTankType; }

		// 是否可见
		bool isVisible() const { return mBody->isVisible(); }
		void setVisible(bool visible);

		// 给定的点是否在tank的包围盒内
		bool intersect(const vector3df &point);

		// 被击中后自动调用
		virtual void onHited();
		// 开火
		virtual void fire();
		// 更新tank
		virtual void update();
		// 获取摄像机应该移动的响亮
		vector3df getCameraStep() const { return mMoveVec; }

	protected:
		friend class GameManager;
		friend class PlayerController;
		Tank(const std::string &name, const std::string &meshName, GameManager *gameMgr);

		// 更新瞄准器朝向, 子弹发射方向
		void updateFireDir(f32 degree);

	protected:
		Mesh				*mHead;
		Mesh				*mBody;

		vector3df			mMoveVec;	// 摄像机应该移动的向量

		vector3df			mFireDir;	// 开火方向
		f32				mSpeed;		// 移动速度
		f32				mFireFrequency;	// 开火频率

		//ECamera				*mCamera;

		s32				mLastFire;	// 开火时间记录
		f32				mColOffset;	// 碰撞偏移量

		SceneManager		*mScene;
		GameManager		*mGameMgr;

		BulletType			mBulletType;
		TankType			mTankType;
	};
}

#endif