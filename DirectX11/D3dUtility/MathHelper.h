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

    // Returns random float in [a, b).
    static float RandF(float a, float b)
    {
        return a + RandF()*(b - a);
    }
};


#endif