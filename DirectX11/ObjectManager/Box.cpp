#include "Box.h"
#include "../D3dUtility/LightManager.h"

Box::Box(int id, int mode) :Object(id), m_Mode(mode)
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
    HR(D3d->GetDevice()->CreateBuffer(&vbd, &vinitData, &m_pBoxVB));


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
    HR(D3d->GetDevice()->CreateBuffer(&ibd, &iinitData, &m_pBoxIB));

    m_BoxCount = 36;

    //载入纹理贴图
    HR(D3DX11CreateShaderResourceViewFromFile(D3d->GetDevice(),
        L"Resource/Textures/WoodCrate01.dds", 0, 0, &m_BoxMapSRV, 0));

    XMMATRIX I = XMMatrixIdentity();
    XMStoreFloat4x4(&m_TexTransform, I);

    XMMATRIX world = XMMatrixTranslation(0.0f, 2.0f, 15.0f);
    XMStoreFloat4x4(&m_World, world);

    m_BoxMat.Ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
    m_BoxMat.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
    m_BoxMat.Specular = XMFLOAT4(0.4f, 0.4f, 0.4f, 16.0f);

	m_ShadowMat.Ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	m_ShadowMat.Diffuse = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.5f);
	m_ShadowMat.Specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 16.0f);
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
	float blendFactor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
    D3d->GetContext()->RSSetState(RenderStates::NoCullRS);
    D3DX11_TECHNIQUE_DESC techDesc;
    Effects::FX->Tech->GetDesc(&techDesc);
    for (UINT p = 0; p < techDesc.Passes; ++p)
    {
        //设置顶点和索引
        UINT stride = sizeof(Vertex::Basic32);
        UINT offset = 0;

        D3d->GetContext()->IASetVertexBuffers(0, 1, &m_pBoxVB, &stride, &offset);
        D3d->GetContext()->IASetIndexBuffer(m_pBoxIB, DXGI_FORMAT_R32_UINT, 0);

        XMFLOAT3 oldLightDirections[3];
        for (int i = 0; i < 3; ++i)
        {
            oldLightDirections[i] = LightManager::Light[i].Direction;
        }
        XMMATRIX world = XMLoadFloat4x4(&m_World);
        if (m_Mode == 1)
        {
            D3d->GetContext()->RSSetState(RenderStates::CullClockwiseRS);
            D3d->GetContext()->OMSetDepthStencilState(RenderStates::DrawReflectionDSS, 1);
            XMVECTOR mirrorPlane = XMVectorSet(0.0f, -1.0f, 0.0f, 0.0f);
            XMMATRIX R = XMMatrixReflect(mirrorPlane);
            world = XMLoadFloat4x4(&m_World) * R;

            //改变光朝向
            for (int i = 0; i < 3; ++i)
            {
                oldLightDirections[i] = LightManager::Light[i].Direction;

                XMVECTOR lightDir = XMLoadFloat3(&LightManager::Light[i].Direction);
                XMVECTOR reflectedLightDir = XMVector3TransformNormal(lightDir, R);
                XMStoreFloat3(&LightManager::Light[i].Direction, reflectedLightDir);
            }
			Effects::FX->SetMaterial(m_BoxMat);
        }
		else if (m_Mode == 2)
		{
			D3d->GetContext()->OMSetDepthStencilState(RenderStates::NoDoubleBlendDSS, 1);
			D3d->GetContext()->OMSetBlendState(RenderStates::TransparentBS, blendFactor, 0xffffffff);
			XMVECTOR shadowPlane = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
			XMVECTOR shadowLight = -XMLoadFloat3(&oldLightDirections[1]);
			XMMATRIX S = XMMatrixShadow(shadowPlane, shadowLight);
			XMMATRIX shadowOffsetY = XMMatrixTranslation(0.0f, 0.001f, 0.0f);
			world = world * S * shadowOffsetY;

			Effects::FX->SetMaterial(m_ShadowMat);
		}
		else
		{
			Effects::FX->SetMaterial(m_BoxMat);
		}
        //变换矩阵
        XMMATRIX view = D3d->GetCamera().View();
        XMMATRIX proj = D3d->GetCamera().Proj();


        XMMATRIX worldViewProj = world*view*proj;
        XMMATRIX worldInvTranspose = MathHelper::InverseTranspose(world);

        Effects::FX->SetWorld(world);
        Effects::FX->SetWorldInvTranspose(worldInvTranspose);
        Effects::FX->SetWorldViewProj(worldViewProj);
        Effects::FX->SetDiffuseMap(m_BoxMapSRV);
        Effects::FX->SetTexTransform(XMLoadFloat4x4(&m_TexTransform));
        Effects::FX->Tech->GetPassByIndex(p)->Apply(0, D3d->GetContext());
        
        D3d->GetContext()->DrawIndexed(m_BoxCount, 0, 0);

        //还原光的朝向
        for (int i = 0; i < 3; ++i)
        {
            LightManager::Light[i].Direction = oldLightDirections[i];
        }
    }
    D3d->GetContext()->RSSetState(RenderStates::DefaultRS);
    D3d->GetContext()->OMSetDepthStencilState(nullptr, 0);
	D3d->GetContext()->OMSetBlendState(nullptr, blendFactor, 0xffffffff);
}
