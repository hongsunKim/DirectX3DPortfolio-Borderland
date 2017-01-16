#include "stdafx.h"
#include "Ending.h"


Ending::Ending()
{
}


Ending::~Ending()
{
}

HRESULT Ending::Scene_Init()
{
	SOUNDMANAGER->stop("BossMap");
	this->End = RESOURCE_TEXTURE->GetResource("../../Resources/Testures/Player_Etc/ending.jpg");
	this->End2 = RESOURCE_TEXTURE->GetResource("../../Resources/Testures/Player_Etc/cccc.tga");
	SOUNDMANAGER->addSound("No_Heaven", "../../Resources/Sounds/No_Heaven.ogg", true, true);
	SOUNDMANAGER->play("No_Heaven");

	return S_OK;
}

void Ending::Scene_Release()
{
}

void Ending::Scene_Update(float timDelta)
{
	if (KEY_MGR->IsOnceDown(VK_RETURN)) { SCENE_MGR->ChangeScene("½ºÅ¸Æ®¾À", 1); }
}

void Ending::Scene_Render1()
{
}

void Ending::Scene_RenderSprite()
{

	RECT Ending_rc;
	SetRect(&Ending_rc, 0, 0, WINSIZE_X, WINSIZE_Y);
	SPRITE_MGR->DrawTexture(
		this->End,
		&Ending_rc,
		0, 0,
		1.25, 1.6,
		0,
		0xffffffff,
		&D3DXVECTOR3(0, 0, 0));

	RECT Ending_rc2;
	SetRect(&Ending_rc2, 0,0, 1100,1100);
	SPRITE_MGR->DrawTexture(
		this->End2,
		&Ending_rc2,
		WINSIZE_X / 2 - 50, WINSIZE_Y / 2 - 20,
		0.7, 0.5,
		0,
		0xffffffff,
		&D3DXVECTOR3(500, 400, 0));
}
