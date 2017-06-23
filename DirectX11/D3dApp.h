#include "D3dUtility/D3dUtility.h"

class D3dApp
{
public:
	static D3dApp* GetInstance();
	virtual ~D3dApp();

	static bool Init(HWND hwnd);
	static bool Update(float dt);
	static bool Render();
	static void Clear();

public:
	bool InitDevice();
private:
	static D3dApp *m_pInstance;
	D3dApp(HWND hwnd);

	HWND					m_Hwnd;

	//�豸��������
	D3D_DRIVER_TYPE			m_DriverType;
	//D3D�豸
	ID3D11Device*           m_pd3dDevice;
	//D3D�ı��豸
	ID3D11DeviceContext*    m_pImmediateContext;
	//D3D������
	IDXGISwapChain*         m_pSwapChain;
	//��ȾĿ����ͼ
	ID3D11RenderTargetView* m_pRenderTargetView;

	ID3D11Texture2D*        m_pDepthStencil;

	ID3D11DepthStencilView* m_pDepthStencilView;

	D3D11_VIEWPORT			m_ScreenViewport;

	int						m_Width;
	int						m_Height;
};