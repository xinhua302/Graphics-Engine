#include "D3dApp.h"
#include "Effect.h"
#include "Vertex.h"
#include "MathHelper.h"
#include "GeometryGenerator.h"
#include "../ObjectManager/BaseObject.h"
#include "LightManager.h"
#include <cassert>

D3dApp * D3dApp::m_pInstance = nullptr;

D3dApp* D3dApp::GetInstance()
{
    return m_pInstance;
}

D3dApp::D3dApp(HWND hwnd)
:				m_DriverType(D3D_DRIVER_TYPE_HARDWARE),
				m_Hwnd(hwnd),
                m_Enable4xMsaa(true),
                m_4xMsaaQuality(0)
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
    if (m_Enable4xMsaa)
    {
        depthStencilDesc.SampleDesc.Count = 4;
        depthStencilDesc.SampleDesc.Quality = m_4xMsaaQuality - 1;
    }
    else
    {
        depthStencilDesc.SampleDesc.Count = 1;
        depthStencilDesc.SampleDesc.Quality = 0;
    }
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

float D3dApp::GetAspectRatio()
{
    return static_cast<float>(m_Width) / static_cast<float>(m_Height);
}

ID3D11Device* D3dApp::GetDevice() const
{
    return m_pd3dDevice;
}

ID3D11DeviceContext* D3dApp::GetContext() const
{
    return m_pImmediateContext;
}

IDXGISwapChain* D3dApp::GeSwapChain() const
{
    return m_pSwapChain;
}

Camera D3dApp::GetCamera() const
{
    return m_Camera; 
}

void D3dApp::OnMouseDown(WPARAM btnState, int x, int y)
{
    m_LastMousePos.x = x;
    m_LastMousePos.y = y;
    SetCapture(m_Hwnd);
}

void D3dApp::OnMouseUp(WPARAM btnState, int x, int y)
{
    ReleaseCapture();
}

void D3dApp::OnMouseMove(WPARAM btnState, int x, int y)
{
    if ((btnState & MK_LBUTTON) != 0)
    {
        float dx = XMConvertToRadians(0.25f*static_cast<float>(x - m_LastMousePos.x));
        float dy = XMConvertToRadians(0.25f*static_cast<float>(y - m_LastMousePos.y));

        m_Camera.Pitch(dy);
        m_Camera.RotateY(dx);
    }

    m_LastMousePos.x = x;
    m_LastMousePos.y = y;
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

    HR(m_pd3dDevice->CheckMultisampleQualityLevels(
        DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m_4xMsaaQuality));
    assert(m_4xMsaaQuality > 0);

	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width = m_Width;
	sd.BufferDesc.Height = m_Height;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    //是否开启多重采样 
    if (m_Enable4xMsaa)
    {
        sd.SampleDesc.Count = 4;
        sd.SampleDesc.Quality = m_4xMsaaQuality - 1;
    }
    else
    {
        sd.SampleDesc.Count = 1;
        sd.SampleDesc.Quality = 0;
    }
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

    //设置相机
    m_Camera.SetPosition(5.0f, 3.0f, -19.0f);
    m_Camera.SetLens(0.25f*MathHelper::Pi, GetAspectRatio(), 1.0f, 1000.0f);

    //创建对象管理器
    ObjectManager::InitAll();

    //初始化Shader
    Effects::InitAll(m_pd3dDevice);

    //初始化输入布局
    InputLayouts::InitAll(m_pd3dDevice);

    //初始化光栅化状态
    RenderStates::InitAll(m_pd3dDevice);

    //初始化光照管理器
    LightManager::InitAll();

    ObjectManager::CreateObject("Box");
    ObjectManager::CreateObject("Land");
    ObjectManager::CreateObject("MarkWater");
    ObjectManager::CreateObject("BoxReflection");
    ObjectManager::CreateObject("Water");
	ObjectManager::CreateObject("BoxShadow");

    LightManager::CreateLight();
    return true;
}

bool D3dApp::Update(float dt)
{
    if (GetAsyncKeyState('W') & 0x8000)
        m_Camera.Walk(10.0f*dt);

    if (GetAsyncKeyState('S') & 0x8000)
        m_Camera.Walk(-10.0f*dt);

    if (GetAsyncKeyState('A') & 0x8000)
        m_Camera.Strafe(-10.0f*dt);

    if (GetAsyncKeyState('D') & 0x8000)
        m_Camera.Strafe(10.0f*dt);

    ObjectManager::Update(dt);
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

    //更新相机
    m_Camera.UpdateViewMatrix();

    //设置光照
    LightManager::Apply();

	//设置雾效
	Effects::FX->SetFogColor(Colors::Silver);
	Effects::FX->SetFogStart(15.0f);
	Effects::FX->SetFogRange(100.0f);

    //渲染对象
    ObjectManager::Render();

    HR(m_pSwapChain->Present(0, 0));
	return true;
}

void D3dApp::Clear()
{
    ReleaseCOM(m_pRenderTargetView);
    ReleaseCOM(m_pDepthStencilView);
    ReleaseCOM(m_pSwapChain);
    ReleaseCOM(m_pDepthStencil);

    if (m_pImmediateContext)
        m_pImmediateContext->ClearState();

    ReleaseCOM(m_pImmediateContext);
    ReleaseCOM(m_pd3dDevice);

    ObjectManager::DestroyAll();
    Effects::DestroyAll();
    InputLayouts::DestroyAll();
    RenderStates::DestroyAll();
    LightManager::DestroyAll();
}
