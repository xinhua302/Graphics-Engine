#ifndef __BLURFILTER_H
#define __BLURFILTER_H
#include "D3dUtility.h"

class BlurFilter
{
private:
    static ID3D11ShaderResourceView* m_BlurredOutputTexSRV;
    static ID3D11UnorderedAccessView* m_BlurredOutputTexUAV;

public:
    static void InitAll();
    static void DestroyAll();

    static void BuildOffscreenViews();
    static void BuildScreenQuadGeometryBuffers();
    static void RenderToTexture(ID3D11RenderTargetView* rtv, ID3D11DepthStencilView* dsv);
    static void BlurInPlace(ID3D11ShaderResourceView* inputSRV, ID3D11UnorderedAccessView* inputUAV, int blurCount);

    static ID3D11ShaderResourceView* OffscreenSRV;
    static ID3D11UnorderedAccessView* OffscreenUAV;
    static ID3D11RenderTargetView* OffscreenRTV;

    static ID3D11Buffer* ScreenQuadVB;
    static ID3D11Buffer* ScreenQuadIB;
};
#endif