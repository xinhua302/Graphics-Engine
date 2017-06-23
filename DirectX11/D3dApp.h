#include "D3dUtility/D3dUtility.h"

class D3dApp
{
public:
	static D3dApp* GetInstance();

	static bool Init(HWND hwnd, int width, int height);
	static bool Run(float dt);

public:
	virtual ~D3dApp();
	bool InitDevice(int width, int height);
	bool Update(float dt);
	bool Render();
	void Clear();
private:
	static D3dApp *m_pInstance;
	D3dApp(HWND hwnd);

	HWND					m_Hwnd;

	//设备驱动类型
	D3D_DRIVER_TYPE			m_DriverType;
	//D3D设备
	ID3D11Device*           m_pd3dDevice;
	//D3D文本设备
	ID3D11DeviceContext*    m_pImmediateContext;
	//D3D交换链
	IDXGISwapChain*         m_pSwapChain;
	//渲染目标视图
	ID3D11RenderTargetView* m_pRenderTargetView;

	ID3D11Texture2D*        m_pDepthStencil;

	ID3D11DepthStencilView* m_pDepthStencilView;

	D3D11_VIEWPORT			m_ScreenViewport;

	int						m_Width;
	int						m_Height;
};