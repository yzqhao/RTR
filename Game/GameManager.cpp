
#include <fstream>

#include "GameManager.h"
#include "SceneManager.h"
#include "Bullet.h"
#include "Tank.h"
#include "AITank.h"
#include "PlayerController.h"

namespace RTR
{
	const std::string Bullet_Ball = "BallBullet.mesh";
	const std::string Bullet_Rocket = "RocketBullet.mesh";

	const vector3df InitPosition(-20.0f, 1.0f, 15.0f);

	const vector3df RandomPos[3] = { vector3df(10, 1.5f, 10),
		vector3df(45, 1.5f, -5),
		vector3df(-20, 1.5f, 45) };

	GameManager::GameManager(SceneManager *scene) : mScene(scene), mBlockSize(0.0f),
		mX(0), mZ(0), mTerrainMesh(NULL), mCurrentEnemyNumber(0), mMaxEnemyNumber(0),
		mVisibleEnemyNumber(3)
	{
		mMainPlayer = new PlayerController(this);
		mMainPlayer->setPosition(InitPosition);

		Bullet *bullet = new Bullet(NULL, "RocketBullet.mesh", this);
		bullet->setVisible(false);
		bullet = new Bullet(NULL, "BallBullet.mesh", this);
		bullet->setVisible(false);
	}

	GameManager::~GameManager()
	{
		SafeDelete(mMainPlayer);
		for (BulletIter itr = mBullets.begin(); itr != mBullets.end(); ++itr)
			SafeDelete(*itr);
		for (TankIter itr = mTanks.begin(); itr != mTanks.end(); ++itr)
			SafeDelete(*itr);
		mBullets.clear();
		mTanks.clear();
	}

	void GameManager::startGame(s32 maxEnemyNumber)
	{
		// 清理当前的子弹和坦克
		for (BulletIter itr = mBullets.begin(); itr != mBullets.end(); ++itr)
			(*itr)->mCurrentLive = (*itr)->mMaxLive + 1;
		for (TankIter itr = mTanks.begin(); itr != mTanks.end(); ++itr)
			(*itr)->mCurrentLive = (*itr)->mMaxLive + 1;

		mMaxEnemyNumber = maxEnemyNumber;
		mCurrentEnemyNumber = 0;
	}

	bool GameManager::finishGame() const
	{
		return mCurrentEnemyNumber == mMaxEnemyNumber;
	}

	Bullet* GameManager::createBullet(Tank *owner)
	{
		std::string meshname = (owner->getBulletTye() == BULLET_ROCKET ? Bullet_Rocket : Bullet_Ball);
		Bullet *bullet = new Bullet(owner, meshname, this);
		if (meshname == Bullet_Ball)
			bullet->setScale(vector3df(1.5f, 1.5f, 1.5f));
		mBullets.push_back(bullet);
		return bullet;
	}

	void GameManager::destroyBullet(Bullet *bullet)
	{
		// 这里直接设置当前生命值比最大生命值大, update的时候自动删除
		bullet->mCurrentLive = bullet->mMaxLive + 1;
	}

	Tank *GameManager::createAITank(const vector3df &pos)
	{
		static s32 ID = 0;
		std::string name = "AI#" + IntToString(ID++);
		Tank *tank = new AITank(name, "Tank1", this);
		tank->setPosition(pos);
		mTanks.push_back(tank);
		return tank;
	}

	void GameManager::update()
	{
		if (isGameBegin() && !finishGame())
		{
			// 不足最大显示那么创建
			if ((s32)mTanks.size() < mVisibleEnemyNumber)
			{
				s32 curNum = (s32)mTanks.size();
				for (s32 i = 0; i < mVisibleEnemyNumber - curNum; i++)
				{
					if (mCurrentEnemyNumber + (s32)mTanks.size() < mMaxEnemyNumber)
						createAITank(RandomPos[RandomInt(0, 2)]);
				}
			}

			// 更新子弹, 如果子弹存活, 那么就更新, 否则将其删除
			for (TankIter itr = mTanks.begin(); itr != mTanks.end();)
			{
				if ((*itr)->isAlive())
				{
					(*itr)->update();
					++itr;
				}
				else
				{
					mCurrentEnemyNumber++;
					SafeDelete(*itr);
					mTanks.erase(itr++);
				}
			}
		}

		// 更新子弹, 如果子弹存活, 那么就更新, 否则将其删除
		for (BulletIter itr = mBullets.begin(); itr != mBullets.end();)
		{
			if ((*itr)->isAlive())
			{
				(*itr)->update();
				++itr;
			}
			else
			{
				SafeDelete(*itr);
				mBullets.erase(itr++);
			}
		}

		mMainPlayer->update();
	}

