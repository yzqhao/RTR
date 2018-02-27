
#ifndef _ENGINE_MESHUTIL_H_
#define _ENGINE_MESHUTIL_H_

#include "OgreMeshInfo.h"
#include "Core.h"

namespace RTR
{
	// 从stream中读取字符串
	// size表示要读取字符串的个数, 如果size==-1,则读取一行字符串
	std::string ReadString(std::ifstream &in, s32 size = -1);

	// 读取Ogre mesh文件时使用
	// 从stream中读取下一个标识符, 对应OGREMESHID
	u16 ReadChunk(std::ifstream &in);

	// 从stream中读取一个bool型数值
	bool ReadBool(std::ifstream &in);

	// 从stream中读取一个unsined int类型的数值
	u32 ReadInt(std::ifstream &in);

	// 从strean中读取一个unsigned short类型的数值
	u16 ReadShort(std::ifstream& in);

	// 从stream中读取T类型的一组数据
	// size标示数组的大小
	template<typename T>
	std::vector<T> ReadArray(std::ifstream &in, s32 size)
	{
		std::vector<T> v(size, 0);
		in.read((char*)(&v[0]), sizeof(T)* size);
		return v;
	}

	// 这个函数加载一个Ogre .Mesh格式的二进制文件, 函数实现了对Ogre .Mesh文件静态模型的正确读取
	// 可以获取模型的顶点索引和顶点数据等模型数据, 并返回一个EObject对象, 如果模型有多个submesh
	// 则可通过EObject.nextObject来获取下一个submesh的指针
	Object4D* LoadOgreMesh(const std::string& meshName, const vector3df &worldPos = vector3df(),
		const vector3df &scale = vector3df(1, 1, 1));
}

#endif