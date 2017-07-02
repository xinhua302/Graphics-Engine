#ifndef __LIGHTMANAGER_H
#define __LIGHTMANAGER_H
#include "D3dUtility.h"
class LightManager
{
public:
    static void InitAll();
    static void DestroyAll();

    static void CreateLight();
    static void Apply();

    static DirectionalLight Light[3];
};
#endif