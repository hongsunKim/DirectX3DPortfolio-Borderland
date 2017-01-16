#pragma once
#include "cScene.h"

class cParticleEmitter;

class ItemEffect
{
private:
//	cParticleEmitter* Sand_Particle;

public:
	cParticleEmitter* Item_Particle;

	ItemEffect();
	~ItemEffect();


	virtual HRESULT Scene_Init(int Item_Type);

	virtual void Scene_Release();

	virtual void Scene_Update(float timDelta);

	virtual void Scene_Render1();

	virtual void Scene_RenderSprite();

	void SetPos(D3DXVECTOR3* setpos);
};

