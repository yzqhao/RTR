
#ifndef _GAME_PLAYERCONTROLLER_H_
#define _GAME_PLAYERCONTROLLER_H_

#include "Tank.h"

namespace RTR
{
	enum DIRECTION
	{
		DIR_UP = 0x000001 << 1,	// 上
		DIR_DOWN = 0x000001 << 2,	// 下
		DIR_LEFT = 0x000001 << 4,	// 左
		DIR_RIGHT = 0x000001 << 8,	// 右
	};

	// 角色控制器, 控制角色模型和绑定摄像机
	class SceneManager;
	class GameManager;
	class PlayerController
	{
	public:
		virtual ~PlayerController();

		// 设置/获取当前角色的坐标
		void setPosition(const vector3df &pos);
		vector3df getPosition() const { return mTank->getPosition(); }

		// 获取控制的Tank
		Tank* getTank() { return mTank; }

		void update();

		void keyPress(s32 key);
		void keyRlease(s32 key);
		void mouseWheel(s32 delta){}
		void mouseButtonDown(s32 mouseButton){}
		void mouseButtonRelease(s32 mouseButton){}
		void mouseMove(s32 x, s32 y){}

	private:
		friend class GameManager;
		PlayerController(GameManager *gameMgr);

	protected:
		SceneManager	*mScene;
		GameManager	*mGameMgr;
		Tank			*mTank;
		Camera			*mCamera;
		s32			mCurDir;

		s32			mFireDir;
	};
}

#endif