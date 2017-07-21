#ifndef __D3DUTILITY_H
#define __D3DUTILITY_H
#pragma warning(disable:4005)
#include <d3dx11.h>
#include <dxerr.h>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <xnamath.h>
#include "Effect.h"
#include "MathHelper.h"
#include "Vertex.h"
#include "RenderStates.h"
#include "GeometryGenerator.h"
#include "LightHelper.h"
#include "Camera.h"
#include "FPS.h"

class D3dHelper
{
public:
    //创建纹理数组
    static ID3D11ShaderResourceView* CreateTexture2DArraySRV(
        ID3D11Device* device, ID3D11DeviceContext* context,
        std::vector<std::wstring>& filenames,
        DXGI_FORMAT format = DXGI_FORMAT_FROM_FILE,
        UINT filter = D3DX11_FILTER_NONE,
        UINT mipFilter = D3DX11_FILTER_LINEAR);
};


namespace Colors
{
	XMGLOBALCONST XMVECTORF32 White = { 1.0f, 1.0f, 1.0f, 1.0f };
	XMGLOBALCONST XMVECTORF32 Black = { 0.0f, 0.0f, 0.0f, 1.0f };
	XMGLOBALCONST XMVECTORF32 Red = { 1.0f, 0.0f, 0.0f, 1.0f };
	XMGLOBALCONST XMVECTORF32 Green = { 0.0f, 1.0f, 0.0f, 1.0f };
	XMGLOBALCONST XMVECTORF32 Blue = { 0.0f, 0.0f, 1.0f, 1.0f };
	XMGLOBALCONST XMVECTORF32 Yellow = { 1.0f, 1.0f, 0.0f, 1.0f };
	XMGLOBALCONST XMVECTORF32 Cyan = { 0.0f, 1.0f, 1.0f, 1.0f };
	XMGLOBALCONST XMVECTORF32 Magenta = { 1.0f, 0.0f, 1.0f, 1.0f };
	XMGLOBALCONST XMVECTORF32 Silver = { 0.75f, 0.75f, 0.75f, 1.0f };
	XMGLOBALCONST XMVECTORF32 LightSteelBlue = { 0.69f, 0.77f, 0.87f, 1.0f };
}

#if defined(DEBUG) | defined(_DEBUG)
#ifndef HR
#define HR(x)                                              \
	{                                                          \
	HRESULT hr = (x);                                      \
if (FAILED(hr))                                         \
		{                                                      \
		DXTrace(__FILE__, (DWORD)__LINE__, hr, L#x, true); \
		}                                                      \
	}
#endif

#else
#ifndef HR
#define HR(x) (x)
#endif
#endif 

#define ReleaseCOM(x) { if(x){ x->Release(); x = 0; } }

#define SafeDelete(x) { delete x; x = 0; }
#endif
