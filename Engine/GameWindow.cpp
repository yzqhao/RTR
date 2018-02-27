
#include <ctime>

#include "GameWindow.h"
#include "Util.h"
#include "Graphics.h"
#include "InputListener.h"
#include "Canvas.h"

#ifndef GET_X_LPARAM
#define GET_X_LPARAM(lParam)    ((int)(short)LOWORD(lParam))
#endif
#ifndef GET_Y_LPARAM
#define GET_Y_LPARAM(lParam)    ((int)(short)HIWORD(lParam))
#endif

namespace RTR
{
	GameWindow	*GameWindow::GWindow;

	LRESULT GameWindow::WinProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
	{
		if (!GameWindow::GWindow)
			return DefWindowProc(hWnd, Msg, wParam, lParam);

		switch (Msg)
		{
			// 绘制
		case WM_MOVING:
		case WM_PAINT:
		{
						 PAINTSTRUCT  ps;
						 HDC hdc = BeginPaint(hWnd, &ps);
						 GameWindow::GWindow->onPaint(hdc);
						 EndPaint(hWnd, &ps);
						 break;
		}
		case WM_TIMER:
		{
			GameWindow::GWindow->updateWindow();
			break;
		}
			// 键盘按下
		case WM_KEYDOWN:
		{
			GameWindow::GWindow->onKeyDown(wParam);
			break;
		}
			// 键盘释放
		case WM_KEYUP:
		{
			GameWindow::GWindow->onKeyRelease(wParam);
			break;
		}
		case WM_MOUSEWHEEL:
		{
			s16 delta = GET_WHEEL_DELTA_WPARAM(wParam); // wheel rotation
			GameWindow::GWindow->onMouseWheel(delta * 0.5f / 3.141592653);
		}
			// 鼠标移动
		case WM_MOUSEMOVE:
		{
							 s32 x = GET_X_LPARAM(lParam);
							 s32 y = GET_Y_LPARAM(lParam);
							 GameWindow::GWindow->onMouseMove(x, y);
							 break;
		}
			// 关闭窗口
		case WM_CLOSE:
		{
						 DestroyWindow(hWnd);
						 break;
		}
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			break;
		}
		default:
			return DefWindowProc(hWnd, Msg, wParam, lParam);
		}
		return 0;
	}

	GameWindow::GameWindow(const std::string& windowTitle, HINSTANCE hInstance) :
		mWindowTitle(windowTitle), mHinstance(hInstance), mCanvasListener(NULL), mQuite(false)
	{
		if (!(registerWindow() && createWindow()))
		{
			throw "Init Window Failed!";
		}
	}

	GameWindow::~GameWindow()
	{
		DestroyWindow(mHwnd);
		for (InputItr itr = mInputListeners.begin(); itr != mInputListeners.end(); ++itr)
		{
			SafeDelete(*itr);
		}

		SafeDelete(mCanvasListener);
	}

	// 显示窗口
	void GameWindow::showWindow(bool show)
	{
		::ShowWindow(mHwnd, SW_SHOW);
		::UpdateWindow(mHwnd);
	}

	void GameWindow::updateWindow()
	{
		// 刷心窗口
		::InvalidateRect(mHwnd, NULL, FALSE);
		::UpdateWindow(mHwnd);
	}

	void GameWindow::setCanvasListener(Canvas *canvasListener)
	{
		mCanvasListener = canvasListener;
	}

	void GameWindow::onUpdate()
	{

	}

	// 窗口绘制函数
	void GameWindow::onPaint(HDC hdc)
	{
		if (mCanvasListener)
		{
			Graphics::clearBuffer(Color(0, 0, 0));
			mCanvasListener->update();
			mCanvasListener->onPaint();
			Graphics::fillBuffer(hdc);
		}
	}

	void GameWindow::addInputListener(InputListener* listener)
	{
		mInputListeners.push_back(listener);
	}

	void GameWindow::removeInputListener(InputListener* listener)
	{
		for (InputItr itr = mInputListeners.begin(); itr != mInputListeners.end();)
		{
			if (*itr == listener)
			{
				mInputListeners.erase(itr++);
			}
			else
				++itr;
		}
	}

	void GameWindow::onKeyDown(s32 msg)
	{
		//::MessageBox(0, "Window onKeyDown", 0, 0);
		for (InputItr itr = mInputListeners.begin(); itr != mInputListeners.end(); ++itr)
			(*itr)->keyPress(msg);
	}

	void GameWindow::onKeyRelease(s32 msg)
	{

		for (InputItr itr = mInputListeners.begin(); itr != mInputListeners.end(); ++itr)
			(*itr)->keyRlease(msg);
	}

	void GameWindow::onMousePress(bool rightPress)
	{
		if (rightPress)
			::MessageBox(0, "Window R onMousePress", 0, 0);
		else
			::MessageBox(0, "Window L onMousePress", 0, 0);
	}

	void GameWindow::onMouseMove(s32 x, s32 y)
	{

		for (InputItr itr = mInputListeners.begin(); itr != mInputListeners.end(); ++itr)
			(*itr)->mouseMove(x, y);
	}

	void GameWindow::onMouseWheel(s32 delta)
	{
		for (InputItr itr = mInputListeners.begin(); itr != mInputListeners.end(); ++itr)
			(*itr)->mouseWheel(delta);
	}

	void GameWindow::quiteApplication()
	{
		mQuite = true;
	}

	void GameWindow::startLoop()
	{

		MSG msg;
		while (!mQuite)
		{
			if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				if (msg.message == WM_QUIT)
					break;
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);

				onUpdate();
			}
			else
				::Sleep(10);
		}
	}

	bool GameWindow::createWindow()
	{
		mHwnd = ::CreateWindow("E3D", mWindowTitle.c_str(),
			// 窗口不可更改大小
			WS_OVERLAPPEDWINDOW & (~WS_MAXIMIZEBOX)& (~WS_THICKFRAME),
			CW_USEDEFAULT, CW_USEDEFAULT,
			SCREEN_WIDTH, SCREEN_HEIGHT,
			0, 0, mHinstance, 0);

		if (!mHwnd)
		{
			::MessageBox(0, "Create Window Failed", 0, 0);
			return false;
		}


		::SetTimer(mHwnd, WM_TIMER, 35, NULL);

		// 手动播种
		srand(time(NULL));

		return true;
	}

	bool GameWindow::registerWindow()
	{
		WNDCLASS wind;
		{
			wind.style = CS_HREDRAW | CS_VREDRAW;
			wind.lpfnWndProc = (WNDPROC)GameWindow::WinProc;
			wind.cbClsExtra = 0;
			wind.cbWndExtra = 0;
			wind.hInstance = mHinstance;
			wind.hIcon = LoadIcon(0, IDI_APPLICATION);
			wind.hCursor = LoadCursor(0, IDC_ARROW);
			wind.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
			wind.lpszMenuName = 0;
			wind.lpszClassName = "E3D";
		}

		if (!RegisterClass(&wind))
		{
			::MessageBox(0, "Register Class Failed", 0, 0);
			return false;
		}

		return true;
	}
}
