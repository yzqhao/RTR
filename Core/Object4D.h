
#ifndef _CORE_OBJECT_H_
#define _CORE_OBJECT_H_


#include <vector>
#include <string>

#include "type.h"
#include "Util.h"
#include "Vector3.h"
#include "Vertex4D.h"
#include "Polyon4D.h"

namespace RTR
{
	// 基于定点列表和多边形列表的物体
	struct Object4D
	{
		std::string	name;			// 物体名称
		s32			state;			// 物体状态
		s32			attribute;		// 物体属性
		f32			avgRadius;		// 物体的平均半径, 用于碰撞检测
		f32			maxRadius;		// 物体的最大半径

		bool			needCull;		// 是否需要执行剔除操作

		std::string			materiaName;	// 模型材质的名称--for Ogre Mesh

		vector3df		minBoundingBox;	// 模型正方体包围盒最小点
		vector3df		maxBoundingBox;	// 模型正方体包围盒做大点

		// 这里不论使评议还是缩放, 均以第一个模型的参数为准
		vector3df		worldPosition;	// 物体当前在世界坐标中的位置
		vector3df		scale;			// 物体当前缩放值

		vector3df		direction;		// 物体的方向向量, 各分量记录延各轴的旋转角度


		s32			vertexNumber;	// 顶点个数

		std::vector<Vertex4D>	localList;		// 物体变换前的物体局部坐标数组
		std::vector<Vertex4D>	transformList;	// 物体变换后的坐标

		s32			polyonNumber;	// 物体的所包含多边形的个数
		std::vector<Polyon4D>	polyonList;		// 存储多边形的数组

		// 这个变量是为了处理一个导入的mesh有多个submesh的情况来考虑的, 如果有多个submesh的话
		// nextObject != NULL 否则该指针会指向下一个submesh, 直至nextObject == NULL
		Object4D		*nextObject;

		Object4D() : nextObject(NULL), materiaName(DEFAULT_NAME),
			scale(1, 1, 1), direction(vector3df(1,0,0)), needCull(true)
		{}
	};
}

#endif

