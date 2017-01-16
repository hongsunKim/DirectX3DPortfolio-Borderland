#pragma once
#include "cScene.h"
class Ending : public cScene
{
protected:

	LPDIRECT3DTEXTURE9		End;
	LPDIRECT3DTEXTURE9		End2;

public:
	Ending();
	~Ending();

	virtual HRESULT Scene_Init();
	virtual void Scene_Release();
	virtual void Scene_Update(float timDelta);
	virtual void Scene_Render1();
	virtual void Scene_RenderSprite();
};

