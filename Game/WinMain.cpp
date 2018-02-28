#include "GameWindow.h"
#include "Graphics.h"
#include "Game.h"

#include "res/resource.h"

using namespace RTR;

int WINAPI WinMain(
	HINSTANCE hInstance,      // handle to current instance
	HINSTANCE hPrevInstance,  // handle to previous instance
	LPSTR lpCmdLine,          // command line
	int nCmdShow              // show state
	)
{	
	Graphics::initGraphics(hInstance);

	GameWindow::GWindow = new GameWindow("E3D [3DTankWar]", hInstance);
	GameWindow::GWindow->showWindow(true);

	// 设置游戏的小图标
	HWND hwnd = GameWindow::GWindow->getHWnd();
	LONG iconID = (LONG)LoadIcon(::GetModuleHandle(0), MAKEINTRESOURCE(IDI_ICON_TANKWAR));
	::SetClassLong(hwnd, GCL_HICON, iconID);

	Game *game = new Game;
	GameWindow::GWindow->setCanvasListener(game);
	GameWindow::GWindow->addInputListener(game);
	GameWindow::GWindow->startLoop();
	
	Graphics::shutdownGraphics();

	return 0;
}