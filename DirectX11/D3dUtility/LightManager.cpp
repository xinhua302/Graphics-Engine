#include "LightManager.h"

void LightManager::InitAll()
{

}

void LightManager::DestroyAll()
{

}

void LightManager::CreateLight()
{
    Light[0].Ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
    Light[0].Diffuse = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
    Light[0].Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
    Light[0].Direction = XMFLOAT3(0.57735f, -0.57735f, 0.57735f);

    Light[1].Ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
    Light[1].Diffuse = XMFLOAT4(0.20f, 0.20f, 0.20f, 1.0f);
    Light[1].Specular = XMFLOAT4(0.25f, 0.25f, 0.25f, 1.0f);
    Light[1].Direction = XMFLOAT3(-0.57735f, -0.57735f, 0.57735f);

    Light[2].Ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
    Light[2].Diffuse = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
    Light[2].Specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
    Light[2].Direction = XMFLOAT3(0.0f, -0.707f, -0.707f);
}

void LightManager::Apply()
{
    //¹Û²ìµÄÎ»ÖÃ
    XMFLOAT3 eyePosW = XMFLOAT3(-2.3f, 5.06f, -19.0f);
    Effects::FX->SetDirLights(Light);
    Effects::FX->SetEyePosW(eyePosW);
}

DirectionalLight LightManager::Light[3];
