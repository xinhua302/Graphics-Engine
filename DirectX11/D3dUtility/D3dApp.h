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
    bool LoadModel();

private:
    bool SetRasterizerState();
    bool InitDevice(int width, int height);
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
    //���ģ�建��
	ID3D11Texture2D*        m_pDepthStencil;
    //���ģ����ͼ
	ID3D11DepthStencilView* m_pDepthStencilView;
    //��Ⱦ״̬
    ID3D11RasterizerState*  m_WireframeRS;
    //�ӿ�
	D3D11_VIEWPORT			m_ScreenViewport;

	int						m_Width;
	int						m_Height;

    //���������
    ID3D11Buffer* m_VB;
    ID3D11Buffer* m_IB;
    //������ͼ
    ID3D11ShaderResourceView* m_DiffuseMapSRV;
    //����任
    XMFLOAT4X4 m_BoxWorld;
    XMFLOAT4X4 m_View;
    XMFLOAT4X4 m_Proj;
};
#endif