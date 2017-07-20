#ifndef __TREESPRITE_H
#define __TREESPRITE_H
#include "../D3dUtility/D3dUtility.h"
#include "BaseObject.h"

class TreeSprite :public Object
{
public:
    TreeSprite(int id);
    virtual ~TreeSprite();

public:
    virtual void Init();
    virtual void Clear();
    virtual void Update(float dt);
    virtual void Render();

private:
    ID3D11Buffer* m_pTreeSpriteVB;
    int m_TreeSpriteCount;

    ID3D11ShaderResourceView* m_TreeSpriteMapSRV;
    Material m_TreeSpriteMat;
};

#endif