#include "Box.h"

Box::Box(int id) :Object(id)
{
    Init();
}

Box::~Box()
{
    Clear();
}

void Box::Init()
{
    Vertex::Basic32 v[24];

    float w2 = 1.0f * 1;
    float h2 = 1.0f * 1;
    float d2 = 1.0f * 1;

    v[0] = { { -w2, -h2, -d2 }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 1.0f } };
    v[1] = { { -w2, +h2, -d2 }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 0.0f } };
    v[2] = { { +w2, +h2, -d2 }, { 0.0f, 0.0f, -1.0f }, { 1.0f, 0.0f } };
    v[3] = { { +w2, -h2, -d2 }, { 0.0f, 0.0f, -1.0f }, { 1.0f, 1.0f } };

    // Fill in the back face vertex data.
    v[4] = { { -w2, -h2, +d2 }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f } };
    v[5] = { { +w2, -h2, +d2 }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f } };
    v[6] = { { +w2, +h2, +d2 }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } };
    v[7] = { { -w2, +h2, +d2 }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f } };

    // Fill in the top face vertex data.
    v[8] = { { -w2, +h2, -d2 }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f } };
    v[9] = { { -w2, +h2, +d2 }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f } };
    v[10] = { { +w2, +h2, +d2 }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 0.0f } };
    v[11] = { { +w2, +h2, -d2 }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 1.0f } };

    // Fill in the bottom face vertex data.
    v[12] = { { -w2, -h2, -d2 }, { 0.0f, -1.0f, 0.0f }, { 1.0f, 1.0f } };
    v[13] = { { +w2, -h2, -d2 }, { 0.0f, -1.0f, 0.0f }, { 0.0f, 1.0f } };
    v[14] = { { +w2, -h2, +d2 }, { 0.0f, -1.0f, 0.0f }, { 0.0f, 0.0f } };
    v[15] = { { -w2, -h2, +d2 }, { 0.0f, -1.0f, 0.0f }, { 1.0f, 0.0f } };

    // Fill in the left face vertex data.
    v[16] = { { -w2, -h2, +d2 }, { -1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f } };
    v[17] = { { -w2, +h2, +d2 }, { -1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } };
    v[18] = { { -w2, +h2, -d2 }, { -1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f } };
    v[19] = { { -w2, -h2, -d2 }, { -1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f } };

    // Fill in the right face vertex data.
    v[20] = { { +w2, -h2, -d2 }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f } };
    v[21] = { { +w2, +h2, -d2 }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } };
    v[22] = { { +w2, +h2, +d2 }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f } };
    v[23] = { { +w2, -h2, +d2 }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f } };


    D3D11_BUFFER_DESC vbd;
    vbd.Usage = D3D11_USAGE_IMMUTABLE;
    vbd.ByteWidth = sizeof(Vertex::Basic32) * 24;
    vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = 0;
    vbd.MiscFlags = 0;
    D3D11_SUBRESOURCE_DATA vinitData;
    vinitData.pSysMem = &v[0];
    HR(ObjectManager::Device->CreateBuffer(&vbd, &vinitData, &m_pBoxVB));


    UINT i[36];

    // Fill in the front face index data
    i[0] = 0; i[1] = 1; i[2] = 2;
    i[3] = 0; i[4] = 2; i[5] = 3;

    // Fill in the back face index data
    i[6] = 4; i[7] = 5; i[8] = 6;
    i[9] = 4; i[10] = 6; i[11] = 7;

    // Fill in the top face index data
    i[12] = 8; i[13] = 9; i[14] = 10;
    i[15] = 8; i[16] = 10; i[17] = 11;

    // Fill in the bottom face index data
    i[18] = 12; i[19] = 13; i[20] = 14;
    i[21] = 12; i[22] = 14; i[23] = 15;

    // Fill in the left face index data
    i[24] = 16; i[25] = 17; i[26] = 18;
    i[27] = 16; i[28] = 18; i[29] = 19;

    // Fill in the right face index data
    i[30] = 20; i[31] = 21; i[32] = 22;
    i[33] = 20; i[34] = 22; i[35] = 23;

    D3D11_BUFFER_DESC ibd;
    ibd.Usage = D3D11_USAGE_IMMUTABLE;
    ibd.ByteWidth = sizeof(UINT)* 36;
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibd.CPUAccessFlags = 0;
    ibd.MiscFlags = 0;
    D3D11_SUBRESOURCE_DATA iinitData;
    iinitData.pSysMem = &i[0];
    HR(ObjectManager::Device->CreateBuffer(&ibd, &iinitData, &m_pBoxIB));

    m_BoxCount = 36;

    //载入纹理贴图
    HR(D3DX11CreateShaderResourceViewFromFile(ObjectManager::Device,
        L"Resource/Textures/WoodCrate02.dds", 0, 0, &m_BoxMapSRV, 0));

    XMMATRIX I = XMMatrixIdentity();
    XMStoreFloat4x4(&m_TexTransform, I);

    XMStoreFloat4x4(&m_World, I);

    //相机变换
    XMVECTOR pos = XMVectorSet(-2.3f, 5.06f, -9.0f, 1.0f);
    XMVECTOR target = XMVectorZero();
    XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    XMMATRIX V = XMMatrixLookAtLH(pos, target, up);
    XMStoreFloat4x4(&m_View, V);

    //投影变换
    XMMATRIX P = XMMatrixPerspectiveFovLH(0.25f*3.1415926f, D3d->GetAspectRatio(), 1.0f, 1000.0f);
    XMStoreFloat4x4(&m_Proj, P);
}

void Box::Clear()
{
    ReleaseCOM(m_pBoxVB);
    ReleaseCOM(m_pBoxIB);
    ReleaseCOM(m_BoxMapSRV);
}

void Box::Update(float dt)
{

}

void Box::Render()
{
    D3DX11_TECHNIQUE_DESC techDesc;
    Effects::FX->Light3TexTech->GetDesc(&techDesc);
    for (UINT p = 0; p < techDesc.Passes; ++p)
    {
        //设置顶点和索引
        UINT stride = sizeof(Vertex::Basic32);
        UINT offset = 0;

        ObjectManager::Context->IASetVertexBuffers(0, 1, &m_pBoxVB, &stride, &offset);
        ObjectManager::Context->IASetIndexBuffer(m_pBoxIB, DXGI_FORMAT_R32_UINT, 0);

        //变换矩阵
        XMMATRIX view = XMLoadFloat4x4(&m_View);
        XMMATRIX proj = XMLoadFloat4x4(&m_Proj);
        XMMATRIX world = XMLoadFloat4x4(&m_World);
        XMMATRIX worldViewProj = world*view*proj;
        XMMATRIX worldInvTranspose = MathHelper::InverseTranspose(world);

        Effects::FX->SetWorld(world);
        Effects::FX->SetWorldInvTranspose(worldInvTranspose);
        Effects::FX->SetWorldViewProj(worldViewProj);
        Effects::FX->SetDiffuseMap(m_BoxMapSRV);
        Effects::FX->SetTexTransform(XMLoadFloat4x4(&m_TexTransform));
        Effects::FX->Light3TexTech->GetPassByIndex(p)->Apply(0, ObjectManager::Context);
        ObjectManager::Context->DrawIndexed(m_BoxCount, 0, 0);
    }
}
