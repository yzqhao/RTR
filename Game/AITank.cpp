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

	// ����
	void AITank::fire()
	{
		Tank::fire();
	}

	void AITank::updateAI()
	{
		// ��ȡ��ǰAITank������ҵľ���
		vector3df length = getPosition() - mGameMgr->getPlayerController()->getPosition();

		// ��⵱ǰAI tank����ҵľ���
		if (length.getLengthSQ() < 1000.0f)
		{
			f32 degree = length.angleBetween(mFireDir);
			degree = degree * 180.0f / 3.141592653 - 180.0f;
			// �鿴�н�����
			if (length.crossProduct(mFireDir)._y > 0)
				degree = -degree;

			// ֻ�е��нǴ���2�ȲŸ�����׼�Ƕ�
			if (Abs(degree) > 2.0f)
			{
				// ������Ͳ�н�
				updateFireDir(degree);
				mHead->yaw(mHead->getRotateVec()._y + degree);
			}

			fire();
		}
		else	// û��Ŀ��򵥵���ת��Ͳ
			fireAimRight();

		// ��ǰ�ƶ�
		if (!moveUp())
		{
			// ���ǰ�����ϰ���, �漴��ת
			if (RandomInt() % 2 == 0)
				turnRight();
			else
				turnLeft();
		}
		else
		{
			// ��һ���ĸ����漴��ת
			s32 ran = RandomInt(0, 100);
			if (ran == 50)
				turnRight();
			else if (ran == 25)
				turnLeft();
		}
	}

	// ����tank
	void AITank::update()
	{
		// ����ETank����
		Tank::update();

		// ����AI����
		updateAI();
	}
}