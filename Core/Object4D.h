
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
	// ���ڶ����б�Ͷ�����б������
	struct Object4D
	{
		std::string	name;			// ��������
		s32			state;			// ����״̬
		s32			attribute;		// ��������
		f32			avgRadius;		// �����ƽ���뾶, ������ײ���
		f32			maxRadius;		// ��������뾶

		bool			needCull;		// �Ƿ���Ҫִ���޳�����

		std::string			materiaName;	// ģ�Ͳ��ʵ�����--for Ogre Mesh

		vector3df		minBoundingBox;	// ģ���������Χ����С��
		vector3df		maxBoundingBox;	// ģ���������Χ�������

		// ���ﲻ��ʹ���黹������, ���Ե�һ��ģ�͵Ĳ���Ϊ׼
		vector3df		worldPosition;	// ���嵱ǰ�����������е�λ��
		vector3df		scale;			// ���嵱ǰ����ֵ

		vector3df		direction;		// ����ķ�������, ��������¼�Ӹ������ת�Ƕ�


		s32			vertexNumber;	// �������

		std::vector<Vertex4D>	localList;		// ����任ǰ������ֲ���������
		std::vector<Vertex4D>	transformList;	// ����任�������

		s32			polyonNumber;	// ���������������εĸ���
		std::vector<Polyon4D>	polyonList;		// �洢����ε�����

		// ���������Ϊ�˴���һ�������mesh�ж��submesh����������ǵ�, ����ж��submesh�Ļ�
		// nextObject != NULL �����ָ���ָ����һ��submesh, ֱ��nextObject == NULL
		Object4D		*nextObject;

		Object4D() : nextObject(NULL), materiaName(DEFAULT_NAME),
			scale(1, 1, 1), direction(vector3df(1,0,0)), needCull(true)
		{}
	};
}

#endif

