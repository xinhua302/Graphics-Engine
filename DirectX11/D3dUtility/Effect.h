#ifndef __EFFECT_H
#define __EFFECT_H
#include "D3dUtility.h"
#include "d3dx11effect.h"
#include "LightHelper.h"

#pragma region BaseEffect
class BaseEffect
{
public:
    BaseEffect(ID3D11Device* device, const std::wstring& filename);
    virtual ~BaseEffect();

protected:
    ID3DX11Effect* m_FX;
};
#pragma endregion

#pragma region Effect
class Effect:public BaseEffect
{
public:
    Effect(ID3D11Device* device, const std::wstring& filename);
    virtual ~Effect();

public:
    void SetWorldViewProj(CXMMATRIX M)                  { WorldViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
    void SetWorld(CXMMATRIX M)                          { World->SetMatrix(reinterpret_cast<const float*>(&M)); }
    void SetWorldInvTranspose(CXMMATRIX M)              { WorldInvTranspose->SetMatrix(reinterpret_cast<const float*>(&M)); }
    void SetTexTransform(CXMMATRIX M)                   { TexTransform->SetMatrix(reinterpret_cast<const float*>(&M)); }
    void SetEyePosW(const XMFLOAT3& v)                  { EyePosW->SetRawValue(&v, 0, sizeof(XMFLOAT3)); }
	void SetFogColor(const FXMVECTOR &color)			{ FogColor->SetFloatVector(reinterpret_cast<const float*>(&color)); }
	void SetFogStart(float t)							{ FogStart->SetFloat(t); }
	void SetFogRange(float t)							{ FogRange->SetFloat(t); }
    void SetDirLights(const DirectionalLight* lights)   { DirLights->SetRawValue(lights, 0, 3 * sizeof(DirectionalLight)); }
    void SetMaterial(const Material& mat)               { Mat->SetRawValue(&mat, 0, sizeof(Material)); }
    void SetDiffuseMap(ID3D11ShaderResourceView* tex)   { DiffuseMap->SetResource(tex); }

    ID3DX11EffectTechnique* Tech;

    ID3DX11EffectMatrixVariable* WorldViewProj;
    ID3DX11EffectMatrixVariable* World;
    ID3DX11EffectMatrixVariable* WorldInvTranspose;
    ID3DX11EffectMatrixVariable* TexTransform;
    ID3DX11EffectVectorVariable* EyePosW;
	ID3DX11EffectVectorVariable* FogColor;
	ID3DX11EffectScalarVariable* FogStart;
	ID3DX11EffectScalarVariable* FogRange;
    ID3DX11EffectVariable* DirLights;
    ID3DX11EffectVariable* Mat;
    ID3DX11EffectShaderResourceVariable* DiffuseMap;
};
#pragma endregion

#pragma region TreeSpriteEffect
class TreeSpriteEffect:public BaseEffect
{
public:
    TreeSpriteEffect(ID3D11Device* device, const std::wstring& filename);
    virtual~TreeSpriteEffect();

public:
    void SetViewProj(CXMMATRIX M)                       { ViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
    void SetEysPosW(const XMFLOAT3& v)                  { EyePosW->SetRawValue(&v, 0, sizeof(XMFLOAT3)); }
    void SetFogColor(const FXMVECTOR &color)			{ FogColor->SetFloatVector(reinterpret_cast<const float*>(&color)); }
    void SetFogStart(float t)							{ FogStart->SetFloat(t); }
    void SetFogRange(float t)							{ FogRange->SetFloat(t); }
    void SetDirLights(const DirectionalLight* lights)   { DirLights->SetRawValue(lights, 0, 3 * sizeof(DirectionalLight)); }
    void SetMaterial(const Material& mat)               { Mat->SetRawValue(&mat, 0, sizeof(Material)); }
    void SetTreeTextureMapArray(ID3D11ShaderResourceView* tex) { TreeTextureMapArray->SetResource(tex); }

    ID3DX11EffectTechnique* Tech;

    ID3DX11EffectMatrixVariable* ViewProj;
    ID3DX11EffectVectorVariable* EyePosW;
    ID3DX11EffectVectorVariable* FogColor;
    ID3DX11EffectScalarVariable* FogStart;
    ID3DX11EffectScalarVariable* FogRange;
    ID3DX11EffectVariable* DirLights;
    ID3DX11EffectVariable* Mat;
    ID3DX11EffectShaderResourceVariable* TreeTextureMapArray;
};
#pragma endregion

#pragma region Effects
class Effects
{
public:
    static void InitAll(ID3D11Device* device);
    static void DestroyAll();

    static Effect* FX;

    static TreeSpriteEffect* TreeFX;
};
#pragma endregion
#endif