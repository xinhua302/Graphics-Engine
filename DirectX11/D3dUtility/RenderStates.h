#ifndef __RENDERSTATE_H
#define __RENDERSTATE_H

#include "D3dUtility.h"

class RenderStates
{
public:
    static void InitAll(ID3D11Device* device);
    static void DestroyAll();

    static ID3D11RasterizerState* WireframeRS;
    static ID3D11RasterizerState* NoCullRS;

    static ID3D11BlendState* AlphaToCoverageBS;
    static ID3D11BlendState* TransparentBS;
};
#endif