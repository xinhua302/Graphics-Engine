#include "MathHelper.h"

const float MathHelper::Pi = 3.1415926535f;

XMMATRIX MathHelper::InverseTranspose(CXMMATRIX M)
{
    XMMATRIX A = M;
    A.r[3] = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

    XMVECTOR det = XMMatrixDeterminant(A);
    return XMMatrixTranspose(XMMatrixInverse(&det, A));
        
}

float MathHelper::RandF()
{
    return (float)(rand()) / (float)RAND_MAX;
}

float MathHelper::RandF(float a, float b)
{
    return a + RandF()*(b - a);
}

float MathHelper::CosineInterpolate(float a, float b, float x)
{
    float ft = x * 3.1415927f;
    float f = (1 - cos(ft)) * 0.5f;
    return a*(1 - f) + b*f;
}

float MathHelper::LinearInterpolate(float a, float b, float x)
{
    return a + (b - a)* x;
}

float MathHelper::Rad2Deg(float r)
{
    return r * 180.0f / Pi;
}

float MathHelper::PerlinNoise(float x, float y)
{
    float total = 0;
    float p = 0.025f;
    int n = 2;
    for (int i = 0; i < n; i++)
    {
        float frequency = pow(2.0f, i);
        float amplitude = pow(p, i);
        total = total + InterpolatedNoise(x * frequency, y * frequency) * amplitude;
    }

    return total;
}

float MathHelper::Noise(int x, int y)
{
    int n = x + y * 57;
    n = (n << 13) ^ n;
    return (1.0f - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0f);
}

float MathHelper::SmoothedNoise(int x, int y)
{
    float corners = (Noise(x - 1, y - 1) + Noise(x + 1, y - 1) + Noise(x - 1, y + 1) + Noise(x + 1, y + 1)) / 16;
    float sides = (Noise(x - 1, y) + Noise(x + 1, y) + Noise(x, y - 1) + Noise(x, y + 1)) / 8;
    float center = Noise(x, y) / 4;
    return corners + sides + center;
}

float MathHelper::InterpolatedNoise(float x, float y)
{
    int integer_X = static_cast<int>(x);
    float  fractional_X = x - integer_X;
    int integer_Y = static_cast<int>(y);
    float fractional_Y = y - integer_Y;
    float v1 = SmoothedNoise(integer_X, integer_Y);
    float v2 = SmoothedNoise(integer_X + 1, integer_Y);
    float v3 = SmoothedNoise(integer_X, integer_Y + 1);
    float v4 = SmoothedNoise(integer_X + 1, integer_Y + 1);
    float i1 = LinearInterpolate(v1, v2, fractional_X);
    float i2 = LinearInterpolate(v3, v4, fractional_X);
    return LinearInterpolate(i1, i2, fractional_Y);
}


