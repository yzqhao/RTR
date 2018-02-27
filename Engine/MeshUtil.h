
#ifndef _ENGINE_MESHUTIL_H_
#define _ENGINE_MESHUTIL_H_

#include "OgreMeshInfo.h"
#include "Core.h"

namespace RTR
{
	// ��stream�ж�ȡ�ַ���
	// size��ʾҪ��ȡ�ַ����ĸ���, ���size==-1,���ȡһ���ַ���
	std::string ReadString(std::ifstream &in, s32 size = -1);

	// ��ȡOgre mesh�ļ�ʱʹ��
	// ��stream�ж�ȡ��һ����ʶ��, ��ӦOGREMESHID
	u16 ReadChunk(std::ifstream &in);

	// ��stream�ж�ȡһ��bool����ֵ
	bool ReadBool(std::ifstream &in);

	// ��stream�ж�ȡһ��unsined int���͵���ֵ
	u32 ReadInt(std::ifstream &in);

	// ��strean�ж�ȡһ��unsigned short���͵���ֵ
	u16 ReadShort(std::ifstream& in);

	// ��stream�ж�ȡT���͵�һ������
	// size��ʾ����Ĵ�С
	template<typename T>
	std::vector<T> ReadArray(std::ifstream &in, s32 size)
	{
		std::vector<T> v(size, 0);
		in.read((char*)(&v[0]), sizeof(T)* size);
		return v;
	}

	// �����������һ��Ogre .Mesh��ʽ�Ķ������ļ�, ����ʵ���˶�Ogre .Mesh�ļ���̬ģ�͵���ȷ��ȡ
	// ���Ի�ȡģ�͵Ķ��������Ͷ������ݵ�ģ������, ������һ��EObject����, ���ģ���ж��submesh
	// ���ͨ��EObject.nextObject����ȡ��һ��submesh��ָ��
	Object4D* LoadOgreMesh(const std::string& meshName, const vector3df &worldPos = vector3df(),
		const vector3df &scale = vector3df(1, 1, 1));
}

#endif