#pragma once
#include "Player_Stat.h"

class cXMesh_Skinned;
class cSkinnedAnimation;
class cBoundBox;
class cCamera;
class cTerrain;
class cTransform;
class cLight_Direction;
class cItem0;
class cSetBoundObject;
class cBaseObject;
class enemyClass2;
class enemyClass1;
class bossClass;
class Effect;

class cPlayer
{
private:
	typedef vector<cSetBoundObject*> VEC_OBJECT;

private:
	vector<BULLET>			m_vecBullet;

private:
	cTerrain*				m_pTerrain;
	int                     Total_Damage = 0;
	float					moveEye;
	//std::vector<cBaseObject*>			WeaponObjects; // 추가 웨폰 벡터

private:
	cXMesh_Skinned*			m_pWeapon1;
	cSkinnedAnimation*		m_pWeaponAni1;
	cTransform*				m_pWeaponTrans1;
	BONE*					m_pWeaponBone1;

	cXMesh_Skinned*			m_pWeapon2;
	cSkinnedAnimation*		m_pWeaponAni2;
	BONE*					m_pWeaponBone2;

	cXMesh_Skinned*			m_pWeapon3;
	cSkinnedAnimation*		m_pWeaponAni3;
	BONE*					m_pWeaponBone3;

	cXMesh_Skinned*			m_pWeapon4;
	cSkinnedAnimation*		m_pWeaponAni4;
	BONE*					m_pWeaponBone4;

	cXMesh_Skinned*			m_pWeapon5;
	cSkinnedAnimation*		m_pWeaponAni5;
	BONE*					m_pWeaponBone5;


	cXMesh_Skinned*			m_pWeapon6;
	cSkinnedAnimation*		m_pWeaponAni6;
	BONE*					m_pWeaponBone6;


	cXMesh_Skinned*			m_pWeapon7;
	cSkinnedAnimation*		m_pWeaponAni7;
	BONE*					m_pWeaponBone7;


private:
	cSkinnedAnimation*		m_pMeshAni;

	cTransform*				m_pPlayerUpperTrans;

	BONE*					m_pDummy21;

	cCamera*				m_pPlayerEye;


	PLAYER_STATE			m_ekPlayerState;

	//이동속도 관련 멤버
	float					m_fRunSpeed;
	float					m_fWalkSpeed;
	float					m_fCurSpeed;

	//점프 관련 멤버
	float					m_fPosTerrainHeight;		//현 위치 고도 정보
	float					m_fGravity;					//중력값
	float					m_fJumpCurPower;			//점프 현재 파워
	float					m_fJumpStartPower;			//점프 시작 파워
	bool					m_bAir;						//현재 공중?
	bool					m_bJump = false;
	bool					m_bOnBox;
	cBaseObject*			m_pkOnBox = NULL;

	bool					m_bReact = false;

	//적 관련 멤버
	//에너미 매니져가 들어갈 것...
	cTransform*				m_pEnemy;
	DWORD					m_checkColor;


	//아이템 창 관련
	cItem0*					m_weaponSlot[4];
	cItem0*					m_equipWeapon;


	//충돌용 박스
	cBoundBox				m_kCollisionBox;
	VEC_OBJECT*				m_pVecObjects;
	bool					Air_Check;
	bool                    Warp_Check;
	//====
	float					m_fCurShield;
	float					m_fFullShield;

	float					m_fCurHp;
	float					m_fFullHp;

	float					m_fShieldRegenCount;
	float					m_fShieldRegenStartCount;

	LPDIRECT3DTEXTURE9		RenderTexture;

private:
	vector<enemyClass1*>*	m_vecEnemy1;
	vector<enemyClass2*>*	m_vecEnemy2;
	vector<bossClass*>*		m_vecBoss;

private:
	Ray*					m_pkRay;

public:
	int						m_iGrenadeMax;
	int						m_iGrenade;
	int						m_iBulletMax;

	float					m_fExpCur;
	float					m_fExpMax;

