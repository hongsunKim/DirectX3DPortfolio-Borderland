#pragma once

#define Player_Damage RandomIntRange(15,30)
#define Item_Random   RandomIntRange(0,1);
#define Player_Bullet_Max 10
#define Player_Total_Bullet_Max 30
#define Player_Full_Hp 100
#define Player_Full_Shield 20
#define Player_Grenade_Max 3

typedef struct tagUIINFO {
	float CurShield;
	float FullShield;
	float CurHp;
	float FullHp;
	int Grenade_Current;
	int Grenade_Max;
	int Bullet_Current;
	int Bullet_Reload_Max;
	int Bullet_Total_Max;
	float Exp_Max;
	float Exp_Current;

}UIINFO, *LPUIINFO;

enum class PLAYER_STATE
{
	IDLE,			//정지상태
	WALK,			//걷기상태
	RUN,			//이동상태
	FIRE,			//총발사
	DRAW,			//한발 쏘고 장전
	RELOAD,			//탄창 교체
	AIM,			//조준
	SETDOWN,		//무기 교체(내림)
	SETUP,			//무기 교체(올림)
	MELEE,			//칼 공격
	THROW			//수류탄투척
};

enum class BULLET_STATE
{
	ON_WAY,
	HIT_TERRAIN,
	HIT_ENEMY_NORMAL,
	HIT_ENEMY_CRITICAL,
	REACHED_END
};
//총알=====================================
typedef struct tagBULLET
{
	D3DXVECTOR3		startPos;
	D3DXVECTOR3		direction;
	float			velocity;
	D3DXVECTOR3		curPos;
	D3DXVECTOR3		prevPos;
	float			distance;
	BULLET_STATE	bulletState;
	cXMesh*			bulletMesh = NULL;
	float			damage = Player_Damage;
	/*
	그 외의 공격력 등의 정보가 필요합니다.
	*/
}BULLET, *LPBULLET;

//수류탄=============================
enum class GRENADE_STATE
{
	READY,
	ON_WAY,
	EXPLODE,
	REACHED_END
};

typedef struct tagGRENADE {
	D3DXVECTOR3			StartPos;
	D3DXVECTOR3			Direction;
	float				Velocity;
	D3DXVECTOR3			CurPos;
	cBaseObject*		GrenadeObject;
	float				Gravity;
	float				Drop;
	float				Power;
	float				ExplosionRadius;
	float				TimeCount = 6;
	float				Damage = 1000;
	GRENADE_STATE		GrenadeState = GRENADE_STATE::READY;
}GRENADE, *LPGRENADE;