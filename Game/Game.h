
#ifndef _GAME_GAME_H_
#define _GAME_GAME_H_

#include "Canvas.h"
#include "InputListener.h"

namespace RTR
{
	// ��Ϸʵ����, Game�൱��һ���ؿ�, �н���������ƺͻ��ƵĹ���
	class Game : public Canvas, public InputListener
	{
	public:
		Game();

		// ����������
		virtual bool keyPress(s32 key);
		virtual bool keyRlease(s32 key);

		virtual bool mouseButtonDown(s32 mouseButton){ return true; }
		virtual bool mouseButtonRelease(s32 mouseButton){ return true; }
		virtual bool mouseMove(s32 x, s32 y){ return true; }
		virtual bool mouseWheel(s32 delta){ return true; }

		// ���ºͻ���
		virtual void onPaint();
		virtual void update();

	protected:
		// ��������
		virtual void createScene();
		// ��ʾ�����Ϣ
		void showInfo();

	protected:
		bool					showHelp;

		class Mesh				*plane;

		class GameManager		*gameMgr;

		s32					mLogicX, mLogicZ;
	};
}

#endif