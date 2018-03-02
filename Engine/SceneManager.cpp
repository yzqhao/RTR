
#include "SceneManager.h"

namespace RTR
{
	SceneManager::SceneManager() : mLightEnable(true), mVisibleObjectNumber(0),
		mVisiblePolyonNumber(0), mTotalPolyonNumber(0)
	{
		mCamera = new Camera();
		mRenderList = new RenderList4D;
	}

	SceneManager::~SceneManager()
	{
		for (MeshIter iter = mMeshs.begin(); iter != mMeshs.end(); ++iter)
			SafeDelete(iter->second);
		for (MeshIter iter = mUsedMesh.begin(); iter != mUsedMesh.end(); ++iter)
			SafeDelete(iter->second);

		mMeshs.clear();
		DestoryAllLights();
		SafeDelete(mCamera);
		SafeDelete(mRenderList);
	}

	void SceneManager::drawString(const std::string &str, s32 x, s32 y, const Color &c)
	{
		Graphics::drawString(str, x, y, c);
	}

	// 创建一个模型实体
	Mesh* SceneManager::createMesh(const std::string &name, const std::string &meshName)
	{
		MeshIter iter = mMeshs.find(name);
		if (iter == mMeshs.end())
		{
			Mesh *mesh = NULL;

			MeshIter meshItr = mUsedMesh.find(meshName);
			if (meshItr != mUsedMesh.end())
			{
				mesh = meshItr->second->clone();
				mesh->mName = name;
			}
			else
			{
				mesh = new Mesh(name, meshName);
				mUsedMesh.insert(std::make_pair(meshName, mesh->clone()));
			}

			mMeshs.insert(std::make_pair(name, mesh));
			return mesh;
		}

		return iter->second;
	}

	void SceneManager::clearMesh()
	{
		for (MeshIter iter = mMeshs.begin(); iter != mMeshs.end(); ++iter)
			SafeDelete(iter->second);

		mMeshs.clear();
	}

	// 获取指定的模型实体
	Mesh* SceneManager::getMesh(const std::string &name)
	{
		MeshIter iter = mMeshs.find(name);
		if (iter == mMeshs.end())
		{
			return iter->second;
		}
		return NULL;
	}

	// 销毁指定的模型实体
	void SceneManager::destroyMesh(const std::string&name)
	{
		MeshIter iter = mMeshs.find(name);
		if (iter != mMeshs.end())
			SafeDelete(iter->second);
		mMeshs.erase(name);
	}

	// 指定开启/关闭灯光(全局)
	void SceneManager::enableLights(bool enable)
	{
		mLightEnable = enable;
		for (int i = 0; i < GetLightSize(); i++)
			getLight(i)->lightOn = enable;
	}

	// 创建一盏灯光
	Light* SceneManager::createLight(LIGHT_TYPE lightType)
	{
		s32 light = CreateLight(lightType);
		return getLight(light);
	}

	// 获得一个灯光, 如果灯光不存在, 则返回NULL
	Light*	SceneManager::getLight(s32 id)
	{
		return GetLight(id);
	}

	// 更新
	void SceneManager::update()
	{
		mVisibleObjectNumber = 0;
		mTotalPolyonNumber = 0;

		// 更新摄像机
		mCamera->update();

		// 将所有模型添加到渲染列表
		for (MeshIter iter = mMeshs.begin(); iter != mMeshs.end(); ++iter)
		{
			mTotalPolyonNumber += iter->second->getPolyonNumber();
			// 只对视野内的物体插入到渲染列表
			if (iter->second->isVisible() && !Cull_Object4D(iter->second->getObject(), mCamera->getFrustum(), CULL_BOX))
			{
				mVisibleObjectNumber++;
				Inert_Object4D_To_RenderList4D(mRenderList, iter->second->getObject(),
					TRANSFORM_TRANS, true);
			}
		}

		// 渲染列表进行3D流水线处理
		// 局部坐标系变换到世界坐标系
		Local_To_World_RenderList4D(mRenderList, vector3df(), TRANSFORM_TRANS);
		// 世界坐标系直接变换到屏幕坐标系
		// 世界坐标系-->背面消除-->摄像机坐标系变换-->
		// 裁剪-->光照计算-->透视变换-->屏幕变换
		World_To_Screen_RenderList4D(mRenderList, mCamera->getFrustum());

		// 当前可见多边形个数
		mVisiblePolyonNumber = mRenderList->polyData.size();

		// 执行渲染
		if (mCamera->getRenderMode() == RENDER_WIRE)
			Draw_RenderList4D_Wire(mRenderList);
		else
			Draw_RenderList4D_Teture_Solid(mRenderList);
	}

}

