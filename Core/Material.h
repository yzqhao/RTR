
#ifndef _CORE_MATERIAL_H_
#define _CORE_MATERIAL_H_

#include <vector>
#include <string>
#include <map>

#include "type.h"
#include "Color.h"
#include "Graphics.h"

namespace RTR
{
	struct Material
	{
		std::string name;
		Color	ambient;	// 环境色
		Color	diffuse;	// 漫反射颜色
		Color	specular;	// 镜面颜色

		Bitmap *bitmap;

		Material();
		~Material();
		inline bool useTexture() { return bitmap && bitmap->isValid(); }
		Color getPixel(f32 u, f32 v);
	};

	typedef std::map<std::string, Material*>::iterator MaterialIter;
	extern std::map<std::string, Material*> *GMaterials;

	extern Material *GetMaterial(const std::string &name);
	extern bool SetMaterial(const std::string &name, Material *material);
	extern bool DestoryMaterial(const std::string &name);
	extern void DestoryAllMaterials();

	// 解析材质脚本, 读取基本材质和贴图
	// 材质脚本必须和模型名称相同例如 : ogreHead.mesh ogreHead.material
	extern void ParseMaterialScript(const std::string &scriptName);
}

#endif

