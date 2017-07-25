#ifndef __D3DAPP_H
#define __D3DAPP_H
#include "D3dUtility.h"

#define D3d D3dApp::GetInstance()
class D3dApp
{
public:
	static D3dApp* GetInstance();

	static bool Init(HWND hwnd, int width, int height);
	static bool Run(float dt);

public:
	virtual ~D3dApp();
    bool Init();
	bool Update(float dt);
	bool Render();
	void Clear();
    void OnResize();
    float GetAspectRatio();
    int GetWidth() const;
    int GetHeight() const;

    ID3D11Device* GetDevice() const;
    ID3D11DeviceContext* GetContext() const;
    IDXGISwapChain* GeSwapChain() const;
    Camera          GetCamera() const;

    //控制鼠标
    void OnMouseDown(WPARAM btnState, int x, int y);
    void OnMouseUp(WPARAM btnState, int x, int y);
    void OnMouseMove(WPARAM btnState, int x, int y);

private:
    bool InitDevice(int width, int height);
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
    //深度模板缓存
	ID3D11Texture2D*        m_pDepthStencil;
    //深度模板视图
	ID3D11DepthStencilView* m_pDepthStencilView;
    //视口
	D3D11_VIEWPORT			m_ScreenViewport;

    //开启多重采样
    bool m_Enable4xMsaa;
    UINT m_4xMsaaQuality;

	int						m_Width;
	int						m_Height;

    Camera                  m_Camera;

    //上一次鼠标位置
    POINT m_LastMousePos;
};
#endif