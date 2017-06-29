#ifndef __LAND_H
#define __LAND_H
#include "BaseObject.h"
class Land :public Object
{
public:
    Land(int id);
    virtual ~Land();
public:
    virtual void Init();
    virtual void Clear();
    virtual void Update(float dt);
    virtual void Render();

private:
    //µØÐÎ
    ID3D11Buffer* m_pLandVB;
    ID3D11Buffer* m_pLandIB;
    int m_LandCount;

    ID3D11ShaderResourceView* m_LandeMapSRV;

    XMFLOAT4X4 m_TexTransform;
    XMFLOAT4X4 m_World;
    XMFLOAT4X4 m_View;
    XMFLOAT4X4 m_Proj;
};
#endif