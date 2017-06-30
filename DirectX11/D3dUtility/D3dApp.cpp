#include "D3dApp.h"
#include "Effect.h"
#include "Vertex.h"
#include "MathHelper.h"
#include "GeometryGenerator.h"
#include <cassert>

bool D3dApp::SetRasterizerState()
{
    D3D11_RASTERIZER_DESC wireframeDesc;
    ZeroMemory(&wireframeDesc, sizeof(D3D11_RASTERIZER_DESC));
    wireframeDesc.FillMode = D3D11_FILL_SOLID;
    wireframeDesc.CullMode = D3D11_CULL_BACK;
    wireframeDesc.FrontCounterClockwise = false;
    wireframeDesc.DepthClipEnable = true;

    HR(m_pd3dDevice->CreateRasterizerState(&wireframeDesc, &m_WireframeRS));
    return false;
}

D3dApp * D3dApp::m_pInstance = nullptr;

D3dApp::D3dApp(HWND hwnd)
:				m_DriverType(D3D_DRIVER_TYPE_HARDWARE),
				m_Hwnd(hwnd)
{
	ZeroMemory(&m_ScreenViewport, sizeof(D3D11_VIEWPORT));
}

D3dApp::~D3dApp()
{
	Clear();
}

bool D3dApp::Init(HWND hwnd, int width, int height)
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new D3dApp(hwnd);
	}

	if (m_pInstance == nullptr)
	{
		return false;
	}

	if (!D3d->InitDevice(width, height))
	{
		return false;
	}

    if (!D3d->Init())
    {
        return false;
    }

	return true;
}

bool D3dApp::Run(float dt)
{

	D3d->Update(dt);
	D3d->Render();
	return true;
}

D3dApp* D3dApp::GetInstance()
{
	return m_pInstance;
}

void D3dApp::OnResize()
{
    assert(m_pImmediateContext);
    assert(m_pd3dDevice);
    assert(m_pSwapChain);

    ReleaseCOM(m_pRenderTargetView);
    ReleaseCOM(m_pDepthStencilView);
    ReleaseCOM(m_pDepthStencil);

    //创建渲染目标视图
    HR(m_pSwapChain->ResizeBuffers(1, m_Width, m_Height, DXGI_FORMAT_R8G8B8A8_UNORM, 0));
    ID3D11Texture2D* backBuffer;
    HR(m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer)));
    HR(m_pd3dDevice->CreateRenderTargetView(backBuffer, 0, &m_pRenderTargetView));
    ReleaseCOM(backBuffer);

    //深度模板视图
    D3D11_TEXTURE2D_DESC depthStencilDesc;

    depthStencilDesc.Width = m_Width;
    depthStencilDesc.Height = m_Height;
    depthStencilDesc.MipLevels = 1;
    depthStencilDesc.ArraySize = 1;
    depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilDesc.SampleDesc.Count = 1;
    depthStencilDesc.SampleDesc.Quality = 0;
    depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
    depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthStencilDesc.CPUAccessFlags = 0;
    depthStencilDesc.MiscFlags = 0;

    HR(m_pd3dDevice->CreateTexture2D(&depthStencilDesc, 0, &m_pDepthStencil));
    HR(m_pd3dDevice->CreateDepthStencilView(m_pDepthStencil, 0, &m_pDepthStencilView));

    m_pImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

    //设置视口
    m_ScreenViewport.TopLeftX = 0;
    m_ScreenViewport.TopLeftY = 0;
    m_ScreenViewport.Width = static_cast<float>(m_Width);
    m_ScreenViewport.Height = static_cast<float>(m_Height);
    m_ScreenViewport.MinDepth = 0.0f;
    m_ScreenViewport.MaxDepth = 1.0f;

    m_pImmediateContext->RSSetViewports(1, &m_ScreenViewport);
}

