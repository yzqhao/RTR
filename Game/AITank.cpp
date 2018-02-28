#include "AITank.h"
#include "SceneManager.h"
#include "GameManager.h"
#include "PlayerController.h"

namespace RTR
{
	AITank::AITank(const std::string &name, const std::string &meshName, GameManager *gameMgr)
		: Tank(name, meshName, gameMgr){
		mFireFrequency = 15;
		mSpeed *= 0.75f;
		setBulletType(BULLET_BALL);
		mTankType = TANK_AI;
	}

	void AITank::onHited()
	{
		mCurrentLive += 26;
	}

	// 开火
	void AITank::fire()
	{
		Tank::fire();
	}

	void AITank::updateAI()
	{
		// 获取当前AITank距离玩家的距离
		vector3df length = getPosition() - mGameMgr->getPlayerController()->getPosition();

		// 检测当前AI tank离玩家的距离
		if (length.getLengthSQ() < 1000.0f)
		{
			f32 degree = length.angleBetween(mFireDir);
			degree = degree * 180.0f / 3.141592653 - 180.0f;
			// 查看夹角正负
			if (length.crossProduct(mFireDir)._y > 0)
				degree = -degree;

			// 只有当夹角大于2度才更改瞄准角度
			if (Abs(degree) > 2.0f)
			{
				// 更新炮筒夹角
				updateFireDir(degree);
				mHead->yaw(mHead->getRotateVec()._y + degree);
			}

			fire();
		}
		else	// 没有目标简单的旋转炮筒
			fireAimRight();

		// 向前移动
		if (!moveUp())
		{
			// 如果前方有障碍物, 随即旋转
			if (RandomInt() % 2 == 0)
				turnRight();
			else
				turnLeft();
		}
		else
		{
			// 有一定的概率随即旋转
			s32 ran = RandomInt(0, 100);
			if (ran == 50)
				turnRight();
			else if (ran == 25)
				turnLeft();
		}
	}

	// 更新tank
	void AITank::update()
	{
		// 更新ETank基类
		Tank::update();

		// 更新AI操作
		updateAI();
	}
}