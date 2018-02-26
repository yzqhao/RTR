
#include "CoreMath.h"
#include "Core.h"

namespace RTR
{
	void Tranform_Object4D(Object4D* object, const Matrix &mat, OBJ_TRANSFORM_TYPE tansformType)
	{
		Object4D* obj = object;
		switch (tansformType)
		{
		case TRANSFORM_LOCAL:
		{
			while (obj)
			{
				// ֻ�Ծֲ�������б任
				for (s32 i = 0; i < obj->vertexNumber; i++)
				{
					GetVertex4DMulMatrix44(obj->localList[i], mat, obj->localList[i]);
				}
				obj = obj->nextObject;
			}
		}
			break;
		case TRANSFORM_TRANS:
		{
			while (obj)
			{
				// �Ա任���������б任
				for (s32 i = 0; i < obj->vertexNumber; i++)
				{
					GetVertex4DMulMatrix44(obj->transformList[i], mat, obj->transformList[i]);
				}
				obj = obj->nextObject;
			}
		}
			break;
		case TRANSFORM_LOCAL_TO_TRANS:
		{
			while (obj)
			{
				// ���ֲ�����任��洢���任������궨���б�
				for (s32 i = 0; i < obj->vertexNumber; i++)
				{
					GetVertex4DMulMatrix44(obj->localList[i], mat, obj->transformList[i]);
				}

				obj = obj->nextObject;
			}
		}
			break;
		}
	}

	void Transform_Renderlist4D(RenderList4D* renderList, const Matrix &mat, OBJ_TRANSFORM_TYPE transformType)
	{
		PolyonF4D *poly = NULL;
		switch (transformType)
		{
		case TRANSFORM_LOCAL:
		{
			for (RenderList4D::Itr itr = renderList->polyData.begin();
				itr != renderList->polyData.end(); ++itr)
			{
				poly = &(*itr);

				GetVertex4DMulMatrix44(poly->localList[0], mat, poly->localList[0]);
				GetVertex4DMulMatrix44(poly->localList[1], mat, poly->localList[1]);
				GetVertex4DMulMatrix44(poly->localList[2], mat, poly->localList[2]);
			}
		}
			break;
		case TRANSFORM_TRANS:
		{
			for (RenderList4D::Itr itr = renderList->polyData.begin();
				itr != renderList->polyData.end(); ++itr)
			{
				poly = &(*itr);
				GetVertex4DMulMatrix44(poly->transformList[0], mat, poly->transformList[0]);
				GetVertex4DMulMatrix44(poly->transformList[1], mat, poly->transformList[1]);
				GetVertex4DMulMatrix44(poly->transformList[2], mat, poly->transformList[2]);
			}
		}
			break;
		case TRANSFORM_LOCAL_TO_TRANS:
		{
			for (RenderList4D::Itr itr = renderList->polyData.begin();
				itr != renderList->polyData.end(); ++itr)
			{
				poly = &(*itr);
				GetVertex4DMulMatrix44(poly->localList[0], mat, poly->transformList[0]);
				GetVertex4DMulMatrix44(poly->localList[1], mat, poly->transformList[1]);
				GetVertex4DMulMatrix44(poly->localList[2], mat, poly->transformList[2]);
			}
		}
			break;
		}
	}

	void Inert_Object4D_To_RenderList4D(RenderList4D *renderList, Object4D *object,
		OBJ_TRANSFORM_TYPE transformType, bool transformScaleAndPosition)
	{
		if (!object)
			return;

		vector3df scale(1, 1, 1);
		vector3df pos(0, 0, 0);

		if (transformScaleAndPosition)
		{
			scale = object->scale;
			pos = object->worldPosition;
		}

		// ���ﱸ�ݵƹ������
		Light *light = NULL;
		for (lightItr itr = gLights->begin(); itr != gLights->end(); ++itr)
		{
			light = *itr;
			light->transPosition = light->position;
			light->transDirection = light->direction;
		}

		Object4D *obj = object;
		Polyon4D *base;
		PolyonF4D poly;
		switch (transformType)
		{
		case TRANSFORM_LOCAL:
			while (obj)
			{
				for (s32 i = 0; i < obj->polyonNumber; i++)
				{
					base = &obj->polyonList[i];

					poly.attribute = base->attribute;
					poly.state = EPOLY_STATE_ACTIVE;
					poly.color = base->color;

					poly.localList[0] = obj->localList[base->verIndex[0]];
					poly.localList[1] = obj->localList[base->verIndex[1]];
					poly.localList[2] = obj->localList[base->verIndex[2]];

					// ���ò���
					poly.material = GetMaterial(obj->materiaName);

					renderList->polyData.push_back(poly);
				}

				obj = obj->nextObject;
			}
			break;
		case TRANSFORM_LOCAL_TO_TRANS:
			while (obj)
			{
				for (s32 i = 0; i < obj->polyonNumber; i++)
				{
					base = &obj->polyonList[i];

					poly.attribute = base->attribute;
					poly.state = EPOLY_STATE_ACTIVE;
					poly.color = base->color;

					poly.transformList[0] = obj->localList[base->verIndex[0]] * scale + pos;
					poly.transformList[1] = obj->localList[base->verIndex[1]] * scale + pos;
					poly.transformList[2] = obj->localList[base->verIndex[2]] * scale + pos;

					// ���ò���
					poly.material = GetMaterial(obj->materiaName);

					renderList->polyData.push_back(poly);
				}

				obj = obj->nextObject;
			}
			break;
		case TRANSFORM_TRANS:
			while (obj)
			{
				for (s32 i = 0; i < obj->polyonNumber; i++)
				{
					base = &obj->polyonList[i];

					poly.attribute = base->attribute;
					poly.state = EPOLY_STATE_ACTIVE;
					poly.color = base->color;

					poly.transformList[0] = obj->transformList[base->verIndex[0]] * scale + pos;
					poly.transformList[1] = obj->transformList[base->verIndex[1]] * scale + pos;
					poly.transformList[2] = obj->transformList[base->verIndex[2]] * scale + pos;

					// ���ò���
					poly.material = GetMaterial(obj->materiaName);

					renderList->polyData.push_back(poly);
				}

				obj = obj->nextObject;
			}
			break;
		}
	}

