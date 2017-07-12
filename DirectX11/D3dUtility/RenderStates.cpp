#include "RenderStates.h"

ID3D11RasterizerState* RenderStates::DefaultRS = nullptr;
ID3D11RasterizerState* RenderStates::WireframeRS = nullptr;
ID3D11RasterizerState* RenderStates::NoCullRS = nullptr;
ID3D11RasterizerState* RenderStates::CullClockwiseRS = nullptr;

ID3D11BlendState*      RenderStates::AlphaToCoverageBS = nullptr;
ID3D11BlendState*      RenderStates::TransparentBS = nullptr;
ID3D11BlendState*      RenderStates::NoRenderTargetWritesBS = nullptr;

ID3D11DepthStencilState* RenderStates::MarkMirrorDSS;
ID3D11DepthStencilState* RenderStates::DrawReflectionDSS;
ID3D11DepthStencilState* RenderStates::NoDoubleBlendDSS;

void RenderStates::InitAll(ID3D11Device* device)
{
	//默认模式
	D3D11_RASTERIZER_DESC defaultDesc;
	ZeroMemory(&defaultDesc, sizeof(D3D11_RASTERIZER_DESC));
	defaultDesc.FillMode = D3D11_FILL_SOLID;
	defaultDesc.CullMode = D3D11_CULL_BACK;
	defaultDesc.FrontCounterClockwise = false;
	defaultDesc.DepthClipEnable = true;

	HR(device->CreateRasterizerState(&defaultDesc, &DefaultRS));

    //线框模式
    D3D11_RASTERIZER_DESC wireframeDesc;
    ZeroMemory(&wireframeDesc, sizeof(D3D11_RASTERIZER_DESC));
    wireframeDesc.FillMode = D3D11_FILL_WIREFRAME;
    wireframeDesc.CullMode = D3D11_CULL_BACK;
    wireframeDesc.FrontCounterClockwise = false;
    wireframeDesc.DepthClipEnable = true;

    HR(device->CreateRasterizerState(&wireframeDesc, &WireframeRS));

    //无背面剔除
    D3D11_RASTERIZER_DESC noCullDesc;
    ZeroMemory(&noCullDesc, sizeof(D3D11_RASTERIZER_DESC));
    noCullDesc.FillMode = D3D11_FILL_SOLID;
    noCullDesc.CullMode = D3D11_CULL_NONE;
    noCullDesc.FrontCounterClockwise = false;
    noCullDesc.DepthClipEnable = true;

    HR(device->CreateRasterizerState(&noCullDesc, &NoCullRS));

    //顺时针
    D3D11_RASTERIZER_DESC cullClockwiseDesc;
    ZeroMemory(&cullClockwiseDesc, sizeof(D3D11_RASTERIZER_DESC));
    cullClockwiseDesc.FillMode = D3D11_FILL_SOLID;
    cullClockwiseDesc.CullMode = D3D11_CULL_BACK;
    cullClockwiseDesc.FrontCounterClockwise = true;
    cullClockwiseDesc.DepthClipEnable = true;

    HR(device->CreateRasterizerState(&cullClockwiseDesc, &CullClockwiseRS));

   
    //范围透明
    D3D11_BLEND_DESC alphaToCoverageDesc = { 0 };
    alphaToCoverageDesc.AlphaToCoverageEnable = true;
    alphaToCoverageDesc.IndependentBlendEnable = false;
    alphaToCoverageDesc.RenderTarget[0].BlendEnable = false;
    alphaToCoverageDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    HR(device->CreateBlendState(&alphaToCoverageDesc, &AlphaToCoverageBS));

    //透明融合
    D3D11_BLEND_DESC transparentDesc = { 0 };
    transparentDesc.AlphaToCoverageEnable = false;
    transparentDesc.IndependentBlendEnable = false;

    transparentDesc.RenderTarget[0].BlendEnable = true;
    transparentDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    transparentDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    transparentDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    transparentDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    transparentDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    transparentDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    transparentDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    HR(device->CreateBlendState(&transparentDesc, &TransparentBS));

    D3D11_BLEND_DESC noRenderTargetWritesDesc = { 0 };
    noRenderTargetWritesDesc.AlphaToCoverageEnable = false;
    noRenderTargetWritesDesc.IndependentBlendEnable = false;

    noRenderTargetWritesDesc.RenderTarget[0].BlendEnable = false;
    noRenderTargetWritesDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
    noRenderTargetWritesDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
    noRenderTargetWritesDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    noRenderTargetWritesDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    noRenderTargetWritesDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    noRenderTargetWritesDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    noRenderTargetWritesDesc.RenderTarget[0].RenderTargetWriteMask = 0;

    HR(device->CreateBlendState(&noRenderTargetWritesDesc, &NoRenderTargetWritesBS));

    
    //标记模板缓存
    D3D11_DEPTH_STENCIL_DESC mirrorDesc;
    mirrorDesc.DepthEnable = true;
    mirrorDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
    mirrorDesc.DepthFunc = D3D11_COMPARISON_LESS;
    mirrorDesc.StencilEnable = true;
    mirrorDesc.StencilReadMask = 0xff;
    mirrorDesc.StencilWriteMask = 0xff;

    mirrorDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    mirrorDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
    mirrorDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
    mirrorDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    mirrorDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    mirrorDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
    mirrorDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
    mirrorDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    HR(device->CreateDepthStencilState(&mirrorDesc, &MarkMirrorDSS));

    //绘制反射
    D3D11_DEPTH_STENCIL_DESC drawReflectionDesc;
    drawReflectionDesc.DepthEnable = true;
    drawReflectionDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    drawReflectionDesc.DepthFunc = D3D11_COMPARISON_LESS;
    drawReflectionDesc.StencilEnable = true;
    drawReflectionDesc.StencilReadMask = 0xff;
    drawReflectionDesc.StencilWriteMask = 0xff;

    drawReflectionDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    drawReflectionDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
    drawReflectionDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    drawReflectionDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;

    drawReflectionDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    drawReflectionDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
    drawReflectionDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    drawReflectionDesc.BackFace.StencilFunc = D3D11_COMPARISON_EQUAL;

    HR(device->CreateDepthStencilState(&drawReflectionDesc, &DrawReflectionDSS));

    //防止双混合
    D3D11_DEPTH_STENCIL_DESC noDoubleBlendDesc;
    noDoubleBlendDesc.DepthEnable = true;
    noDoubleBlendDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    noDoubleBlendDesc.DepthFunc = D3D11_COMPARISON_LESS;
    noDoubleBlendDesc.StencilEnable = true;
    noDoubleBlendDesc.StencilReadMask = 0xff;
    noDoubleBlendDesc.StencilWriteMask = 0xff;

    noDoubleBlendDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    noDoubleBlendDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
    noDoubleBlendDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR;
    noDoubleBlendDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;

    noDoubleBlendDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    noDoubleBlendDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
    noDoubleBlendDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_INCR;
    noDoubleBlendDesc.BackFace.StencilFunc = D3D11_COMPARISON_EQUAL;

    HR(device->CreateDepthStencilState(&noDoubleBlendDesc, &NoDoubleBlendDSS));
}

void RenderStates::DestroyAll()
{
    ReleaseCOM(DefaultRS);
    ReleaseCOM(WireframeRS);
    ReleaseCOM(NoCullRS);

    ReleaseCOM(AlphaToCoverageBS);
    ReleaseCOM(TransparentBS);
    ReleaseCOM(NoRenderTargetWritesBS);

    ReleaseCOM(MarkMirrorDSS);
    ReleaseCOM(DrawReflectionDSS);
    ReleaseCOM(NoDoubleBlendDSS);
}


