#include "stdafx.h"
#include "UI_Hit.h"


UI_Hit::UI_Hit()
{
}


UI_Hit::~UI_Hit()
{
}

HRESULT UI_Hit::Scene_Init()
{
	this->Hit_Hp_Front = RESOURCE_TEXTURE->GetResource("../../Resources/Testures/Player_Etc/hit.tga");


	this->Hit_Shield_Front = RESOURCE_TEXTURE->GetResource("../../Resources/Testures/Player_Etc/HitShield.tga");

	return S_OK;
}
void UI_Hit::Scene_Release()
{

}
void UI_Hit::Scene_Update(float timDelta)
{

}
void UI_Hit::Scene_Render1()
{

}
void UI_Hit::Scene_RenderSprite()
{

}
void UI_Hit::Hit_HpFront()
{
	static float HpFrontAngle = ����(180);
	RECT Hit_HPFront_rc;
	SetRect(&Hit_HPFront_rc, 0, 1, 511, 130);
	SPRITE_MGR->DrawTexture(
		this->Hit_Hp_Front,
		&Hit_HPFront_rc,
		WINSIZE_X - 150 , 129,
		2, 1,
		HpFrontAngle,
		0x80ffffff,
		&D3DXVECTOR3(0, 0, 0));
}

void UI_Hit::Hit_HpBack()
{
	static float HpBackAngle = ����(0);
	RECT Hit_HPback_rc;
	SetRect(&Hit_HPback_rc, 0, 1, 511, 130);
	SPRITE_MGR->DrawTexture(
		this->Hit_Hp_Front,
		&Hit_HPback_rc,
		WINSIZE_X /2 - 500, WINSIZE_Y - 129,
		2, 1,
		HpBackAngle,
		0x80ffffff,
		&D3DXVECTOR3(0, 0, 0));
}

void UI_Hit::Hit_HpLeft()
{
	static float HpLeftAngle = ����(90);
	RECT Hit_HPLeft_rc;
	SetRect(&Hit_HPLeft_rc, 0, 1, 511, 130);
	SPRITE_MGR->DrawTexture(
		this->Hit_Hp_Front,
		&Hit_HPLeft_rc,
		129, WINSIZE_Y / 6,
		1, 1,
		HpLeftAngle,
		0x80ffffff,
		&D3DXVECTOR3(0, 0, 0));
}

void UI_Hit::Hit_HpRight()
{
	static float HpAngle = ����(270);
	RECT Hit_HPRight_rc;
	SetRect(&Hit_HPRight_rc, 0, 1, 511, 130);
	SPRITE_MGR->DrawTexture(
		this->Hit_Hp_Front,
		&Hit_HPRight_rc,
		WINSIZE_X - 129, WINSIZE_Y / 1.25,
		1, 1,
		HpAngle,
		0x80ffffff,
		&D3DXVECTOR3(0, 0, 0));
}

void UI_Hit::Hit_ShieldFront()
{
	static float ShieldFrontAngle = ����(180);
	RECT Hit_ShieldFront_rc;
	SetRect(&Hit_ShieldFront_rc, 0, 0, 511, 160);
	SPRITE_MGR->DrawTexture(
		this->Hit_Shield_Front,
		&Hit_ShieldFront_rc,
		WINSIZE_X - 150, 160,
		2, 1,
		ShieldFrontAngle,
		0xffffffff,
		&D3DXVECTOR3(0, 0, 0));
}

void UI_Hit::Hit_ShieldBack()
{
	static float ShieldBackAngle = ����(0);
	RECT Hit_Shieldback_rc;
	SetRect(&Hit_Shieldback_rc, 0, 0, 511, 160);
	SPRITE_MGR->DrawTexture(
		this->Hit_Shield_Front,
		&Hit_Shieldback_rc,
		WINSIZE_X / 2 - 500, WINSIZE_Y - 160,
		2, 1,
		ShieldBackAngle,
		0xffffffff,
		&D3DXVECTOR3(0, 0, 0));

	
}

void UI_Hit::Hit_ShieldLeft()
{
	static float ShieldLeftAngle = ����(90);
	RECT Hit_ShieldLeft_rc;
	SetRect(&Hit_ShieldLeft_rc, 0, 0, 511, 160);
	SPRITE_MGR->DrawTexture(
		this->Hit_Shield_Front,
		&Hit_ShieldLeft_rc,
		160, WINSIZE_Y / 6,
		1, 1,
		ShieldLeftAngle,
		0xffffffff,
		&D3DXVECTOR3(0, 0, 0));
}

void UI_Hit::Hit_ShieldRight()
{
	static float ShieldAngle = ����(270);
	RECT Hit_ShieldRight_rc;
	SetRect(&Hit_ShieldRight_rc, 0, 0, 511, 160);
	SPRITE_MGR->DrawTexture(
		this->Hit_Shield_Front,
		&Hit_ShieldRight_rc,
		WINSIZE_X - 160, WINSIZE_Y / 1.25,
		1, 1,
		ShieldAngle,
		0xffffffff,
		&D3DXVECTOR3(0, 0, 0));
}


//��Ʈ�� �ǵ� �� ���������� ���� ���ߴ� �Ķ����̵Ǿ����