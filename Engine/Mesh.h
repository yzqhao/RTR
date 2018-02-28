
#ifndef _ENGINE_MESH_H_
#define _ENGINE_MESH_H_

#include "Material.h"
#include "Object.h"
#include "Core.h"

namespace RTR
{
	enum COLLSION_TYPE
	{
		COLLSION_COLLSION,
		COLLSION_IGNORE,
	};

	class Mesh : public Object
	{
	public:
		~Mesh();

		void setScale(const vector3df &scale);
		void setPosition(const vector3df &pos);
		vector3df getPosition() const;
		void move(const vector3df &mov);

		// 给定点是否在Mesh的包围盒内, 如果是则返回true, 否则返回false
		bool intersect(const vector3df &point);

		void setVisible(bool visible) { mIsVisible = visible; }
		bool isVisible() const { return mIsVisible; }

		// 是否需要执行剔除操作
		void setCullFlag(bool needCull) { mObject->needCull = needCull; }

		// 绕Y轴旋转
		void yaw(f32 yDegree);
		// 绕Z轴旋转
		void roll(f32 rDegree);
		// 绕X轴旋转
		void pitch(f32 pDegree);

		void setCollsionType(COLLSION_TYPE type) { mCollsionType = type; }
		COLLSION_TYPE getCollsionType() const { return mCollsionType; }

		Material *getMaterial(s32 index = 0);
		void setMateria(s32 index, std::string matName);

		Object4D* getObject() const { return mObject; }
		s32 getPolyonNumber() const { return mPolyonNumber; }
		s32 getVertexNumber() const { return mVertexNumber; }
		s32 getSubmeshNumber() const { return mSubmeshNumber; }

		// 获取各轴的旋转分量, 以角度表示
		vector3df getRotateVec() const { return mRotate; }

		// 对此模型进行clone, 避免再次读盘读取模型
		Mesh *clone();

	protected:
		friend class SceneManager;
		Mesh();
		Mesh(const std::string &name, const std::string &meshName);

		Object4D		*mObject;

		s32			mPolyonNumber;	// 模型多边形个数
		s32			mVertexNumber;	// 模型顶点数

		s32			mSubmeshNumber;	// 模型子模型个数

		vector3df		mRotate;		// 模型各轴的旋转分量

		bool			mIsVisible;

		COLLSION_TYPE	mCollsionType;
	};
}

#endif