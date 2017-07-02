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

void ObjectManager::InitAll()
{
    DestroyAll();
    Id = 0;
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
    else if (name == "BoxReflection")
    {
        object = new Box(Id, 1);
    }
    else if (name == "Box")
    {
        object = new Box(Id, 0);
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
