#ifndef __MATHHELPER_H
#define __MATHHELPER_H
#include "D3dUtility.h"

class MathHelper
{
public:
    static const float Pi;

    //��ת�þ���
    static XMMATRIX InverseTranspose(CXMMATRIX M);

    //���[0, 1)
    static float RandF();

    // ���Ҳ�ֵ  
    static float CosineInterpolate(float a, float b, float x);

    //���Բ�ֵ
    static float LinearInterpolate(float a, float b, float x);

    //���[a, b)
    static float RandF(float a, float b);
    
    //����ת�Ƕ�
    static float Rad2Deg(float r);

    //��������
    static float PerlinNoise(float x, float y);    

private:
    // ����(x,y)��ȡһ����������ֵ  
    static float Noise(int x, int y);

    //�⻬����  
    static float SmoothedNoise(int x, int y);

    // ��ȡ��ֵ����  
    static float InterpolatedNoise(float x, float y);

};


#endif