
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

		// �������Ƿ���Mesh�İ�Χ����, ������򷵻�true, ���򷵻�false
		bool intersect(const vector3df &point);

		void setVisible(bool visible) { mIsVisible = visible; }
		bool isVisible() const { return mIsVisible; }

		// �Ƿ���Ҫִ���޳�����
		void setCullFlag(bool needCull) { mObject->needCull = needCull; }

		// ��Y����ת
		void yaw(f32 yDegree);
		// ��Z����ת
		void roll(f32 rDegree);
		// ��X����ת
		void pitch(f32 pDegree);

		void setCollsionType(COLLSION_TYPE type) { mCollsionType = type; }
		COLLSION_TYPE getCollsionType() const { return mCollsionType; }

		Material *getMaterial(s32 index = 0);
		void setMateria(s32 index, std::string matName);

		Object4D* getObject() const { return mObject; }
		s32 getPolyonNumber() const { return mPolyonNumber; }
		s32 getVertexNumber() const { return mVertexNumber; }
		s32 getSubmeshNumber() const { return mSubmeshNumber; }

		// ��ȡ�������ת����, �ԽǶȱ�ʾ
		vector3df getRotateVec() const { return mRotate; }

		// �Դ�ģ�ͽ���clone, �����ٴζ��̶�ȡģ��
		Mesh *clone();

	protected:
		friend class SceneManager;
		Mesh();
		Mesh(const std::string &name, const std::string &meshName);

		Object4D		*mObject;

		s32			mPolyonNumber;	// ģ�Ͷ���θ���
		s32			mVertexNumber;	// ģ�Ͷ�����

		s32			mSubmeshNumber;	// ģ����ģ�͸���

		vector3df		mRotate;		// ģ�͸������ת����

		bool			mIsVisible;

		COLLSION_TYPE	mCollsionType;
	};
}

#endif