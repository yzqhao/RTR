
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
	// 游戏窗口管理类
	class GameWindow
	{
	public:
		// 全局类指针, 用于在WinProc中回调
		static GameWindow *GWindow;

		// window事件处理
		void startLoop();

		GameWindow(const std::string& windowTitle, HINSTANCE hInstance);
		~GameWindow();

		// 显示窗口
		void showWindow(bool show);
		void updateWindow();

		void quiteApplication();

		HWND getHWnd() const { return mHwnd; }
		HINSTANCE getHInstance() const { return mHinstance; }

		// 注册/移除IO事件监听
		void addInputListener(InputListener* listener);
		void removeInputListener(InputListener* listener);

		// 设置绘制监听器, 同时只能有一个绘制Canvas
		void setCanvasListener(Canvas *canvasListener);

	protected:

		// 全局Win事件回调函数
		static LRESULT WinProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

		// 窗口绘制函数
		virtual void onPaint(HDC hdc);
		virtual void onUpdate();
		virtual void onKeyDown(s32 msg);
		virtual void onKeyRelease(s32 msg);
		virtual void onMousePress(bool rightPress);
		virtual void onMouseMove(s32 x, s32 y);
		virtual void onMouseWheel(s32 delta);

	protected:
		// 注册窗口
		bool registerWindow();
		// 创建窗口
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