#include "stdafx.h"
#include "Ui_Bar.h"

Ui_Bar::Ui_Bar()
	: Hp_Max(0)
	, Hp_Current(0)
	, Enemy_Max(0)
	, Enemy_Current(0)
	, shield_Current(0)
	, shield_Max(0)
	, Bullet_Current(0)
	, Bullet_Max(0)
	, Grenade_Current(0)
	, Grenade_Max(0)
	, Exp_Max(0)
	, Exp_Current(0)
{
}


Ui_Bar::~Ui_Bar()
{
}

HRESULT Ui_Bar::Scene_Init()

{
	//	_Max2 = Max_Hp;
	//	_Current2 = cur_Hp;

		//////////////////////////////체력//////////////////////////////////////////////
	this->HpBar = RESOURCE_TEXTURE->GetResource("../../Resources/Testures/HP/HpBar.tga");
	this->HpBar2 = RESOURCE_TEXTURE->GetResource("../../Resources/Testures/HP/HpBar.tga");
	this->HpBack = RESOURCE_TEXTURE->GetResource("../../Resources/Testures/HP/BackHp.tga");
	this->HpIcon = RESOURCE_TEXTURE->GetResource("../../Resources/Testures/HP/Hp_icon.tga");
	this->HpIconBack = RESOURCE_TEXTURE->GetResource("../../Resources/Testures/HP/Hp_iconBack.tga");
	this->LowHp = RESOURCE_TEXTURE->GetResource("../../Resources/Testures/HP/Low_Hp.tga");




	this->HpBackShake.SetLocalPosition(D3DXVECTOR3(97, 703, 0));
	this->HpBackShake.ShakePos(D3DXVECTOR3(97, 703, 0));

	this->HpShake.SetLocalPosition(D3DXVECTOR3(134, 697, 0));
	this->HpShake.ShakePos(D3DXVECTOR3(134, 697, 0));
	////////////////////////////////////////////////////////////////////////////////////////////

	///////////////////////////////Shield///////////////////////////////////////////////////////
	this->Shield_Bar = RESOURCE_TEXTURE->GetResource("../../Resources/Testures/Shield/ShieldBar.tga");
	this->Shield_Bar2 = RESOURCE_TEXTURE->GetResource("../../Resources/Testures/Shield/ShieldBar.tga");
	this->Shield_Back = RESOURCE_TEXTURE->GetResource("../../Resources/Testures/Shield/BackShield.tga");
	this->Shield_Icon = RESOURCE_TEXTURE->GetResource("../../Resources/Testures/Shield/ShieldIcon.tga");
	this->Shield_IconBack = RESOURCE_TEXTURE->GetResource("../../Resources/Testures/Shield/Shield_icon_Back.tga");
	this->Low_Shield = RESOURCE_TEXTURE->GetResource("../../Resources/Testures/Shield/Low_Shield.tga");


	this->Shield_Shake.SetLocalPosition(D3DXVECTOR3(134, 670, 0));
	this->Shield_Shake.ShakePos(D3DXVECTOR3(134, 670, 0));

	this->Shield_BackShake.SetLocalPosition(D3DXVECTOR3(107, 676, 0));
	this->Shield_BackShake.ShakePos(D3DXVECTOR3(107, 676, 0));


	//////////////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////Bullet//////////////////////////////////////////////////////
	this->bullet_Bar = RESOURCE_TEXTURE->GetResource("../../Resources/Testures/bullet/bullet_bar.tga");
	this->bullet_Back = RESOURCE_TEXTURE->GetResource("../../Resources/Testures/bullet/BackBullet.tga");

	this->ReLoad = RESOURCE_TEXTURE->GetResource("../../Resources/Testures/bullet/RELOAD.tga");


	this->Sna_Ammo = RESOURCE_TEXTURE->GetResource("../../Resources/Testures/Player_Etc/Sna_ammo.tga");
	this->Pistol_Ammo = RESOURCE_TEXTURE->GetResource("../../Resources/Testures/Player_Etc/pistol_ammo.tga");


	this->bullet_Shake.SetLocalPosition(D3DXVECTOR3(1130, 690, 0));
	this->bullet_Shake.ShakePos(D3DXVECTOR3(1130, 690, 0));

	this->bullet_BackShake.SetLocalPosition(D3DXVECTOR3(910, 680, 0));
	this->bullet_BackShake.ShakePos(D3DXVECTOR3(910, 680, 0));
	////////////////////////////////////////////////////////////////////////////////////////////
	this->EXP_Bar = RESOURCE_TEXTURE->GetResource("../../Resources/Testures/Player_Etc/EXP.tga");

	/////////////////////////////////////////////////////////////////////////////////////////

	this->Enemy_bar = RESOURCE_TEXTURE->GetResource("../../Resources/Testures/Enemy/E_bar2.tga");

	/////////////////////////////////////////////////////////////////////////////////////////
	this->Grenade_Icon = RESOURCE_TEXTURE->GetResource("../../Resources/Testures/Player_Etc/ammo.tga");
	this->Grenade_Num = RESOURCE_TEXTURE->GetResource("../../Resources/Testures/Player_Etc/Grenade.tga");
	///////////////////////////////////////////////////////////////////////////////////////////////////
	this->Last_Chance = RESOURCE_TEXTURE->GetResource("../../Resources/Testures/Player_Etc/Last_bar.tga");
	this->Last_Tex = RESOURCE_TEXTURE->GetResource("../../Resources/Testures/Player_Etc/Last_Tex.tga");
	this->Last_bar = RESOURCE_TEXTURE->GetResource("../../Resources/Testures/Player_Etc/Last_bar2.tga");
	/////////////////////////////////////////////////////////////////////////////////////////
	this->Mouse_Aim = RESOURCE_TEXTURE->GetResource("../../Resources/Testures/Player_Etc/Aim.tga");
	this->Taget_Aim = RESOURCE_TEXTURE->GetResource("../../Resources/Testures/Player_Etc/TagetAim.tga");
	this->cPick = RESOURCE_TEXTURE->GetResource("../../Resources/Testures/Player_Etc/UseMe_Ring.tga");
	//////////////////////////////////////////////////////////////////////////////////////
	BackShake_f = 3.0f;
	Shake_f = 3.0f;

	TimeMax = 500;
	TimeCek = 500;

	D3DXCreateFont(
		Device,				//디바이스
		60.f,				//폰트의 높이
		0.f,				//폰트의 폭(이 값이 0이면 앞에서 설정한 폰트 높이에 비례)
		FW_BOLD,			//폰트의 두께 설정(FW_BOLD 폰트의 BOLD 스타일 크기를 갖는다)
		1,					//밉맵(그냥 1)
		FALSE,				//이텔릭 여부
		DEFAULT_CHARSET,	//캐릭터 set( 그냥 DEFAULT_CHARSET )
		OUT_DEFAULT_PRECIS,	//외각 처리방식 ( 그냥 OUT_DEFAULT_PRECIS )
		DEFAULT_QUALITY,	//퀄리티 ( 그냥 DEFAULT_QUALITY )
		DEFAULT_PITCH | FF_DONTCARE,// 글씨체 패밀리셋 설정
		"Cambria",			//사용될 폰트 이름 
		&cFont			//생성된 폰트객체 받을 더블 포인터
	);
	SOUNDMANAGER->addSound("Shield_Power_Down", "../../Resources/Sounds/Shield_Power_Down.ogg", false, false);
	SOUNDMANAGER->addSound("Shield_Power_Up", "../../Resources/Sounds/Shield_Power_Up.ogg", false, false);
	SOUNDMANAGER->addSound("death", "../../Resources/Sounds/death.ogg", false, false);

	if (shield_Current == 0) {

		SOUNDMANAGER->play("Shield_Power_Down");
		shield_Current = 0.1f;
	}

	_Font = new Font;
	_Font->Init();

	return S_OK;
}

