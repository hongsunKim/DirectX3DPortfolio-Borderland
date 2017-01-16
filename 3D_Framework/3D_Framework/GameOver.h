
#pragma once
#include "cScene.h"
class GameOver : public cScene
{
protected:

	LPDIRECT3DTEXTURE9		GameOverScene;
	LPDIRECT3DTEXTURE9		GameOverScene_Back;
	LPD3DXFONT				Font;

public:

	GameOver();
	~GameOver();

	virtual HRESULT Scene_Init();
	virtual void Scene_Release();
	virtual void Scene_Update(float timDelta);
	virtual void Scene_Render1();
	virtual void Scene_RenderSprite();
};

