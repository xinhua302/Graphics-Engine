#ifndef __LIGHTHELPER_H
#define __LIGHTHELPER_H

#include <Windows.h>
#include <xnamath.h>

//方向光
struct DirectionalLight
{
    DirectionalLight() 
    {
        ZeroMemory(this, sizeof(this)); 
    }

    XMFLOAT4 Ambient;
    XMFLOAT4 Diffuse;
    XMFLOAT4 Specular;
    XMFLOAT3 Direction;
    float Pad; // Pad the last float so we can set an array of lights if we wanted.
};

//点光源
struct PointLight
{
    PointLight() 
    {
        ZeroMemory(this, sizeof(this));
    }

    XMFLOAT4 Ambient;
    XMFLOAT4 Diffuse;
    XMFLOAT4 Specular;

    XMFLOAT3 Position;
    float Range;

    //衰减银子
    XMFLOAT3 Att;
    float Pad; // Pad the last float so we can set an array of lights if we wanted.
};

struct SpotLight
{
    SpotLight() 
    {
        ZeroMemory(this, sizeof(this));
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
    float Pad; // Pad the last float so we can set an array of lights if we wanted.
};

//材质
struct Material
{
    Material() 
    { 
        ZeroMemory(this, sizeof(this)); 
    }

    XMFLOAT4 Ambient;
    XMFLOAT4 Diffuse;
    XMFLOAT4 Specular; 
    XMFLOAT4 Reflect;
};
#endif