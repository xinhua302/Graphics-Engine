#include "Land.h"

Land::Land(int id) :Object(id)
{
    Init();
}

Land::~Land()
{
    Clear();
}

void Land::Init()
{
    //地形
    GeometryGenerator::MeshData mesh;
    GeometryGenerator::CreateGrid(mesh, 10.0f, 10.0f, 10, 10);
    //地形顶点
    int landVBCount = mesh.Vertices.size();
    std::vector<Vertex::Basic32> landVB(landVBCount);
    for (int i = 0; i < landVBCount; i++)
    {
        landVB[i].Pos = mesh.Vertices[i].Position;
        landVB[i].Normal = mesh.Vertices[i].Normal;
        landVB[i].Tex = mesh.Vertices[i].TexC;
    }
    D3D11_BUFFER_DESC vLandbd;
    vLandbd.Usage = D3D11_USAGE_IMMUTABLE;
    vLandbd.ByteWidth = sizeof(Vertex::Basic32) * landVBCount;
    vLandbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vLandbd.CPUAccessFlags = 0;
    vLandbd.MiscFlags = 0;
    D3D11_SUBRESOURCE_DATA vLandInitData;
    vLandInitData.pSysMem = &landVB[0];
    HR(D3d->GetDevice()->CreateBuffer(&vLandbd, &vLandInitData, &m_pLandVB));

    //索引
    int landIBCount = mesh.Indices.size();
    std::vector<UINT> landIB(landIBCount);
    for (int i = 0; i < landIBCount; i++)
    {
        landIB[i] = mesh.Indices[i];
    }

    D3D11_BUFFER_DESC iLandbd;
    iLandbd.Usage = D3D11_USAGE_IMMUTABLE;
    iLandbd.ByteWidth = sizeof(UINT)* landIBCount;
    iLandbd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    iLandbd.CPUAccessFlags = 0;
    iLandbd.MiscFlags = 0;
    D3D11_SUBRESOURCE_DATA iLandInitData;
    iLandInitData.pSysMem = &landIB[0];
    HR(D3d->GetDevice()->CreateBuffer(&iLandbd, &iLandInitData, &m_pLandIB));
    //顶点总数
    m_LandCount = landIBCount;

    //载入草地的纹理贴图
    HR(D3DX11CreateShaderResourceViewFromFile(D3d->GetDevice(),
        L"Resource/Textures/grass.dds", 0, 0, &m_LandeMapSRV, 0));

    XMMATRIX grassTexScale = XMMatrixScaling(10.0f, 10.0f, 0.0f);
    XMStoreFloat4x4(&m_TexTransform, grassTexScale);

    XMMATRIX world = XMMatrixTranslation(0.0f, -1.0f, 0.0f);
    XMStoreFloat4x4(&m_World, world);

    //相机变换
    XMVECTOR pos = XMVectorSet(-2.3f, 5.06f, -9.0f, 1.0f);
    XMVECTOR target = XMVectorZero();
    XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    XMMATRIX V = XMMatrixLookAtLH(pos, target, up);
    XMStoreFloat4x4(&m_View, V);

    //投影变换
    XMMATRIX P = XMMatrixPerspectiveFovLH(0.25f*3.1415926f, D3d->GetAspectRatio(), 1.0f, 1000.0f);
    XMStoreFloat4x4(&m_Proj, P);

    m_LandMat.Ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
    m_LandMat.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
    m_LandMat.Specular = XMFLOAT4(0.2f, 0.2f, 0.2f, 16.0f);
}

void Land::Clear()
{
    ReleaseCOM(m_pLandVB);
    ReleaseCOM(m_pLandIB);
    ReleaseCOM(m_LandeMapSRV);
}

void Land::Update(float dt)
{

}

void Land::Render()
{   
    D3DX11_TECHNIQUE_DESC techDesc;
    Effects::FX->Light3TexTech->GetDesc(&techDesc);
    for (UINT p = 0; p < techDesc.Passes; ++p)
    {
        //设置顶点和索引
        UINT stride = sizeof(Vertex::Basic32);
        UINT offset = 0;

        D3d->GetContext()->IASetVertexBuffers(0, 1, &m_pLandVB, &stride, &offset);
        D3d->GetContext()->IASetIndexBuffer(m_pLandIB, DXGI_FORMAT_R32_UINT, 0);

        //变换矩阵
        XMMATRIX view = XMLoadFloat4x4(&m_View);
        XMMATRIX proj = XMLoadFloat4x4(&m_Proj);
        XMMATRIX world = XMLoadFloat4x4(&m_World);
        XMMATRIX worldViewProj = world*view*proj;
        XMMATRIX worldInvTranspose = MathHelper::InverseTranspose(world);

        Effects::FX->SetWorld(world);
        Effects::FX->SetWorldInvTranspose(worldInvTranspose);
        Effects::FX->SetWorldViewProj(worldViewProj);
        Effects::FX->SetDiffuseMap(m_LandeMapSRV);
        Effects::FX->SetTexTransform(XMLoadFloat4x4(&m_TexTransform));
        Effects::FX->SetMaterial(m_LandMat);
        Effects::FX->Light3TexTech->GetPassByIndex(p)->Apply(0, D3d->GetContext());
        D3d->GetContext()->DrawIndexed(m_LandCount, 0, 0);
    }
}
