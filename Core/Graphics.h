
#ifndef _CORE_GRAPHICS_H_
#define _CORE_GRAPHICS_H_

#include <Windows.h>
#include <string>

#include "type.h"
#include "Util.h"
#include "Color.h"

// 图形渲染底层分实现, 

namespace RTR
{
	extern HINSTANCE GHInstance;

	// 图片封装, 由于图片使用的GDI+的图片类, 
	// 所以这里对他进行封装, 将平台相关的东西隐藏
	class Bitmap
	{
	public:
		s32 width, height;

		Bitmap(const std::string &filename);
		~Bitmap();

		inline std::string getName() const { return name; }
		inline bool isValid() const { return valid;}
		Color getPixel(s32 x, s32 y);
		inline s32 getHeight() const { return height;}
		inline s32 getWidth() const { return width;}

	private:
		std::string					name;


		HBITMAP					hBitmap;
		BITMAP					bitmap;
		HDC						bitmapHDC;

	public:
		Color					*pixels;		

		s32					pitch;

		bool					valid;
	};


	class Graphics
	{
	public:

		// 初始化绘图系统
		static bool initGraphics(HINSTANCE hinstance);
		// 关闭绘图系统
		static void shutdownGraphics();

		// 检测z值, 返回true则表示通过, 可以调用setPixel
		static bool checkZ(s32 x, s32 y, f32 z);
		static void setPixel(s32 x, s32 y, /*f32 z, */const Color &c);
		static Color getPixel(s32 x, s32 y);
		// 在当前缓冲区内绘制一条线
		static void drawLine(s32 x0, s32 y0, s32 x1, s32 y1, const Color &c);
		static void drawString(const std::string &str, s32 x, s32 y, const Color &c = Color(255, 255, 255));
		static void fillTriangle(s32 x0, s32 y0, s32 x1, s32 y1, s32 x2, s32 y2, 
							const Color &c = Color(255, 255, 255));

		static void enableSmoothingMode(bool enable);

		// 清空当前缓冲区, 并将其颜色设置为黑色
		static void clearBuffer(const Color &c = Color());
		// 将已经绘制好的缓冲区递交给Graphics在屏幕上绘制, 并将当前缓冲区设置为另一个缓冲区
		static void fillBuffer(HDC hdc);

		static s32 getScreenWidth() { return SCREEN_WIDTH; }
		static s32 getScreenHeight() { return SCREEN_HEIGHT; }

	private:

		// 保存变量
		static HBITMAP							GBufferedHandle;
		static HDC								GBufferedHDC;
		static HBRUSH							GBgBrush;
		static HPEN								GPen;
		static HINSTANCE						GInstance;

		static BYTE								*GDatas;
		static s32								GPitch;

		static DIBSECTION						GDIBSection;
	
		static RECT								GBufferSize;

		static f32							*GZBuffer;
	};
}

#endif