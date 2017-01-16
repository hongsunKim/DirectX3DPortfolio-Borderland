#pragma once

#include "cscene.h"

class cBaseObject;
class cCamera;
class cTerrain;
class cSkinnedAnimation;

class cScene_02 : public cScene
{
private:
	cBaseObject*						object;

public:
	cScene_02();
	~cScene_02();


	virtual HRESULT Scene_Init();

	virtual void Scene_Release();

	virtual void Scene_Update(float timeDelta);

	virtual void Scene_Render1();

	virtual void Scene_RenderSprite();
};

