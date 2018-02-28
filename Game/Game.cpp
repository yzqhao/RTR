#include "Game.h"
#include "SceneManager.h"
#include "Camera.h"
#include "Mesh.h"
#include "GameManager.h"
#include "PlayerController.h"

namespace RTR
{
	const static std::string map001 = "map001.map";
	const static std::string map002 = "map002.map";

	Game::Game() : plane(NULL), showHelp(false), scene(NULL),
		player(NULL), gameMgr(NULL), mLogicX(-1), mLogicZ(-1)
	{
		scene = new SceneManager();
		gameMgr = new GameManager(scene);
		player = gameMgr->getPlayerController();

		currentObjectNum = 1;
		visiblePolyons = 0;

		createScene();
	}

	void Game::createScene()
	{
		Light *light = scene->createLight(LIGHT_DIRECTION);
		light->diffuse = Color(255, 255, 255);
		light->direction = vector3df(-1, -1, -1);
		light->shadowFactor = 0.5f;
		light->power = 1.5f;

		gameMgr->loadMap(map001);

		plane = scene->createMesh("Object2", "Plane_2.mesh");
		plane->setPosition(vector3df(30, 15.0f, 30));

		gameMgr->startGame();
	}

	bool Game::keyPress(s32 key)
	{
		if (key == 'Y')
		{
			scene->enableLights(!scene->isLightsEnable());
		}
		else if (key == 'H')
		{
			showHelp = !showHelp;
		}
		else if (key == 'C')
		{
			if (gameMgr->getCurrentMapName() == map001)
				gameMgr->changeMap(map002);
			else
				gameMgr->changeMap(map001);

			// 由于场景全部重新加载, 所以需要重新获取角色控制器和飞机
			plane = scene->createMesh("Object2", "Plane_2.mesh");
			player = gameMgr->getPlayerController();
		}
		else
		{
			player->keyPress(key);
		}

		return true;
	}

	bool Game::keyRlease(s32 key)
	{
		player->keyRlease(key);
		return true;
	}

	void Game::update()
	{
		static f32 degree = 0;
		degree += 10.f;
		plane->yaw(degree + 10.f);
		plane->setPosition(vector3df(30 * sin(degree * 0.01f), 15.0f, 30 * cos(degree * 0.01f)));

		gameMgr->update();
		scene->update();

		mLogicX = player->getPosition()._x;
		mLogicZ = player->getPosition()._z;
		gameMgr->getLogicXZ(mLogicX, mLogicZ, mLogicX, mLogicZ);
	}

	void Game::onPaint()
	{
		showInfo();
	}

	void Game::showInfo()
	{
		currentObjectNum = scene->getVisibleObjectNumber();
		visiblePolyons = scene->getVisiblePolyonNumber();

		if (showHelp)
		{
			scene->drawString("1. W A S D 移动Tank", 10, 10);
			scene->drawString("2. <-   -> 瞄准", 10, 30);
			scene->drawString("3. Space   发射子弹", 10, 50);
			scene->drawString("4. F       切换线框/实体模式", 10, 70);
			scene->drawString("5. Y       开启/关闭灯光", 10, 90);
			scene->drawString("6. G       切换子弹", 10, 110);
			scene->drawString("7. C       切换场景", 10, 130);

			scene->drawString("当前物体数 : " + IntToString(scene->getVisibleObjectNumber()), 650, 10, Color(255, 255, 0));
			scene->drawString("所有面数 : " + IntToString(scene->getTotalPolyonNumber()), 650, 30, Color(255, 255, 0));
			scene->drawString("可见面数 : " + IntToString(scene->getVisiblePolyonNumber()), 650, 50, Color(255, 255, 0));
		}
		else
			scene->drawString("H 显示帮助", 10, 10);

		scene->drawString("击毁敌人 - " + IntToString(gameMgr->getDestoryEnemyNumber()), 20, 510, Color(255, 255, 0));
		scene->drawString("剩余敌人 - " + IntToString(gameMgr->getLastEnemyNumber()), 20, 530, Color(255, 255, 0));

		scene->drawString("GridPos # " + IntToString(mLogicX) + " : " + IntToString(mLogicZ), 200, 510, Color(255, 255, 0));

	}
}