	void GameManager::changeMap(const std::string& mapName)
	{
		// 删除场景
		SafeDelete(mMainPlayer);
		for (BulletIter itr = mBullets.begin(); itr != mBullets.end(); ++itr)
			SafeDelete(*itr);
		for (TankIter itr = mTanks.begin(); itr != mTanks.end(); ++itr)
			SafeDelete(*itr);
		mBullets.clear();
		mTanks.clear();

		mCollisionValue.clear();
		mCollisionMeshs.clear();

		// 敌人置于0
		mCurrentEnemyNumber = 0;

		// 清理实际的模型
		mScene->clearMesh();

		// 新建角色
		mMainPlayer = new PlayerController(this);
		mMainPlayer->setPosition(InitPosition);

		// 加载场景
		loadMap(mapName);
	}

	std::string getBlock(const std::string &line, char ltip, char rtip)
	{
		s32 beg = line.find(ltip);
		s32 end = line.rfind(rtip);
		return line.substr(beg + 1, end - beg - 1);
	}

	vector3df getPos(const std::string &line)
	{
		// <Position X="-49.649979" Y="4.247236" Z="-5.005510" />
		s32 xb = line.find("X");
		s32 yb = line.find("Y");
		s32 zb = line.find("Z");
		s32 end = line.find("/");
		std::string xs = line.substr(xb + 3, yb - 5 - xb);
		std::string ys = line.substr(yb + 3, zb - 5 - yb);
		std::string zs = line.substr(zb + 3, end - zb - 5);

		return vector3df(StringToFloat(xs), StringToFloat(ys), StringToFloat(zs));
	}

	void getXZ(const std::string& line, s32 &x, s32 &z, f32 &blockSize)
	{
		// <Grid X="15" Z="14" Size="10" />
		s32 xb = line.find("X");
		s32 zb = line.find("Z");
		s32 sb = line.find("Size");
		s32 end = line.find("/");
		std::string xs = line.substr(xb + 3, zb - 5 - xb);
		std::string zs = line.substr(zb + 3, sb - 5 - zb);
		std::string ss = line.substr(sb + 6, end - sb - 8);

		x = StringToFloat(xs);
		z = StringToFloat(zs);
		blockSize = StringToFloat(ss);
	}

	void getValue(const std::string& line, s32 &x, s32 &z, s32 &value)
	{
		// <Map X="6" Z="2" Value="0" />
		s32 xb = line.find("X");
		s32 zb = line.find("Z");
		s32 sb = line.find("Value");
		s32 end = line.find("/");
		std::string xs = line.substr(xb + 3, zb - 5 - xb);
		std::string zs = line.substr(zb + 3, sb - 5 - zb);
		std::string ss = line.substr(sb + 7, end - sb - 9);

		x = StringToFloat(xs);
		z = StringToFloat(zs);
		value = StringToFloat(ss);
	}

