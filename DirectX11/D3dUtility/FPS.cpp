#include "FPS.h"

UINT FPS::CalculateFPS(float dt)
{
    static UINT fps = 0;
    static float t = 0.0f;

    t += dt;
    fps++;


    if (t > 1.0f)
    {
        CurFps = fps;
        t = 0.0f;
        fps = 0;
    }

    return CurFps;
}

UINT FPS::CurFps = 0;
