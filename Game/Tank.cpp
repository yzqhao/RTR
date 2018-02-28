#include "Tank.h"
#include "Bullet.h"
#include "Core.h"
#include "CoreMath.h"
#include "GameManager.h"

namespace RTR
{
	Tank::Tank(const std::string &name, const std::string &meshName, GameManager *gameMgr)
		: mGameMgr(gameMgr), mScene(gameMgr->getSceneManager()), mSpeed(0.75f),
		mFireDir(0, 0, -1), mLastFire(0), mBulletType(BULLET_ROCKET),
		mMoveVec(0, 0, 0), mColOffset(3.5f), mTankType(TANK_PLAYER)
	{
		mMaxLive = 100;
		mCurrentLive = 0;
		mFireFrequency = 8;

		mName = name;
		mBody = mScene->createMesh(name + "_Body", meshName + "_Body.mesh");
		mHead = mScene->createMesh(name + "_Head", meshName + "_Head.mesh");
	}

	Tank::~Tank()
	{
		mScene->destroyMesh(mName + "_Body");
		mScene->destroyMesh(mName + "_Head");
	}

	void Tank::fire()
	{
		if (mGameMgr && mLastFire > mFireFrequency)
		{
			Bullet *bullet = mGameMgr->createBullet(this);
			bullet->setPosition(getPosition() + mFireDir * 5.0f + vector3df(0, 2, 0));
			bullet->setDirection(mFireDir);
			bullet->yaw(mHead->getRotateVec()._y);
			mLastFire = 0;

			mGameMgr->playSound(SOUND_FIRE);
		}
	}

	void Tank::update()
	{
		mLastFire++;
	}

	void Tank::setVisible(bool visible)
	{
		mBody->setVisible(visible);
		mHead->setVisible(visible);
	}

	void Tank::move(const vector3df &mov)
	{
		mBody->move(mov);
		mHead->move(mov);
	}

	bool Tank::moveUp()
	{
		// �����߼�����ǰ�ƶ�
		// �Ե�ǰtank�ĳ���Ϊǰ
		s32 degree = mBody->getRotateVec()._y;
		s32 dir = degree > 0 ? 1 : -1;
		degree = Abs(degree) % 360;

		const static s32 offset = 3;
		f32 xOff = 0, zOff = 0;	// tank�����������ƫ����

		vector3df pos = getPosition();	// ���յķ���
		if (degree < 5)
		{
			mMoveVec = vector3df(0, 0, -mSpeed);
			pos._z = pos._z - mColOffset;
			xOff = offset;
		}
		else if (degree < 95)
		{
			mMoveVec = vector3df(-mSpeed * dir, 0, 0);
			pos._x = pos._x - mColOffset * dir;
			zOff = offset;
		}
		else if (degree < 185)
		{
			mMoveVec = vector3df(0, 0, mSpeed);
			pos._z = pos._z + mColOffset;
			xOff = offset;
		}
		else
		{
			mMoveVec = vector3df(mSpeed * dir, 0, 0);
			pos._x = pos._x + mColOffset * dir;
			zOff = offset;
		}

		// ���ȼ���Ƿ����ϰ�����ֹ
		// ��tank�������Ҳ෢����������
		if (mGameMgr->canGo(pos._x + mMoveVec._x + xOff, pos._z + mMoveVec._z + zOff) &&
			mGameMgr->canGo(pos._x + mMoveVec._x - xOff, pos._z + mMoveVec._z - zOff))
		{
			move(mMoveVec);
			return true;
		}
		else	// ��, ��ô̹�˺������ƫ������Ϊ0
		{
			mMoveVec = vector3df();
			return false;
		}
	}

	bool Tank::moveDown()
	{
		// �����߼�������ƶ�
		// �Ե�ǰtank�ĳ���ķ�����Ϊ��
		s32 degree = mBody->getRotateVec()._y;
		s32 dir = degree > 0 ? 1 : -1;
		degree = Abs(degree) % 360;

		const static s32 offset = 3;
		f32 xOff = 0, zOff = 0;	// tank�����������ƫ����

		vector3df pos = getPosition();
		if (degree < 5)
		{
			mMoveVec = vector3df(0, 0, mSpeed);
			pos._z = pos._z + mColOffset;
			xOff = offset;
		}
		else if (degree < 95)
		{
			mMoveVec = vector3df(mSpeed * dir, 0, 0);
			pos._x = pos._x + mColOffset * dir;
			zOff = offset;
		}
		else if (degree < 185)
		{
			mMoveVec = vector3df(0, 0, -mSpeed);
			pos._z = pos._z - mColOffset;
			xOff = offset;
		}
		else
		{
			mMoveVec = vector3df(-mSpeed * dir, 0, 0);
			pos._x = pos._x - mColOffset * dir;
			zOff = offset;
		}

		// ���ȼ���Ƿ����ϰ�����ֹ
		if (mGameMgr->canGo(pos._x + mMoveVec._x + xOff, pos._z + mMoveVec._z + zOff) &&
			mGameMgr->canGo(pos._x + mMoveVec._x - xOff, pos._z + mMoveVec._z - zOff))
		{
			move(mMoveVec);
			return true;
		}
		else	// ��, ��ô̹�˺������ƫ������Ϊ0
		{
			mMoveVec = vector3df();
			return false;
		}
	}

	void Tank::onHited()
	{

	}

	bool Tank::intersect(const vector3df &point)
	{
		return mHead->intersect(point) || mBody->intersect(point);
	}

	void Tank::updateFireDir(f32 degree)
	{
		Matrix mat;
		GetRotateMatrix44Y(mat, degree);
		GetVector3DMulMatrix44(mFireDir, mat, mFireDir);
	}

	void Tank::fireAimLeft()
	{
		updateFireDir(-4.0f);
		mHead->yaw(mHead->getRotateVec()._y - 4.0f);
	}

	void Tank::fireAimRight()
	{
		updateFireDir(4.0f);
		mHead->yaw(mHead->getRotateVec()._y + 4.0f);
	}

	void Tank::setPosition(const vector3df &pos) {
		mBody->setPosition(pos);
		mHead->setPosition(pos);
	}

	void Tank::yaw(f32 degree)
	{
		mBody->yaw(degree);
		mHead->yaw(degree);
	}

	void Tank::turnLeft()
	{
		//updateFireDir(-90.0f);
		//mHead->yaw(mHead->getRotateVec().y - 90.0f);
		mBody->yaw(mBody->getRotateVec()._y - 90.0f);
	}

	void Tank::turnRight()
	{
		//updateFireDir(90.0f);
		//mHead->yaw(mHead->getRotateVec().y + 90.0f);
		mBody->yaw(mBody->getRotateVec()._y + 90.0f);
	}
}