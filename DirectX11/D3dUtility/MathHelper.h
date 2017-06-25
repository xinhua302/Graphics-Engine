#ifndef __MATHHELPER_H
#define __MATHHELPER_H

class MathHelper
{
public:
    static XMMATRIX InverseTranspose(CXMMATRIX M)
    {
        XMMATRIX A = M;
        A.r[3] = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

        XMVECTOR det = XMMatrixDeterminant(A);
        return XMMatrixTranspose(XMMatrixInverse(&det, A));
    }
};


#endif