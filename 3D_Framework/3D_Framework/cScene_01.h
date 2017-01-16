#pragma once

#include "cscene.h"

class cBaseObject;
class cCamera;
class cTerrain;
class cSkinnedAnimation;

#define OBJECT_NUM 30

class cScene_01 : public cScene
{
private:
	std::vector<cBaseObject*>			renderObjects;			//���� ��ġ�� ���� ������Ʈ �迭
	std::vector<cBaseObject*>			cullObjects;			//�ø��� ������Ʈ


	cTerrain*		m_pTerrain;
	cTerrain*		m_pTerrainShadow;


public:
	cScene_01();
	~cScene_01();


	virtual HRESULT Scene_Init();

	virtual void Scene_Release();

	virtual void Scene_Update(float timeDelta);

	virtual void Scene_Render1();

	virtual void Scene_RenderSprite();
};

