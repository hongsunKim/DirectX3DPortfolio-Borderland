#pragma once

#include "cScene.h"
#include "cCamera.h"

class UI_Hit
{
protected:
	
	LPDIRECT3DTEXTURE9		Hit_Hp_Front;
	LPDIRECT3DTEXTURE9		Hit_Shield_Front;

public:
	UI_Hit();
	~UI_Hit();

	HRESULT Scene_Init();

	virtual void Scene_Release();

	virtual void Scene_Update(float timDelta);

	virtual void Scene_Render1();

	virtual void Scene_RenderSprite();

	void Hit_HpFront();

	void Hit_HpBack();

	void Hit_HpLeft();

	void Hit_HpRight();

	void Hit_ShieldFront();

	void Hit_ShieldBack();

	void Hit_ShieldLeft();

	void Hit_ShieldRight();
};
