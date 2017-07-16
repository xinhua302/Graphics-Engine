#ifndef __MATHHELPER_H
#define __MATHHELPER_H
#include "D3dUtility.h"

class MathHelper
{
public:
    static XMMATRIX InverseTranspose(CXMMATRIX M);
    static const float Pi;

    static float RandF()
    {
        return (float)(rand()) / (float)RAND_MAX;
    }

    //[a, b).
    static float RandF(float a, float b)
    {
        return a + RandF()*(b - a);
    }

    static float PerlinNoise(float x, float y)   // 获取插值噪声  
    {
        int integer_X = static_cast<int>(x);
        float  fractional_X = x - integer_X;
        int integer_Y = static_cast<int>(y);
        float fractional_Y = y - integer_Y;
        float v1 = SmoothedNoise(integer_X, integer_Y);
        float v2 = SmoothedNoise(integer_X + 1, integer_Y);
        float v3 = SmoothedNoise(integer_X, integer_Y + 1);
        float v4 = SmoothedNoise(integer_X + 1, integer_Y + 1);
        float i1 = CosineInterpolate(v1, v2, fractional_X);
        float i2 = CosineInterpolate(v3, v4, fractional_X);
        return CosineInterpolate(i1, i2, fractional_Y);
    }

private:
    static float Noise(int x, int y)    // 根据(x,y)获取一个初步噪声值  
    {
        int n = x + y * 57;
        n = (n << 13) ^ n;
        return (1.0f - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0f);
    }

    static float SmoothedNoise(int x, int y)   //光滑噪声  
    {
        float corners = (Noise(x - 1, y - 1) + Noise(x + 1, y - 1) + Noise(x - 1, y + 1) + Noise(x + 1, y + 1)) / 16;
        float sides = (Noise(x - 1, y) + Noise(x + 1, y) + Noise(x, y - 1) + Noise(x, y + 1)) / 8;
        float center = Noise(x, y) / 4;
        return corners + sides + center;
    }

    static float CosineInterpolate(float a, float b, float x)  // 余弦插值  
    {
        float ft = x * 3.1415927f;
        float f = (1 - cos(ft)) * 0.5f;
        return a*(1 - f) + b*f;
    }
};


#endif