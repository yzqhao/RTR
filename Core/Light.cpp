
#include "Light.h"
#include "Util.h"

namespace RTR
{
	Light::Light(s32 lightId, LIGHT_TYPE type) : id(lightId), lightOn(true), lightType(type),
		ambient(255, 255, 255), diffuse(255, 255, 255), specular(255, 255, 255),
		position(0, 0, 0), direction(0, 0, 1), power(1.0f), shadowFactor(0.05f){}

	std::vector<Light*> *gLights = NULL;

	s32 CreateLight(LIGHT_TYPE lightType)
	{
		if (gLights == NULL)
			gLights = new std::vector<Light*>;

		if ((s32)gLights->size() > MAX_LIGHTS)
			return -1;

		s32 id = (s32)gLights->size();
		gLights->push_back(new Light(id, lightType));

		return id;
	}

	// 获得一个灯光, 如果灯光不存在, 则创建一个新灯光并返回
	Light* GetLight(s32 id)
	{
		if (gLights == NULL ||
			(s32)gLights->size() > MAX_LIGHTS ||
			id < 0 ||
			id >(s32)gLights->size() - 1)
			return NULL;

		return gLights->at(id);
	}

	s32 GetLightSize()
	{
		return gLights->size();
	}

	void DestoryAllLights()
	{
		for (lightItr itr = gLights->begin(); itr != gLights->end(); ++itr)
			SafeDelete((*itr));

		gLights->clear();
		SafeDelete(gLights);
	}
}