#include "Vertex.h"
#include "Effect.h"

const D3D11_INPUT_ELEMENT_DESC InputLayoutDesc::Basic32[3] =
{
    { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

const D3D11_INPUT_ELEMENT_DESC InputLayoutDesc::TreePointSprite[2] =
{
    { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "SIZE", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

ID3D11InputLayout* InputLayouts::Basic32 = nullptr;
ID3D11InputLayout* InputLayouts::TreePointSprite = nullptr;

void InputLayouts::InitAll(ID3D11Device* device)
{
    D3DX11_PASS_DESC passDesc;
    Effects::FX->Tech->GetPassByIndex(0)->GetDesc(&passDesc);
    HR(device->CreateInputLayout(InputLayoutDesc::Basic32, 3, passDesc.pIAInputSignature,
        passDesc.IAInputSignatureSize, &Basic32));

    Effects::TreeFX->Tech->GetPassByIndex(0)->GetDesc(&passDesc);
    HR(device->CreateInputLayout(InputLayoutDesc::TreePointSprite, 2, passDesc.pIAInputSignature,
        passDesc.IAInputSignatureSize, &TreePointSprite));
}

void InputLayouts::DestroyAll()
{
    ReleaseCOM(Basic32);
}


