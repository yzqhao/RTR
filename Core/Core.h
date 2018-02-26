
#ifndef _CORE_CORE_H_
#define _CORE_CORE_H_

#include <list>

#include "type.h"
#include "Object4D.h"
#include "Polyon4D.h"
#include "Plane3d.h"
#include "Frustum.h"
#include "Light.h"
#include "Material.h"

namespace RTR
{
#define EPOLY_ATTR_2SIDE				0x0001	// ˫��
#define EPOLY_ATTR_TRANSPARENT			0x0002	// ͸��

#define EPOLY_ATTR_SHADE_MODE_PURE      0x0004
#define EPOLY_ATTR_SHADE_MODE_CONSTANT  0x0004 // (alias)
#define EPOLY_ATTR_SHADE_MODE_FLAT      0x0008
#define EPOLY_ATTR_SHADE_MODE_GOURAUD   0x0010
#define EPOLY_ATTR_SHADE_MODE_PHONG     0x0020
#define EPOLY_ATTR_SHADE_MODE_FASTPHONG 0x0020 // (alias)
#define EPOLY_ATTR_SHADE_MODE_TEXTURE   0x0040 

#define EPOLY_ATTR_VERTEX_POSITION		0x1000
#define EPOLY_ATTR_VERTEX_NORMAL		0x2000
#define EPOLY_ATTR_VERTEX_UV			0x4000

#define EPOLY_STATE_ACTIVE				0x0100	// ��Ч��״̬
#define EPOLY_STATE_CLIPPED				0x0200	// ���ü�״̬
#define EPOLY_STATE_BACKFACE			0x0400	// ����״̬

#define EOBJECT_STATE_ACTIVE			0x0001
#define EOBJECT_STATE_VISIBLE			0x0002 
#define EOBJECT_STATE_CULLED			0x0004

	// �����޳�����
	enum CULL_TYPE
	{
		CULL_BOX = 0,	// �����Χ��AABB�޳�
		CULL_SPHERE = 1,	// ����������޳�
	};

	// ����任���Ʊ��
	enum OBJ_TRANSFORM_TYPE
	{
		TRANSFORM_LOCAL = 0,			// ֻ�Ծֲ������б���б任
		TRANSFORM_TRANS = 1,			// ֻ�Ա任��Ķ����б���б任
		TRANSFORM_LOCAL_TO_TRANS = 2,	// �Ծֲ������б���б任, ��������洢�ڱ任��Ķ����б���
	};

	// ��Ⱦ�б�
	struct RenderList4D
	{
		s32						state;		// ����״̬
		s32						attribute;	// ��������

		std::list<PolyonF4D>		polyData;	// ����Ķ��������, ʹ��˫�˶���, ɾ������Ч

		typedef std::list<PolyonF4D>::iterator Itr;
	};

	// ��EObject���б任, ���Խ���λ�Ʊ任����ת�任
	// tansformTypeָ���˶���һ�������б���б任
	// transformBasis ָ�����Ƿ�Ҫ�Գ����������б任
	void Tranform_Object4D(Object4D* object, const Matrix &mat, OBJ_TRANSFORM_TYPE tansformType);

	// ��ERenderList4D���б任, ���Խ���λ�Ʊ任����ת�任
	// tansformTypeָ���˶���һ�������б���б任
	void Transform_Renderlist4D(RenderList4D* renderList, const Matrix &mat, OBJ_TRANSFORM_TYPE transformType);

	// ��Object���뵽RenderList, ����LocalList�洢����ԭʼ����, TransformList�洢���Ǿ���
	// ƽ�ƺ����ŵ�����
	void Inert_Object4D_To_RenderList4D(RenderList4D *renderList, Object4D *obj,
		OBJ_TRANSFORM_TYPE transformType = TRANSFORM_LOCAL_TO_TRANS, bool transformScaleAndPosition = false);

	// �ֲ����궨���б�任����������, ������ֱ��ʹ�üӷ�, ��û��ʹ���������˵ķ���, ��Ϊû�б�Ҫ
	void Local_To_World_Object4D(Object4D* obj, OBJ_TRANSFORM_TYPE transformType = TRANSFORM_LOCAL_TO_TRANS);

	// �ֲ����궨���б�任����������, ������ֱ��ʹ�üӷ�, ��û��ʹ���������˵ķ���, ��Ϊû�б�Ҫ
	void Local_To_World_RenderList4D(RenderList4D *renderList, const vector3df &pos,
		OBJ_TRANSFORM_TYPE transformType = TRANSFORM_LOCAL_TO_TRANS);

	// ����������޳�
	bool Cull_Object4D(Object4D *object, Frustum *camera, CULL_TYPE cullType = CULL_SPHERE);

	// ��������任�����������ϵ
	void World_To_Camera_Object4D(Object4D *obj, Frustum *camera);
	void World_To_Camera_RenderList4D(RenderList4D *renderList, Frustum *camera);

	// ��RenderList�Ķ���ν����������, ʹ�û����㷨
	// ������Ҫ�ڶ���δ������������ϵʱ�ſ��Ե���, ������������
	void Sort_RenderList4D(RenderList4D *renderList);

	// ��������, ���������㷨��������ӵ��ǻ�����������, ������Ҫ�ڶ���任�����������ǰ����
	// ��������
	bool Remove_Backface(Polyon4D *poly, Frustum *camera);
	bool Remove_Backface(PolyonF4D *poly, Frustum *camera);
	void Remove_Backface_Object4D(Object4D *object, Frustum *camera);
	// light �Ƿ�Զ���ν��й��մ���
	void Remove_Backface_RenderList4D(RenderList4D *renderList, Frustum *camera);

	// ���ռ���
	// ���ռ���ֻ������������ϵ��������֮����������������ϵ�н���
	void Light_PolyonF4D(PolyonF4D *poly, Frustum *camera);
	void Light_RenderList4D(RenderList4D *renderList, Frustum *camera);

	// ���������ϵת��Ϊ͸������ϵ
	void Camera_To_Perspective_Object4D(Object4D *obj, Frustum *camera);
	void Camera_To_Perspective_RenderList4D(RenderList4D *renderList, Frustum *camera);

	// ͸������ϵת������Ļ����ϵ
	void Perspective_To_Screen_Object4D(Object4D *obj, Frustum *camera);
	void Perspective_To_Screen_RenderList4D(RenderList4D *renderList, Frustum *camera);

	// ��RenderList4D���вü�����, �ü��������������Χ�ڵĶ����, �˲���������������ռ���ִ��
	void Clip_RenderList4D(RenderList4D *renderList, Frustum *camera);

	// ��������任����Ļ����
	// �൱�ڵ���
	// World_To_Camera
	// Perspective_To_Screen��Ч��
	// ���ڲ��Ѿ�ִ�����޳�, ��������, ���մ���Ȳ���
	void World_To_Screen_RenderList4D(RenderList4D *renderList, Frustum *camera);

	// ��Ⱦ�߿�ģ��
	void Draw_Object4D_Wire(Object4D *obj);
	void Draw_RenderList4D_Wire(RenderList4D *renderList);

	// ��Ⱦʵ��ģ��(��ʵ�ֻ�ͼ)
	void Draw_Object4D_Solid(Object4D *object);
	void Draw_RenderList4D_Solid(RenderList4D *renderList);

	// ���ƴ������renderlist
	void Draw_RenderList4D_Teture_Solid(RenderList4D * renderList);
}

#endif

