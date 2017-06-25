#ifndef __D3DUTILITY_H
#define __D3DUTILITY_H
#include <d3dx11.h>
#include <dxerr.h>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <xnamath.h>

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
