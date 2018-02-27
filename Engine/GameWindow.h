
#ifndef _ENGINE_GAMEWINDOW_H_
#define _ENGINE_GAMEWINDOW_H_

#include <Windows.h>
#include <string>
#include <vector>

#include "type.h"

namespace RTR
{
	class InputListener;
	class Canvas;
	// ��Ϸ���ڹ�����
	class GameWindow
	{
	public:
		// ȫ����ָ��, ������WinProc�лص�
		static GameWindow *GWindow;

		// window�¼�����
		void startLoop();

		GameWindow(const std::string& windowTitle, HINSTANCE hInstance);
		~GameWindow();

		// ��ʾ����
		void showWindow(bool show);
		void updateWindow();

		void quiteApplication();

		HWND getHWnd() const { return mHwnd; }
		HINSTANCE getHInstance() const { return mHinstance; }

		// ע��/�Ƴ�IO�¼�����
		void addInputListener(InputListener* listener);
		void removeInputListener(InputListener* listener);

		// ���û��Ƽ�����, ͬʱֻ����һ������Canvas
		void setCanvasListener(Canvas *canvasListener);

	protected:

		// ȫ��Win�¼��ص�����
		static LRESULT WinProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

		// ���ڻ��ƺ���
		virtual void onPaint(HDC hdc);
		virtual void onUpdate();
		virtual void onKeyDown(s32 msg);
		virtual void onKeyRelease(s32 msg);
		virtual void onMousePress(bool rightPress);
		virtual void onMouseMove(s32 x, s32 y);
		virtual void onMouseWheel(s32 delta);

	protected:
		// ע�ᴰ��
		bool registerWindow();
		// ��������
		bool createWindow();

	protected:

		HWND							mHwnd;
		std::string						mWindowTitle;
		HINSTANCE						mHinstance;
		std::vector<InputListener*>	mInputListeners;
		Canvas							*mCanvasListener;

		bool							mQuite;

		typedef std::vector<InputListener*>::iterator	InputItr;
	};
}

#endif