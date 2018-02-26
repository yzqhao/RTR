
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
	// �˶���νṹ�ǻ��ڶ���������ʽ�����ݽṹ
	struct Polyon4D
	{
		s32 state;			// ״̬��Ϣ
		s32 attribute;		// �������������
		s32 color;			// �������ɫ


		std::vector<Vertex4D>	*verList;		// �����������ָ��transformList
		s32			verIndex[3];	// ��������

		Polyon4D() : state(0), attribute(0), color(0xffffff), verList(NULL){}
	};


	// POLYF4DV1_TYP
	// �˶���νṹ�ǻ��ڶ��㷽ʽ�����ݽṹ
	struct PolyonF4D
	{
		s32 state;			// ״̬��Ϣ
		s32 attribute;		// �������������
		s32 color;			// ����ι���ǿ����ɫ

		Vertex4D localList[3];			// ����ֲ�����
		Vertex4D transformList[3];		// ����ֲ����꾭�任֮�������

		struct Material *material;

		PolyonF4D() : state(0), attribute(0), color(0xffffff){}
	};

	struct CompEPolyonF4D
	{
		bool operator()(const PolyonF4D &polyA, const PolyonF4D &polyB)
		{
			f32 zA = Max(polyA.transformList[0]._z, Max(polyA.transformList[1]._z, polyA.transformList[2]._z));
			f32 zB = Max(polyB.transformList[0]._z, Max(polyB.transformList[1]._z, polyB.transformList[2]._z));
			// ����д�� < ��debugģʽ�»����
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