	void Local_To_World_Object4D(Object4D* object, OBJ_TRANSFORM_TYPE transformType)
	{
		if (!object)
			return;

		Object4D* obj = object;
		vector3df pos = obj->worldPosition;
		if (TRANSFORM_LOCAL_TO_TRANS == transformType)
		{
			while (obj)
			{
				for (s32 i = 0; i < obj->vertexNumber; i++)
				{
					// ���ö��������ֵ,���ٱ�����������
					GetVertex4DAddVector4D(obj->localList[i] * obj->scale, pos, obj->transformList[i]);
				}
				obj = obj->nextObject;
			}

		}
		else if (TRANSFORM_TRANS == transformType)
		{
			while (obj)
			{
				for (s32 i = 0; i < obj->vertexNumber; i++)
				{
					GetVertex4DAddVector4D(obj->transformList[i], pos, obj->transformList[i]);
				}
				obj = obj->nextObject;
			}
		}
	}

	void Local_To_World_RenderList4D(RenderList4D *renderList, const vector3df &pos, OBJ_TRANSFORM_TYPE transformType)
	{
		PolyonF4D *poly = NULL;
		switch (transformType)
		{
		case TRANSFORM_LOCAL:
		{
			for (RenderList4D::Itr itr = renderList->polyData.begin();
				itr != renderList->polyData.end(); ++itr)
			{
				poly = &(*itr);

				GetVertex4DAddVector4D(poly->localList[0], pos, poly->localList[0]);
				GetVertex4DAddVector4D(poly->localList[1], pos, poly->localList[1]);
				GetVertex4DAddVector4D(poly->localList[2], pos, poly->localList[2]);
			}
		}
			break;
		case TRANSFORM_LOCAL_TO_TRANS:
		{
			for (RenderList4D::Itr itr = renderList->polyData.begin();
				itr != renderList->polyData.end(); ++itr)
			{
				poly = &(*itr);
				GetVertex4DAddVector4D(poly->localList[0], pos, poly->transformList[0]);
				GetVertex4DAddVector4D(poly->localList[1], pos, poly->transformList[1]);
				GetVertex4DAddVector4D(poly->localList[2], pos, poly->transformList[2]);
			}
		}
			break;
		case TRANSFORM_TRANS:
		{
			for (RenderList4D::Itr itr = renderList->polyData.begin();
				itr != renderList->polyData.end(); ++itr)
			{
				poly = &(*itr);
				GetVertex4DAddVector4D(poly->transformList[0], pos, poly->transformList[0]);
				GetVertex4DAddVector4D(poly->transformList[1], pos, poly->transformList[1]);
				GetVertex4DAddVector4D(poly->transformList[2], pos, poly->transformList[2]);
			}
		}
			break;
		}
	}

	bool Cull_Object4D(Object4D *object, Frustum *camera, CULL_TYPE cullType)
	{
		if (!object)
			return true;

		if (!object->needCull)
			return false;

		// �Ȱѵ�ǰ���������任�����������ϵ��
		vector3df centerPos;
		GetVector3DMulMatrix44(object->worldPosition, camera->mWorldToCamera, centerPos);

		// ���ȰѰ뾶��������
		f32 scaleRadiusZ;
		f32 scaleRadiusX;
		f32 scaleRadiusY;

		if (cullType == CULL_SPHERE)
		{
			scaleRadiusZ = object->maxRadius * object->scale._z;
			scaleRadiusX = object->maxRadius * object->scale._x;
			scaleRadiusY = object->maxRadius * object->scale._y;
		}
		else if (cullType == CULL_BOX)
		{
			// ���޳�ƫ����ģ�����󶥵�ĸ�ֵΪ��׼
			scaleRadiusZ = object->maxBoundingBox._z * object->scale._z;
			scaleRadiusX = object->maxBoundingBox._x * object->scale._x;
			scaleRadiusY = object->maxBoundingBox._y * object->scale._y;
		}
		else
			return true;

		// Զ�����޳�
		if ((centerPos._z + scaleRadiusZ) < camera->clip_z_near ||
			(centerPos._z - scaleRadiusZ) > camera->clip_z_far)
			return true;

		// �������޳�
		// ����Ĳ����ǻ�����xzƽ���ͶӰ�������
		f32 x_test = 0.5f * camera->viewplane_width * centerPos._z / camera->view_dist;
		if ((centerPos._x - scaleRadiusX) > x_test ||
			(centerPos._x + scaleRadiusX) < -x_test)
			return true;

		// �������޳�
		// ����Ĳ����ǻ�����zyƽ���ͶӰ�������
		f32 y_test = 0.5f * camera->viewplane_height * centerPos._z / camera->view_dist;
		if ((centerPos._y - scaleRadiusY) > y_test ||
			(centerPos._y + scaleRadiusY) < -y_test)
			return true;

		return false;
	}

