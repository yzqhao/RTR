
#ifndef _ENGINE_CANVAS_H_
#define _ENGINE_CANVAS_H_

namespace RTR
{
	// ��ͼ�ӿ�
	class Canvas
	{
	public:
		virtual ~Canvas(){}

		// ����ֻ�������ģ��
		virtual void onPaint() = 0;

		// ������³���
		virtual void update() = 0;
	};
}

#endif