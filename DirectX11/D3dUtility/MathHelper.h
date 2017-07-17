#ifndef __MATHHELPER_H
#define __MATHHELPER_H
#include "D3dUtility.h"

class MathHelper
{
public:
    static const float Pi;

    //逆转置矩阵
    static XMMATRIX InverseTranspose(CXMMATRIX M);

    //随机[0, 1)
    static float RandF();

    // 余弦插值  
    static float CosineInterpolate(float a, float b, float x);

    //线性插值
    static float LinearInterpolate(float a, float b, float x);

    //随机[a, b)
    static float RandF(float a, float b);
    
    //弧度转角度
    static float Rad2Deg(float r);

    //柏林噪声
    static float PerlinNoise(float x, float y);    

private:
    // 根据(x,y)获取一个初步噪声值  
    static float Noise(int x, int y);

    //光滑噪声  
    static float SmoothedNoise(int x, int y);

    // 获取插值噪声  
    static float InterpolatedNoise(float x, float y);

};


#endif