	void World_To_Camera_Object4D(Object4D *object, Frustum *camera)
	{
		Object4D *obj = object;
		while (obj)
		{
			for (s32 i = 0; i < obj->vertexNumber; i++)
			{
				GetVertex4DMulMatrix44(obj->transformList[i], camera->mWorldToCamera, obj->transformList[i]);
			}
			obj = obj->nextObject;
		}
	}

	void World_To_Camera_RenderList4D(RenderList4D *renderList, Frustum *camera)
	{
		// ����Եƹ�λ�ý��б任, ����任�����������ϵ����
		Matrix mat = camera->mWorldToCamera;
		// ����ֻ������ת�任, ��������ƽ�Ʊ任, �������ֵƹ����������ƶ���Ч��
		mat._m[12] = mat._m[13] = mat._m[14] = 0;

		Light *light = NULL;
		for (lightItr itr = gLights->begin(); itr != gLights->end(); ++itr)
		{
			light = *itr;
			GetVector3DMulMatrix44(light->position, mat, light->transPosition);
			GetVector3DMulMatrix44(light->direction, mat, light->transDirection);
		}

		PolyonF4D *poly = NULL;
		for (RenderList4D::Itr itr = renderList->polyData.begin();
			itr != renderList->polyData.end(); ++itr)
		{
			poly = &(*itr);


			GetVertex4DMulMatrix44(poly->transformList[0], camera->mWorldToCamera, poly->transformList[0]);
			GetVertex4DMulMatrix44(poly->transformList[1], camera->mWorldToCamera, poly->transformList[1]);
			GetVertex4DMulMatrix44(poly->transformList[2], camera->mWorldToCamera, poly->transformList[2]);
		}
	}

	void Sort_RenderList4D(RenderList4D *renderList)
	{
		// ����Զ�����б�����������(�����㷨)
		renderList->polyData.sort(CompEPolyonF4D());
	}

	bool Remove_Backface(Polyon4D *poly, Frustum *camera)
	{
		s32 index0 = poly->verIndex[0];
		s32 index1 = poly->verIndex[1];
		s32 index2 = poly->verIndex[2];

		// ����ʹ�ñ任��Ķ���ζ����б�, ��Ϊ��������ֻ���ڶ��㱻�任δ��������֮����ܽ���
		// ����������һ�������������ϵ�ڶ�����б�������
		// ����ʱ��˳ʱ�뷽�����е� u = p0->p1 v = p0->p2 n = uXv
		vector3df u = (poly->verList->at(index0) - poly->verList->at(index1)).toVector4D();
		vector3df v = (poly->verList->at(index0) - poly->verList->at(index2)).toVector4D();

		// ����η���
		vector3df n = u.crossProduct(v);
		vector3df view = camera->position - poly->verList->at(index0).toVector4D();

		// ����������ĵ��, ���С��0�� ��ô��Ϊ����汳�������, ������ʾ
		f32 dot = n.dotProduct(view);
		if (dot < 0.0f)
		{
			poly->state |= EPOLY_STATE_BACKFACE;
			return true;
		}

		return false;
	}

	bool Remove_Backface(PolyonF4D *poly, Frustum *camera)
	{
		if (!(poly->state & EPOLY_STATE_ACTIVE) ||
			(poly->state & EPOLY_STATE_BACKFACE) ||
			(poly->state & EPOLY_STATE_CLIPPED))
			return true;

		// ����ʹ�ñ任��Ķ���ζ����б�, ��Ϊ��������ֻ���ڶ��㱻�任δ��������֮����ܽ���
		// ����������һ�������������ϵ�ڶ�����б�������
		// ����ʱ��˳ʱ�뷽�����е� u = p0->p1 v = p0->p2 n = uXv
		vector3df u = (poly->transformList[0] - poly->transformList[1]).toVector4D();
		vector3df v = (poly->transformList[0] - poly->transformList[2]).toVector4D();

		vector3df n = u.crossProduct(v);
		vector3df view = camera->position - poly->transformList[0].toVector4D();

		// TODO
		f32 dot = n.dotProduct(view);
		if (dot < 0)
		{
			poly->state |= EPOLY_STATE_BACKFACE;
			return true;
		}

		return false;
	}

	void Remove_Backface_Object4D(Object4D *object, Frustum *camera)
	{
		Object4D *obj = object;
		Polyon4D *poly = NULL;
		while (obj)
		{
			// ����û�б��޳�
			if (!(obj->state & EOBJECT_STATE_CULLED))
			{
				for (s32 i = 0; i < obj->polyonNumber; i++)
				{
					poly = &obj->polyonList[i];
					Remove_Backface(poly, camera);

				}
			}

			obj = obj->nextObject;
		}
	}

	void Remove_Backface_RenderList4D(RenderList4D *renderList, Frustum *camera)
	{
		PolyonF4D *poly = NULL;

		for (RenderList4D::Itr itr = renderList->polyData.begin();
			itr != renderList->polyData.end();)
		{
			poly = &(*itr);

			// �����Ƴ����ɼ��Ķ����
			// ������RenderList֮��Ĳ����в���Ҫ�жϵ�ǰ������Ƿ�ɼ�
			if (Remove_Backface(poly, camera))
				renderList->polyData.erase(itr++);
			else
			{
				++itr;
			}

		}
	}

