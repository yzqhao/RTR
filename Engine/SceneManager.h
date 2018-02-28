
#ifndef _ENGINE_SCENEMANAGER_H_
#define _ENGINE_SCENEMANAGER_H_

#include <map>
#include "Mesh.h"
#include "Core.h"
#include "Camera.h"

namespace RTR
{
	// ������Դ������
	class SceneManager
	{
		typedef std::map<std::string, Mesh*>::iterator MeshIter;

	public:
		SceneManager();
		~SceneManager();

		// ��������
		void drawString(const std::string &str, s32 x, s32 y, const Color &c = Color(255, 255, 255));

		// ����һ��ģ��ʵ��
		Mesh* createMesh(const std::string &name, const std::string &meshName);
		// ��ȡָ����ģ��ʵ��
		Mesh* getMesh(const std::string &name);
		// ����ָ����ģ��ʵ��
		void destroyMesh(const std::string&name);

		void clearMesh();

		// ָ������/�رյƹ�(ȫ��)
		void enableLights(bool enable);
		// ��ǰ�Ƿ����ƹ�(ȫ��)
		bool isLightsEnable() const { return mLightEnable; }
		// ����һյ�ƹ�
		Light* createLight(LIGHT_TYPE lightType);
		// ���һ���ƹ�, ����ƹⲻ����, �򷵻�NULL
		Light*	getLight(s32 id);

		// ��ȡ�����ָ��
		Camera* getCamera(){ return mCamera; }

		// ��ȡ����(�ɼ�/���ɼ�)�������
		s32 getTotalObjectNumber() const { return mMeshs.size(); }
		// ��ȡ��ǰ�ɼ��������
		s32 getVisibleObjectNumber() const { return mVisibleObjectNumber; }
		// ��ȡ��ǰ�ɼ�����θ���
		s32 getVisiblePolyonNumber() const { return mVisiblePolyonNumber; }
		// ��ȡ����(�ɼ�/���ɼ�)����θ���
		s32 getTotalPolyonNumber() const { return mTotalPolyonNumber; }

		// ��������
		Mesh* createTerrain(const std::string& rawFileName, const std::string &heightMapFileName,
			f32 uTitle = 1, f32 vTitle = 1, f32 blockSize = 5.0f);
		Mesh* getTerrain() { return mTerrainMesh; }

		// ����
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