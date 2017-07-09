#ifndef __MATHHELPER_H
#define __MATHHELPER_H
#include "D3dUtility.h"

class MathHelper
{
public:
    static XMMATRIX InverseTranspose(CXMMATRIX M);
    static const float Pi;
};


#endif