HRESULT Ui_Bar::Enemy_Init(float current, float Max)
{

	Enemy_Max = Max;
	Enemy_Current = current;
	return S_OK;
}

void Ui_Bar::Player_SetUp(UIINFO st)
{
	Hp_Max = st.FullHp;
	Hp_Current = st.CurHp;
	shield_Current = st.CurShield;
	shield_Max = st.FullShield;
	Bullet_Current = st.Bullet_Current;
	Bullet_Max = st.Bullet_Reload_Max;
	Grenade_Current = st.Grenade_Current;
	Grenade_Max = st.Grenade_Max;
	Exp_Current = st.Exp_Current;
	Exp_Max = st.Exp_Max;
}

void Ui_Bar::Scene_Release()
{
	_Font->Release();
	SAFE_DELETE(_Font);

}

void Ui_Bar::Scene_Update(float timeDelta)
{
	this->HpShake.ShakeUpdate(timeDelta);
	this->HpBackShake.ShakeUpdate(timeDelta);
	this->Shield_Shake.ShakeUpdate(timeDelta);
	this->Shield_BackShake.ShakeUpdate(timeDelta);
	if (Hp_Current <= 0) {
		if (TimeCek <= 500)
			TimeCek = TimeCek--;
		if (TimeCek <= 0) TimeCek = 0;
	}
	if (TimeCek == 0)
	{
		SCENE_MGR->ChangeScene("게임오버", 1);
	}
	if (TimeCek == 0) { SOUNDMANAGER->play("death"); }
}
void Ui_Bar::Update(/*UIINFO current,*/float EnemyCurrent)
{
	Enemy_Current = EnemyCurrent;

	//	Hp_Current = current.CurHp;
	////	Hp_Max = current.FullHp;
	//	shield_Current = current.CurShield;
	//
	//	Bullet_Current = current.Bullet_Current;
	//
	//	Grenade_Current = current.Grenade_Current;
	//
	//	Exp_Current = current.Exp_Current;


}
void Ui_Bar::Scene_Render1()
{

}


