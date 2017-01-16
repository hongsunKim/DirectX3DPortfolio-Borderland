#pragma once

#include "cSceneChangeEffect.h"

class cSceneChangeEffectUpDown : public cSceneChangeEffect
{
public:
	cSceneChangeEffectUpDown();
	~cSceneChangeEffectUpDown();

	virtual void ChangeEffect(LPDIRECT3DTEXTURE9 pPrev, LPDIRECT3DTEXTURE9 pNext, float factor);
};

