
#include "SceneManager.h"

namespace RTR
{
	SceneManager::SceneManager() : mLightEnable(true), mVisibleObjectNumber(0),
		mVisiblePolyonNumber(0), mTotalPolyonNumber(0), mTerrainMesh(NULL)
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

	Mesh* SceneManager::createTerrain(const std::string& heightMapFileName,
		const std::string &textureFileName, f32 uTitle, f32 vTitle, f32 blockSize)
	{
		if (mTerrainMesh)
			SafeDelete(mTerrainMesh);

		//Log("Loading Terrain HeightMap : #%s Texture : ...", heightMapFileName.c_str(), textureFileName.c_str());

		Bitmap *heigthMap = new Bitmap(heightMapFileName);

		if (!heigthMap->isValid() ||
			heigthMap->getWidth() % 2 == 0 ||
			heigthMap->getHeight() % 2 == 0)
		{
			SafeDelete(heigthMap);
			return NULL;
		}

		Material *mat = new Material();
		mat->bitmap = new Bitmap(textureFileName);
		SetMaterial("Terrain", mat);

		s32 row = heigthMap->getHeight();
		s32 col = heigthMap->getWidth();

		// Y = 0.299R+0.587G+0.114B
		mTerrainMesh = new Mesh();
		Object4D *obj = new Object4D();
		mTerrainMesh->mObject = obj;

		obj->name = "Terrain";
		obj->materiaName = "Terrain";
		obj->state = EOBJECT_STATE_ACTIVE | EOBJECT_STATE_VISIBLE;
		obj->scale = vector3df(1, 1, 1);
		obj->worldPosition = vector3df();
		obj->vertexNumber = row * col;
		obj->polyonNumber = (row - 1) * (col - 1) * 2;

		mTerrainMesh->mVertexNumber = obj->vertexNumber;
		mTerrainMesh->mPolyonNumber = obj->polyonNumber;

		// 定点列表
		obj->localList.reserve(obj->vertexNumber);
		obj->transformList.reserve(obj->vertexNumber);
		// 多边形列表
		obj->polyonList.reserve(obj->polyonNumber);

		f32 offsetX = (col - 1) * blockSize / 2;
		f32 offsetZ = (row - 1) * blockSize / 2;

		f32 minY = 0, maxY = 0;
		f32 u = uTitle / (col - 1);
		f32 v = vTitle / (row - 1);
		for (s32 r = 0; r < row; r++)
		{
			for (s32 c = 0; c < col; c++)
			{
				Vertex4D vex;
				// 顶点坐标
				Color color = heigthMap->getPixel(c, r);
				vex._x = c * blockSize - offsetX;
				vex._y = 0.1f * (0.299f * color.r + 0.587f * color.g + 0.114f * color.b);
				vex._z = r * blockSize - offsetZ;

				if (vex._y < minY)
					minY = vex._y;
				if (vex._y > maxY)
					maxY = vex._y;

				// 顶点UV
				vex.u = c * u;
				vex.v = r * v;

				obj->localList.push_back(vex);
				obj->transformList.push_back(vex);

				// 计算顶点索引
				if (r < row - 1 && c < col - 1)
				{
					Polyon4D poly;
					poly.state = EPOLY_STATE_ACTIVE;
					poly.attribute = EPOLY_ATTR_VERTEX_POSITION | EPOLY_ATTR_VERTEX_UV;

					poly.verList = &obj->transformList;

					poly.verIndex[0] = r * col + c;
					poly.verIndex[1] = (r + 1) * col + c;
					poly.verIndex[2] = r * col + c + 1;
					obj->polyonList.push_back(poly);

					poly.verIndex[0] = r * col + c + 1;
					poly.verIndex[1] = (r + 1) * col + c;
					poly.verIndex[2] = (r + 1) * col + c + 1;
					obj->polyonList.push_back(poly);
				}
			}
		}

		obj->maxBoundingBox = vector3df(offsetX, maxY, offsetZ);
		obj->minBoundingBox = vector3df(-offsetX, minY, -offsetZ);
		obj->maxRadius = std::sqrt(offsetX * offsetX + offsetZ * offsetZ);

		SafeDelete(heigthMap);
		mMeshs.insert(std::make_pair("Terrain", mTerrainMesh));
		// 设置不执行剔除操作
		mTerrainMesh->setCullFlag(false);

		//Log("Terrain Load Sucessed!");
		return mTerrainMesh;
	}

}

