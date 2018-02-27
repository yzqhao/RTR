
#ifndef _ENGINE_CANVAS_H_
#define _ENGINE_CANVAS_H_

namespace RTR
{
	// 绘图接口
	class Canvas
	{
	public:
		virtual ~Canvas(){}

		// 这里只负责绘制模型
		virtual void onPaint() = 0;

		// 这里更新场景
		virtual void update() = 0;
	};
}

#endif