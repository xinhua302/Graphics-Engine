#ifndef __RENDERSTATE_H
#define __RENDERSTATE_H

#include "D3dUtility.h"

class RenderStates
{
public:
    static void InitAll(ID3D11Device* device);
    static void DestroyAll();

    //光栅化状态
	static ID3D11RasterizerState* DefaultRS;
    static ID3D11RasterizerState* WireframeRS;
    static ID3D11RasterizerState* NoCullRS;
    static ID3D11RasterizerState* CullClockwiseRS;
    //混合状态
    static ID3D11BlendState* AlphaToCoverageBS;
    static ID3D11BlendState* TransparentBS;
    static ID3D11BlendState* NoRenderTargetWritesBS;
    //深度模板状态
    static ID3D11DepthStencilState* MarkMirrorDSS;
    static ID3D11DepthStencilState* DrawReflectionDSS;
    static ID3D11DepthStencilState* NoDoubleBlendDSS;
};
#endif