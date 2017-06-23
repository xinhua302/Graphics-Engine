#include "D3dApp.h"

#define D3d D3dApp::GetInstance()

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

bool D3dApp::Init(HWND hwnd)
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new D3dApp(hwnd);
	}

	if (m_pInstance == nullptr)
	{
		return false;
	}

	if (!D3d->InitDevice())
	{
		return false;
	}

	return true;
}

bool D3dApp::Update(float dt)
{

	Render();
	return true;
}

bool D3dApp::Render()
{
	return true;
}

void D3dApp::Clear()
{

}

D3dApp* D3dApp::GetInstance()
{
	return m_pInstance;
}

bool D3dApp::InitDevice()
{
	UINT createDeviceFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)  
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL featureLevel;
	HRESULT hr = D3D11CreateDevice(
		0,                 // default adapter
		m_DriverType,
		0,                 // no software device
		createDeviceFlags,
		0, 0,              // default feature level array
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
	sd.BufferDesc.Width = 800;
	sd.BufferDesc.Height = 600;
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

D3dApp * D3dApp::m_pInstance = nullptr;
