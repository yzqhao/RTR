#include "Bullet.h"
#include "Tank.h"

namespace RTR
{
	s32 Bullet::mID = 0;

	Bullet::Bullet(Tank *owner, const std::string &meshName, GameManager *gameMgr)
		: mOwner(owner), mGameMgr(gameMgr), mDirection(0, 0, 0), mSpeed(1.75f)
	{
		mMaxLive = 100;
		mCurrentLive = 0;

		mName = "Bullet" + IntToString(mID++);
		mMesh = mGameMgr->getSceneManager()->createMesh(mName, meshName);
	}

	Bullet::~Bullet()
	{
		mGameMgr->getSceneManager()->destroyMesh(mName);
	}

	void Bullet::setPosition(const vector3df &pos)
	{
		mMesh->setPosition(pos);
	}

	void Bullet::setDirection(const vector3df &dir)
	{
		mDirection = dir;
	}

	void Bullet::yaw(f32 degree)
	{
		mMesh->yaw(degree);
	}

	void Bullet::update()
	{
		mCurrentLive++;
		mMesh->setPosition(mMesh->getPosition() + mDirection * mSpeed);

		// �����ȡ�ӵ�ͷ��λ��
		vector3df pos = getPosition() + mDirection * 3.0f;
		// ����Ƿ����Ŀ��
		// ���ȼ�⾲̬����
		Mesh* obj = mGameMgr->checkObject(pos._x, pos._z);
		if (obj)
		{
			mGameMgr->playSound(SOUND_EXPLODE);
			mGameMgr->destroyBullet(this);
		}
		else
		{
			// ���û�о�̬����, ���tank
			Tank *tank = mGameMgr->checkTank(pos._x, pos._y - 1.5f, pos._z);
			if (tank && tank->getTankType() != mOwner->getTankType())
			{
				// ������
				tank->onHited();
				mGameMgr->playSound(SOUND_EXPLODE);
				mGameMgr->destroyBullet(this);
			}
		}
	}
}