void Ui_Bar::Scene_RenderSprite()
{

}


void Ui_Bar::HpRender()
{

	///////////////////////////////////////////////HP//////////////////////////////////////////////////////
	static float HpAngle = -0.05f;

	float Hp_per = Hp_Current / Hp_Max;


	//	if (Hp_per <= 0) Hp_per = 0;
		//if (per >= 1.0f) per = 1.0f;

	RECT Hp_rc;
	RECT Hp_rc2;
	RECT Hp_Back_rc;
	RECT Hp_Icon_rc;
	RECT Low_Hp_rc;
	RECT Hp_Icon_Back_rc;
	RECT Last_bar_rc;
	RECT Last_bar2_rc;
	//	SetRect(&rc, 50 * per, 0, 255 , 32);
	SetRect(&Hp_rc, 0, 0, 255, 32);
	SetRect(&Hp_rc2, 0, 0, 255, 32);
	SetRect(&Hp_Back_rc, 0, 0, 255, 32);
	SetRect(&Hp_Icon_rc, 0, 0, 35, 35);
	SetRect(&Hp_Icon_Back_rc, 0, 0, 60, 60);
	SetRect(&Low_Hp_rc, 0, 0, 35, 35);
	SetRect(&Last_bar_rc, 0, 0, 500, 130);
	SetRect(&Last_bar2_rc, 0, 0, 500, 130);


	SPRITE_MGR->DrawTexture(
		this->HpBack,
		&Hp_Back_rc,
		this->HpBackShake.GetLocalPosition().x, this->HpBackShake.GetLocalPosition().y,
		1.1, 1,
		HpAngle,
		0xffffffff,
		&D3DXVECTOR3(0, 0, 0));


	SPRITE_MGR->DrawTexture(
		this->HpBar,
		&Hp_rc,
		this->HpShake.GetLocalPosition().x, this->HpShake.GetLocalPosition().y,
		1 * Hp_per, 1,
		HpAngle,
		0xffffffff,
		&D3DXVECTOR3(0, 0, 0));




	if (Hp_Current < 30)
	{
		float Test1 = 0.2;
		float Test2 = 0.8;
		float Test3;

		int Test4 = TIME_MGR->GetFrameCountSec() / 0.125;



		if (Test4 % 2 == 1)
			Test3 = Test1;
		else
			Test3 = Test2;

		SPRITE_MGR->DrawTexture(
			this->HpBar2,
			&Hp_rc2,
			this->HpShake.GetLocalPosition().x, this->HpShake.GetLocalPosition().y,
			1 * Hp_per, 1,
			HpAngle,
			0xff000000 * Test3,
			&D3DXVECTOR3(0, 0, 0));



		SPRITE_MGR->DrawTexture(
			this->LowHp,
			&Low_Hp_rc,
			this->HpShake.GetLocalPosition().x + 245, this->HpShake.GetLocalPosition().y - 15,
			0.8, 0.8,
			HpAngle,
			0xffffffff * Test3,
			&D3DXVECTOR3(0, 0, 0));

		this->HpBackShake.ShakePos(BackShake_f, BackShake_f);
		this->HpShake.ShakePos(Shake_f, Shake_f);

	}
	if (Hp_Current <= 0) {
		if (BackShake_f < 1) {
			this->HpBackShake.ShakePos(0.0f, 0.0f);
		}
		else {
			BackShake_f -= 0.05f;
			this->HpBackShake.ShakePos(BackShake_f, BackShake_f);
		}

		if (Shake_f < 1) {
			this->HpShake.ShakePos(0.0f, 0.0f);
		}
		else {
			Shake_f -= 0.05f;
			this->HpShake.ShakePos(Shake_f, Shake_f);
		}
		this->HpBackShake.SetLocalPosition(D3DXVECTOR3(97, 703, 0));
		this->HpShake.SetLocalPosition(D3DXVECTOR3(134, 697, 0));


	}


	SPRITE_MGR->DrawTexture(
		this->HpIcon,
		&Hp_Icon_rc,
		95, 698,
		1, 1,
		HpAngle,
		0xffffffff,
		&D3DXVECTOR3(0, 0, 0));

	SPRITE_MGR->DrawTexture(
		this->HpIconBack,
		&Hp_Icon_Back_rc,
		65, 700,
		0.8, 0.8,
		HpAngle,
		0xffffffff,
		&D3DXVECTOR3(0, 0, 0));

	if (Hp_Current <= 0) {

		float TimePer = TimeCek / TimeMax;
		SPRITE_MGR->DrawTexture(
			this->Last_Chance,
			&Last_bar_rc,
			WINSIZE_X / 2, WINSIZE_Y / 2,
			1, 1,
			0.0f,
			0x95ffffff,
			&D3DXVECTOR3(250, 0, 0));

		SPRITE_MGR->DrawTexture(
			this->Last_bar,
			&Last_bar2_rc,
			WINSIZE_X / 2 - 140, WINSIZE_Y / 2 + 22,
			0.75 * TimePer, 0.65,
			0.0f,
			0x90ff0000,
			&D3DXVECTOR3(0, 0, 0));

		RECT rcText3 = { WINSIZE_X / 2 - 130, WINSIZE_Y / 2 - 10, WINSIZE_X / 2  , WINSIZE_Y / 2 };

		cFont->DrawText(
			NULL,
			"최후의 저항",
			-1,
			&rcText3,
			DT_LEFT | DT_NOCLIP,
			0xffffffff
		);
		//	SPRITE_MGR->DrawTexture(
		//		this->Last_Tex,
		//		&Last_bar_rc,
		//		WINSIZE_X / 2, WINSIZE_Y / 2,
		//		1, 1,
		//		0.0f,
		//		0xffffffff,
		//		&D3DXVECTOR3(250, 0, 0));
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

void Ui_Bar::ShieldRender()
{
	float Shield_per = shield_Current / shield_Max;



	RECT Shield_Bar_rc;
	SetRect(&Shield_Bar_rc, 0, 0, 255, 32);
	RECT Shield_Bar_rc2;
	SetRect(&Shield_Bar_rc2, 0, 0, 255, 32);
	RECT Shield_Back_rc;
	SetRect(&Shield_Back_rc, 0, 0, 255, 30);
	RECT Shield_Icon_rc;
	SetRect(&Shield_Icon_rc, 0, 0, 35, 35);
	RECT Shield_IconBack_rc;
	SetRect(&Shield_IconBack_rc, 0, 0, 60, 60);
	RECT Low_Shield_rc;
	SetRect(&Low_Shield_rc, 0, 0, 35, 35);
	static float Shield_Angle = -0.05f;


	SPRITE_MGR->DrawTexture(
		this->Shield_Back,
		&Shield_Back_rc,
		this->Shield_BackShake.GetLocalPosition().x, this->Shield_BackShake.GetLocalPosition().y,
		0.75, 1,
		Shield_Angle,
		0xffffffff,
		&D3DXVECTOR3(0, 0, 0));

	SPRITE_MGR->DrawTexture(
		this->Shield_Bar,
		&Shield_Bar_rc,
		this->Shield_Shake.GetLocalPosition().x, this->Shield_Shake.GetLocalPosition().y,
		0.9 * Shield_per, 1,
		Shield_Angle,
		0xffffffff,
		&D3DXVECTOR3(0, 0, 0));
	if (shield_Current == 1)
	{
		SOUNDMANAGER->play("Shield_Power_Up");
	}
	if (shield_Current < Player_Full_Shield/3)
	{
		float Test1 = 0.2;
		float Test2 = 0.8;
		float Test3;

		int Test4 = TIME_MGR->GetFrameCountSec() / 0.125;

		if (Test4 % 2 == 1)
			Test3 = Test1;
		else
			Test3 = Test2;

		SPRITE_MGR->DrawTexture(
			this->Shield_Bar2,
			&Shield_Bar_rc2,
			this->Shield_Shake.GetLocalPosition().x, this->Shield_Shake.GetLocalPosition().y,
			0.9 * Shield_per, 1,
			Shield_Angle,
			0xff0000ff * Test3,
			&D3DXVECTOR3(0, 0, 0));


		SPRITE_MGR->DrawTexture(
			this->Low_Shield,
			&Low_Shield_rc,
			this->Shield_Shake.GetLocalPosition().x + 245, this->Shield_Shake.GetLocalPosition().y - 15,
			0.8, 0.8,
			Shield_Angle,
			0xffffffff * Test3,
			&D3DXVECTOR3(0, 0, 0));

		this->Shield_BackShake.ShakePos(BackShake_f, BackShake_f);
		this->Shield_Shake.ShakePos(Shake_f, Shake_f);

	}
	if (shield_Current < 0) {
		if (BackShake_f > 1) {
			this->Shield_BackShake.ShakePos(0.0f, 0.0f);
		}
		else {
			BackShake_f -= 0.05f;
			this->Shield_BackShake.ShakePos(BackShake_f, BackShake_f);
		}

		if (Shake_f > 1) {
			this->Shield_Shake.ShakePos(0.0f, 0.0f);
		}
		else {
			Shake_f -= 0.05f;
			this->Shield_Shake.ShakePos(Shake_f, Shake_f);
		}
		this->Shield_BackShake.SetLocalPosition(D3DXVECTOR3(107, 676, 0));
		this->Shield_Shake.SetLocalPosition(D3DXVECTOR3(134, 670, 0));


	}



	SPRITE_MGR->DrawTexture(
		this->Shield_Icon,
		&Shield_Icon_rc,
		95, 668,
		1, 1,
		Shield_Angle,
		0xffffffff,
		&D3DXVECTOR3(0, 0, 0));

	SPRITE_MGR->DrawTexture(
		this->Shield_IconBack,
		&Shield_IconBack_rc,
		70, 669,
		1.2, 0.7,
		Shield_Angle,
		0xffffffff,
		&D3DXVECTOR3(0, 0, 0));
}

void Ui_Bar::BulletRender()
{
	float Bullet_per = Bullet_Current / Bullet_Max;
	RECT Bullet_Bar_rc;
	SetRect(&Bullet_Bar_rc, 255 * Bullet_per, 0, 0, 32);
	RECT Bullet_BarBack_rc;
	SetRect(&Bullet_BarBack_rc, 0, 0, 255, 32);
	static float Bullet_Angle = 0.05f;
	RECT Reload_rc;
	SetRect(&Reload_rc, 0, 0, 260, 60);

	SPRITE_MGR->DrawTexture(
		this->bullet_Back,
		&Bullet_BarBack_rc,
		this->bullet_BackShake.GetLocalPosition().x, this->bullet_BackShake.GetLocalPosition().y,
		1, 1.1,
		Bullet_Angle,
		0xffffffff,
		&D3DXVECTOR3(0, 0, 0));
	/*if (Bullet_Current < -5)
	{

		this->bullet_BackShake.ShakePos(BackShake_f, BackShake_f);
		this->bullet_Shake.ShakePos(Shake_f, Shake_f);

		//		if (Bullet_Current <= 0) {
		//			if (BackShake_f <= 1) {
		//				this->Shield_BackShake.ShakePos(0.0f, 0.0f);
		//			}
		//			else {
		//				BackShake_f -= 0.2f;
		//				this->Shield_BackShake.ShakePos(BackShake_f, BackShake_f);
		//			}
		//
		//			if (Shake_f <= 1) {
		//				this->Shield_Shake.ShakePos(0.0f, 0.0f);
		//			}
		//			else {
		//				Shake_f -= 0.2f;
		//				this->Shield_Shake.ShakePos(Shake_f, Shake_f);
		//			}
		//
		//			this->bullet_BackShake.SetLocalPosition(D3DXVECTOR3(910, 680, 0));
		//			this->bullet_Shake.SetLocalPosition(D3DXVECTOR3(902, 675, 0));
		//
		//
		//		}
	}*/

	SPRITE_MGR->DrawTexture(
		this->bullet_Bar,
		&Bullet_Bar_rc,
		this->bullet_Shake.GetLocalPosition().x, this->bullet_Shake.GetLocalPosition().y,
		0.9, 0.9,
		Bullet_Angle,
		0xffffffff,
		&D3DXVECTOR3(0, 0, 0));


	if (Bullet_Current < Bullet_Max / 4)
	{
		float Test1 = 0.2;
		float Test2 = 0.8;
		float Test3;

		int Test4 = TIME_MGR->GetFrameCountSec() / 0.125;


		if (Test4 % 2 == 1)
			Test3 = Test1;
		else
			Test3 = Test2;

		SPRITE_MGR->DrawTexture(
			this->ReLoad,
			&Reload_rc,
			WINSIZE_X / 2, WINSIZE_Y / 2,
			0.6, 0.6,
			0.0f,
			0xffffffff * -Test3,
			&D3DXVECTOR3(0, 0, 0));

	}


}

void Ui_Bar::Player_EXP()
{
	float EXP_per = Exp_Current / Exp_Max;
	static float EXP_angle = 0.0f;
	RECT EXP_rc;
	SetRect(&EXP_rc, 0, 0, 512 * EXP_per, 16);

	SPRITE_MGR->DrawTexture(
		this->EXP_Bar,
		&EXP_rc,
		WINSIZE_X / 2 - 257, WINSIZE_Y / 2 + 300,
		1, 1,
		EXP_angle,
		0xffffffff,
		&D3DXVECTOR3(0, 0, 0));

}

void Ui_Bar::Enemy_HpRender()
{

	float Enemy_per = Enemy_Current / Enemy_Max;


	static float EnemyHP_angle = 0.0f;
	RECT E_bar;
	RECT E_back;
	SetRect(&E_bar, 0, 0, 128 * Enemy_per, 16);
	SetRect(&E_back, 0, 0, 128, 16);

	SPRITE_MGR->DrawTexture(
		this->Enemy_bar,
		&E_back,
		WINSIZE_X / 2 - 100, WINSIZE_Y / 2 - 250,
		1.5, 1.5,
		EnemyHP_angle,
		0x6000ffff,
		&D3DXVECTOR3(0, 0, 0));

	SPRITE_MGR->DrawTexture(
		this->Enemy_bar,
		&E_bar,
		WINSIZE_X / 2 - 100, WINSIZE_Y / 2 - 250,
		1.5, 1.5,
		EnemyHP_angle,
		0xffffffff,
		&D3DXVECTOR3(0, 0, 0));



}

void Ui_Bar::Grenade_Render()
{
	float Grenade_per = Grenade_Current / Grenade_Max;
	static float Grenade_angle = 0.0f;
	RECT g_IconRc;
	SetRect(&g_IconRc, 0, 0, 64, 64);
	RECT G_rc;
	SetRect(&G_rc, 0.5, 0, 64 * Grenade_per, 16);
	SPRITE_MGR->DrawTexture(
		this->Grenade_Icon,
		&g_IconRc,
		1100, 640,
		0.6, 0.6,
		Grenade_angle,
		0xffffffff,
		&D3DXVECTOR3(0, 0, 0));



	SPRITE_MGR->DrawTexture(
		this->Grenade_Num,
		&G_rc,
		1025, 660,
		1, 1,
		Grenade_angle,
		0xffffffff,
		&D3DXVECTOR3(0, 0, 0));

}

void Ui_Bar::Ammo()
{
	RECT Ammo_rc;
	SetRect(&Ammo_rc, 0, 0, 32, 51);
	static float Ammo_angle = 0.0f;
	SPRITE_MGR->DrawTexture(
		this->Sna_Ammo,
		&Ammo_rc,
		this->bullet_BackShake.GetLocalPosition().x + 230, this->bullet_BackShake.GetLocalPosition().y + 10,
		0.6, 0.6,
		Ammo_angle,
		0xffffffff,
		&D3DXVECTOR3(0, 0, 0));

	/////////////////////////////////홍순이가 권총을 만든다면 주석을 풀어라/////////////////////////////////////////////
	//	SPRITE_MGR->DrawTexture(
	//		this->Pistol_Ammo,
	//		&Ammo_rc,
	//		this->bullet_BackShake.GetLocalPosition().x + 230, this->bullet_BackShake.GetLocalPosition().y ,
	//		0.8, 0.8,
	//		Ammo_angle,
	//		0xffffffff,
	//		&D3DXVECTOR3(0, 0, 0));
	//

}


void Ui_Bar::Aim()
{
	RECT Aim_rc;
	SetRect(&Aim_rc, 0, 0, 64, 64);
	static float Aim_angle = 0.0f;
	SPRITE_MGR->DrawTexture(
		this->Mouse_Aim,
		&Aim_rc,
		GetMousePos().x, GetMousePos().y,
		1, 1,
		Aim_angle,
		0xffffffff,
		&D3DXVECTOR3(32, 32, 0));

}

void Ui_Bar::TagetAim()
{
	RECT TagetAim_rc;
	SetRect(&TagetAim_rc, 0, 0, 64, 64);
	static float Aim_angle = 0.0f;
	SPRITE_MGR->DrawTexture(
		this->Taget_Aim,
		&TagetAim_rc,
		GetMousePos().x, GetMousePos().y,
		1, 1,
		Aim_angle,
		0xffffffff,
		&D3DXVECTOR3(32, 32, 0));
}
void Ui_Bar::Pick()
{
	RECT Pick_rc;
	SetRect(&Pick_rc, 0, 0, 256, 256);

	SPRITE_MGR->DrawTexture(
		this->cPick,
		&Pick_rc,
		WINSIZE_X / 2, WINSIZE_Y / 2,
		0.35, 0.35,
		0,
		0xffffffff,
		&D3DXVECTOR3(148, 148, 0));

}
void Ui_Bar::Total_Render(cCamera* Main_Cam, cPlayer* m_pkPlayer, vector<bossClass*> Boss, vector<enemyClass1*> Enemy1, vector<enemyClass2*> Enemy2, vector<item*> item0)
{
	if (!m_pkPlayer->IsPlayerAim()) {
		Aim();
	}
	_Font->SetUiInfo(*m_pkPlayer->GetUiInfo());
	_Font->shieldFont(135, 670);
	_Font->HpFont(135, 696);
	_Font->BulletFont(1050, 688);
	HpRender();
	ShieldRender();
	Grenade_Render();
	BulletRender();
	Player_EXP();
	Ammo();
	for (auto &Boss : Boss) {
		if (Boss->getEnemy_Stat().F_ok) {
			for (int i = 0; i < Boss->GetBodyPosition().size(); i++) {
				if (PHYSICS_MGR->IsRayHitSphere(m_pkPlayer->GetRay(), &Boss->GetBodyPosition()[i]->GetWorldPosition(), Boss->GetEnemyRadius(), NULL, NULL)
					|| PHYSICS_MGR->IsRayHitSphere(m_pkPlayer->GetRay(), &Boss->GetHeadPosition(), Boss->GetEnemyRadius(), NULL, NULL)) {
					if (!m_pkPlayer->IsPlayerAim()) {
						TagetAim();
						Enemy_Init(Boss->getEnemy_Stat().Hp, Boss->getEnemy_Stat().Max_Hp);
						Update(Boss->getEnemy_Stat().Hp);
						Enemy_HpRender();
					}
				}
			}
		}
	}

	for (auto &enemy1 : Enemy1) {
		if (enemy1->getEnemy_Stat().F_ok) {
			for (int i = 0; i < enemy1->GetBodyPosition().size(); i++) {
				if (PHYSICS_MGR->IsRayHitSphere(m_pkPlayer->GetRay(), &enemy1->GetBodyPosition()[i]->GetWorldPosition(), enemy1->GetEnemyRadius(), NULL, NULL)
					|| PHYSICS_MGR->IsRayHitSphere(m_pkPlayer->GetRay(), &enemy1->GetHeadPosition(), enemy1->GetEnemyRadius(), NULL, NULL)) {
					if (!m_pkPlayer->IsPlayerAim()) {
						TagetAim();
						Enemy_Init(enemy1->getEnemy_Stat().Hp, enemy1->getEnemy_Stat().Max_Hp);
						Update(enemy1->getEnemy_Stat().Hp);
						Enemy_HpRender();
					}
				}
			}
		}
	}

	for (auto enemy2 : Enemy2) {
		if (enemy2->getEnemy_Stat().F_ok) {
			for (int i = 0; i < enemy2->GetBodyPosition().size(); i++) {
				if (PHYSICS_MGR->IsRayHitSphere(m_pkPlayer->GetRay(), &enemy2->GetBodyPosition()[i]->GetWorldPosition(), enemy2->GetEnemyRadius(), NULL, NULL)
					|| PHYSICS_MGR->IsRayHitSphere(m_pkPlayer->GetRay(), &enemy2->GetHeadPosition(), enemy2->GetEnemyRadius(), NULL, NULL)) {
					if (!m_pkPlayer->IsPlayerAim()) {
						TagetAim();
						Enemy_Init(enemy2->getEnemy_Stat().Hp, enemy2->getEnemy_Stat().Max_Hp);
						Update(enemy2->getEnemy_Stat().Hp);
						Enemy_HpRender();
					}
				}
			}
		}
	}

	for (auto &item0 : item0) {
		float Item_Length = D3DXVec3Length(&(m_pkPlayer->m_pPlayerTrans->GetWorldPosition() - item0->getTrans().GetWorldPosition()));
		if (Item_Length < 4) {
			if (PHYSICS_MGR->IsRayHitSphere(m_pkPlayer->GetRay(), &item0->getTrans().GetWorldPosition(), 0.3F, NULL, NULL)) {
				Pick();
				item0->set_Item_Pic_ok(true);
			}
			else {
				item0->set_Item_Pic_ok(false);
			}
		}
	}
}
