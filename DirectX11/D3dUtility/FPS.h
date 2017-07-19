#ifndef __FPS_H
#define __FPS_H
#include "D3dUtility.h"
class FPS
{
private:
    static UINT CurFps;
public:
    static UINT CalculateFPS(float dt);
};

#endif