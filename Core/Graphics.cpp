
#include "Graphics.h"
#include "Util.h"

namespace RTR
{
	Bitmap::Bitmap(const std::string &filename) : name(filename), pixels(NULL),
		pitch(0), width(0), height(0), valid(false)
	{
		// 材质脚本, 首先获取默认路径
		hBitmap = (HBITMAP)::LoadImage(GHInstance, GetPath(filename).c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

		if (hBitmap != NULL)
		{
			bitmapHDC = ::CreateCompatibleDC(NULL);
			::SelectObject(bitmapHDC, (HGDIOBJ)hBitmap);
			::GetObject(hBitmap,sizeof(BITMAP),&bitmap);

			width	= bitmap.bmWidth;
			height  = bitmap.bmHeight;
			pitch	= bitmap.bmHeight;
			valid	= true;

			pixels = new Color[width * height];
			for (s32 i = 0; i < getHeight(); i++)
			{
				for (s32 j = 0; j < getWidth(); j++)
				{
					COLORREF color = ::GetPixel(bitmapHDC, i, j);
					pixels[j * width + i] = Color(GetRValue(color), GetGValue(color), GetBValue(color));
				}
			}
		}

	}

	Bitmap::~Bitmap()
	{
		DeleteObject(hBitmap);
		DeleteDC(bitmapHDC);
		SafeDeleteArray(pixels);
	}

	Color Bitmap::getPixel(s32 x, s32 y)
	{
		return pixels[y * pitch + x];
	}

	HINSTANCE GHInstance;

	HBITMAP							Graphics::GBufferedHandle;
	HDC								Graphics::GBufferedHDC;
	HBRUSH							Graphics::GBgBrush;
	HPEN							Graphics::GPen;
	HINSTANCE						Graphics::GInstance;
	RECT							Graphics::GBufferSize;
	DIBSECTION						Graphics::GDIBSection;
	BYTE							*Graphics::GDatas;
	s32							Graphics::GPitch;
	f32							*Graphics::GZBuffer;

	// 初始化绘图系统
	bool Graphics::initGraphics(HINSTANCE hinstance)
	{
		GBufferedHDC = ::CreateCompatibleDC(NULL);

		////////////////////
		BITMAPINFO info					= {0};    
		info.bmiHeader.biSize			= sizeof(info.bmiHeader);    
		info.bmiHeader.biWidth			= SCREEN_WIDTH;
		info.bmiHeader.biHeight			= -SCREEN_HEIGHT;    
		info.bmiHeader.biPlanes			= 1;    
		info.bmiHeader.biBitCount		= 32;    
		info.bmiHeader.biCompression	= BI_RGB;    
		info.bmiHeader.biSizeImage		= SCREEN_WIDTH * SCREEN_HEIGHT * 32 / 8;  

		// 创建一块内存纹理并获取其数据指针
		void* pBits = NULL;
		GBufferedHandle = ::CreateDIBSection(GBufferedHDC, &info, DIB_RGB_COLORS, &pBits, NULL, 0);
		::SelectObject(GBufferedHDC, GBufferedHandle);

		//here: "dib.dsBm.bmBits" will points to the pixels of hdib.
		::GetObject(GBufferedHandle , sizeof(DIBSECTION), &GDIBSection);
		GDatas = (BYTE*)GDIBSection.dsBm.bmBits;
		GPitch = GDIBSection.dsBm.bmWidthBytes;

		// 设置刷新区域大小
		::SetRect(&GBufferSize, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

		//GPen = (HPEN)::GetStockObject(WHITE_PEN); 
		GPen = ::CreatePen(PS_SOLID, 1, RGB(255, 255, 255)); 
		::SelectObject(GBufferedHDC, GPen);

		GBgBrush= ::CreateSolidBrush(RGB(0, 0, 0));
		::SelectObject (GBufferedHDC, GBgBrush) ;

		// 设置字体
		HFONT hfnt = (HFONT)::GetStockObject(OEM_FIXED_FONT); 
		::SelectObject(GBufferedHDC, hfnt);
		// 设置文字背景为透明色
		::SetBkMode(GBufferedHDC, TRANSPARENT); 

		GZBuffer = new f32[SCREEN_WIDTH * SCREEN_HEIGHT];
		memset(GZBuffer, 0, sizeof(f32) * SCREEN_WIDTH * SCREEN_HEIGHT);

		return true;
	}

	// 关闭绘图系统
	void Graphics::shutdownGraphics()
	{
		::DeleteObject(GPen);
		::DeleteObject(GBgBrush);
		::DeleteObject(GBufferedHandle);
		::DeleteDC(GBufferedHDC);
	}

	// 清空当前缓冲区, 并将其颜色设置为黑色
	void Graphics::clearBuffer(const Color &c)
	{
		::FillRect(GBufferedHDC, &GBufferSize, GBgBrush);
		// 重置深度缓存
		// 注 : 这里memset只能对Int类型的数组进行初始化, 所以这里直接使用了Int类型
		// 而没有使用float类型, 应该使用float
		::memset(GZBuffer, 0, sizeof(f32) * SCREEN_WIDTH * SCREEN_HEIGHT);
	}

	// 在当前缓冲区内绘制一条线
	void Graphics::drawLine(s32 x0, s32 y0, s32 x1, s32 y1, const Color &c)
	{
		::SelectObject(GBufferedHDC,GetStockObject(DC_PEN));
		::SetDCPenColor(GBufferedHDC, RGB(c.r, c.g, c.b));
		
		::MoveToEx(GBufferedHDC, x0, y0, NULL);
		::LineTo(GBufferedHDC, x1, y1);
	}

	void Graphics::drawString(const std::string &str, s32 x, s32 y, const Color &c)
	{
		::SetTextColor(GBufferedHDC, RGB(c.r, c.g, c.b));
		::TextOut(GBufferedHDC, x, y, str.c_str(), str.length()); 

	}

	void Graphics::fillTriangle(s32 x0, s32 y0, s32 x1, s32 y1, s32 x2, s32 y2, const Color &c)
	{

	}

	void Graphics::enableSmoothingMode(bool enable)
	{

	}

	bool Graphics::checkZ(s32 x, s32 y, f32 z)
	{
		// 这里Z应该使用float类型来存储, 如果使用int类型, 那么会导致精度丢失
		// 产生错误的现象
		s32 index = y * SCREEN_WIDTH + x;
		f32 divZ = 1.0f / z;
		// 这里是基于1/z做的比较
		if (GZBuffer[index] > divZ)
			return false;

		GZBuffer[index] = divZ;
		return true;
	}

	void Graphics::setPixel(s32 x, s32 y, /*f32 z, */const Color &c)
	{
		// 这里本来应该计算z值,但是为了避免对Image像素的读取, 我将z检查分离了出来
		// 所以, 在调用setPixel之前应该先检测checkZ, 返回true在调用setPixel
		BYTE* pSrcPix = GDatas + (GPitch * y);
		pSrcPix += x * 4;
		// blue green red alpha
		pSrcPix[0] = c.b;
		pSrcPix[1] = c.g;
		pSrcPix[2] = c.r;
		pSrcPix[3] = c.a;
	}

	Color Graphics::getPixel(s32 x, s32 y)
	{
		//int pitch = GDIBSection.dsBm.bmWidthBytes;
		//pointer that will advance in memory one pixel at a time...
		BYTE* pSrcPix = GDatas + (GPitch * y);
		pSrcPix += x * 4;
		return Color(pSrcPix[2], pSrcPix[1], pSrcPix[0]);
	}

	// 将已经绘制好的缓冲区递交给Graphics在屏幕上绘制, 并将当前缓冲区设置为另一个缓冲区
	void Graphics::fillBuffer(HDC hdc)
	{	
		::BitBlt(hdc,0,0,SCREEN_WIDTH,SCREEN_HEIGHT,GBufferedHDC,0,0,SRCCOPY);
	}
}