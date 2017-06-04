#include <windows.h>
#include <d3d11.h>
#include <D3DX11.h>
#include <d3dcompiler.h>
#include <xnamath.h>

//��ʼ���豸
HRESULT InitDevice(HWND hwnd);

//������ɫ������
HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);

//����豸
void CleanupDevice();

//��Ⱦ
void Render();