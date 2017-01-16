#include "stdafx.h"
#include "cStart_Scene.h"


cStart_Scene::cStart_Scene()
{
}


cStart_Scene::~cStart_Scene()
{
}

HRESULT cStart_Scene::Scene_Init()
{
	this->Start = RESOURCE_TEXTURE->GetResource("../../Resources/Testures/Player_Etc/start.tga");
	this->Start_Back = RESOURCE_TEXTURE->GetResource("../../Resources/Testures/Player_Etc/startback.tga");
	SOUNDMANAGER->addSound("Intro", "../../Resources/Sounds/IntroMusic.ogg", true, true);
	SOUNDMANAGER->play("Intro");
	return S_OK;
}

void cStart_Scene::Scene_Release()
{
}

void cStart_Scene::Scene_Update(float timDelta)
{
	if (KEY_MGR->IsOnceDown(VK_RETURN)) { SCENE_MGR->ChangeScene("¾À1", 1); }
}

void cStart_Scene::Scene_Render1()
{
}
void cStart_Scene::Scene_RenderSprite()
{
	static float Angle = -0.0f;

	RECT StartB_rc;
	SetRect(&StartB_rc, 0, 0, WINSIZE_X, WINSIZE_Y);
	SPRITE_MGR->DrawTexture(
		this->Start_Back,
		&StartB_rc,
		0,	0,
		WINSIZE_X, WINSIZE_Y,
		Angle,
		0xffffffff,
		&D3DXVECTOR3(0, 0, 0));

	RECT Start_rc;
	SetRect(&Start_rc, 0, 0, 512, 510);
	SPRITE_MGR->DrawTexture(
		this->Start,
		&Start_rc,
		WINSIZE_X / 2, WINSIZE_Y / 2,
		1, 1,
		Angle,
		0xffffffff,
		&D3DXVECTOR3(256, 256, 0));
}

