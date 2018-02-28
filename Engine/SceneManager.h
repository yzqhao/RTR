
#ifndef _ENGINE_SCENEMANAGER_H_
#define _ENGINE_SCENEMANAGER_H_

#include <map>
#include "Mesh.h"
#include "Core.h"
#include "Camera.h"

namespace RTR
{
	// 各种资源管理类
	class SceneManager
	{
		typedef std::map<std::string, Mesh*>::iterator MeshIter;

	public:
		SceneManager();
		~SceneManager();

		// 绘制文字
		void drawString(const std::string &str, s32 x, s32 y, const Color &c = Color(255, 255, 255));

		// 创建一个模型实体
		Mesh* createMesh(const std::string &name, const std::string &meshName);
		// 获取指定的模型实体
		Mesh* getMesh(const std::string &name);
		// 销毁指定的模型实体
		void destroyMesh(const std::string&name);

		void clearMesh();

		// 指定开启/关闭灯光(全局)
		void enableLights(bool enable);
		// 当前是否开启灯光(全局)
		bool isLightsEnable() const { return mLightEnable; }
		// 创建一盏灯光
		Light* createLight(LIGHT_TYPE lightType);
		// 获得一个灯光, 如果灯光不存在, 则返回NULL
		Light*	getLight(s32 id);

		// 获取摄像机指针
		Camera* getCamera(){ return mCamera; }

		// 获取所有(可见/不可见)物体个数
		s32 getTotalObjectNumber() const { return mMeshs.size(); }
		// 获取当前可见物体个数
		s32 getVisibleObjectNumber() const { return mVisibleObjectNumber; }
		// 获取当前可见多边形个数
		s32 getVisiblePolyonNumber() const { return mVisiblePolyonNumber; }
		// 获取所有(可见/不可见)多边形个数
		s32 getTotalPolyonNumber() const { return mTotalPolyonNumber; }

		// 创建地形
		Mesh* createTerrain(const std::string& rawFileName, const std::string &heightMapFileName,
			f32 uTitle = 1, f32 vTitle = 1, f32 blockSize = 5.0f);
		Mesh* getTerrain() { return mTerrainMesh; }

		// 更新
		void update();

	protected:
		std::map<std::string, Mesh*>	mMeshs;
		std::map<std::string, Mesh*>	mUsedMesh;
		RenderList4D				*mRenderList;

		Mesh						*mTerrainMesh;

		Camera						*mCamera;
		bool						mLightEnable;

		s32						mVisibleObjectNumber;
		s32						mVisiblePolyonNumber;
		s32						mTotalPolyonNumber;
	};
}

#endif