	void Light_PolyonF4D(PolyonF4D *poly, Frustum *camera)
	{
		bool lightOn = false;
		Color baseColor(poly->color);

		// ����ʹ����Eint���Ͷ�û��ʹ��Color����, ԭ���Ǻ���Ҫ������ɫ���ۼ�
		// ʹ��unsigned char���ܻᵼ�����, �Ӷ�������ʾ������
		s32 sumColorR = 0, sumColorG = 0, sumColorB = 0;	// ȫ����ɫ�ܺ�

		// ���㰴��˳ʱ�뷽������, ���u=p0->p1 v=p0->p2 n = uXv
		vector3df u = (poly->transformList[0] - poly->transformList[1]).toVector4D();
		vector3df v = (poly->transformList[0] - poly->transformList[2]).toVector4D();
		vector3df n = u.crossProduct(v);
		// ���߳���
		f32 length = n.getLength();

		Light *light = NULL;
		for (lightItr itr = gLights->begin(); itr != gLights->end(); ++itr)
		{
			light = *itr;
			// �ƹ�ر��򲻴���
			if (!light->lightOn)
				continue;

			lightOn = true;
			// ������
			if (light->lightType == LIGHT_AMBIENT)
			{
				// ��ÿ��������������ɫ���, ������256�� ȷ�������0~255֮��
				sumColorR += light->ambient.r * baseColor.r >> 8;
				sumColorG += light->ambient.g * baseColor.g >> 8;
				sumColorB += light->ambient.b * baseColor.b >> 8;
			}
			// ƽ�й�
			else if (light->lightType == LIGHT_DIRECTION)
			{
				// ����Զ�ƹ�, �����Ҫ֪���淢�ֺ͹�Դ����
				// ����ʹ�õƹⷽ����淽����Ϊ���淢�ּнǵ�����, 
				// �������н�С��90ʱ, ����������
				f32 dot = n.dotProduct(-light->transDirection);

				// ƽ�й����ģ��
				// I(d)dir = IOdir * Cldir
				// ɢ���ļ��㹫ʽ
				// Itotal = Rsdiffuse * Idiffuse * (n * l)
				// ������������˼���
				if (dot > 0.0f)
				{
					f32 dirLeng = light->transDirection.getLength();

					// ���չ��յ�ǿ��, ����η�������շ���ļн�ԽС, ��ô�����ǿ��Խ��
					// �н�Խ��, ����ǿ��ԽС, �����Խ��
					f32 temp = light->power * dot / (length * dirLeng * 256);
					sumColorR += temp * light->diffuse.r * baseColor.r;
					sumColorG += temp * light->diffuse.g * baseColor.g;
					sumColorB += temp * light->diffuse.b * baseColor.b;
				}
				else
				{
					// ���ﵱ������Ǳ�����Դʱ, Ҳ������һЩ����, ֻ�ǰ�������ɫ����
					// �ܰ�, �����űȽ�Э��^___^

					sumColorR += baseColor.r * light->shadowFactor;
					sumColorG += baseColor.g * light->shadowFactor;
					sumColorB += baseColor.b * light->shadowFactor;
				}
			}
			else if (light->lightType == LIGHT_POINT)
			{
				// ���Դ�Ĺ���ģ��
				//					IOpoint * Clpoint
				// I(d)point = --------------------------
				//				kc + kl * d + kq * d * d
				// ����d = |p-s| �����Դ������εľ���

				// ����ε����Դ�ķ�������
				vector3df dir = light->transPosition - poly->transformList[0].toVector4D();
				f32 dot = n.dotProduct(dir);

				if (dot > 0.0f)
				{
					// ɢ���ļ��㹫ʽ
					// Itotal = Rsdiffuse * Idiffuse * (n * l)
					// ������������˼���
					// ���Դ������εľ���
					f32 dist = dir.getLength();
					f32 atten = light->kc + light->kl * dist + light->kq * dist * dist;
					f32 temp = dot / (length * dist * atten * 256);

					sumColorR += light->diffuse.r * baseColor.r * temp;
					sumColorG += light->diffuse.g * baseColor.g * temp;
					sumColorB += light->diffuse.b * baseColor.b * temp;
				}
				else
				{
					// ���ﵱ������Ǳ�����Դʱ, Ҳ������һЩ����, ֻ�ǰ�������ɫ����
					// �ܰ�, �����űȽ�Э��^___^
					sumColorR += baseColor.r * light->shadowFactor;
					sumColorG += baseColor.g * light->shadowFactor;
					sumColorB += baseColor.b * light->shadowFactor;
				}
			}
		}

		if (lightOn)
		{
			// �����ٴν�����ֵ���
			if (sumColorR > 255) sumColorR = 255;
			if (sumColorG > 255) sumColorG = 255;
			if (sumColorB > 255) sumColorB = 255;

			poly->color = COLOR_16BIT(sumColorR, sumColorG, sumColorB);
		}
	}

	void Light_RenderList4D(RenderList4D *renderList, Frustum *camera)
	{
		PolyonF4D *poly = NULL;

		for (RenderList4D::Itr itr = renderList->polyData.begin();
			itr != renderList->polyData.end(); ++itr)
		{
			poly = &(*itr);
			Light_PolyonF4D(poly, camera);
		}
	}

	void Camera_To_Perspective_Object4D(Object4D *object, Frustum *camera)
	{
		if (!object)
			return;

		Object4D *obj = object;
		f32 d = camera->view_dist;
		f32 temp = 0;

		while (obj)
		{
			for (s32 i = 0; i < obj->vertexNumber; i++)
			{
				temp = d / obj->transformList[i]._z;

				// �任X, Y��, Z�᲻�任
				obj->transformList[i]._x = temp * obj->transformList[i]._x;
				obj->transformList[i]._y = camera->aspect_ratio * temp * obj->transformList[i]._y;
			}
			obj = obj->nextObject;
		}
	}

