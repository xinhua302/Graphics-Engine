#include <windows.h>
#include <d3d11.h>
#include <D3DX11.h>
#include <d3dcompiler.h>
#include <xnamath.h>

//初始化设备
HRESULT InitDevice(HWND hwnd);

//编译着色器语言
HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);

//清除设备
void CleanupDevice();

//渲染
void Render();