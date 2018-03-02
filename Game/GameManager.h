
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

		// 开始游戏
		bool init(std::string mapName);
		void startGame(s32 maxEnemyNumber = 10);
		bool isGameBegin() const { return mMaxEnemyNumber > 0; }
		// 是否结束游戏
		bool finishGame() const;

		// 击毁敌人数目
		s32 getDestoryEnemyNumber() const { return mCurrentEnemyNumber; }
		// 剩余敌人数目
		s32 getLastEnemyNumber() const { return mMaxEnemyNumber - mCurrentEnemyNumber; }

		// 创建子弹
		Bullet* createBullet(Tank *owner);
		void destroyBullet(Bullet *bullet);
		// 创建AI坦克
		Tank *createAITank(const vector3df &pos);
		// 获取当前角色坦克的控制器
		PlayerController *getPlayerController() { return mMainPlayer; }

		// 播放声音
		void playSound(SOUND_YYPE sound, bool loop = false);

		// 获得场景管理器
		SceneManager *getSceneManager() { return mScene; }

		// 加载场景地图
		bool loadMap(const std::string& mapName);
		void changeMap(const std::string& mapName);
		std::string getCurrentMapName() const { return mCurMap; }
		// 获取地图的高宽
		f32 getMapWidth() const { return mXL; }
		f32 getMapHeight() const { return mZL; }

		// 通过canGo, checkObject来模拟简单的碰撞效果
		// XZ平面坐标中x, z坐标是否可通过
		bool canGo(f32 x, f32 z);
		void getLogicXZ(f32 x, f32 z, s32 &outX, s32 &outZ);
		// 给定x,z坐标, 返回在此坐标的物体(静态物体, 不包括子弹或者坦克)
		Mesh* checkObject(f32 x, f32 z);
		// 给定x,y,z坐标, 查看是否击中tank, 没有集中返回NULL
		Tank* checkTank(f32 x, f32 y, f32 z);

		// 更新
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
		f32							mHXL, mHZL;	// map宽, 高
		f32							mXL, mZL;	// map宽/2 高/2
		std::vector<std::vector<s32>>	mCollisionValue;	// 存储当前加载地图的障碍物
		std::vector<std::vector<Mesh*>>mCollisionMeshs;

		Mesh							*mTerrainMesh;

		s32							mMaxEnemyNumber;	// 最大敌人数目
		s32							mCurrentEnemyNumber; // 当前击毁敌人数
		s32							mVisibleEnemyNumber; // 一次最多显示的敌人数目
	};
}

#endif