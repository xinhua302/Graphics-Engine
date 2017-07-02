#ifndef __BOX_H
#define __BOX_H
#include "BaseObject.h"
class Box :public Object
{
public:
    Box(int id, int mode);
    virtual ~Box();
public:
    virtual void Init();
    virtual void Clear();
    virtual void Update(float dt);
    virtual void Render();

private:
    int m_Mode;
    //µØÐÎ
    ID3D11Buffer* m_pBoxVB;
    ID3D11Buffer* m_pBoxIB;
    int m_BoxCount;

    ID3D11ShaderResourceView* m_BoxMapSRV;

    XMFLOAT4X4 m_TexTransform;
    XMFLOAT4X4 m_World;
    XMFLOAT4X4 m_View;
    XMFLOAT4X4 m_Proj;

    Material m_BoxMat;
};

#endif