bool D3dApp::InitDevice(int width, int height)
{
	m_Width = width;
	m_Height = height;

	UINT createDeviceFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)  
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL featureLevel;
	HRESULT hr = D3D11CreateDevice(
		0,                 
		m_DriverType,
		0,                 
		createDeviceFlags,
		0, 0,              
		D3D11_SDK_VERSION,
		&m_pd3dDevice,
		&featureLevel,
		&m_pImmediateContext);

	if (FAILED(hr))
	{
		MessageBox(0, L"D3D11CreateDevice Failed.", 0, 0);
		return false;
	}

	if (featureLevel != D3D_FEATURE_LEVEL_11_0)
	{
		MessageBox(0, L"Direct3D Feature Level 11 unsupported.", 0, 0);
		return false;
	}

	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width = m_Width;
	sd.BufferDesc.Height = m_Height;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;	
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = m_Hwnd;
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	IDXGIDevice* dxgiDevice = 0;
	HR(m_pd3dDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice));

	IDXGIAdapter* dxgiAdapter = 0;
	HR(dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter));

	IDXGIFactory* dxgiFactory = 0;
	HR(dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory));

	HR(dxgiFactory->CreateSwapChain(m_pd3dDevice, &sd, &m_pSwapChain));

	ReleaseCOM(dxgiDevice);
	ReleaseCOM(dxgiAdapter);
	ReleaseCOM(dxgiFactory);

	return true;
}

bool D3dApp::Init()
{
    OnResize();

    //世界变换
    XMMATRIX T = XMMatrixTranslation(0.0f, 0.0f, 0.0f);
    XMStoreFloat4x4(&m_BoxWorld, T);

    //相机变换
    XMVECTOR pos = XMVectorSet(-2.3f, 5.06f, -9.0f, 1.0f);
    XMVECTOR target = XMVectorZero();
    XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    XMMATRIX V = XMMatrixLookAtLH(pos, target, up);
    XMStoreFloat4x4(&m_View, V);

    //投影变换
    XMMATRIX P = XMMatrixPerspectiveFovLH(0.25f*3.1415926, m_Width / m_Height, 1.0f, 1000.0f);
    XMStoreFloat4x4(&m_Proj, P);

    //加载模型
    LoadModel();

    //初始化Shader
    Effects::InitAll(m_pd3dDevice);

    //初始化输入布局
    InputLayouts::InitAll(m_pd3dDevice);

    //设置光栅化状态
    SetRasterizerState();
    m_pImmediateContext->RSSetState(m_WireframeRS);

    //载入纹理贴图
    HR(D3DX11CreateShaderResourceViewFromFile(m_pd3dDevice,
        L"Resource/Textures/WoodCrate02.dds", 0, 0, &m_DiffuseMapSRV, 0));

	//载入草地的纹理贴图
	HR(D3DX11CreateShaderResourceViewFromFile(m_pd3dDevice,
		L"Resource/Textures/grass.dds", 0, 0, &m_LandeMapSRV, 0));

    return true;
}

bool D3dApp::Update(float dt)
{
	return true;
}