	UIINFO					m_stUiInfo;

	bool					_cri;

private:
	Effect*					m_pEffect;
	bool					m_bBlood = false;
	float					m_fBloodCount;

private:
	float					m_fGrenadeCount = 0.f;
	bool					m_bExpStart = true;

	int						m_iBulletMaxCell = 5;
	int						m_iBulletCurCell = 5;

private:
	vector<GRENADE>			m_vecGrenade;

	bool					m_bThrow;
	bool					m_bIsThrown;
	float					m_fThrowTimeCount;

public:
	cBoundSphere*			playerSphere;
	cBoundSphere*			playerCoreSphere;
	cTransform*				m_pPlayerTrans;
	cXMesh_Skinned*			m_pMesh;
	static int				m_iBulletCount;
	static int				Bullet_Total_Max;
	bool getWarp_Check() { return Warp_Check; }
	int getTotal_Damage() { return Total_Damage; }
	cPlayer();
	~cPlayer();

	HRESULT Init(cTerrain* terrain);

	void Release();

	void Update(float timDelta);

	void Render(cCamera* camera, cLight_Direction* Light_D);

	cCamera* GetPlayerEye() { return m_pPlayerEye; }

	void InitObjectSet(VEC_OBJECT* pVecObjects) { m_pVecObjects = pVecObjects; }
	void CollisionBox();

	vector<BULLET>* GetVecBullet() { return &m_vecBullet; }

	//데미지 입력
	void AttackedByEnemy(float damage);

	bool IsPlayerAlive();

	void SetEnemyVector(vector<enemyClass1*>*, vector<enemyClass2*>*, vector<bossClass*>*);

	Ray* GetRay() { return m_pkRay; }

	float GetCurHp() { return m_fCurHp; }

	bool IsPlayerAim() {
		if (PLAYER_STATE::AIM == m_ekPlayerState) {
			return true;
		}
		else {
			return false;
		}
	}

	UIINFO* GetUiInfo() { return &m_stUiInfo; }

	void SetRenderTexture(LPDIRECT3DTEXTURE9 Texture) {
		this->RenderTexture = Texture;
	};

private:
	void ShiledUpdate(float timeDelta, float startSec, float regenSec);

private:
	void W_IsStayDownUpdate(float timeDelta);

	void S_IsStayDownUpdate(float timeDelta);

	void A_IsStayDownUpdate(float timeDelta);

	void D_IsStayDownUpdate(float timeDelta);

	void G_IsOnceDownUpdate(float timeDelta);

	void R_IsOnceDownUpdate(float timeDelta);

	void SPACE_IsOnceDownUpdate(float timeDelta);

	void LBUTTON_IsOnceDownUpdate(float timeDelta);

	void LSHIFT_IsStayDownUpdate(float timeDelta);

	void LSHIFT_IsNotStayDownUpdate(float timeDelta);

	void RBUTTON_IsStayDownUpdate(float timeDelta);

	void RBUTTON_IsNotStayDownUpdate(float timeDelta);

	void ShowPlayerMotionState();

	void ShowBulletState();

	float GetHeightGap();

	void UpdateBullet(float timedelta);

	void fire();

	bool LineSphereCollision(D3DXVECTOR3 lineStart, D3DXVECTOR3 lineEnd, D3DXVECTOR3 center, float radius);

public:				
	bool getAir()
	{
		return m_bAir;
	}

	void Set_Hp_Healing(int Set_Hp) {
		m_fCurHp += Set_Hp;
		if (m_fCurHp > Player_Full_Hp) 
			m_fCurHp = Player_Full_Hp;
	}
	void Set_Bullet_Reload(int Set_Bullet) {
		Bullet_Total_Max += Set_Bullet;
		if (Bullet_Total_Max > Player_Total_Bullet_Max)
			Bullet_Total_Max = Player_Total_Bullet_Max;
	}

private:
	void GrenadeUpdate(float timeDelta);
};