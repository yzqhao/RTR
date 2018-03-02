
#include "PlayerController.h"
#include "GameManager.h"

namespace RTR
{
	PlayerController::PlayerController(Camera* camera, GameManager* gameManager)
		: mCurDir(0), mFireDir(0)
	{
		mCamera = camera;
		mTank = new Tank("Player", "Green", gameManager);
	}

	PlayerController:: ~PlayerController()
	{
		SafeDelete(mTank);
	}

	void PlayerController::setPosition(const vector3df &pos)
	{
		// 由于坦克和摄像机被绑定到一起所以在修改坦克的位置时
		// 需要修改摄像机的位置和目标点
		mTank->setPosition(pos);
		mCamera->setPosition(pos + vector3df(0, 25, 30));
		mCamera->setTarget(pos);
	}

	void PlayerController::keyPress(s32 key)
	{
		if (key == 'W')
			mCurDir |= DIR_UP;
		else if (key == 'S')
			mCurDir |= DIR_DOWN;
		else if (key == 'A')
		{
			mCurDir |= DIR_LEFT;
			mTank->turnLeft();
		}
		else if (key == 'D')
		{
			mCurDir |= DIR_RIGHT;
			mTank->turnRight();
		}
		else if (key == VK_SPACE)
			mTank->fire();
		else if (key == VK_LEFT)
		{
			mFireDir = -1;
			//mTank->fireAimLeft();
		}
		else if (key == VK_RIGHT)
		{
			mFireDir = 1;
			//mTank->fireAimRight();
		}
		else if (key == 'F')
		{
			mCamera->setRenderMode(mCamera->getRenderMode() == RENDER_WIRE ? RENDER_SOILD : RENDER_WIRE);
		}
		else if (key == 'G')
		{
			mTank->setBulletType(mTank->getBulletTye() == BULLET_ROCKET ? BULLET_BALL : BULLET_ROCKET);
		}
	}

	void PlayerController::keyRlease(s32 key)
	{
		if (key == 'W')
			mCurDir &= ~DIR_UP;
		else if (key == 'S')
			mCurDir &= ~DIR_DOWN;
		else if (key == 'A')
			mCurDir &= ~DIR_LEFT;
		else if (key == 'D')
			mCurDir &= ~DIR_RIGHT;
		else if (key == VK_LEFT || key == VK_RIGHT)
			mFireDir = 0;
	}

	void PlayerController::update()
	{
		if (mCurDir & DIR_UP)
		{
			mTank->moveUp();
			mCamera->move(mTank->getCameraStep());
		}
		else if (mCurDir & DIR_DOWN)
		{
			mTank->moveDown();
			mCamera->move(mTank->getCameraStep());
		}

		if (mFireDir == -1)
			mTank->fireAimLeft();
		else if (mFireDir == 1)
			mTank->fireAimRight();

		mTank->update();
	}

}
