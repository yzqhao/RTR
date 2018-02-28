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

			// ���ڳ���ȫ�����¼���, ������Ҫ���»�ȡ��ɫ�������ͷɻ�
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
			scene->drawString("1. W A S D �ƶ�Tank", 10, 10);
			scene->drawString("2. <-   -> ��׼", 10, 30);
			scene->drawString("3. Space   �����ӵ�", 10, 50);
			scene->drawString("4. F       �л��߿�/ʵ��ģʽ", 10, 70);
			scene->drawString("5. Y       ����/�رյƹ�", 10, 90);
			scene->drawString("6. G       �л��ӵ�", 10, 110);
			scene->drawString("7. C       �л�����", 10, 130);

			scene->drawString("��ǰ������ : " + IntToString(scene->getVisibleObjectNumber()), 650, 10, Color(255, 255, 0));
			scene->drawString("�������� : " + IntToString(scene->getTotalPolyonNumber()), 650, 30, Color(255, 255, 0));
			scene->drawString("�ɼ����� : " + IntToString(scene->getVisiblePolyonNumber()), 650, 50, Color(255, 255, 0));
		}
		else
			scene->drawString("H ��ʾ����", 10, 10);

		scene->drawString("���ٵ��� - " + IntToString(gameMgr->getDestoryEnemyNumber()), 20, 510, Color(255, 255, 0));
		scene->drawString("ʣ����� - " + IntToString(gameMgr->getLastEnemyNumber()), 20, 530, Color(255, 255, 0));

		scene->drawString("GridPos # " + IntToString(mLogicX) + " : " + IntToString(mLogicZ), 200, 510, Color(255, 255, 0));

	}
}