bool D3dApp::Render()
{
    //请屏
    float color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, color);
    m_pImmediateContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    //设置输入布局和拓扑结构
    m_pImmediateContext->IASetInputLayout(InputLayouts::Basic32);
    m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    //材质
    Material Mat;
    Mat.Ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
    Mat.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
    Mat.Specular = XMFLOAT4(0.6f, 0.6f, 0.6f, 16.0f);

    //观察的位置
    XMFLOAT3 mEyePosW = XMFLOAT3(-2.3f, 5.06f, -19.0f);

    //方向光源
    DirectionalLight mDirLights[3];
    mDirLights[0].Ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
    mDirLights[0].Diffuse = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
    mDirLights[0].Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
    mDirLights[0].Direction = XMFLOAT3(0.57735f, -0.57735f, 0.57735f);

    mDirLights[1].Ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
    mDirLights[1].Diffuse = XMFLOAT4(0.20f, 0.20f, 0.20f, 1.0f);
    mDirLights[1].Specular = XMFLOAT4(0.25f, 0.25f, 0.25f, 1.0f);
    mDirLights[1].Direction = XMFLOAT3(-0.57735f, -0.57735f, 0.57735f);

    mDirLights[2].Ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
    mDirLights[2].Diffuse = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
    mDirLights[2].Specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
    mDirLights[2].Direction = XMFLOAT3(0.0f, -0.707f, -0.707f);

    Effects::FX->SetDirLights(mDirLights);
    Effects::FX->SetEyePosW(mEyePosW);

    D3DX11_TECHNIQUE_DESC techDesc;
    Effects::FX->Light3TexTech->GetDesc(&techDesc);
    for (UINT p = 0; p < techDesc.Passes; ++p)
    {
		//设置顶点和索引
		UINT stride = sizeof(Vertex::Basic32);
		UINT offset = 0;
		m_pImmediateContext->IASetVertexBuffers(0, 1, &m_VB, &stride, &offset);
		m_pImmediateContext->IASetIndexBuffer(m_IB, DXGI_FORMAT_R32_UINT, 0);

		//变换矩阵
		XMFLOAT4X4 mTexTransform;
		XMMATRIX I = XMMatrixIdentity();
		XMStoreFloat4x4(&mTexTransform, I);
		XMMATRIX view = XMLoadFloat4x4(&m_View);
		XMMATRIX proj = XMLoadFloat4x4(&m_Proj);
		XMMATRIX world = XMLoadFloat4x4(&m_BoxWorld);
		XMMATRIX worldViewProj = world*view*proj;

        world = XMLoadFloat4x4(&m_BoxWorld);
        XMMATRIX worldInvTranspose = MathHelper::InverseTranspose(world);
        worldViewProj = world*view*proj;
        Effects::FX->SetWorld(world);
        Effects::FX->SetWorldInvTranspose(worldInvTranspose);
        Effects::FX->SetWorldViewProj(worldViewProj);
        Effects::FX->SetTexTransform(XMLoadFloat4x4(&mTexTransform));
        Effects::FX->SetMaterial(Mat);
        Effects::FX->SetDiffuseMap(m_DiffuseMapSRV);
        //应用
        Effects::FX->Light3TexTech->GetPassByIndex(p)->Apply(0, m_pImmediateContext);
        m_pImmediateContext->DrawIndexed(36, 0, 0);

		//设置顶点和索引
		m_pImmediateContext->IASetVertexBuffers(0, 1, &m_pLandVB, &stride, &offset);
		m_pImmediateContext->IASetIndexBuffer(m_pLandIB, DXGI_FORMAT_R32_UINT, 0);

		XMMATRIX grassTexScale = XMMatrixScaling(10.0f, 10.0f, 0.0f);
		XMStoreFloat4x4(&mTexTransform, grassTexScale);
		//变换矩阵
		view = XMLoadFloat4x4(&m_View);
		proj = XMLoadFloat4x4(&m_Proj);
		world = XMMatrixTranslation(0.0f, -1.0f, 0.0f);
		worldViewProj = world*view*proj;
		worldInvTranspose = MathHelper::InverseTranspose(world);

		Effects::FX->SetWorld(world);
		Effects::FX->SetWorldInvTranspose(worldInvTranspose);
		Effects::FX->SetWorldViewProj(worldViewProj);
		Effects::FX->SetDiffuseMap(m_LandeMapSRV);
		Effects::FX->SetTexTransform(XMLoadFloat4x4(&mTexTransform));
		Effects::FX->Light3TexTech->GetPassByIndex(p)->Apply(0, m_pImmediateContext);
		m_pImmediateContext->DrawIndexed(m_LandCount, 0, 0);

    }

    HR(m_pSwapChain->Present(0, 0));
	return true;
}

void D3dApp::Clear()
{
    ReleaseCOM(m_pRenderTargetView);
    ReleaseCOM(m_pDepthStencilView);
    ReleaseCOM(m_pSwapChain);
    ReleaseCOM(m_pDepthStencil);
    ReleaseCOM(m_WireframeRS);
    ReleaseCOM(m_DiffuseMapSRV);
    ReleaseCOM(m_VB);
    ReleaseCOM(m_IB);

    if (m_pImmediateContext)
        m_pImmediateContext->ClearState();

    ReleaseCOM(m_pImmediateContext);
    ReleaseCOM(m_pd3dDevice);

    Effects::DestroyAll();
    InputLayouts::DestroyAll();
}

bool D3dApp::LoadModel()
{
    Vertex::Basic32 v[24];

    float w2 = 1.0f*1;
    float h2 = 1.0f*1;
    float d2 = 1.0f*1;

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
    vbd.ByteWidth = sizeof(Vertex::Basic32)* 24;
    vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = 0;
    vbd.MiscFlags = 0;
    D3D11_SUBRESOURCE_DATA vinitData;
    vinitData.pSysMem = &v[0];
    HR(m_pd3dDevice->CreateBuffer(&vbd, &vinitData, &m_VB));


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
    HR(m_pd3dDevice->CreateBuffer(&ibd, &iinitData, &m_IB));


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
	HR(m_pd3dDevice->CreateBuffer(&vLandbd, &vLandInitData, &m_pLandVB));
	
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
	HR(m_pd3dDevice->CreateBuffer(&iLandbd, &iLandInitData, &m_pLandIB));
	//顶点总数
	m_LandCount = landIBCount;

    return true;
}
