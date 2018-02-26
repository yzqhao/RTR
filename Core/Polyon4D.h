
#ifndef _CORE_POLYON4D_H_
#define _CORE_POLYON4D_H_


#include <vector>

#include "type.h"
#include "Util.h"
#include "Vector3.h"
#include "Vertex4D.h"

namespace RTR
{
	typedef std::vector<Vertex4D> Vertex4DArray;

	// Poly4D1_TYPE
	// 此多边形结构是基于顶点索引方式的数据结构
	struct Polyon4D
	{
		s32 state;			// 状态信息
		s32 attribute;		// 多边形物理属性
		s32 color;			// 多边形颜色


		std::vector<Vertex4D>	*verList;		// 这里的引用是指的transformList
		s32			verIndex[3];	// 定点索引

		Polyon4D() : state(0), attribute(0), color(0xffffff), verList(NULL){}
	};


	// POLYF4DV1_TYP
	// 此多边形结构是基于顶点方式的数据结构
	struct PolyonF4D
	{
		s32 state;			// 状态信息
		s32 attribute;		// 多边形物理属性
		s32 color;			// 多边形光照强度颜色

		Vertex4D localList[3];			// 物体局部坐标
		Vertex4D transformList[3];		// 物体局部坐标经变换之后的坐标

		struct Material *material;

		PolyonF4D() : state(0), attribute(0), color(0xffffff){}
	};

	struct CompEPolyonF4D
	{
		bool operator()(const PolyonF4D &polyA, const PolyonF4D &polyB)
		{
			f32 zA = Max(polyA.transformList[0]._z, Max(polyA.transformList[1]._z, polyA.transformList[2]._z));
			f32 zB = Max(polyB.transformList[0]._z, Max(polyB.transformList[1]._z, polyB.transformList[2]._z));
			// 这里写成 < 在debug模式下会崩溃
			if (Abs(zA - zB) < 0.005f)
			{
				zA = (polyA.transformList[0]._z + polyA.transformList[1]._z + polyA.transformList[2]._z) / 3.0f;
				zB = (polyB.transformList[0]._z + polyB.transformList[1]._z + polyB.transformList[2]._z) / 3.0f;

				if (zA <= zB)
					return false;
				else
					return true;
			}
			else if (zA < zB)
				return false;
			else
				return true;
		}
	};

}

#endif

