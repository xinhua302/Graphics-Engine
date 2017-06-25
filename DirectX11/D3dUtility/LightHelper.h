#ifndef __LIGHTHELPER_H
#define __LIGHTHELPER_H

#include <Windows.h>
#include <xnamath.h>

//�����
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

//���Դ
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

    //˥������
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

    //����
    XMFLOAT3 Direction;
    //ϵ��
    float Spot;

    //˥������
    XMFLOAT3 Att;
    float Pad; // Pad the last float so we can set an array of lights if we wanted.
};

//����
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