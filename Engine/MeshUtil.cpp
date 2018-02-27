
#include <fstream>

#include "MeshUtil.h"

namespace RTR
{
	std::string ReadString(std::ifstream &in, s32 size)
	{
		std::string ss;
		if (size == -1) // 读取一行
		{
			c8 c;
			while (in.read(&c, sizeof(c8)) && c != '\n' && c != '\r')
				ss += c;
		}
		else
		{
			c8 c[256];
			in.read(c, sizeof(c8)* size);
			ss.append(c);
		}

		return ss;
	}

	u16 ReadChunk(std::ifstream &in)
	{
		u16 id;
		in.read((c8*)&id, sizeof(u16));

		u32 length;
		in.read((c8*)&length, sizeof(u32));

		return id;
	}

	bool ReadBool(std::ifstream &in)
	{
		bool re = false;
		in.read((c8*)&re, sizeof(bool));
		return re;
	}

	u32 ReadInt(std::ifstream &in)
	{
		u32 re;
		in.read((char*)&re, sizeof(u32));
		return re;
	}

	u16 ReadShort(std::ifstream& in)
	{
		u16 re;
		in.read((c8*)&re, sizeof(u16));
		return re;
	}

	Object4D* LoadOgreMesh(const std::string& meshName, const vector3df &worldPos, const vector3df &scale)
	{
		std::ifstream in;
		// 模型文件, 首先获取默认路径
		in.open(GetPath(meshName).c_str(), std::ios_base::binary | std::ios_base::in);
		if (in.bad())
		{
			in.close();
			return NULL;
		}

		std::string name = GetNameFromPath(meshName);
		// 加载材质
		ParseMaterialScript(name + ".material");

		Object4D *head = NULL;
		{
			// Chunk--M_HEADER
			u16 HEADER = ReadShort(in);
			// Version
			std::string Version = ReadString(in);

			// Chunk--M_MESH
			u16 MESH = ReadChunk(in);
			// skeletallyAnimated
			bool SkeletallyAnimated = ReadBool(in);

			// Chunk--M_SUBMESH
			u16 SUBMESH = ReadChunk(in);
			Object4D* curObject = NULL;
			while (!in.eof() && (SUBMESH == M_SUBMESH))
			{
				std::vector<u32> indexArray;
				std::vector<f32> vertexArray;

				if (head == NULL)
				{
					head = new Object4D;
					curObject = head;
				}
				else
				{
					curObject->nextObject = new Object4D;
					curObject = curObject->nextObject;
				}

				curObject->name = name;
				curObject->state = EOBJECT_STATE_ACTIVE | EOBJECT_STATE_VISIBLE;
				// 设置坐标
				curObject->worldPosition = worldPos;
				curObject->scale = scale;

				// Materia
				std::string Materia = ReadString(in);
				// 读取多边形的材质
				curObject->materiaName = Materia;

				// bool useSharedVertices
				bool UseSharedVertices = ReadBool(in);

				// unsigned int indexCount
				u32 indexCount = ReadInt(in);
				// 获取模型多边形个数
				curObject->polyonNumber = indexCount / 3;
				curObject->polyonList.reserve(indexCount / 3);

				// bool indexes32Bit
				bool indexes32Bit = ReadBool(in);
				if (indexes32Bit)
				{
					// unsigned int* faceVertexIndices (indexCount)
					std::vector<u32> faceVertexIndices = ReadArray<u32>(in, indexCount);
					// 读取模型顶点索引
					indexArray.assign(faceVertexIndices.begin(), faceVertexIndices.end());
				}
				else
				{
					// unsigned short* faceVertexIndices (indexCount)
					std::vector<u16> faceVertexIndices = ReadArray<u16>(in, indexCount);
					// 读取模型定点索引
					indexArray.assign(faceVertexIndices.begin(), faceVertexIndices.end());
				}

				// Chunk--M_GEOMETRY
				u16 GEOMETRY = ReadChunk(in);

				// unsigned int vertexCount
				u32 vertexCount = ReadInt(in);
				// 模型顶点个数
				curObject->vertexNumber = vertexCount;
				curObject->localList.reserve(vertexCount);
				curObject->transformList.reserve(vertexCount);

				// Chunk--M_GEOMETRY_VERTEX_DECLARATION
				u16 GEOMETRY_VERTEX_DECLARATION = ReadChunk(in);
				// Chunk--M_GEOMETRY_VERTEX_ELEMENT
				u16 GEOMETRY_VERTEX_ELEMENT = ReadChunk(in);
				while (!in.eof() && GEOMETRY_VERTEX_ELEMENT == M_GEOMETRY_VERTEX_ELEMENT)
				{
					// unsigned short source;  	// buffer bind source
					u16 source = ReadShort(in);
					// unsigned short type;    	// VertexElementType
					u16 type = ReadShort(in);
					// unsigned short semantic; // VertexElementSemantic
					u16 semantic = ReadShort(in);
					// unsigned short offset;	// start offset in buffer in bytes
					u16 offset = ReadShort(in);
					// unsigned short index;	// index of the semantic (for colours and texture coords)
					u16 index = ReadShort(in);
					GEOMETRY_VERTEX_ELEMENT = ReadChunk(in);
				}

				//Chunk--M_GEOMETRY_VERTEX_BUFFER
				u16 GEOMETRY_VERTEX_BUFFER = GEOMETRY_VERTEX_ELEMENT;
				// unsigned short bindIndex;	// Index to bind this buffer to
				u16 bindIndex = ReadShort(in);

				// unsigned short vertexSize;	// Per-vertex size, must agree with declaration at this index
				u16 vertexSize = ReadShort(in);

				// Chunk--M_GEOMETRY_VERTEX_BUFFER_DATA
				u16 GEOMETRY_VERTEX_BUFFER_DATA = ReadChunk(in);

				s32 vertexNum = vertexSize / sizeof(f32);
				// data buffer
				s32 bufferSize = vertexCount * vertexNum;

				// 读取顶点数据

				vertexArray = ReadArray<f32>(in, bufferSize);

				for (s32 i = 0; i < (s32)vertexArray.size(); i += vertexNum)
				{
					Vertex4D vex;
					// 顶点坐标
					vex._x = vertexArray[i];
					vex._y = vertexArray[i + 1];
					vex._z = vertexArray[i + 2];
					// 顶点法线
					//vex.nx	= vertexArray[i + 3];
					//vex.ny	= vertexArray[i + 4];
					//vex.nz	= vertexArray[i + 5];
					// 顶点UV
					vex.u = vertexArray[i + 6];
					vex.v = vertexArray[i + 7];

					// 这里对u，v坐标进行限制在[0, 1]之间
					//vex.u	= std::abs(vex.u - (EInt)vex.u);
					//vex.v	= std::abs(vex.v - (EInt)vex.v);

					curObject->localList.push_back(vex);
					curObject->transformList.push_back(vex);
				}

				for (s32 i = 0; i < curObject->polyonNumber; i++)
				{
					s32 index = i * 3;
					Polyon4D poly;
					poly.state = EPOLY_STATE_ACTIVE;
					poly.attribute = EPOLY_ATTR_VERTEX_NORMAL | EPOLY_ATTR_VERTEX_POSITION | EPOLY_ATTR_VERTEX_UV;
					poly.verIndex[0] = indexArray[index];
					poly.verIndex[1] = indexArray[index + 1];
					poly.verIndex[2] = indexArray[index + 2];

					// 这里将变换后的顶点指针交给每个多边形, 是为了防止变换之后定点索引依旧是
					// 最初的顶点坐标
					poly.verList = &curObject->transformList;
					curObject->polyonList.push_back(poly);
				}

				// Chunk--M_SUBMESH_OPERATION
				u8 SUBMESH_OPERATION = ReadChunk(in);
				// unsigned short operationType
				u8 operationType = ReadShort(in);

				SUBMESH = ReadChunk(in);
			}

			// Chunk--M_MESH_BOUNDS
			u8 MESH_BOUNDS = SUBMESH;
			if (MESH_BOUNDS == M_MESH_BOUNDS)
			{
				// float minx, miny, minz
				// float maxx, maxy, maxz
				// float radius
				std::vector<f32> bounds = ReadArray<f32>(in, 7);
				head->minBoundingBox = vector3df(bounds[0], bounds[1], bounds[2]);
				head->maxBoundingBox = vector3df(bounds[3], bounds[4], bounds[5]);
				head->avgRadius = (head->maxBoundingBox - head->minBoundingBox).getLength() / 2;

				f32 minR = head->minBoundingBox.getLength();
				f32 maxR = head->maxBoundingBox.getLength();
				head->maxRadius = maxR > minR ? maxR : minR;

				//Chunk--M_SUBMESH_NAME_TABLE
				u8 SUBMESH_NAME_TABLE = ReadChunk(in);
				if (SUBMESH_NAME_TABLE == M_SUBMESH_NAME_TABLE)
				{
					// Chunk--M_SUBMESH_NAME_TABLE_ELEMENT
					u8 SUBMESH_NAME_TABLE_ELEMENT = ReadChunk(in);
					while (!in.eof() && SUBMESH_NAME_TABLE_ELEMENT == M_SUBMESH_NAME_TABLE_ELEMENT)
					{
						// short index
						// char* name
						s8 index = ReadShort(in);
						std::string name = ReadString(in);
						SUBMESH_NAME_TABLE_ELEMENT = ReadChunk(in);
					}
				}

			}
		}

		in.close();
		return head;
	}
}
