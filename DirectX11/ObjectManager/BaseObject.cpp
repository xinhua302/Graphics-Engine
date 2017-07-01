#include "BaseObject.h"
#include "Land.h"
#include "Box.h"
#include "Water.h"

Object::Object(UINT id)
{
    m_Id = id;
}

Object::~Object()
{
    
}

void Object::Init()
{

}

void Object::Clear()
{

}

void Object::Update(float dt)
{

}

void Object::Render()
{

}

void ObjectManager::InitAll(ID3D11Device* device, ID3D11DeviceContext* context, IDXGISwapChain* swapChain)
{
    ObjectManager::Device = device;
    ObjectManager::Context = context;
    ObjectManager::SwapChain = swapChain;
}

void ObjectManager::DestroyAll()
{
    auto it = ObjectMap.begin();
    while (it != ObjectMap.end())
    {
        SafeDelete(it->second);
        it++;
    }
}

void ObjectManager::Update(float dt)
{
    auto it = ObjectMap.begin();
    while (it != ObjectMap.end())
    {
        it->second->Update(dt);
        it++;
    }
}

void ObjectManager::Render()
{
    auto it = ObjectMap.begin();
    while (it != ObjectMap.end())
    {
        it->second->Render();
        it++;
    }
}

void ObjectManager::CreateObject(string name)
{
    Object* object;
    if (name == "Land")
    {
        object = new Land(Id);
    }
    else if (name == "Box")
    {
        object = new Box(Id);
    }
    else if (name == "Water")
    {
        object = new Water(Id);
    }
    ObjectMap.insert(make_pair(Id, object));
    Id++;
}

map<int, Object*> ObjectManager::ObjectMap;

UINT ObjectManager::Id = 0;

ID3D11Device* ObjectManager::Device = nullptr;

ID3D11DeviceContext* ObjectManager::Context = nullptr;

IDXGISwapChain* ObjectManager::SwapChain = nullptr;
