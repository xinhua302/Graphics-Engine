#ifndef __LIGHTMANAGER_H
#define __LIGHTMANAGER_H
#include <Windows.h>
#include <xnamath.h>

//方向光
struct DirectionalLight
{
    DirectionalLight()
    {
        ZeroMemory(this, sizeof(DirectionalLight));
    }

    XMFLOAT4 Ambient;
    XMFLOAT4 Diffuse;
    XMFLOAT4 Specular;
    XMFLOAT3 Direction;
    float Pad; //字节对齐
};

//点光源
struct PointLight
{
    PointLight()
    {
        ZeroMemory(this, sizeof(PointLight));
    }

    XMFLOAT4 Ambient;
    XMFLOAT4 Diffuse;
    XMFLOAT4 Specular;

    XMFLOAT3 Position;
    float Range;

    //衰减银子
    XMFLOAT3 Att;
    float Pad; //字节对齐
};

struct SpotLight
{
    SpotLight()
    {
        ZeroMemory(this, sizeof(SpotLight));
    }

    XMFLOAT4 Ambient;
    XMFLOAT4 Diffuse;
    XMFLOAT4 Specular;

    XMFLOAT3 Position;
    float Range;

    //方向
    XMFLOAT3 Direction;
    //系数
    float Spot;

    //衰减因子
    XMFLOAT3 Att;
    float Pad; //字节对齐
};

//材质
struct Material
{
    Material()
    {
        ZeroMemory(this, sizeof(Material));
    }

    XMFLOAT4 Ambient;
    XMFLOAT4 Diffuse;
    XMFLOAT4 Specular;
    XMFLOAT4 Reflect;
};

class LightManager
{
public:
    static void InitAll();
    static void DestroyAll();

    static void CreateLight();
    static void Apply();

    static DirectionalLight Light[3];
};
#endif