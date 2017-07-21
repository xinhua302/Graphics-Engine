#ifndef __LIGHTMANAGER_H
#define __LIGHTMANAGER_H
#include <Windows.h>
#include <xnamath.h>

//�����
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
    float Pad; //�ֽڶ���
};

//���Դ
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

    //˥������
    XMFLOAT3 Att;
    float Pad; //�ֽڶ���
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

    //����
    XMFLOAT3 Direction;
    //ϵ��
    float Spot;

    //˥������
    XMFLOAT3 Att;
    float Pad; //�ֽڶ���
};

//����
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