#pragma once
#include "cScene.h"
class cStart_Scene : public cScene
{
protected:

	LPDIRECT3DTEXTURE9		Start;
	LPDIRECT3DTEXTURE9		Start_Back;

public:
	cStart_Scene();
	~cStart_Scene();

	virtual HRESULT Scene_Init();
	virtual void Scene_Release();
	virtual void Scene_Update(float timDelta);
	virtual void Scene_Render1();
	virtual void Scene_RenderSprite();
};