	void Camera_To_Perspective_RenderList4D(RenderList4D *renderList, Frustum *camera)
	{
		f32 d = camera->view_dist;
		f32 temp = 0;
		PolyonF4D *poly = NULL;
		for (RenderList4D::Itr itr = renderList->polyData.begin();
			itr != renderList->polyData.end(); ++itr)
		{
			poly = &(*itr);

			for (s32 i = 0; i < 3; i++)
			{
				temp = d / poly->transformList[i]._z;
				poly->transformList[i]._x = temp * poly->transformList[i]._x;
				poly->transformList[i]._y = camera->aspect_ratio * temp * poly->transformList[i]._y;
			}
		}
	}

	void Perspective_To_Screen_Object4D(Object4D *object, Frustum *camera)
	{
		if (!object)
			return;

		f32 XOffset = 0.5f * (camera->viewport_width - 1);
		f32 YOffset = 0.5f * (camera->viewport_height - 1);

		Object4D *obj = object;
		while (obj)
		{
			for (s32 i = 0; i < obj->vertexNumber; i++)
			{
				// ������, x,y�Ѿ�����һ��Ϊ[-1�� 1]�ķ�Χ֮��
				obj->transformList[i]._x = XOffset *(1 + obj->transformList[i]._x);
				obj->transformList[i]._y = YOffset *(1 - obj->transformList[i]._y);
			}
			obj = obj->nextObject;
		}
	}

	void Perspective_To_Screen_RenderList4D(RenderList4D *renderList, Frustum *camera)
	{
		f32 XOffset = 0.5f * (camera->viewport_width - 1);
		f32 YOffset = 0.5f * (camera->viewport_height - 1);

		PolyonF4D *poly = NULL;
		for (RenderList4D::Itr itr = renderList->polyData.begin();
			itr != renderList->polyData.end(); ++itr)
		{
			poly = &(*itr);


			for (s32 i = 0; i < 3; i++)
			{
				poly->transformList[i]._x = XOffset *(1 + poly->transformList[i]._x);
				poly->transformList[i]._y = YOffset *(1 - poly->transformList[i]._y);
			}
		}
	}

	// �жϴ˶�����Ƿ����������Χ֮��
	static bool isClipped(PolyonF4D *poly, const Plane3D &plane)
	{
		s32 flag = 0;
		for (s32 i = 0; i < 3; i++)
		{
			f32 temp = plane.normal.dotProduct(poly->transformList[i].toVector4D() - plane.point);
			if (temp < 0)
				flag++;
		}

		if (flag == 3)
			return true;
		else
			return false;
	}

	static bool isClipped(PolyonF4D *point, f32 znear, f32 zfar)
	{
		s32 num = 0;

		for (int i = 0; i < 3; i++)
		{
			if (point->transformList[i]._z < znear || point->transformList[i]._z > zfar)
				num++;
		}

		if (num == 3)
			return true;
		return false;
	}

	void Clip_RenderList4D(RenderList4D *renderList, Frustum *camera)
	{
		PolyonF4D *poly = NULL;
		for (RenderList4D::Itr itr = renderList->polyData.begin();
			itr != renderList->polyData.end(); ++itr)
		{
			poly = &(*itr);
			// ������ж���βü�
			if (isClipped(poly, camera->clip_plane_L) ||
				isClipped(poly, camera->clip_plane_R) ||
				isClipped(poly, camera->clip_plane_B) ||
				isClipped(poly, camera->clip_plane_T))
			{
				renderList->polyData.erase(itr++);
				continue;
			}
		}
	}

	void World_To_Screen_RenderList4D(RenderList4D *renderList, Frustum *camera)
	{
		f32 XOffset = 0.5f * (camera->viewport_width - 1);
		f32 YOffset = 0.5f * (camera->viewport_height - 1);
		f32 dist = camera->view_dist;
		f32 temp = 0;

		// ����Եƹ�λ�ý��б任, ����任�����������ϵ����
		Matrix mat = camera->mWorldToCamera;
		// ����ֻ������ת�任, ��������ƽ�Ʊ任, �������ֵƹ����������ƶ���Ч��
		mat._m[12] = mat._m[12] = mat._m[13] = 0.0f; mat._m[14] = 1.0f;

		Light *light = NULL;
		for (lightItr itr = gLights->begin(); itr != gLights->end(); ++itr)
		{
			light = *itr;
			GetVector3DMulMatrix44(light->position, mat, light->transPosition);
			GetVector3DMulMatrix44(light->direction, mat, light->transDirection);
		}

		PolyonF4D *poly = NULL;
		for (RenderList4D::Itr itr = renderList->polyData.begin();
			itr != renderList->polyData.end();)
		{
			poly = &(*itr);

			// ���Ƚ��б�������
			if (Remove_Backface(poly, camera))
			{
				renderList->polyData.erase(itr++);
				continue;
			}

			// ������������������ϵ
			GetVertex4DMulMatrix44(poly->transformList[0], camera->mWorldToCamera, poly->transformList[0]);
			GetVertex4DMulMatrix44(poly->transformList[1], camera->mWorldToCamera, poly->transformList[1]);
			GetVertex4DMulMatrix44(poly->transformList[2], camera->mWorldToCamera, poly->transformList[2]);

			// ���вü�
			if (isClipped(poly, camera->clip_z_near, camera->clip_z_far) ||
				isClipped(poly, camera->clip_plane_L) ||
				isClipped(poly, camera->clip_plane_R) ||
				isClipped(poly, camera->clip_plane_T) ||
				isClipped(poly, camera->clip_plane_B))
			{
				renderList->polyData.erase(itr++);
				continue;
			}

			// ���δ���ü��Ҳ��Ǳ�������й��ռ���
			Light_PolyonF4D(poly, camera);

			// ֮��ִ�н������ı任
			for (s32 i = 0; i < 3; i++)
			{
				// ������������������ϵ
				//GetVertex4DMulMatrix44(poly->transformList[i], camera->mWorldToCamera, poly->transformList[i]);


				// ͸�ӱ任
				temp = dist / poly->transformList[i]._z;
				poly->transformList[i]._x = temp * poly->transformList[i]._x;
				poly->transformList[i]._y = camera->aspect_ratio * temp * poly->transformList[i]._y;

				// ��Ļ�任
				poly->transformList[i]._x = XOffset *(1 + poly->transformList[i]._x);
				poly->transformList[i]._y = YOffset *(1 - poly->transformList[i]._y);
			}

			++itr;
		}

		// �����������ϵ�ж���Ⱦ�б�Ķ���Խ����������
		// �����������һ����ʵ, ����ȻX,Y���귢���˱仯, ����Z����������֮ǰ���������ϵ�µ�Zֵ
		Sort_RenderList4D(renderList);
	}

