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
    m_Wave.Init(50, 50, 0.5f, 0.03f, 5.0f, 0.3f);
    //水
    int waterVBCount = m_Wave.GetVertexCount();
    std::vector<Vertex::Basic32> landVB(waterVBCount);
    for (int i = 0; i < waterVBCount; i++)
    {
        landVB[i].Pos = m_Wave[i];
        landVB[i].Normal = m_Wave.Normal(i);
        landVB[i].Tex.x = 0.5f + m_Wave[i].x / m_Wave.GetWidth();
        landVB[i].Tex.y = 0.5f - m_Wave[i].z / m_Wave.GetDepth();
    }
    D3D11_BUFFER_DESC vLandbd;
    vLandbd.Usage = D3D11_USAGE_DYNAMIC;
    vLandbd.ByteWidth = sizeof(Vertex::Basic32) * waterVBCount;
    vLandbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vLandbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    vLandbd.MiscFlags = 0;
    D3D11_SUBRESOURCE_DATA vLandInitData;
    vLandInitData.pSysMem = &landVB[0];
    HR(D3d->GetDevice()->CreateBuffer(&vLandbd, &vLandInitData, &m_pWaterVB));

    //索引
    int landIBCount = m_Wave.GetTriangleCount() * 3;
    std::vector<UINT> landIB(landIBCount);
    int index = 0;
    int m = m_Wave.GetVertexCountX();
    int n = m_Wave.GetVertexCountZ();
    for (int i = 0; i < n-1; i++)
    {
        for (int j = 0; j < m-1; j++)
        {
            landIB[index++] =  i * m + j;
            landIB[index++] = i * m + j+1;
            landIB[index++] = (i + 1) * m + j;
            landIB[index++] =  (i + 1) * m + j;
            landIB[index++] = i * m + j + 1;
            landIB[index++] = (i + 1) * m + j + 1;
        }
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
        L"Resource/Textures/water2.dds", 0, 0, &m_WaterMapSRV, 0));

    XMMATRIX world = XMMatrixTranslation(0.0f, 0.0f, 0.0f);
    XMStoreFloat4x4(&m_World, world);

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
    m_WaterOffset.y -= dt * 0.01f;
    m_WaterOffset.x -= dt * 0.02f;
    XMMATRIX waterOffset = XMMatrixTranslation(m_WaterOffset.x, m_WaterOffset.y, 0.0f);
    XMMATRIX waterScale = XMMatrixScaling(5.0f, 5.0f, 0.0f);
    XMStoreFloat4x4(&m_TexTransform, waterOffset * waterScale);

	if (m_Mode == 0)
	{
		return;
	}

    //随机
    static float t = 0.0f;
    t += dt;
    if (t > 5.0f)
    {
        DWORD i = 10 + rand() % (m_Wave.GetVertexCountX() - 30);
        DWORD j = 10 + rand() % (m_Wave.GetVertexCountZ() - 30);

        float r = MathHelper::RandF(0.7f, 0.8f);
        m_Wave.Disturb(i, j, r);
        t = 0.0f;
    }
    

    m_Wave.Update(dt);
    D3D11_MAPPED_SUBRESOURCE mappedData;
    HR(D3d->GetContext()->Map(m_pWaterVB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
    Vertex::Basic32* v = reinterpret_cast<Vertex::Basic32*>(mappedData.pData);
    for (UINT i = 0; i < m_Wave.GetVertexCount(); i++)
    {
        v[i].Pos = m_Wave[i];
        v[i].Normal = m_Wave.Normal(i);
        v[i].Tex.x = 0.5f + m_Wave[i].x / m_Wave.GetWidth();
        v[i].Tex.y = 0.5f - m_Wave[i].z / m_Wave.GetDepth();
    }

    D3d->GetContext()->Unmap(m_pWaterVB, 0);
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
        XMMATRIX view = D3d->GetCamera().View();
        XMMATRIX proj = D3d->GetCamera().Proj();
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
            D3d->GetContext()->OMSetDepthStencilState(RenderStates::NoDoubleBlendDSS, 1);
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
