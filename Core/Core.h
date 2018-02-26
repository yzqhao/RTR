
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
#define EPOLY_ATTR_2SIDE				0x0001	// 双面
#define EPOLY_ATTR_TRANSPARENT			0x0002	// 透明

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

#define EPOLY_STATE_ACTIVE				0x0100	// 有效的状态
#define EPOLY_STATE_CLIPPED				0x0200	// 被裁减状态
#define EPOLY_STATE_BACKFACE			0x0400	// 背面状态

#define EOBJECT_STATE_ACTIVE			0x0001
#define EOBJECT_STATE_VISIBLE			0x0002 
#define EOBJECT_STATE_CULLED			0x0004

	// 物体剔除类型
	enum CULL_TYPE
	{
		CULL_BOX = 0,	// 物体包围盒AABB剔除
		CULL_SPHERE = 1,	// 物体球体积剔除
	};

	// 顶点变换控制标记
	enum OBJ_TRANSFORM_TYPE
	{
		TRANSFORM_LOCAL = 0,			// 只对局部顶点列表进行变换
		TRANSFORM_TRANS = 1,			// 只对变换后的顶点列表进行变换
		TRANSFORM_LOCAL_TO_TRANS = 2,	// 对局部定点列表进行变换, 并将结果存储在变换后的顶点列表中
	};

	// 渲染列表
	struct RenderList4D
	{
		s32						state;		// 物体状态
		s32						attribute;	// 物体属性

		std::list<PolyonF4D>		polyData;	// 具体的多边形数据, 使用双端队列, 删除更有效

		typedef std::list<PolyonF4D>::iterator Itr;
	};

	// 对EObject进行变换, 可以进行位移变换和旋转变换
	// tansformType指定了对哪一个定点列表进行变换
	// transformBasis 指定了是否要对朝向向量进行变换
	void Tranform_Object4D(Object4D* object, const Matrix &mat, OBJ_TRANSFORM_TYPE tansformType);

	// 对ERenderList4D进行变换, 可以进行位移变换和旋转变换
	// tansformType指定了对哪一个定点列表进行变换
	void Transform_Renderlist4D(RenderList4D* renderList, const Matrix &mat, OBJ_TRANSFORM_TYPE transformType);

	// 将Object插入到RenderList, 这里LocalList存储的是原始坐标, TransformList存储的是经过
	// 平移和缩放的坐标
	void Inert_Object4D_To_RenderList4D(RenderList4D *renderList, Object4D *obj,
		OBJ_TRANSFORM_TYPE transformType = TRANSFORM_LOCAL_TO_TRANS, bool transformScaleAndPosition = false);

	// 局部坐标定编列表变换至世界坐标, 这里是直接使用加法, 而没有使用与矩阵相乘的方法, 因为没有必要
	void Local_To_World_Object4D(Object4D* obj, OBJ_TRANSFORM_TYPE transformType = TRANSFORM_LOCAL_TO_TRANS);

	// 局部坐标定编列表变换至世界坐标, 这里是直接使用加法, 而没有使用与矩阵相乘的方法, 因为没有必要
	void Local_To_World_RenderList4D(RenderList4D *renderList, const vector3df &pos,
		OBJ_TRANSFORM_TYPE transformType = TRANSFORM_LOCAL_TO_TRANS);

	// 对物体进行剔除
	bool Cull_Object4D(Object4D *object, Frustum *camera, CULL_TYPE cullType = CULL_SPHERE);

	// 世界坐标变换成摄像机坐标系
	void World_To_Camera_Object4D(Object4D *obj, Frustum *camera);
	void World_To_Camera_RenderList4D(RenderList4D *renderList, Frustum *camera);

	// 对RenderList的多边形进行深度排序, 使用画家算法
	// 这里需要在多边形处于摄像机坐标系时才可以调用, 否则会出现问题
	void Sort_RenderList4D(RenderList4D *renderList);

	// 消除背面, 背面消除算法的摄像机视点是基于世界坐标, 所以需要在定点变换成摄像机坐标前进行
	// 背面消除
	bool Remove_Backface(Polyon4D *poly, Frustum *camera);
	bool Remove_Backface(PolyonF4D *poly, Frustum *camera);
	void Remove_Backface_Object4D(Object4D *object, Frustum *camera);
	// light 是否对多边形进行光照处理
	void Remove_Backface_RenderList4D(RenderList4D *renderList, Frustum *camera);

	// 光照计算
	// 光照计算只能在世界坐标系消除背面之后或者在摄像机坐标系中进行
	void Light_PolyonF4D(PolyonF4D *poly, Frustum *camera);
	void Light_RenderList4D(RenderList4D *renderList, Frustum *camera);

	// 摄像机坐标系转换为透视坐标系
	void Camera_To_Perspective_Object4D(Object4D *obj, Frustum *camera);
	void Camera_To_Perspective_RenderList4D(RenderList4D *renderList, Frustum *camera);

	// 透视坐标系转换至屏幕坐标系
	void Perspective_To_Screen_Object4D(Object4D *obj, Frustum *camera);
	void Perspective_To_Screen_RenderList4D(RenderList4D *renderList, Frustum *camera);

	// 对RenderList4D进行裁剪操作, 裁剪掉超出摄像机范围内的多边形, 此操作必须在摄像机空间内执行
	void Clip_RenderList4D(RenderList4D *renderList, Frustum *camera);

	// 世界坐标变换到屏幕坐标
	// 相当于调用
	// World_To_Camera
	// Perspective_To_Screen的效用
	// 其内部已经执行了剔除, 背面消除, 光照处理等操作
	void World_To_Screen_RenderList4D(RenderList4D *renderList, Frustum *camera);

	// 渲染线框模型
	void Draw_Object4D_Wire(Object4D *obj);
	void Draw_RenderList4D_Wire(RenderList4D *renderList);

	// 渲染实体模型(无实现绘图)
	void Draw_Object4D_Solid(Object4D *object);
	void Draw_RenderList4D_Solid(RenderList4D *renderList);

	// 绘制带纹理的renderlist
	void Draw_RenderList4D_Teture_Solid(RenderList4D * renderList);
}

#endif

