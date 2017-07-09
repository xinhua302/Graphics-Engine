#include "Water.h"

Water::Water(int id,int mode) :Object(id) , m_Mode(mode)
{
    Init();
}

Water::~Water()
{
    Clear();
}

void Water::Init()
{
    //水
    GeometryGenerator::MeshData mesh;
    GeometryGenerator::CreateGrid(mesh, 10.0f, 10.0f, 10, 10);
    //水
    int waterVBCount = mesh.Vertices.size();
    std::vector<Vertex::Basic32> landVB(waterVBCount);
    for (int i = 0; i < waterVBCount; i++)
    {
        landVB[i].Pos = mesh.Vertices[i].Position;
        landVB[i].Normal = mesh.Vertices[i].Normal;
        landVB[i].Tex = mesh.Vertices[i].TexC;
    }
    D3D11_BUFFER_DESC vLandbd;
    vLandbd.Usage = D3D11_USAGE_IMMUTABLE;
    vLandbd.ByteWidth = sizeof(Vertex::Basic32) * waterVBCount;
    vLandbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vLandbd.CPUAccessFlags = 0;
    vLandbd.MiscFlags = 0;
    D3D11_SUBRESOURCE_DATA vLandInitData;
    vLandInitData.pSysMem = &landVB[0];
    HR(D3d->GetDevice()->CreateBuffer(&vLandbd, &vLandInitData, &m_pWaterVB));

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
    HR(D3d->GetDevice()->CreateBuffer(&iLandbd, &iLandInitData, &m_pWaterIB));
    //顶点总数
    m_WaterCount = landIBCount;

    //载入草地的纹理贴图
    HR(D3DX11CreateShaderResourceViewFromFile(D3d->GetDevice(),
        L"Resource/Textures/water1.dds", 0, 0, &m_WaterMapSRV, 0));

    XMMATRIX world = XMMatrixTranslation(0.0f, 0.0f, 0.0f);
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

    m_WaterMat.Ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
    m_WaterMat.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f);
    m_WaterMat.Specular = XMFLOAT4(0.8f, 0.8f, 0.8f, 32.0f);
}

void Water::Clear()
{
    ReleaseCOM(m_pWaterVB);
    ReleaseCOM(m_pWaterIB);
    ReleaseCOM(m_WaterMapSRV);
}

void Water::Update(float dt)
{
    m_WaterOffset.y += dt * 0.05f;
    m_WaterOffset.x += dt * 0.1f;
    XMMATRIX waterOffset = XMMatrixTranslation(m_WaterOffset.x, m_WaterOffset.y, 0.0f);
    XMMATRIX waterScale = XMMatrixScaling(1.0f, 1.0f, 0.0f);
    XMStoreFloat4x4(&m_TexTransform, waterOffset * waterScale);
}

void Water::Render()
{
    D3DX11_TECHNIQUE_DESC techDesc;
    Effects::FX->Tech->GetDesc(&techDesc);
    for (UINT p = 0; p < techDesc.Passes; ++p)
    {
        //设置顶点和索引
        UINT stride = sizeof(Vertex::Basic32);
        UINT offset = 0;

        D3d->GetContext()->IASetVertexBuffers(0, 1, &m_pWaterVB, &stride, &offset);
        D3d->GetContext()->IASetIndexBuffer(m_pWaterIB, DXGI_FORMAT_R32_UINT, 0);

        //变换矩阵
        XMMATRIX view = XMLoadFloat4x4(&m_View);
        XMMATRIX proj = XMLoadFloat4x4(&m_Proj);
        XMMATRIX world = XMLoadFloat4x4(&m_World);
        XMMATRIX worldViewProj = world*view*proj;
        XMMATRIX worldInvTranspose = MathHelper::InverseTranspose(world);

        Effects::FX->SetWorld(world);
        Effects::FX->SetWorldInvTranspose(worldInvTranspose);
        Effects::FX->SetWorldViewProj(worldViewProj);
        Effects::FX->SetDiffuseMap(m_WaterMapSRV);
        Effects::FX->SetTexTransform(XMLoadFloat4x4(&m_TexTransform));
        Effects::FX->SetMaterial(m_WaterMat);
        float blendFactor[] = { 0.0f, 0.0f, 0.0f, 0.0f };

        if (m_Mode == 1)
        {
            D3d->GetContext()->OMSetBlendState(RenderStates::TransparentBS, blendFactor, 0xffffffff);
            D3d->GetContext()->OMSetDepthStencilState(nullptr, 0);
        }
        else
        {
            D3d->GetContext()->OMSetBlendState(RenderStates::NoRenderTargetWritesBS, blendFactor, 0xffffffff);
            D3d->GetContext()->OMSetDepthStencilState(RenderStates::MarkMirrorDSS, 1);
        }
        

        Effects::FX->Tech->GetPassByIndex(p)->Apply(0, D3d->GetContext());
        D3d->GetContext()->DrawIndexed(m_WaterCount, 0, 0);
        D3d->GetContext()->OMSetBlendState(0, blendFactor, 0xffffffff);
        D3d->GetContext()->OMSetDepthStencilState(nullptr, 0);
    }
}
