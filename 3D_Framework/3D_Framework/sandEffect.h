#pragma once

#include "cscene.h"

class cParticleEmitter;

class sandEffect
{
private:
	cParticleEmitter* Sand_Particle;

public:
	sandEffect();
	~sandEffect();

	virtual HRESULT Scene_Init();

	virtual void Scene_Release();

	virtual void Scene_Update(float timDelta);

	virtual void Scene_Render1();


	virtual void Scene_RenderSprite();
};
