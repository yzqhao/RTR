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

	Game::Game() : plane(NULL), showHelp(false), gameMgr(NULL), mLogicX(-1), mLogicZ(-1)
	{
		gameMgr = new GameManager(new SceneManager());

		createScene();
	}

	void Game::createScene()
	{
		if (gameMgr->init(map001))
		{
			gameMgr->startGame();

			plane = gameMgr->getSceneManager()->createMesh("Object2", "Plane_2.mesh");
			plane->setPosition(vector3df(30, 15.0f, 30));
		}
	}

	bool Game::keyPress(s32 key)
	{
		if (key == 'Y')
		{
			gameMgr->getSceneManager()->enableLights(!gameMgr->getSceneManager()->isLightsEnable());
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
			plane = gameMgr->getSceneManager()->createMesh("Object2", "Plane_2.mesh");
		}
		else
		{
			gameMgr->getPlayerController()->keyPress(key);
		}

		return true;
	}

	bool Game::keyRlease(s32 key)
	{
		gameMgr->getPlayerController()->keyRlease(key);
		return true;
	}

	void Game::update()
	{
		static f32 degree = 0;
		degree += 10.f;
		plane->yaw(degree + 10.f);
		plane->setPosition(vector3df(30 * sin(degree * 0.01f), 15.0f, 30 * cos(degree * 0.01f)));

		gameMgr->update();
	}

	void Game::onPaint()
	{
		showInfo();
	}

	void Game::showInfo()
	{
		gameMgr->showInfo(showHelp);
	}
}