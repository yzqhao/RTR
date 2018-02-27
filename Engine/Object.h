
#ifndef _ENGINE_OBJECT_H_
#define _ENGINE_OBJECT_H_

#include <string>
#include "type.h"

namespace RTR
{
	class Object
	{
	public:
		virtual ~Object(){}

		virtual std::string getName() const { return mName; }
		virtual void update() {}

		// 当前物体是否还存活着
		virtual bool isAlive() const { return mCurrentLive < mMaxLive; }

	protected:
		Object() : mMaxLive(1), mCurrentLive(0){}

		std::string	mName;

		s32			mMaxLive;		// 最大生命时间
		s32			mCurrentLive;	// 当前已经经过的时间
	};
}

#endif