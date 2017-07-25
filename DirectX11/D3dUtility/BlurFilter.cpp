#include "BlurFilter.h"
#include "D3dApp.h"

ID3D11ShaderResourceView* BlurFilter::OffscreenSRV = nullptr;

ID3D11UnorderedAccessView* BlurFilter::OffscreenUAV = nullptr;

ID3D11RenderTargetView* BlurFilter::OffscreenRTV = nullptr;

ID3D11Buffer* BlurFilter::ScreenQuadVB = nullptr;

ID3D11Buffer* BlurFilter::ScreenQuadIB = nullptr; 


void BlurFilter::BuildOffscreenViews()
{
    ReleaseCOM(OffscreenSRV);
    ReleaseCOM(OffscreenRTV);
    ReleaseCOM(OffscreenUAV);

    D3D11_TEXTURE2D_DESC texDesc;

    texDesc.Width = D3d->GetWidth();
    texDesc.Height = D3d->GetHeight();
    texDesc.MipLevels = 1;
    texDesc.ArraySize = 1;
    texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    texDesc.SampleDesc.Count = 1;
    texDesc.SampleDesc.Quality = 0;
    texDesc.Usage = D3D11_USAGE_DEFAULT;
    texDesc.BindFlags =  D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
    texDesc.CPUAccessFlags = 0;
    texDesc.MiscFlags = 0;

    ID3D11Texture2D* offscreenTex = nullptr;
    HR(D3d->GetDevice()->CreateTexture2D(&texDesc, 0, &offscreenTex));
    HR(D3d->GetDevice()->CreateRenderTargetView(offscreenTex, nullptr, &OffscreenRTV));
    HR(D3d->GetDevice()->CreateShaderResourceView(offscreenTex, nullptr, &OffscreenSRV));

    //HR(D3d->GetDevice()->CreateUnorderedAccessView(offscreenTex, 0, &OffscreenUAV));

    ReleaseCOM(offscreenTex);
}

void BlurFilter::BuildScreenQuadGeometryBuffers()
{
    GeometryGenerator::MeshData quad;
    GeometryGenerator ::CreateFullscreenQuad(quad);

    std::vector<Vertex::Basic32> vertices(quad.Vertices.size());

    for (UINT i = 0; i < quad.Vertices.size(); ++i)
    {
        vertices[i].Pos = quad.Vertices[i].Position;
        vertices[i].Normal = quad.Vertices[i].Normal;
        vertices[i].Tex = quad.Vertices[i].TexC;
    }

    D3D11_BUFFER_DESC vbd;
    vbd.Usage = D3D11_USAGE_IMMUTABLE;
    vbd.ByteWidth = sizeof(Vertex::Basic32) * quad.Vertices.size();
    vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = 0;
    vbd.MiscFlags = 0;
    D3D11_SUBRESOURCE_DATA vinitData;
    vinitData.pSysMem = &vertices[0];
    HR(D3d->GetDevice()->CreateBuffer(&vbd, &vinitData, &ScreenQuadVB));

    D3D11_BUFFER_DESC ibd;
    ibd.Usage = D3D11_USAGE_IMMUTABLE;
    ibd.ByteWidth = sizeof(UINT)* quad.Indices.size();
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibd.CPUAccessFlags = 0;
    ibd.MiscFlags = 0;
    D3D11_SUBRESOURCE_DATA iinitData;
    iinitData.pSysMem = &quad.Indices[0];
    HR(D3d->GetDevice()->CreateBuffer(&ibd, &iinitData, &ScreenQuadIB));
}

void BlurFilter::InitAll()
{
    BuildOffscreenViews();
    BuildScreenQuadGeometryBuffers();
}

void BlurFilter::DestroyAll()
{
    ReleaseCOM(OffscreenSRV);
    ReleaseCOM(OffscreenUAV);
    ReleaseCOM(OffscreenRTV);
    ReleaseCOM(ScreenQuadVB);
    ReleaseCOM(ScreenQuadIB);
}

void BlurFilter::RenderToTexture(ID3D11RenderTargetView* rtv, ID3D11DepthStencilView* dsv)
{
    D3d->GetContext()->OMSetRenderTargets(1, &rtv, dsv);
    D3d->GetContext()->ClearRenderTargetView(rtv, reinterpret_cast<const float*>(&Colors::Silver));
    D3d->GetContext()->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    D3d->GetContext()->IASetInputLayout(InputLayouts::Basic32);
    D3d->GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    UINT stride = sizeof(Vertex::Basic32);
    UINT offset = 0;

    XMMATRIX identity = XMMatrixIdentity();

    D3DX11_TECHNIQUE_DESC techDesc;
    Effects::FX->TechBlur->GetDesc(&techDesc);
    for (UINT p = 0; p < techDesc.Passes; ++p)
    {
        D3d->GetContext()->IASetVertexBuffers(0, 1, &ScreenQuadVB, &stride, &offset);
        D3d->GetContext()->IASetIndexBuffer(ScreenQuadIB, DXGI_FORMAT_R32_UINT, 0);
        D3d->GetContext()->RSSetState(RenderStates::DefaultRS);
        Effects::FX->SetWorld(identity);
        Effects::FX->SetWorldInvTranspose(identity);
        Effects::FX->SetWorldViewProj(identity);
        Effects::FX->SetTexTransform(identity);
        Effects::FX->SetDiffuseMap(OffscreenSRV);

        Effects::FX->TechBlur->GetPassByIndex(p)->Apply(0, D3d->GetContext());
        D3d->GetContext()->DrawIndexed(6, 0, 0);
    }
}


