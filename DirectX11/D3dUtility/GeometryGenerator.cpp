#include "GeometryGenerator.h"

void GeometryGenerator::CreateGrid(MeshData &meshData, float width, float depth, UINT wVertexCount, UINT dVertexCount, float frequency, float height)
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

    for (UINT i = 0; i < dVertexCount; i++)
	{
		float z = offsetZ + i * dz;
        for (UINT j = 0; j < wVertexCount; j++)
		{
			meshData.Vertices[i * wVertexCount + j].Position = XMFLOAT3(offsetX + j * dw, 0.0f, z);
            meshData.Vertices[i * wVertexCount + j].Position.y = MathHelper::PerlinNoise(j * dw * frequency, (z - offsetZ) *frequency) * height;
			meshData.Vertices[i * wVertexCount + j].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
			meshData.Vertices[i * wVertexCount + j].TangentU = XMFLOAT3(1.0f, 0.0f, 0.0f);
			meshData.Vertices[i * wVertexCount + j].TexC = XMFLOAT2(i * dv, j * du);
		}
	}

	meshData.Indices.resize(triangleCount * 3);

	int index = 0;
	for (UINT i = 0; i < dVertexCount - 1; i++)
	{
        for (UINT j = 0; j < wVertexCount - 1; j++)
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

    for (UINT i = 0; i < triangleCount; i++)
    {
        XMFLOAT3 p1 = meshData.Vertices[meshData.Indices[i * 3]].Position;
        XMFLOAT3 p2 = meshData.Vertices[meshData.Indices[i * 3 + 1]].Position;
        XMFLOAT3 p3 = meshData.Vertices[meshData.Indices[i * 3 + 2]].Position;
        XMVECTOR v1 = XMLoadFloat3(&p1);
        XMVECTOR v2 = XMLoadFloat3(&p2);
        XMVECTOR v3 = XMLoadFloat3(&p3);
        XMVECTOR v1_v2 = v1 - v2;
        XMVECTOR v2_v3 = v2 - v3;
        XMVECTOR normal = XMVector3Cross(v1_v2, v2_v3);
        
        XMFLOAT3 n1 = meshData.Vertices[meshData.Indices[i * 3]].Normal;
        XMFLOAT3 n2 = meshData.Vertices[meshData.Indices[i * 3+1]].Normal;
        XMFLOAT3 n3 = meshData.Vertices[meshData.Indices[i * 3+2]].Normal;
        XMVECTOR v4 = XMLoadFloat3(&n1);
        XMVECTOR v5 = XMLoadFloat3(&n2);
        XMVECTOR v6 = XMLoadFloat3(&n3);

        v4 += normal;
        v5 += normal;
        v6 += normal;

        XMStoreFloat3(&meshData.Vertices[meshData.Indices[i * 3]].Normal, v4);
        XMStoreFloat3(&meshData.Vertices[meshData.Indices[i * 3+1]].Normal, v5);
        XMStoreFloat3(&meshData.Vertices[meshData.Indices[i * 3+2]].Normal, v6);
    }

    for (UINT i = 0; i < meshData.Vertices.size(); i++)
    {
        XMVECTOR normal = XMLoadFloat3(&meshData.Vertices[i].Normal);
        XMStoreFloat3(&meshData.Vertices[i].Normal, XMVector3Normalize(normal));
    }
}
