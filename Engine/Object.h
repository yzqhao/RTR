
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

		// ��ǰ�����Ƿ񻹴����
		virtual bool isAlive() const { return mCurrentLive < mMaxLive; }

	protected:
		Object() : mMaxLive(1), mCurrentLive(0){}

		std::string	mName;

		s32			mMaxLive;		// �������ʱ��
		s32			mCurrentLive;	// ��ǰ�Ѿ�������ʱ��
	};
}

#endif