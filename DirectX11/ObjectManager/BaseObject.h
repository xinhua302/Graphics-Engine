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
    static void InitAll();
    static void DestroyAll();

    static void Update(float dt);
    static void Render();

    static void CreateObject(string name);

    static map<int, Object*> ObjectMap;
    static UINT Id;
};

#endif