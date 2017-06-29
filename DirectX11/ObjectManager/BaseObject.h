#ifndef __BASEOBJECT_H
#define __BASEOBJECT_H
#include "../D3dUtility/D3dUtility.h"
#include "../D3dUtility/D3dApp.h"
#include <map>
using namespace std;

class Object
{
public:
    Object(UINT id);

    virtual ~Object();

public:
    virtual void Init();
    virtual void Clear();
    virtual void Update(float dt);
    virtual void Render();

protected:
    int m_Id;
};

class ObjectManager
{
public: 
    static void InitAll(ID3D11Device* device, ID3D11DeviceContext* context, IDXGISwapChain* swapChain);
    static void DestroyAll();

    static void Update(float dt);
    static void Render();

    static void CreateObject(string name);

    static map<int, Object*> ObjectMap;
    static UINT Id;
    static ID3D11Device*           Device;
    //D3D文本设备
    static ID3D11DeviceContext*    Context;
    //D3D交换链
    static IDXGISwapChain*         SwapChain;
};

#endif