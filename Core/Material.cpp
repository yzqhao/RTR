#include <cmath>
#include <fstream>

#include "Material.h"
#include "Util.h"

namespace RTR
{
	Material::Material():ambient(0xffffff), diffuse(0xffffff), bitmap(NULL){}

	Material::~Material()
	{
		SafeDelete(bitmap);
	}

	Color Material::getPixel(f32 u, f32 v)
	{
		static Color defaultColor(255, 255, 255);
		if (bitmap && bitmap->isValid())
		{
			// 这是一个更精确的方法, 但是效率低一点
			s32 x = Abs(u - (s32)u) * bitmap->width;
			s32 y = Abs(v - (s32)v) * bitmap->height;
			return bitmap->pixels[y * bitmap->pitch + x];
		}
		else
			return defaultColor;
	}

	std::map<std::string, Material*> *GMaterials = NULL;

	static void initMaterial()
	{
		static bool init = false;
		if (!init)
			GMaterials = new std::map<std::string, Material*>();
		init = true;
	}

	Material *GetMaterial(const std::string &name)
	{
		initMaterial();

		MaterialIter iter = GMaterials->find(name);
		if (iter == GMaterials->end())
			return NULL;
		else
			return iter->second;
	}

	bool SetMaterial(const std::string &name, Material *material)
	{
		initMaterial();

		GMaterials->insert(std::make_pair(name, material));
		return true;
	}

	bool DestoryMaterial(const std::string &name)
	{
		initMaterial();
		
		MaterialIter iter = GMaterials->find(name);
		if (iter != GMaterials->end())
			SafeDelete(iter->second);
		GMaterials->erase(iter);
		return true;
	}

	void DestoryAllMaterials()
	{
		for (MaterialIter iter = GMaterials->begin(); iter != GMaterials->end(); ++iter)
			SafeDelete(iter->second);

		GMaterials->clear();
	}

	void ParseMaterialScript(const std::string &scriptName)
	{
		std::ifstream in;
		// 材质脚本, 首先获取默认路径
		in.open(GetPath(scriptName).c_str());

		if (in.good())
		{
			char line[256];
			while (in.good())
			{
				in.getline(line, 256);
				std::string info = Trim(line);

				if (!info.empty())
				{
					// 遇到{加1, 遇到}减1, 当brackets为0时,跳出当前材质
					s32  brackets = 0;
					std::size_t index = info.find("material");
					// 出现新材质
					if (index == 0)
					{
						Material *materail = new Material();
						materail->name = Trim(info.substr(index + 8));

						// 装到全局材质库
						SetMaterial(materail->name, materail);

						in.getline(line, 256);
						info = Trim(line);
						char temp[64];

						if (info == "{") brackets++;
						while (brackets != 0)
						{
							in.getline(line, 256);
							info = Trim(line);

							if (info == "{") brackets++;
							else if (info == "}") brackets--;
							else
							{
								// 环境光
								std::size_t ambientIndex = info.find("ambient");
								if (ambientIndex == 0)
								{
									sscanf_s(info.c_str(), "%s %f %f %f", temp,
											&materail->ambient.r, 
											&materail->ambient.g, 
											&materail->ambient.b);
									
									continue;
								}

								// 漫反射
								std::size_t diffuseIndex = info.find("diffuse");
								if (diffuseIndex == 0)
								{
									sscanf_s(info.c_str(), "%s %f %f %f", temp, 
											&materail->diffuse.r, 
											&materail->diffuse.g, 
											&materail->diffuse.b);
									continue;
								}

								// 镜面反射
								std::size_t specularIndex = info.find("specular");
								if (specularIndex == 0)
								{
									sscanf_s(info.c_str(), "%s %f %f %f", temp,
											&materail->specular.r, 
											&materail->specular.g, 
											&materail->specular.b);
									continue;
								}

								// 贴图
								std::size_t textureIndex = info.find("texture ");	// 这里使用texture ,后面保留一个空格, 与texture_unit区别
								if (textureIndex == 0)
								{
									char textureName[64];
									sscanf_s(info.c_str(), "%s %s", temp, textureName);
									materail->bitmap = new Bitmap(textureName);
									continue;
								}
							}
						}
					}
				}
			}
		}

		in.close();
	}
}