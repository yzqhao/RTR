
#ifndef _GAME_GAME_H_
#define _GAME_GAME_H_

#include "Canvas.h"
#include "InputListener.h"

namespace RTR
{
	// 游戏实体类, Game相当于一个关卡, 有接受输入控制和绘制的功能
	class Game : public Canvas, public InputListener
	{
	public:
		Game();

		// 键盘鼠标操作
		virtual bool keyPress(s32 key);
		virtual bool keyRlease(s32 key);

		virtual bool mouseButtonDown(s32 mouseButton){ return true; }
		virtual bool mouseButtonRelease(s32 mouseButton){ return true; }
		virtual bool mouseMove(s32 x, s32 y){ return true; }
		virtual bool mouseWheel(s32 delta){ return true; }

		// 更新和绘制
		virtual void onPaint();
		virtual void update();

	protected:
		// 创建场景
		virtual void createScene();
		// 显示相关信息
		void showInfo();

	protected:
		bool					showHelp;

		class Mesh				*plane;

		class GameManager		*gameMgr;

		s32					mLogicX, mLogicZ;
	};
}

#endif