#ifndef __WAVE_H
#define __WAVE_H
#include "D3dUtility.h"
class Wave
{
public:
    Wave();
    Wave(int m, int n, float space, float dt, float speed, float damping);
    ~Wave();

    float GetWidth() const;
    float GetDepth() const;

    UINT GetVertexCountX() const;
    UINT GetVertexCountZ() const;

    UINT GetTriangleCount();
    UINT GetVertexCount();

    const XMFLOAT3& operator[] (int i) const
    {
        return m_Cur[i];
    }

    const XMFLOAT3& Normal(int i) const 
    { 
        return m_Normals[i];
    }

    void Init(int m, int n, float space, float dt, float speed, float damping);

    void Update(float dt);

    void Disturb(UINT i, UINT j, float magnitude);
private:
    float GetHeight(float x, float z, float dt);


    UINT m_VertexCountX;
    UINT m_VertexCountZ;

    //Íø¸ñ¼ä¾à
    float m_Space;

    XMFLOAT3* m_Pre;
    XMFLOAT3* m_Cur;
    XMFLOAT3* m_Normals;

    float m_K1;
    float m_K2;
    float m_K3;

    float m_TimeStep;
};
#endif