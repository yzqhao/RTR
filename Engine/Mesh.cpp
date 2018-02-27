
#include "Mesh.h"
#include "MeshUtil.h"
#include "CoreMath.h"
#include "Util.h"

namespace RTR
{
	Mesh::Mesh() : mObject(NULL), mSubmeshNumber(1), mCollsionType(COLLSION_COLLSION),
		mPolyonNumber(0), mVertexNumber(0), mRotate(0, 0, 0), mIsVisible(true)
	{

	}

	Mesh::Mesh(const std::string &name, const std::string &meshName)
		: mSubmeshNumber(0), mPolyonNumber(0), mVertexNumber(0), mRotate(0, 0, 0), mCollsionType(COLLSION_COLLSION), mIsVisible(true)
	{
		mName = name;
		mObject = LoadOgreMesh(meshName);

		if (!mObject)
		{
			std::string errorInfo = "Mesh #" + meshName + " Not Found";
			throw errorInfo;
		}

		Object4D *obj = mObject;
		while (obj)
		{
			mSubmeshNumber++;
			mPolyonNumber += obj->polyonNumber;
			mVertexNumber += obj->vertexNumber;
			obj = obj->nextObject;
		}
	}

	Mesh::~Mesh()
	{
		SafeDelete(mObject);
	}

	void Mesh::setScale(const vector3df &scale)
	{
		mObject->scale = vector3df(scale._x, scale._y, scale._z);
	}

	void Mesh::setPosition(const vector3df &pos)
	{
		mObject->worldPosition = vector3df(pos._x, pos._y, pos._z);
	}

	vector3df Mesh::getPosition() const
	{
		vector3df pos = mObject->worldPosition;
		return vector3df(pos._x, pos._y, pos._z);
	}

	void Mesh::move(const vector3df &mov)
	{
		mObject->worldPosition = mObject->worldPosition + vector3df(mov._x, mov._y, mov._z);
	}

	// ÈÆYÖáÐý×ª
	void Mesh::yaw(f32 yDegree)
	{
		mRotate._y = yDegree;
		Matrix rotMat;
		GetRotateMatrix44Y(rotMat, yDegree);
		Tranform_Object4D(mObject, rotMat, TRANSFORM_LOCAL_TO_TRANS);
	}

	// ÈÆZÖáÐý×ª
	void Mesh::roll(f32 rDegree)
	{
		mRotate._z = rDegree;
		Matrix rotMat;
		GetRotateMatrix44Z(rotMat, rDegree);
		Tranform_Object4D(mObject, rotMat, TRANSFORM_LOCAL_TO_TRANS);
	}

	// ÈÆXÖáÐý×ª
	void Mesh::pitch(f32 pDegree)
	{
		mRotate._x = pDegree;
		Matrix rotMat;
		GetRotateMatrix44X(rotMat, pDegree);
		Tranform_Object4D(mObject, rotMat, TRANSFORM_LOCAL_TO_TRANS);
	}

	Material* Mesh::getMaterial(s32 index)
	{
		if (index < 0 || index >= mSubmeshNumber)
			return NULL;

		Object4D *object = mObject;
		s32 temp = 0;
		while (temp < index)
		{
			temp++;
			object = object->nextObject;
		}

		if (object)
			return GetMaterial(object->materiaName);
		return NULL;
	}

	void Mesh::setMateria(s32 index, std::string matName)
	{
		if (index < 0 || index >= mSubmeshNumber)
			return;

		Object4D *object = mObject;
		s32 temp = 0;
		while (temp < index)
		{
			temp++;
			object = object->nextObject;
		}

		if (object)
			object->materiaName = matName;
	}

	bool Mesh::intersect(const vector3df &point)
	{
		f32 x = Abs(mObject->maxBoundingBox._x) * mObject->scale._x / 2.0f;
		f32 y = Abs(mObject->maxBoundingBox._y) * mObject->scale._y / 2.0f;
		f32 z = Abs(mObject->maxBoundingBox._z) * mObject->scale._z / 2.0f;

		if (point._x < mObject->worldPosition._x - x ||
			point._x > mObject->worldPosition._x + x)
			return false;

		if (point._y < mObject->worldPosition._y - y ||
			point._y > mObject->worldPosition._y + y)
			return false;

		if (point._z < mObject->worldPosition._z - z ||
			point._z > mObject->worldPosition._z + z)
			return false;

		return true;
	}

	Mesh *Mesh::clone()
	{
		Mesh *mesh = new Mesh();
		{
			mesh->mName = mName + "_CLone";
			mesh->mPolyonNumber = mPolyonNumber;
			mesh->mRotate = mRotate;
			mesh->mSubmeshNumber = mSubmeshNumber;
			mesh->mVertexNumber = mVertexNumber;

			Object4D *object = new Object4D();
			{
				object->attribute = mObject->attribute;
				object->avgRadius = mObject->avgRadius;
				object->direction = mObject->direction;
				object->localList = mObject->localList;
				object->transformList = mObject->transformList;
				object->materiaName = mObject->materiaName;
				object->maxBoundingBox = mObject->maxBoundingBox;
				object->maxRadius = mObject->maxRadius;
				object->minBoundingBox = mObject->minBoundingBox;
				object->needCull = mObject->needCull;
				object->name = mObject->name;
				object->polyonList = mObject->polyonList;
				object->polyonNumber = mObject->polyonNumber;
				object->scale = mObject->scale;
				object->state = mObject->state;
				object->vertexNumber = mObject->vertexNumber;
				object->worldPosition = mObject->worldPosition;
				object->nextObject = NULL;
			}
			mesh->mObject = object;
			mesh->mIsVisible = true;
		}

		return mesh;
	}
}
