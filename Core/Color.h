
#ifndef _CORE_COLOR_H_
#define _CORE_COLOR_H_

#include "type.h"
#include <iostream>

#define COLOR_16BIT(r, g, b) (((r) & 0xff) << 16 | ((g) & 0xff) << 8 | (b) & 0xff)

namespace RTR
{
	class Color
	{
	public:
		u8 r, g, b, a;
		Color(s32 color, u8 alpha = 255)
		{
			r = (u8)((color & 0xff0000) >> 16);
			g = (u8)((color & 0x00ff00) >> 8);
			b = (u8)(color & 0x0000ff);
			a = alpha;
		}
		Color(u8 ri = 0, u8 gi = 0, u8 bi = 0, u8 ai = 255) :
			r(ri), g(gi), b(bi), a(ai){}

		s32 ToInt() const { return COLOR_16BIT(r, g, b); }
		Color operator *(const Color &c) const
		{
			s32 ri = r * c.r >> 8;	// r * c.r / 256
			s32 gi = g * c.g >> 8;
			s32 bi = b * c.b >> 8;
			s32 ai = a * c.a >> 8;
			return Color(ri, gi, bi, ai);
		}

		void print()
		{
			std::cout << "r : " << (s32)r << ", g : " << (s32)g << ", b : " << (s32)b << ", a : " << (s32)a << std::endl;
		}
	};
}

#endif

