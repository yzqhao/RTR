
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

	// ����һ��ģ��ʵ��
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

	// ��ȡָ����ģ��ʵ��
	Mesh* SceneManager::getMesh(const std::string &name)
	{
		MeshIter iter = mMeshs.find(name);
		if (iter == mMeshs.end())
		{
			return iter->second;
		}
		return NULL;
	}

	// ����ָ����ģ��ʵ��
	void SceneManager::destroyMesh(const std::string&name)
	{
		MeshIter iter = mMeshs.find(name);
		if (iter != mMeshs.end())
			SafeDelete(iter->second);
		mMeshs.erase(name);
	}

	// ָ������/�رյƹ�(ȫ��)
	void SceneManager::enableLights(bool enable)
	{
		mLightEnable = enable;
		for (int i = 0; i < GetLightSize(); i++)
			getLight(i)->lightOn = enable;
	}

	// ����һյ�ƹ�
	Light* SceneManager::createLight(LIGHT_TYPE lightType)
	{
		s32 light = CreateLight(lightType);
		return getLight(light);
	}

	// ���һ���ƹ�, ����ƹⲻ����, �򷵻�NULL
	Light*	SceneManager::getLight(s32 id)
	{
		return GetLight(id);
	}

	// ����
	void SceneManager::update()
	{
		mVisibleObjectNumber = 0;
		mTotalPolyonNumber = 0;

		// ���������
		mCamera->update();

		// ������ģ����ӵ���Ⱦ�б�
		for (MeshIter iter = mMeshs.begin(); iter != mMeshs.end(); ++iter)
		{
			mTotalPolyonNumber += iter->second->getPolyonNumber();
			// ֻ����Ұ�ڵ�������뵽��Ⱦ�б�
			if (iter->second->isVisible() && !Cull_Object4D(iter->second->getObject(), mCamera->getFrustum(), CULL_BOX))
			{
				mVisibleObjectNumber++;
				Inert_Object4D_To_RenderList4D(mRenderList, iter->second->getObject(),
					TRANSFORM_TRANS, true);
			}
		}

		// ��Ⱦ�б����3D��ˮ�ߴ���
		// �ֲ�����ϵ�任����������ϵ
		Local_To_World_RenderList4D(mRenderList, vector3df(), TRANSFORM_TRANS);
		// ��������ϵֱ�ӱ任����Ļ����ϵ
		// ��������ϵ-->��������-->���������ϵ�任-->
		// �ü�-->���ռ���-->͸�ӱ任-->��Ļ�任
		World_To_Screen_RenderList4D(mRenderList, mCamera->getFrustum());

		// ��ǰ�ɼ�����θ���
		mVisiblePolyonNumber = mRenderList->polyData.size();

		// ִ����Ⱦ
		if (mCamera->getRenderMode() == RENDER_WIRE)
			Draw_RenderList4D_Wire(mRenderList);
		else
			Draw_RenderList4D_Teture_Solid(mRenderList);
	}

}

