#ifndef __WATER_H
#define __WATER_H
#include "BaseObject.h"
#include "../D3dUtility/Wave.h"

class Water:public Object
{
public:
    Water(int id, int mode);
    virtual ~Water();
public:
    virtual void Init();
    virtual void Clear();
    virtual void Update(float dt);
    virtual void Render();

private:
    int m_Mode;
    //µØÐÎ
    ID3D11Buffer* m_pWaterVB;
    ID3D11Buffer* m_pWaterIB;
    int m_WaterCount;

    ID3D11ShaderResourceView* m_WaterMapSRV;

    XMFLOAT4X4 m_TexTransform;
    XMFLOAT4X4 m_World;

    XMFLOAT2 m_WaterOffset;

    Material m_WaterMat;

    Wave m_Wave;
};
#endif