	void Draw_Object4D_Wire(Object4D *object)
	{
		if (!object)
			return;

		Object4D *obj = object;
		while (obj)
		{
			for (s32 i = 0; i < obj->polyonNumber; i++)
			{
				if ((obj->polyonList[i].state & EPOLY_STATE_ACTIVE) &&
					!(obj->polyonList[i].state & EPOLY_STATE_CLIPPED) &&
					!(obj->polyonList[i].state & EPOLY_STATE_BACKFACE))
				{
					s32 index0 = obj->polyonList[i].verIndex[0];
					s32 index1 = obj->polyonList[i].verIndex[1];
					s32 index2 = obj->polyonList[i].verIndex[2];

					Graphics::drawLine(obj->transformList[index0]._x, obj->transformList[index0]._y,
						obj->transformList[index1]._x, obj->transformList[index1]._y,
						Color(obj->polyonList[i].color));
					Graphics::drawLine(obj->transformList[index1]._x, obj->transformList[index1]._y,
						obj->transformList[index2]._x, obj->transformList[index2]._y,
						Color(obj->polyonList[i].color));
					Graphics::drawLine(obj->transformList[index0]._x, obj->transformList[index0]._y,
						obj->transformList[index2]._x, obj->transformList[index2]._y,
						Color(obj->polyonList[i].color));
				}

				// �������, ������ָ������, ��������ʾ
				obj->polyonList[i].state = EPOLY_STATE_ACTIVE;
			}

			obj->state = EOBJECT_STATE_ACTIVE;
			obj = obj->nextObject;
		}
	}

	void Draw_RenderList4D_Wire(RenderList4D *renderList)
	{
		PolyonF4D *poly = NULL;
		for (RenderList4D::Itr itr = renderList->polyData.begin();
			itr != renderList->polyData.end(); ++itr)
		{
			poly = &(*itr);

			{
				Graphics::drawLine(poly->transformList[0]._x, poly->transformList[0]._y,
					poly->transformList[1]._x, poly->transformList[1]._y, Color(poly->color));
				Graphics::drawLine(poly->transformList[1]._x, poly->transformList[1]._y,
					poly->transformList[2]._x, poly->transformList[2]._y, Color(poly->color));
				Graphics::drawLine(poly->transformList[0]._x, poly->transformList[0]._y,
					poly->transformList[2]._x, poly->transformList[2]._y, Color(poly->color));
			}

			// �������, ������ָ������, ��������ʾ
			poly->state = EPOLY_STATE_ACTIVE;
		}

		renderList->polyData.clear();
	}

	void Draw_Object4D_Solid(Object4D *object)
	{
		if (!object)
			return;

		Object4D *obj = object;
		while (obj)
		{
			for (s32 i = 0; i < obj->polyonNumber; i++)
			{
				if ((obj->polyonList[i].state & EPOLY_STATE_ACTIVE) &&
					!(obj->polyonList[i].state & EPOLY_STATE_CLIPPED) &&
					!(obj->polyonList[i].state & EPOLY_STATE_BACKFACE))
				{
					s32 index0 = obj->polyonList[i].verIndex[0];
					s32 index1 = obj->polyonList[i].verIndex[1];
					s32 index2 = obj->polyonList[i].verIndex[2];

					Graphics::fillTriangle(obj->transformList[index0]._x, obj->transformList[index0]._y,
						obj->transformList[index1]._x, obj->transformList[index1]._y,
						obj->transformList[index2]._x, obj->transformList[index2]._y,
						Color(obj->polyonList[i].color));
				}

				// �������, ������ָ������, ��������ʾ
				obj->polyonList[i].state = EPOLY_STATE_ACTIVE;
			}

			obj->state = EOBJECT_STATE_ACTIVE;
			obj = obj->nextObject;
		}
	}

	void Draw_RenderList4D_Solid(RenderList4D *renderList)
	{
		PolyonF4D *poly = NULL;
		for (RenderList4D::Itr itr = renderList->polyData.begin();
			itr != renderList->polyData.end(); ++itr)
		{
			poly = &(*itr);

			{
				Graphics::fillTriangle(poly->transformList[0]._x, poly->transformList[0]._y,
					poly->transformList[1]._x, poly->transformList[1]._y,
					poly->transformList[2]._x, poly->transformList[2]._y,
					Color(poly->color));
			}

			// �������, ������ָ������, ��������ʾ
			poly->state = EPOLY_STATE_ACTIVE;
		}

		renderList->polyData.clear();
	}

