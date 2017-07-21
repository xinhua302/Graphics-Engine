#include "D3dUtility.h"
#include "D3dApp.h"

ID3D11ShaderResourceView* D3dHelper::CreateTexture2DArraySRV(ID3D11Device* device, 
    ID3D11DeviceContext* context,
    std::vector<std::wstring>& filenames, 
    DXGI_FORMAT format, UINT filter, 
    UINT mipFilter)
{
    UINT size = filenames.size();

    std::vector<ID3D11Texture2D*> srcTex(size);
    for (UINT i = 0; i < size; ++i)
    {
        D3DX11_IMAGE_LOAD_INFO loadInfo;

        loadInfo.Width = D3DX11_FROM_FILE;
        loadInfo.Height = D3DX11_FROM_FILE;
        loadInfo.Depth = D3DX11_FROM_FILE;
        loadInfo.FirstMipLevel = 0;
        loadInfo.MipLevels = D3DX11_FROM_FILE;
        loadInfo.Usage = D3D11_USAGE_STAGING;
        loadInfo.BindFlags = 0;
        loadInfo.CpuAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
        loadInfo.MiscFlags = 0;
        loadInfo.Format = format;
        loadInfo.Filter = filter;
        loadInfo.MipFilter = mipFilter;
        loadInfo.pSrcInfo = 0;

        HR(D3DX11CreateTextureFromFile(device, filenames[i].c_str(),
            &loadInfo, 0, (ID3D11Resource**)&srcTex[i], 0));
    }

    D3D11_TEXTURE2D_DESC texElementDesc;
    srcTex[0]->GetDesc(&texElementDesc);

    D3D11_TEXTURE2D_DESC texArrayDesc;
    texArrayDesc.Width = texElementDesc.Width;
    texArrayDesc.Height = texElementDesc.Height;
    texArrayDesc.MipLevels = texElementDesc.MipLevels;
    texArrayDesc.ArraySize = size;
    texArrayDesc.Format = texElementDesc.Format;
    texArrayDesc.SampleDesc.Count = 1;
    texArrayDesc.SampleDesc.Quality = 0;
    texArrayDesc.Usage = D3D11_USAGE_DEFAULT;
    texArrayDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    texArrayDesc.CPUAccessFlags = 0;
    texArrayDesc.MiscFlags = 0;

    ID3D11Texture2D* texArray = nullptr;
    HR(device->CreateTexture2D(&texArrayDesc, 0, &texArray));


    for (UINT texElement = 0; texElement < size; ++texElement)
    {
        for (UINT mipLevel = 0; mipLevel < texElementDesc.MipLevels; ++mipLevel)
        {
            D3D11_MAPPED_SUBRESOURCE mappedTex2D;
            HR(context->Map(srcTex[texElement], mipLevel, D3D11_MAP_READ, 0, &mappedTex2D));

            context->UpdateSubresource(texArray,
                D3D11CalcSubresource(mipLevel, texElement, texElementDesc.MipLevels),
                0, mappedTex2D.pData, mappedTex2D.RowPitch, mappedTex2D.DepthPitch);

            context->Unmap(srcTex[texElement], mipLevel);
        }
    }


    D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
    viewDesc.Format = texArrayDesc.Format;
    viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
    viewDesc.Texture2DArray.MostDetailedMip = 0;
    viewDesc.Texture2DArray.MipLevels = texArrayDesc.MipLevels;
    viewDesc.Texture2DArray.FirstArraySlice = 0;
    viewDesc.Texture2DArray.ArraySize = size;

    ID3D11ShaderResourceView* texArraySRV = nullptr;
    HR(device->CreateShaderResourceView(texArray, &viewDesc, &texArraySRV));

    ReleaseCOM(texArray);

    for (UINT i = 0; i < size; ++i)
        ReleaseCOM(srcTex[i]);

    return texArraySRV;
}
