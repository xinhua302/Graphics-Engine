#include "TreeSprite.h"
#include "../D3dUtility/LightManager.h"

TreeSprite::TreeSprite(int id) :Object(id)
{
    Init();
}

TreeSprite::~TreeSprite()
{
    Clear();
}

void TreeSprite::Init()
{
    float space = 80.0f;
    int count = 80;
    float frequency = 0.1f;
    float height = 50.0f;
    float treeHeight = 10.0f;
    GeometryGenerator::MeshData mesh;
    GeometryGenerator::CreateGrid(mesh, space, space, count, count, frequency, height);

    //顶点
    m_TreeSpriteCount = 30;
    std::vector<Vertex::TreePointSprite> v(m_TreeSpriteCount);
    for (int k = 0; k < m_TreeSpriteCount; k++)
    {
        DWORD i = rand() % count;
        DWORD j = rand() % count;

        float height = mesh.Vertices[i * count + j].Position.y + treeHeight/2.0f;
        float x = mesh.Vertices[i * count + j].Position.x;
        float z = mesh.Vertices[i * count + j].Position.z;
        v[k].Pos = XMFLOAT3(x, height, z);
        v[k].Size = XMFLOAT2(treeHeight, treeHeight);
    }

    D3D11_BUFFER_DESC vbd;
    vbd.Usage = D3D11_USAGE_IMMUTABLE;
    vbd.ByteWidth = sizeof(Vertex::TreePointSprite) * m_TreeSpriteCount;
    vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = 0;
    vbd.MiscFlags = 0;
    D3D11_SUBRESOURCE_DATA vinitData;
    vinitData.pSysMem = &v[0];
    HR(D3d->GetDevice()->CreateBuffer(&vbd, &vinitData, &m_pTreeSpriteVB));

    //纹理数组
    std::vector<std::wstring> treeFilenames;
    treeFilenames.push_back(L"Resource/Textures/tree0.dds");
    treeFilenames.push_back(L"Resource/Textures/tree1.dds");
    treeFilenames.push_back(L"Resource/Textures/tree2.dds");
    treeFilenames.push_back(L"Resource/Textures/tree3.dds");

    m_TreeSpriteMapSRV = D3dHelper::CreateTexture2DArraySRV(
        D3d->GetDevice(), D3d->GetContext(), treeFilenames, DXGI_FORMAT_R8G8B8A8_UNORM);

    //材质
    m_TreeSpriteMat.Ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
    m_TreeSpriteMat.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
    m_TreeSpriteMat.Specular = XMFLOAT4(0.2f, 0.2f, 0.2f, 16.0f);
}

void TreeSprite::Clear()
{
    ReleaseCOM(m_pTreeSpriteVB);
    ReleaseCOM(m_TreeSpriteMapSRV);
}

void TreeSprite::Update(float dt)
{

}

void TreeSprite::Render()
{
    Effects::TreeFX->SetDirLights(LightManager::Light);
    Effects::TreeFX->SetEysPosW(D3d->GetCamera().GetPosition());
    XMMATRIX view = D3d->GetCamera().View();
    XMMATRIX proj = D3d->GetCamera().Proj();
    XMMATRIX viewProj = view * proj;
    Effects::TreeFX->SetViewProj(viewProj);
    Effects::TreeFX->SetMaterial(m_TreeSpriteMat);
    Effects::TreeFX->SetTreeTextureMapArray(m_TreeSpriteMapSRV);

    D3d->GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
    D3d->GetContext()->IASetInputLayout(InputLayouts::TreePointSprite);
    UINT stride = sizeof(Vertex::TreePointSprite);
    UINT offset = 0;

    ID3DX11EffectTechnique* treeTech = Effects::TreeFX->Tech;
    D3DX11_TECHNIQUE_DESC techDesc;
    treeTech->GetDesc(&techDesc);
    for (UINT p = 0; p < techDesc.Passes; ++p)
    {
        D3d->GetContext()->IASetVertexBuffers(0, 1, &m_pTreeSpriteVB, &stride, &offset);

        float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

        D3d->GetContext()->OMSetBlendState(RenderStates::AlphaToCoverageBS, blendFactor, 0xffffffff);

        treeTech->GetPassByIndex(p)->Apply(0, D3d->GetContext());
        D3d->GetContext()->Draw(m_TreeSpriteCount, 0);

        D3d->GetContext()->OMSetBlendState(0, blendFactor, 0xffffffff);
    }
    D3d->GetContext()->IASetInputLayout(InputLayouts::Basic32);
    D3d->GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

