
#ifndef _GAME_GAMEMANAGER_H_
#define _GAME_GAMEMANAGER_H_

#include <list>
#include <vector>

#include "type.h"
#include "Util.h"
#include "Vector3.h"

namespace RTR
{
	enum SOUND_YYPE
	{
		SOUND_BACKGROUND,
		SOUND_FIRE,
		SOUND_EXPLODE,
	};

	class Mesh;
	class Bullet;
	class Tank;
	class PlayerController;
	class SceneManager;
	class GameManager
	{
		typedef std::list<Bullet*>::iterator BulletIter;
		typedef std::list<Tank*>::iterator TankIter;

	public:
		GameManager(SceneManager *scene);
		~GameManager();

		// ��ʼ��Ϸ
		bool init(std::string mapName);
		void startGame(s32 maxEnemyNumber = 10);
		bool isGameBegin() const { return mMaxEnemyNumber > 0; }
		// �Ƿ������Ϸ
		bool finishGame() const;

		// ���ٵ�����Ŀ
		s32 getDestoryEnemyNumber() const { return mCurrentEnemyNumber; }
		// ʣ�������Ŀ
		s32 getLastEnemyNumber() const { return mMaxEnemyNumber - mCurrentEnemyNumber; }

		// �����ӵ�
		Bullet* createBullet(Tank *owner);
		void destroyBullet(Bullet *bullet);
		// ����AI̹��
		Tank *createAITank(const vector3df &pos);
		// ��ȡ��ǰ��ɫ̹�˵Ŀ�����
		PlayerController *getPlayerController() { return mMainPlayer; }

		// ��������
		void playSound(SOUND_YYPE sound, bool loop = false);

		// ��ó���������
		SceneManager *getSceneManager() { return mScene; }

		// ���س�����ͼ
		bool loadMap(const std::string& mapName);
		void changeMap(const std::string& mapName);
		std::string getCurrentMapName() const { return mCurMap; }
		// ��ȡ��ͼ�ĸ߿�
		f32 getMapWidth() const { return mXL; }
		f32 getMapHeight() const { return mZL; }

		// ͨ��canGo, checkObject��ģ��򵥵���ײЧ��
		// XZƽ��������x, z�����Ƿ��ͨ��
		bool canGo(f32 x, f32 z);
		void getLogicXZ(f32 x, f32 z, s32 &outX, s32 &outZ);
		// ����x,z����, �����ڴ����������(��̬����, �������ӵ�����̹��)
		Mesh* checkObject(f32 x, f32 z);
		// ����x,y,z����, �鿴�Ƿ����tank, û�м��з���NULL
		Tank* checkTank(f32 x, f32 y, f32 z);

		// ����
		void update();

		void showInfo(bool showHelp);

	protected:
		SceneManager					*mScene;
		std::list<Bullet*>				mBullets;
		std::list<Tank*>				mTanks;

		PlayerController				*mMainPlayer;

		std::string							mCurMap;

		f32							mBlockSize;
		s32							mX, mZ;
		f32							mHXL, mHZL;	// map��, ��
		f32							mXL, mZL;	// map��/2 ��/2
		std::vector<std::vector<s32>>	mCollisionValue;	// �洢��ǰ���ص�ͼ���ϰ���
		std::vector<std::vector<Mesh*>>mCollisionMeshs;

		Mesh							*mTerrainMesh;

		s32							mMaxEnemyNumber;	// ��������Ŀ
		s32							mCurrentEnemyNumber; // ��ǰ���ٵ�����
		s32							mVisibleEnemyNumber; // һ�������ʾ�ĵ�����Ŀ
	};
}

#endif