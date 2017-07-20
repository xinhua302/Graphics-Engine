#include "Effect.h"

#pragma region Effect
Effect::Effect(ID3D11Device* device, const std::wstring& filename)
: BaseEffect(device, filename)
{
    Tech = m_FX->GetTechniqueByName("Tech");

    WorldViewProj = m_FX->GetVariableByName("gWorldViewProj")->AsMatrix();
    World = m_FX->GetVariableByName("gWorld")->AsMatrix();
    WorldInvTranspose = m_FX->GetVariableByName("gWorldInvTranspose")->AsMatrix();
    TexTransform = m_FX->GetVariableByName("gTexTransform")->AsMatrix();
    EyePosW = m_FX->GetVariableByName("gEyePosW")->AsVector();
	FogColor = m_FX->GetVariableByName("gFogColor")->AsVector();
	FogStart = m_FX->GetVariableByName("gFogStart")->AsScalar();
	FogRange = m_FX->GetVariableByName("gFogRange")->AsScalar();
    DirLights = m_FX->GetVariableByName("gDirLights");
    Mat = m_FX->GetVariableByName("gMaterial");
    DiffuseMap = m_FX->GetVariableByName("gDiffuseMap")->AsShaderResource();
}

Effect::~Effect()
{
    
}

#pragma endregion

#pragma region Effects
Effect* Effects::FX = nullptr;
TreeSpriteEffect* Effects::TreeFX = nullptr;

void Effects::InitAll(ID3D11Device* device)
{
    FX = new Effect(device, L"Resource/FX/Basic.fxo");
    TreeFX = new TreeSpriteEffect(device, L"Resource/FX/TreeSprite.fxo");
}

void Effects::DestroyAll()
{
    SafeDelete(FX);
    SafeDelete(TreeFX);
}



#pragma endregion

#pragma region BaseEffect
BaseEffect::BaseEffect(ID3D11Device* device, const std::wstring& filename) :m_FX(nullptr)
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
}

BaseEffect::~BaseEffect()
{
    ReleaseCOM(m_FX);
}
#pragma endregion

#pragma region TreeSpriteEffect
TreeSpriteEffect::TreeSpriteEffect(ID3D11Device* device, const std::wstring& filename) :BaseEffect(device, filename)
{
    Tech = m_FX->GetTechniqueByName("Tech");

    ViewProj = m_FX->GetVariableByName("gViewProj")->AsMatrix();
    EyePosW = m_FX->GetVariableByName("gEyePosW")->AsVector();
    FogColor = m_FX->GetVariableByName("gFogColor")->AsVector();
    FogStart = m_FX->GetVariableByName("gFogStart")->AsScalar();
    FogRange = m_FX->GetVariableByName("gFogRange")->AsScalar();
    DirLights = m_FX->GetVariableByName("gDirLights");
    Mat = m_FX->GetVariableByName("gMaterial");
    TreeTextureMapArray = m_FX->GetVariableByName("gTreeMapArray")->AsShaderResource();
}

TreeSpriteEffect::~TreeSpriteEffect()
{

}

#pragma endregion
