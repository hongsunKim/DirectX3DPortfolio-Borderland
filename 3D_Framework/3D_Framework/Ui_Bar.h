#pragma once



#include "cScene.h"
#include "cCamera.h"
#include "cPlayer.h"
#include "cPlayer.h"
#include "bossClass.h"
#include "enemyClass1.h"
#include "enemyClass2.h"
#include "cCamera.h"
#include "item.h"
#include "Font.h"

class Ui_Bar 
{
protected:

	//////////////////////shake//////////////////
	//자연스럽게 멈추기위해 만든 변수///////////////
	float					BackShake_f;
	float					Shake_f;
	///////////////////////////////////////////

	///////////////////////HP/////////////////////
	cTransform				HpShake;
	cTransform				HpBackShake;


	LPDIRECT3DTEXTURE9		HpBar;
	LPDIRECT3DTEXTURE9		HpBar2;
	LPDIRECT3DTEXTURE9		HpIcon;
	LPDIRECT3DTEXTURE9		HpIconBack;
	LPDIRECT3DTEXTURE9		HpBack;
	LPDIRECT3DTEXTURE9		LowHp;



	/////////////////////Shield/////////////////////////////
	cTransform				Shield_Shake;
	cTransform				Shield_BackShake;

	LPDIRECT3DTEXTURE9		Shield_Bar;
	LPDIRECT3DTEXTURE9		Shield_Bar2;
	LPDIRECT3DTEXTURE9		Shield_Icon;
	LPDIRECT3DTEXTURE9		Shield_IconBack;
	LPDIRECT3DTEXTURE9		Shield_Back;
	LPDIRECT3DTEXTURE9		Low_Shield;



	///////////////////////////////////////////////////////

	///////////////////////Bullet//////////////////////////
	cTransform				bullet_Shake;
	cTransform				bullet_BackShake;

	LPDIRECT3DTEXTURE9		bullet_Bar;
	LPDIRECT3DTEXTURE9		bullet_Back;

	LPDIRECT3DTEXTURE9		ReLoad;

	LPDIRECT3DTEXTURE9		Sna_Ammo;
	LPDIRECT3DTEXTURE9		Pistol_Ammo;
	//////////////////////////////////////////////////////
	LPDIRECT3DTEXTURE9		EXP_Bar;

	///////////////////////////////////////////////////////
	LPDIRECT3DTEXTURE9		Enemy_bar;


	////////////////////////////////////////////////////
	LPDIRECT3DTEXTURE9		Grenade_Icon;
	LPDIRECT3DTEXTURE9		Grenade_Num;

	///////////////////////////////////////////////////////
	LPDIRECT3DTEXTURE9		Last_Chance;
	LPDIRECT3DTEXTURE9		Last_Tex;
	LPDIRECT3DTEXTURE9		Last_bar;
/////////////////////////////////////////////////////////////////////////////////////////
	LPDIRECT3DTEXTURE9		Mouse_Aim;
	LPDIRECT3DTEXTURE9		Taget_Aim;
////////////////////////////////////////////////////////////////////////////////////////
	float Hp_Current;
	float Hp_Max;
	float shield_Current;
	float shield_Max;
	float Bullet_Current;
	float Bullet_Max;
	float Grenade_Current;
	float Grenade_Max;
	float Enemy_Current;
	float Enemy_Max;
	float Exp_Max;
	float Exp_Current;
	float TimeMax;
	float TimeCek;

	LPDIRECT3DTEXTURE9		cPick;
	LPD3DXFONT cFont;

	Font* _Font;

public:
	Ui_Bar();
	~Ui_Bar();

	HRESULT Scene_Init();

	HRESULT Enemy_Init(float current, float Max);

	void Player_SetUp(UIINFO);

	void Update(/*UIINFO current,*/float EnemyCurrent);

//	void Update(float cur_Hp);
	virtual void Scene_Release();

	virtual void Scene_Update(float timDelta);

	virtual void Scene_Render1();


	virtual void Scene_RenderSprite();



	void HpRender();

	void ShieldRender();

	void BulletRender();

	void Enemy_HpRender();

	void Player_EXP();

	void Grenade_Render();

	void Ammo();

	void Aim();

	void TagetAim();

	void Pick();

	void Total_Render(cCamera* Main_Cam, cPlayer* m_pkPlayer, vector<bossClass*> Boss, vector<enemyClass1*> Enemy1, vector<enemyClass2*> Enemy2, vector<item*> item0);
//	void SetCurrent(float _Current) { _Current2 = _Current; }
//	void SetMax(float _max) { _Max2 = _max; }

};


