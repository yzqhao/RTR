
#ifndef _ENGINE_INPUTLISTENER_H_
#define _ENGINE_INPUTLISTENER_H_

#include "type.h"

namespace RTR
{
	// Êó±ê¼üÅÌ¼àÌýÆ÷
	class InputListener
	{
	public:
		virtual bool keyPress(s32 key) = 0;
		virtual bool keyRlease(s32 key) = 0;

		virtual bool mouseButtonDown(s32 mouseButton) = 0;
		virtual bool mouseButtonRelease(s32 mouseButton) = 0;
		virtual bool mouseMove(s32 x, s32 y) = 0;
		virtual bool mouseWheel(s32 delta) = 0;

		virtual ~InputListener(){}
	};
}

#endif