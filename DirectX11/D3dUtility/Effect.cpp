#include "Effect.h"

#pragma region Effect
Effect::Effect(ID3D11Device* device, const std::wstring& filename)
: m_FX(nullptr)
{
    std::ifstream fin(filename, std::ios::binary);

    fin.seekg(0, std::ios_base::end);
    int size = (int)fin.tellg();
    fin.seekg(0, std::ios_base::beg);
    std::vector<char> compiledShader(size);

    fin.read(&compiledShader[0], size);
    fin.close();

    HR(D3DX11CreateEffectFromMemory(&compiledShader[0], size,
        0, device, &m_FX));

    Light1Tech = m_FX->GetTechniqueByName("Light1");
    Light2Tech = m_FX->GetTechniqueByName("Light2");
    Light3Tech = m_FX->GetTechniqueByName("Light3");

    Light0TexTech = m_FX->GetTechniqueByName("Light0Tex");
    Light1TexTech = m_FX->GetTechniqueByName("Light1Tex");
    Light2TexTech = m_FX->GetTechniqueByName("Light2Tex");
    Light3TexTech = m_FX->GetTechniqueByName("Light3Tex");

    WorldViewProj = m_FX->GetVariableByName("gWorldViewProj")->AsMatrix();
    World = m_FX->GetVariableByName("gWorld")->AsMatrix();
    WorldInvTranspose = m_FX->GetVariableByName("gWorldInvTranspose")->AsMatrix();
    TexTransform = m_FX->GetVariableByName("gTexTransform")->AsMatrix();
    EyePosW = m_FX->GetVariableByName("gEyePosW")->AsVector();
    DirLights = m_FX->GetVariableByName("gDirLights");
    Mat = m_FX->GetVariableByName("gMaterial");
    DiffuseMap = m_FX->GetVariableByName("gDiffuseMap")->AsShaderResource();
}

Effect::~Effect()
{
    ReleaseCOM(m_FX);
}

#pragma endregion

#pragma region Effects
Effect* Effects::FX = 0;

void Effects::InitAll(ID3D11Device* device)
{
    FX = new Effect(device, L"Resource/FX/Basic.fxo");
}

void Effects::DestroyAll()
{
    SafeDelete(FX);
}
#pragma endregion