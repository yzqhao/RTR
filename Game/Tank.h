
#ifndef _GAME_TANK_H_
#define _GAME_TANK_H_

#include "Object.h"
#include "SceneManager.h"

namespace RTR
{
	// �ӵ�����
	enum BulletType
	{
		BULLET_BALL,	// �����ӵ�
		BULLET_ROCKET,	// �������ӵ�
	};

	enum TankType
	{
		TANK_AI,
		TANK_PLAYER,
	};

	// tank�Ļ���, tank��������, һ�������Դ�, һ�����ǳ���, �Դ�������ת
	// �Դ��ǻ��ڳ����ƶ���
	class Bullet;
	class Camera;
	class GameManager;
	class Tank : public Object
	{
		typedef std::list<Bullet*>::iterator BulletIter;

	public:
		virtual ~Tank();

		// �ƶ�tank
		void move(const vector3df &mov);
		// ��ǰ�ƶ�tank, ָ���յ�ǰtank�ĳ���Ϊǰ
		bool moveUp();
		// ����ƶ�tank, ָ���յ�ǰtank�ĳ���ķ�����
		bool moveDown();

		// ����תtank����
		void turnLeft();
		// ����תtank����
		void turnRight();

		// ��׼������ת
		void fireAimLeft();
		// ��׼������ת
		void fireAimRight();

		// ��õ�ǰtank����������
		vector3df getPosition() const  { return mBody->getPosition(); }
		void setPosition(const vector3df &pos);

		// tank��Y����ת, ����Ϊ�Ƕ�
		void yaw(f32 degree);

		// �����ӵ�����BULLET_BALL, BULLET_ROCKET
		void setBulletType(BulletType type){ mBulletType = type; }
		BulletType getBulletTye() const { return mBulletType; }
		TankType getTankType() { return mTankType; }

		// �Ƿ�ɼ�
		bool isVisible() const { return mBody->isVisible(); }
		void setVisible(bool visible);

		// �����ĵ��Ƿ���tank�İ�Χ����
		bool intersect(const vector3df &point);

		// �����к��Զ�����
		virtual void onHited();
		// ����
		virtual void fire();
		// ����tank
		virtual void update();
		// ��ȡ�����Ӧ���ƶ�������
		vector3df getCameraStep() const { return mMoveVec; }

	protected:
		friend class GameManager;
		friend class PlayerController;
		Tank(const std::string &name, const std::string &meshName, GameManager *gameMgr);

		// ������׼������, �ӵ����䷽��
		void updateFireDir(f32 degree);

	protected:
		Mesh				*mHead;
		Mesh				*mBody;

		vector3df			mMoveVec;	// �����Ӧ���ƶ�������

		vector3df			mFireDir;	// ������
		f32				mSpeed;		// �ƶ��ٶ�
		f32				mFireFrequency;	// ����Ƶ��

		//ECamera				*mCamera;

		s32				mLastFire;	// ����ʱ���¼
		f32				mColOffset;	// ��ײƫ����

		SceneManager		*mScene;
		GameManager		*mGameMgr;

		BulletType			mBulletType;
		TankType			mTankType;
	};
}

#endif