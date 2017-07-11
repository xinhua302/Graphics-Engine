#include "Wave.h"

Wave::Wave(int m, int n, float space, float dt, float speed, float damping)
{
    Init(m, n, space, dt, speed, damping);
}

Wave::Wave()
{
    m_VertexCountX = 0;
    m_VertexCountZ = 0;
    m_Space = 0.0f;

    m_TimeStep = 0.0f;
    m_K1 = m_K2 = m_K3 = 0.0f;
}

Wave::~Wave()
{
    delete[] m_Pre;
    delete[] m_Cur;
    delete[] m_Normals;
}

float Wave::GetWidth() const
{
    return m_Space * (m_VertexCountX - 1);
}

float Wave::GetDepth() const
{
    return m_Space *(m_VertexCountZ - 1); 
}

UINT Wave::GetVertexCountX() const
{
    return m_VertexCountX;
}

UINT Wave::GetVertexCountZ() const
{
    return m_VertexCountZ;
}

UINT Wave::GetTriangleCount()
{
    return (m_VertexCountX - 1) * (m_VertexCountZ - 1) * 2;
}

UINT Wave::GetVertexCount()
{
    return m_VertexCountX * m_VertexCountZ;
}

void Wave::Init(int m, int n, float space, float dt, float speed, float damping)
{
    m_VertexCountX = m;
    m_VertexCountZ = n;
    m_Space = space;

    delete[] m_Pre;
    delete[] m_Cur;
    delete[] m_Normals;

    m_Pre = new XMFLOAT3[m * n];
    m_Cur = new XMFLOAT3[m * n];
    m_Normals = new XMFLOAT3[m * n];

    m_TimeStep = dt;
    float d = damping*dt + 2.0f;
    float e = (speed*speed)*(dt*dt) / (space*space);
    m_K1 = (damping*dt - 2.0f) / d;
    m_K2 = (4.0f - 8.0f*e) / d;
    m_K3 = (2.0f*e) / d;

    float offsetX = -GetWidth() / 2.0f;
    float offsetZ = GetDepth() / 2.0f;
    for (int i = 0; i < n; i++)
    {
        float z = offsetZ - space * i;
        for (int j = 0; j < m; j++)
        {
            float x = offsetX + space * j;
            
            m_Cur[i * m + j] = XMFLOAT3(x, 0.0f, z);
            m_Pre[i * m + j] = XMFLOAT3(x, 0.0f, z);
            m_Normals[i * m + j] = XMFLOAT3(0.0f, 1.0f, 0.0f);
        }
    }
}

void Wave::Update(float dt)
{
    static float t = 0.0f;

    t += dt;
    if (t < m_TimeStep)
    {
        return;
    }
    t = 0.0f;

    for (UINT i = 1; i < m_VertexCountZ-1; i++)
    {
        for (UINT j = 1; j < m_VertexCountX - 1; j++)
        {
            m_Pre[i * m_VertexCountX + j].y = (m_Cur[(i - 1) * m_VertexCountX + j].y +
                m_Cur[(i + 1) * m_VertexCountX + j].y +
                m_Cur[i * m_VertexCountX + (j - 1)].y +
                m_Cur[i * m_VertexCountX + (j + 1)].y) * m_K3 +
                m_Pre[i * m_VertexCountX + j].y * m_K1 +
                m_Cur[i * m_VertexCountX + j].y * m_K2;
        }
    }
    std::swap(m_Pre, m_Cur);

    for (UINT i = 1; i < m_VertexCountZ - 1; i++)
    {
        for (UINT j = 1; j < m_VertexCountX - 1; j++)
        {
            float l = m_Cur[(i - 1)* m_VertexCountX + j].y;
            float r = m_Cur[(i + 1)* m_VertexCountX + j].y;
            float t = m_Cur[i* m_VertexCountX + (j - 1)].y;
            float b = m_Cur[i* m_VertexCountX + (j + 1)].y;
            m_Normals[i * m_VertexCountX + j].x = l - r;
            m_Normals[i * m_VertexCountX + j].y = 2.0f * m_Space;
            m_Normals[i * m_VertexCountX + j].z = b - t;

            XMVECTOR n = XMVector3Normalize(XMLoadFloat3(&m_Normals[i * m_VertexCountX + j]));
            XMStoreFloat3(&m_Normals[i * m_VertexCountX + j], n);
        }
    }

}

void Wave::Disturb(UINT i, UINT j, float magnitude)
{
    if (i <= 1 || i > m_VertexCountX - 1 || j <= 1 || j > m_VertexCountZ - 1)
    {
        return;
    }

    float half = magnitude / 2.0f;
    m_Cur[j * m_VertexCountX + i].y = magnitude;
    m_Cur[(j + 1) * m_VertexCountX + i].y = half;
    m_Cur[(j - 1) * m_VertexCountX + i].y = half;
    m_Cur[j * m_VertexCountX + (i + 1)].y = half;
    m_Cur[j * m_VertexCountX + (i - 1)].y = half;
}
