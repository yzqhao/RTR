
#ifndef _CORE_LIGHT_H_
#define _CORE_LIGHT_H_

#include <vector>

#include "type.h"
#include "Color.h"
#include "Vector3.h"

// 最大灯光数
#define MAX_LIGHTS 8

namespace RTR
{
	enum LIGHT_TYPE
	{
		LIGHT_AMBIENT = 0,	// 环境光
		LIGHT_DIRECTION = 1,	// 平行光
		LIGHT_POINT = 2,	// 点灯光
	};

	struct Light
	{
		s32		id;			// 灯光的分配id, 全局唯一
		bool		lightOn;	// 灯光是否启用

		LIGHT_TYPE	lightType;	// 灯光类型

		f32		power;			// 灯光的强度, 默认为1.0f
		f32		shadowFactor;	// 灯光背面强度, 默认为0.05f

		Color		ambient;	// 灯光的环境色(环境光使用)
		Color		diffuse;	// 灯光漫反射颜色
		Color		specular;	// 灯光高光颜色

		vector3df	position;	// 灯光世界坐标位置
		vector3df	direction;	// 光源朝向, 点灯光无效

		vector3df	transPosition;	// 灯光摄像机坐标系位置
		vector3df	transDirection;	// 摄像机坐标系光源朝向, 点灯光无效

		f32		kc, kl, kq;	// 衰减因子

		f32		spot_inner;	// 聚光灯内锥角
		f32		spot_outer;	// 聚光灯外锥角
		f32		pf;			// 聚光灯指数因子

		Light(s32 lightId, LIGHT_TYPE type);
	};

	// 全局灯光管理
	extern std::vector<Light*> *gLights;

	typedef std::vector<Light*>::iterator lightItr;

	// 创建一个新的灯光, 创建成功返回灯光id(全局唯一), 创建失败则返回-1
	// 场景内最大光源数为MAX_LIGHTS
	extern s32		CreateLight(LIGHT_TYPE lightType);

	// 获得一个灯光, 如果灯光不存在, 则返回NULL
	extern Light*	GetLight(s32 id);
	extern s32		GetLightSize();

	// 清楚全部光源
	extern void		DestoryAllLights();
}

#endif