	// ����ƽ��Ϊ�����ǵ�������
	//       v0
	//       /\
	//      /  \
	//  v1 ------ v2
	static void Draw_Top_Trangle(f32 x0, f32 y0, f32 z0, f32 u0, f32 v0,
		f32 x1, f32 y1, f32 z1, f32 u1, f32 v1,
		f32 x2, f32 y2, f32 z2, f32 u2, f32 v2,
		Material *mat, const Color &color)
	{
		if (y2 < 0 || y0 > SCREEN_HEIGHT - 1)
			return;

		f32 dxdl = (x0 - x1) / (y0 - y1);
		f32 dxdr = (x0 - x2) / (y0 - y2);

		f32 dzdl = (z0 - z1) / (y0 - y1);
		f32 dzdr = (z0 - z2) / (y0 - y2);

		f32 startL = x0;
		f32 startR = x0;

		f32 begX = 0;
		f32 endX = 0;

		if (!mat || !mat->useTexture())
		{
			if (y0 < 0)
				y0 = 0;
			if (y2 > SCREEN_HEIGHT - 1)
				y2 = SCREEN_HEIGHT - 1;

			// û���������û����ͼ��ôʹ��color��ɫ
			for (f32 y = y0; y < y2; y++)
			{
				//DrawLine(startL, y, startR, y, color);
				begX = startL;
				endX = startR;
				if (endX < begX) std::swap(begX, endX);

				if (begX < 0)
					begX = 0;
				if (endX > SCREEN_WIDTH - 1)
					endX = SCREEN_WIDTH - 1;

				for (f32 x = begX; x < endX; x++)
				{
					if (Graphics::checkZ(x, y, z0))
						Graphics::setPixel(x, y, /*z0, */color);
				}

				startL += dxdl;
				startR += dxdr;
			}

			return;
		}

		f32 dudl = (u0 - u1) / (y0 - y1);
		f32 dvdl = (v0 - v1) / (y0 - y1);
		f32 dudr = (u0 - u2) / (y0 - y2);
		f32 dvdr = (v0 - v2) / (y0 - y2);

		f32 startLU = u0;
		f32 startLV = v0;
		f32 startRU = u0;
		f32 startRV = v0;

		f32 begU = 0;
		f32 endU = 0;
		f32 begV = 0;
		f32 endV = 0;

		f32 dx = 0;
		f32 ui = 0;
		f32 vi = 0;

		f32 zl = 0;
		f32 zr = 0;
		f32 z = 0;

		for (f32 y = y0; y < y2; y++)
		{

			begX = (s32)startL;
			endX = (s32)startR;
			begU = startLU;
			endU = startRU;
			begV = startLV;
			endV = startRV;

			//------------
			zl = z0 + (y - y0) * dzdl;
			zr = z0 + (y - y0) * dzdr;

			if (endX < begX)
			{
				std::swap(begX, endX);
				std::swap(begU, endU);
				std::swap(begV, endV);

				//---------------
				std::swap(zl, zr);
			}

			// ����x�ķ�Χ, ��������ͼ��u����
			if (begX < 0)
			{
				// ����U
				begU = begU - begX * (startLU - startRU) / (startL - startR);
				begX = 0;
			}
			if (endX > SCREEN_WIDTH - 1)
			{
				// ����U
				endU = endU - (endX - SCREEN_WIDTH) * (startLU - startRU) / (startL - startR);
				endX = SCREEN_WIDTH - 1;
			}

			dx = endX - begX;
			ui = (endU - begU) / dx;
			vi = (endV - begV) / dx;

			//----------------
			z = zl;
			f32 zd = (zl - zr) / (begX - endX);
			for (f32 x = begX; x <= endX; x++)
			{
				if (y > -1 && y < SCREEN_HEIGHT && Graphics::checkZ(x, y, z))
				{
					const Color tex = mat->getPixel(begU, begV);
					Graphics::setPixel(x, y,/* z, */color * tex);
				}

				begU += ui;
				begV += vi;
				z += zd;
			}

			startL += dxdl;
			startR += dxdr;

			startLU += dudl;
			startLV += dvdl;
			startRU += dudr;
			startRV += dvdr;
		}
	}