	bool GameManager::loadMap(const std::string& mapName)
	{
		mCurMap = mapName;

		//Log("Loading map #%s...", mapName.c_str());

		mCollisionValue.clear();
		mCollisionMeshs.clear();

		std::ifstream in;
		in.open(GetPath(mapName).c_str());

		if (in.bad())
		{
			in.close();
			return false;
		}

		char line[256];
		while (in.good())
		{
			std::string info;
			in.getline(line, 256);
			info = Trim(line);

			int beg = 0, end = -1;

			if (info.find("Config") != -1)
			{
				// <Config Name="map001">
				std::string mapName = getBlock(info, '\"', '\"');

				// <Mesh>Terr_Forest_1</Mesh>
				in.getline(line, 256);
				info = Trim(line);
				std::string meshName = getBlock(info, '>', '<') + ".mesh";

				// <Grid X="15" Z="14" Size="10" />
				in.getline(line, 256);

				getXZ(line, mX, mZ, mBlockSize);
				mXL = mX * mBlockSize;
				mZL = mZ * mBlockSize;
				mHXL = mXL / 2.0f;
				mHZL = mZL / 2.0f;
				// 初始化碰撞位置数据
				for (s32 i = 0; i <= mZ; i++)
				{
					mCollisionValue.push_back(std::vector<s32>(mX + 1, 0));
					mCollisionMeshs.push_back(std::vector<Mesh*>(mX + 1, 0));
				}

				// </Config>
				in.getline(line, 256);

				mTerrainMesh = mScene->createMesh(mapName, meshName);
				mTerrainMesh->setCullFlag(false);
			}

			in.getline(line, 256);
			info = Trim(line);

			if (info.find("Nodes") != -1)
			{
				in.getline(line, 256);
				info = Trim(line);
				while (info.find("Node") != -1)
				{
					int i = info.find("</Nodes>");
					if (i != -1)
						break;

					// <Node Name="Tree_3#120">
					std::string nodeName = getBlock(info, '\"', '\"');

					// <Mesh>Tree_3</Mesh>
					in.getline(line, 256);
					info = Trim(line);
					std::string meshName = getBlock(info, '>', '<') + ".mesh";

					// <Position X="-49.649979" Y="4.247236" Z="-5.005510" />
					in.getline(line, 256);
					info = Trim(line);
					vector3df pos = getPos(info);

					Mesh *object = mScene->createMesh(nodeName, meshName);
					object->setPosition(pos);

					// <Map X="6" Z="2" Value="0" />
					in.getline(line, 256);
					s32 x, z, value;
					getValue(line, x, z, value);
					if (value >= mCollisionValue[z][x])
					{
						mCollisionValue[z][x] = value;
						mCollisionMeshs[z][x] = object;
					}

					// </Node>
					in.getline(line, 256);
					in.getline(line, 256);
					info = Trim(line);
				}
			}
		}

		in.close();

		//Log("Map Load Successed!");

		return true;
	}

	bool GameManager::canGo(f32 x, f32 z)
	{
		// 超出边界
		if (x <= -mHXL || x >= mHXL || z <= -mHZL || z >= mHZL)
			return false;

		s32 xoff = (s32)((x + mHXL) / mBlockSize);
		s32 zoff = (s32)((z + mHZL) / mBlockSize);

		if (mCollisionValue[xoff][zoff] > 0)
			return false;

		return true;
	}

	void GameManager::getLogicXZ(f32 x, f32 z, s32 &outX, s32 &outZ)
	{
		// 超出边界
		if (x <= -mHXL || x >= mHXL || z <= -mHZL || z >= mHZL)
		{
			outX = outZ = -1;
		}
		else
		{
			outX = (s32)((x + mHXL) / mBlockSize);
			outZ = (s32)((z + mHZL) / mBlockSize);
		}
	}

	Mesh* GameManager::checkObject(f32 x, f32 z)
	{
		// 超出边界
		if (x <= -mHXL || x >= mHXL || z <= -mHZL || z >= mHZL)
			return mTerrainMesh;

		s32 xoff = (s32)((x + mHXL) / mBlockSize);
		s32 zoff = (s32)((z + mHZL) / mBlockSize);

		if (mCollisionValue[xoff][zoff] > 0)
			return mCollisionMeshs[xoff][zoff];
		else
			return NULL;
	}

	Tank* GameManager::checkTank(f32 x, f32 y, f32 z)
	{
		// 首先检查主角
		if (mMainPlayer->getTank()->intersect(vector3df(x, y, z)))
			return mMainPlayer->getTank();

		for (TankIter itr = mTanks.begin(); itr != mTanks.end(); ++itr)
		{
			if ((*itr)->intersect(vector3df(x, y, z)))
				return *itr;
		}
		return NULL;
	}

	void GameManager::playSound(SOUND_YYPE sound, bool loop)
	{
		const static std::string Background = "Background.wav";
		const static std::string Fire = "Fire.wav";
		const static std::string Explode = "Explosion.wav";
		s32 flag = /*SND_NOSTOP | */SND_ASYNC;
		if (loop)
			flag |= SND_LOOP;

		std::string soundName = Background;
		switch (sound)
		{
		case SOUND_BACKGROUND:
			soundName = Background;
			break;
		case SOUND_FIRE:
			soundName = Fire;
			break;
		case SOUND_EXPLODE:
			soundName = Explode;
			break;
		};

		::sndPlaySound(GetPath(soundName).c_str(), flag);
	}
}