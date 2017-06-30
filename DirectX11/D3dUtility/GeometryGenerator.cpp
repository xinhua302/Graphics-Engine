#include "GeometryGenerator.h"

void GeometryGenerator::CreateGrid(MeshData &meshData, float width, float depth, UINT wVertexCount, UINT dVertexCount)
{
	float dw = width / static_cast<float>(wVertexCount - 1);
	float dz = depth / static_cast<float>(dVertexCount - 1);

	int vertexCount = wVertexCount * dVertexCount;
	int triangleCount = (wVertexCount - 1) * (dVertexCount - 1) * 2;

	meshData.Vertices.resize(vertexCount);
	float offsetX = -static_cast<float>(wVertexCount - 1) / 2.0f;
	float offsetZ = -static_cast<float>(dVertexCount - 1) / 2.0f;

	float du = 1.0f / static_cast<float>(wVertexCount - 1);
	float dv = 1.0f / static_cast<float>(dVertexCount - 1);

	for (int i = 0; i < dVertexCount; i++)
	{
		float z = offsetZ + i * dz;
		for (int j = 0; j < wVertexCount; j++)
		{
			meshData.Vertices[i * wVertexCount + j].Position = XMFLOAT3(offsetX + j * dw, 0.0f, z);
			meshData.Vertices[i * wVertexCount + j].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
			meshData.Vertices[i * wVertexCount + j].TangentU = XMFLOAT3(1.0f, 0.0f, 0.0f);
			meshData.Vertices[i * wVertexCount + j].TexC = XMFLOAT2(i * dv, j * du);
		}
	}

	meshData.Indices.resize(triangleCount * 3);

	int index = 0;
	for (int i = 0; i < dVertexCount - 1; i++)
	{
		for (int j = 0; j < wVertexCount-1; j++)
		{
			meshData.Indices[index] = (i + 1) * wVertexCount + j;
			meshData.Indices[index + 1] = i * wVertexCount + j + 1;
			meshData.Indices[index + 2] = i * wVertexCount + j;
			meshData.Indices[index + 3] = (i + 1) * wVertexCount + j + 1;
			meshData.Indices[index + 4] = i * wVertexCount + j + 1;
			meshData.Indices[index + 5] = (i + 1) * wVertexCount + j;

			index += 6;
		}
	}


}