	// ����������
	//  v1     v0
	//   ------
	//    \  /
	//     \/
	//     v2
	static void Draw_Botton_Trangle(f32 x0, f32 y0, f32 z0,
		f32 u0, f32 v0,
		f32 x1, f32 y1, f32 z1,
		f32 u1, f32 v1,
		f32 x2, f32 y2, f32 z2,
		f32 u2, f32 v2,
		Material *mat, const Color &color)
	{
		if (y2 < 0 || y0 > SCREEN_HEIGHT - 1)
			return;

		f32 dxdl = (x1 - x2) / (y1 - y2);
		f32 dxdr = (x0 - x2) / (y0 - y2);

		f32 dzdl = (z1 - z2) / (y1 - y2);
		f32 dzdr = (z0 - z2) / (y0 - y2);

		f32 startL = x1;
		f32 startR = x0;

		f32 begX = 0;
		f32 endX = 0;

		if (!mat || !mat->useTexture())
		{
			if (y0 < 0)
				y0 = 0;
			if (y2 > SCREEN_HEIGHT - 1)
				y2 = SCREEN_HEIGHT - 1;

			// û���������û����ͼ��ôʹ��color��ɫ
			for (f32 y = y0; y < y2; y++)
			{

				begX = startL;
				endX = startR;
				if (endX < begX) std::swap(begX, endX);

				if (begX < 0)
					begX = 0;
				if (endX > SCREEN_WIDTH - 1)
					endX = SCREEN_WIDTH - 1;

				for (f32 x = begX; x < endX; x++)
				{
					if (Graphics::checkZ(x, y, z0))
						Graphics::setPixel(x, y,/* z0, */color);
				}

				startL += dxdl;
				startR += dxdr;
			}

			return;
		}

		f32 dudl = (u1 - u2) / (y1 - y2);
		f32 dvdl = (v1 - v2) / (y1 - y2);
		f32 dudr = (u0 - u2) / (y0 - y2);
		f32 dvdr = (v0 - v2) / (y0 - y2);

		f32 startLU = u1;
		f32 startLV = v1;
		f32 startRU = u0;
		f32 startRV = v0;

		f32 begU = 0;
		f32 endU = 0;
		f32 begV = 0;
		f32 endV = 0;

		f32 dx = 0;
		f32 ui = 0;
		f32 vi = 0;

		f32 zl = 0;
		f32 zr = 0;
		f32 z = 0;
		for (f32 y = y0; y < y2; y++)
		{

			begX = (s32)startL;
			endX = (s32)startR;
			begU = startLU;
			endU = startRU;
			begV = startLV;
			endV = startRV;

			//---------------
			// ����Zֵ
			zl = z1 + (y - y0) * dzdl;
			zr = z0 + (y - y0) * dzdr;

			if (endX < begX)
			{
				std::swap(begX, endX);
				std::swap(begU, endU);
				std::swap(begV, endV);

				//-------------------
				std::swap(zl, zr);
			}

			// ����x�ķ�Χ, ��������ͼ��u����
			if (begX < 0)
			{
				// ����U
				begU = begU - begX * (startLU - startRU) / (startL - startR);
				begX = 0;
			}
			if (endX > SCREEN_WIDTH - 1)
			{
				// ����U
				endU = endU - (endX - SCREEN_WIDTH) * (startLU - startRU) / (startL - startR);
				endX = SCREEN_WIDTH - 1;
			}

			dx = endX - begX;
			ui = (endU - begU) / dx;
			vi = (endV - begV) / dx;

			//----------------
			z = zl;
			f32 zd = (zl - zr) / (begX - endX);
			for (f32 x = begX; x <= endX; x++)
			{
				if (y > -1 && y < SCREEN_HEIGHT && Graphics::checkZ(x, y, z))
				{
					const Color tex = mat->getPixel(begU, begV);
					Graphics::setPixel(x, y,/* z, */color * tex);
				}

				begU += ui;
				begV += vi;
				z += zd;
			}

			startL += dxdl;
			startR += dxdr;

			startLU += dudl;
			startLV += dvdl;
			startRU += dudr;
			startRV += dvdr;
		}
	}

	static void Draw_Trangle(PolyonF4D *poly)
	{
		f32 x0 = poly->transformList[0]._x, y0 = poly->transformList[0]._y, z0 = poly->transformList[0]._z;
		f32 u0 = poly->transformList[0].u, v0 = poly->transformList[0].v;
		f32 x1 = poly->transformList[1]._x, y1 = poly->transformList[1]._y, z1 = poly->transformList[1]._z;
		f32 u1 = poly->transformList[1].u, v1 = poly->transformList[1].v;
		f32 x2 = poly->transformList[2]._x, y2 = poly->transformList[2]._y, z2 = poly->transformList[2]._z;
		f32 u2 = poly->transformList[2].u, v2 = poly->transformList[2].v;

		// ��֤ y0 < y1 < y2
		if (y1 < y0)
		{
			std::swap(x0, x1);
			std::swap(y0, y1);
			std::swap(z0, z1);
			std::swap(u0, u1);
			std::swap(v0, v1);
		}

		if (y2 < y0)
		{
			std::swap(x0, x2);
			std::swap(y0, y2);
			std::swap(z0, z2);
			std::swap(u0, u2);
			std::swap(v0, v2);
		}

		if (y2 < y1)
		{
			std::swap(x1, x2);
			std::swap(y1, y2);
			std::swap(z1, z2);
			std::swap(u1, u2);
			std::swap(v1, v2);
		}

		f32 factor = (y1 - y0) / (y2 - y0);
		f32 newX = x0 + factor * (x2 - x0);
		f32 newZ = z0 + factor * (z2 - z0);
		f32 newU = u0 + factor * (u2 - u0);
		// ���ﲻ�ܼ򵥵�ʹ��v1������v
		f32 newV = v0 + factor * (v2 - v0);
		Draw_Top_Trangle(x0, y0, z0, u0, v0, x1, y1, z1, u1, v1, newX, y1, newZ, newU, newV, poly->material, Color(poly->color));
		Draw_Botton_Trangle(newX, y1, newZ, newU, newV, x1, y1, z1, u1, v1, x2, y2, z2, u2, v2, poly->material, Color(poly->color));
	}

	void Draw_RenderList4D_Teture_Solid(RenderList4D * renderList)
	{
		PolyonF4D *poly = NULL;
		for (RenderList4D::Itr itr = renderList->polyData.begin();
			itr != renderList->polyData.end(); ++itr)
		{
			poly = &(*itr);

			Draw_Trangle(poly);

			// �������, ������ָ������, ��������ʾ
			poly->state = EPOLY_STATE_ACTIVE;
		}

		renderList->polyData.clear();
	}
}
