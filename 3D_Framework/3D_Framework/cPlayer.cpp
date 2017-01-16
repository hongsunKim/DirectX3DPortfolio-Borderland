#include "stdafx.h"
#include "cPlayer.h"
#include "cTerrain.h"
#include "cCamera.h"
#include "cXMesh_Skinned.h"
#include "cSkinnedAnimation.h"
#include "cLight.h"
#include "cLight_Direction.h"
#include "cLight_Point.h"
#include "enemyClass2.h"
#include "enemyClass1.h"
#include "bossClass.h"
#include "Effect.h"
#include "cBaseObject.h"

int cPlayer::m_iBulletCount = Player_Bullet_Max;
int cPlayer::Bullet_Total_Max = Player_Total_Bullet_Max;

cPlayer::cPlayer() : Warp_Check(false)
{
}

cPlayer::~cPlayer()
{
}

HRESULT cPlayer::Init(cTerrain* terrain)
{
	D3DXMATRIXA16 matCorrection;
	D3DXMatrixScaling(&matCorrection, 0.01f, 0.01f, 0.01f);

	m_pMesh = new cXMesh_Skinned;
	m_pMesh->Init("../../Resources/Player/PlayerArmSniper/PlayerArmSniper.X",
		&matCorrection);

	m_pMeshAni = new cSkinnedAnimation;
	m_pMeshAni->Init(m_pMesh);

	m_pWeapon1 = RESOURCE_SKINNEDXMESH->
		GetResource("../../Resources/Player/PlayerWeaponSniper/acc5.X",
			&matCorrection);

	m_pWeaponAni1 = new cSkinnedAnimation;
	m_pWeaponAni1->Init(m_pWeapon1);
	m_pWeaponAni1->Play("idle_01", 0.3);
	m_pWeaponAni1->Stop();

	m_pWeapon2 = RESOURCE_SKINNEDXMESH->
		GetResource("../../Resources/Player/PlayerWeaponSniper/barrel5.X",
			&matCorrection);

	m_pWeaponAni2 = new cSkinnedAnimation;
	m_pWeaponAni2->Init(m_pWeapon2);
	m_pWeaponAni2->Play("idle_01", 0.3);
	m_pWeaponAni2->Stop();

	m_pWeapon3 = RESOURCE_SKINNEDXMESH->
		GetResource("../../Resources/Player/PlayerWeaponSniper/body5.X",
			&matCorrection);

	m_pWeaponAni3 = new cSkinnedAnimation;
	m_pWeaponAni3->Init(m_pWeapon3);
	m_pWeaponAni3->Play("idle_01", 0.3);
	m_pWeaponAni3->Stop();

	m_pWeapon4 = RESOURCE_SKINNEDXMESH->
		GetResource("../../Resources/Player/PlayerWeaponSniper/grip5.X",
			&matCorrection);

	m_pWeaponAni4 = new cSkinnedAnimation;
	m_pWeaponAni4->Init(m_pWeapon4);
	m_pWeaponAni4->Play("idle_01", 0.3);
	m_pWeaponAni4->Stop();

	m_pWeapon5 = RESOURCE_SKINNEDXMESH->
		GetResource("../../Resources/Player/PlayerWeaponSniper/mag5.X",
			&matCorrection);

	m_pWeaponAni5 = new cSkinnedAnimation;
	m_pWeaponAni5->Init(m_pWeapon5);
	m_pWeaponAni5->Play("idle_01", 0.3);
	m_pWeaponAni5->Stop();

	m_pWeapon6 = RESOURCE_SKINNEDXMESH->
		GetResource("../../Resources/Player/PlayerWeaponSniper/sight5.X",
			&matCorrection);

	m_pWeaponAni6 = new cSkinnedAnimation;
	m_pWeaponAni6->Init(m_pWeapon6);
	m_pWeaponAni6->Play("idle_01", 0.3);
	m_pWeaponAni6->Stop();

	m_pWeapon7 = RESOURCE_SKINNEDXMESH->
		GetResource("../../Resources/Player/PlayerWeaponSniper/stock5.X",
			&matCorrection);

	m_pWeaponAni7 = new cSkinnedAnimation;
	m_pWeaponAni7->Init(m_pWeapon6);
	m_pWeaponAni7->Play("idle_01", 0.3);
	m_pWeaponAni7->Stop();

	m_pPlayerEye = new cCamera;

	m_pPlayerTrans = new cTransform;

	m_pPlayerUpperTrans = new cTransform;
	m_pMeshAni->AddApplyTransform("Bip01_Spine", m_pPlayerUpperTrans);

	m_pDummy21 = m_pMesh->GetFineBONE("Dummy21");
	m_pDummy21->pBoneTransform = new cTransform;
	m_pPlayerEye->AttachTo(m_pDummy21->pBoneTransform);
	m_pPlayerEye->SetLocalPosition(-0.1, 0, -0.1);

	m_pPlayerTrans->AddChild(m_pDummy21->pBoneTransform);

	m_pWeaponTrans1 = new cTransform;

	m_pMeshAni->AddBoneTransform("Weapon", m_pWeaponTrans1);

	m_ekPlayerState = PLAYER_STATE::IDLE;


	//=======================================추가========================================================

	//=========================================================================================================
	////////////////////////////
	//	  총 반동 초기화	  //
	////////////////////////////
	m_bReact = false;

	////////////////////////////
	//		속도 초기화		  //
	////////////////////////////
	m_fRunSpeed = 6.f;
	m_fWalkSpeed = 3.f;
	m_fCurSpeed = 3.f;

	////////////////////////////
	//		점프 초기화		  //
	////////////////////////////
	m_fPosTerrainHeight = 0;
	m_fGravity = 0.005;
	m_fJumpCurPower = 0.2;
	m_fJumpStartPower = 0.2;
	m_bAir = false;

	m_pMeshAni->Play("idle_01");

	//m_pWeaponAni->Stop();

	////////////////////////////
	//		터레인 초기화	  //
	////////////////////////////
	m_pTerrain = terrain;

	m_pPlayerTrans->SetWorldPosition(m_pTerrain->Player_Start_Pos);

	//에너미 초기화
	m_pEnemy = new cTransform;
	m_pEnemy->SetWorldPosition(50, m_pTerrain->GetHeight(50, 50), 50);
	m_checkColor = 0xffffffff;

	m_pVecObjects = NULL;

	m_bOnBox = false;
	moveEye = 0.0f;
	D3DXVECTOR3 center(0.f, 1.5f, 0.f);
	D3DXVECTOR3 halfSize(0.5f, 1.8f, 0.5f);
	m_kCollisionBox.SetBound(&center, &halfSize);

	m_pPlayerEye->SetShakePosFlag(SHAKE_X | SHAKE_Y);
	m_pPlayerEye->SetShakeRotFlag(SHAKE_X | SHAKE_Y);

	m_pPlayerEye->ShakePos(D3DXVECTOR3(0.5f, 0.5f, 0.f));

	m_vecEnemy1 = NULL;
	m_vecEnemy2 = NULL;
	m_vecBoss = NULL;

	m_fCurShield = Player_Full_Shield;
	m_fFullShield = Player_Full_Shield;

	m_fCurHp = Player_Full_Hp;
	m_fFullHp = Player_Full_Hp;

	m_fShieldRegenCount = 0;
	m_fShieldRegenStartCount = 0;

	m_pkRay = new Ray;

	m_iGrenade = Player_Grenade_Max;
	m_iGrenadeMax = Player_Grenade_Max;

	m_fExpCur = 0;
	m_fExpMax = 1000;

	playerSphere = new cBoundSphere;
	playerCoreSphere = new cBoundSphere;
	playerSphere->SetBound(&D3DXVECTOR3(0.f, 0.8f, 0.f), &D3DXVECTOR3(0.5f, 0.5f, 0.5f));
	playerCoreSphere->SetBound(&D3DXVECTOR3(0.f, 0.8f, 0.f), &D3DXVECTOR3(0.3f, 0.3f, 0.3f));

	m_pEffect = new Effect;

	m_pEffect->Scene_Init();

	m_bBlood = false;
	m_fBloodCount = 0.3;

	m_vecGrenade.resize(3);
	for (int i = 0; i < 3; ++i) {
		m_vecGrenade[i].GrenadeObject = new cBaseObject;
		m_vecGrenade[i].GrenadeObject->pMesh = RESOURCE_STATICXMESH->GetResource("../../Resources/Player/Grenade/Grenade.X");
		m_vecGrenade[i].GrenadeObject->pTransform = new cTransform;
		m_vecGrenade[i].GrenadeObject->SetActive(true);
		m_vecGrenade[i].GrenadeObject->BoundBox.SetBound(&D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0.1, 0.1, 0.1));
		m_vecGrenade[i].GrenadeObject->Bound_Active = true;
	}

	m_bThrow = false;

	m_fGrenadeCount = 0;

//	m_iBulletCount = cScene::bu0;
	m_iBulletMax = Player_Bullet_Max;
//	Bullet_Total_Max = Player_Total_Bullet_Max;
	m_iBulletCurCell = 0;
	m_iBulletMaxCell = 3;

	return S_OK;
} 

void cPlayer::Release()
{
	SAFE_DELETE(m_pMesh);
	SAFE_DELETE(m_pMeshAni);
	SAFE_DELETE(m_pPlayerEye);
	SAFE_DELETE(m_pPlayerTrans);

	m_pTerrain->Release();
	SAFE_DELETE(m_pTerrain);

	SAFE_DELETE(m_pWeapon1);
	SAFE_DELETE(m_pWeaponAni1);
	SAFE_DELETE(m_pWeaponTrans1);
	SAFE_DELETE(m_pPlayerUpperTrans);
	SAFE_DELETE(m_pEnemy);
	SAFE_DELETE(m_pkRay);
	m_pEffect->Scene_Release();
	SAFE_DELETE(m_pEffect);
}

void cPlayer::Update(float timDelta)
{
	//=============추가

	if (KEY_MGR->IsStayDown(VK_RBUTTON) && (m_ekPlayerState != PLAYER_STATE::FIRE)
		&& (m_ekPlayerState != PLAYER_STATE::RELOAD)
		&& (m_ekPlayerState != PLAYER_STATE::DRAW))
	{
		if (!FLOATEQUAL(m_pPlayerEye->fov, 20.f*ONE_RAD))
		{
			m_pPlayerEye->fov -= ONE_RAD*2.f;
			moveEye -= 0.004f*2.f;
		}
	}
	else if (!FLOATEQUAL(m_pPlayerEye->fov, 60.f*ONE_RAD))
	{
		m_pPlayerEye->fov += ONE_RAD*2.f;
		moveEye += 0.004f*2.f;
	}

	m_pPlayerEye->SetLocalPosition(-0.1 + moveEye / 2, moveEye, -0.1);
	//============


	if (Camera_Check) {
		if (KEY_MGR->IsStayDown('W'))
			W_IsStayDownUpdate(timDelta);

		if (KEY_MGR->IsStayDown('S'))
			S_IsStayDownUpdate(timDelta);

		if (KEY_MGR->IsStayDown('A'))
			A_IsStayDownUpdate(timDelta);

		if (KEY_MGR->IsStayDown('D'))
			D_IsStayDownUpdate(timDelta);

		if (!KEY_MGR->IsStayDown('W') && !KEY_MGR->IsStayDown('A') && !KEY_MGR->IsStayDown('S') && !KEY_MGR->IsStayDown('D'))
		{
			if (m_ekPlayerState == PLAYER_STATE::RUN || m_ekPlayerState == PLAYER_STATE::WALK)
				m_ekPlayerState = PLAYER_STATE::IDLE;
		}

		if (KEY_MGR->IsOnceDown('G'))
			G_IsOnceDownUpdate(timDelta);
		if (KEY_MGR->IsOnceDown('R')) {
			if (Bullet_Total_Max > 0) {
				m_ekPlayerState = PLAYER_STATE::RELOAD;
			}
		}
		if (KEY_MGR->IsOnceDown(VK_LBUTTON))
			LBUTTON_IsOnceDownUpdate(timDelta);

		if (KEY_MGR->IsStayDown(VK_LSHIFT))
			LSHIFT_IsStayDownUpdate(timDelta);
		else
			LSHIFT_IsNotStayDownUpdate(timDelta);

		if (KEY_MGR->IsStayDown(VK_RBUTTON))
			RBUTTON_IsStayDownUpdate(timDelta);
		else
			RBUTTON_IsNotStayDownUpdate(timDelta);
	}

	switch (m_ekPlayerState)
	{
	case PLAYER_STATE::IDLE:
		if (!m_pMeshAni->IsPlay("idle_01"))
			m_pMeshAni->Play("idle_01", 0.3f);
		break;
	case PLAYER_STATE::WALK:
		if (!m_pMeshAni->IsPlay("idle_01"))
			m_pMeshAni->Play("idle_01", 0.3f);
		break;
	case PLAYER_STATE::RUN:
		if (!m_pMeshAni->IsPlay("Sprint"))
			m_pMeshAni->Play("Sprint", 0.3f);
		break;
	case PLAYER_STATE::FIRE:
		if (!m_pMeshAni->IsPlay("fire_02"))
			m_pMeshAni->PlayOneShotAfterHold("fire_02", 0.05f);
		if (m_pMeshAni->IsEnd())
		{
			if (m_iBulletCount > 0)
				m_ekPlayerState = PLAYER_STATE::DRAW;
		}
		break;
	case PLAYER_STATE::DRAW:
		if (!m_pMeshAni->IsPlay("Draw_01"))
		{
			m_pMeshAni->SetPlaySpeed(0.5);
			m_pMeshAni->PlayOneShot("Draw_01", 0.05f);
		}
		if (m_pMeshAni->IsEnd())
		{
			m_pMeshAni->SetPlaySpeed(1.f);
			m_ekPlayerState = PLAYER_STATE::IDLE;
		}
		break;
	case PLAYER_STATE::RELOAD:
		if (!m_pMeshAni->IsPlay("RELOAD_02"))
			m_pMeshAni->PlayOneShotAfterHold("RELOAD_02", 0.1f);
		if (m_pMeshAni->IsEnd()) {
			if (Bullet_Total_Max > 0) {
				int Reload_Bullet = Player_Bullet_Max - m_iBulletCount;
				if (Bullet_Total_Max > Reload_Bullet) {
					Bullet_Total_Max -= Reload_Bullet;
					m_iBulletCount += Reload_Bullet;
				}
				else {
					m_iBulletCount += Bullet_Total_Max;
					Bullet_Total_Max = 0;
				}
			}
			m_ekPlayerState = PLAYER_STATE::IDLE;
		}
		break;
	case PLAYER_STATE::AIM:
		if (!m_pMeshAni->IsPlay("idle_01"))
			m_pMeshAni->Play("idle_01", 0.3f);

		break;
	case PLAYER_STATE::SETDOWN:
		break;
	case PLAYER_STATE::SETUP:
		break;
	case PLAYER_STATE::MELEE:
		break;
	case PLAYER_STATE::THROW:
		if (!m_pMeshAni->IsPlay("grenade_throw_01"))
			m_pMeshAni->PlayOneShotAfterHold("grenade_throw_01", 0.1f);

		if (m_bThrow == false) {
			if (m_pMeshAni->ReachPoint(0.5f)) {

				size_t size = m_vecGrenade.size();
				for (size_t i = 0; i < size; ++i) {
					if (m_vecGrenade[i].GrenadeState == GRENADE_STATE::READY) {
						m_vecGrenade[i].StartPos = m_pPlayerEye->GetWorldPosition();
						D3DXMATRIXA16 matRot;
						D3DXMatrixRotationAxis(&matRot, &m_pPlayerEye->GetRight(), 각도(-30));
						D3DXVECTOR3 dir;
						D3DXVec3TransformNormal(&dir, &m_pPlayerEye->GetForward(), &matRot);
						m_vecGrenade[i].Direction = dir;
						m_vecGrenade[i].CurPos = m_pPlayerEye->GetWorldPosition()
							/*+ (m_pPlayerEye->GetForward() * -15)*/ + (m_pPlayerEye->GetRight() * -0.25);
						m_vecGrenade[i].Power = 0.9f;
						m_vecGrenade[i].Gravity = 0.f;
						m_vecGrenade[i].GrenadeState = GRENADE_STATE::ON_WAY;
						m_vecGrenade[i].GrenadeObject->pTransform->SetWorldPosition(m_vecGrenade[i].CurPos);
						break;
					}
				}

				m_bThrow = true;
			}
		}

		if (m_pMeshAni->IsEnd())
		{
			m_bThrow = false;
			m_ekPlayerState = PLAYER_STATE::IDLE;
		}
		break;
	}

	m_weaponSlot[4] = { NULL, };
	m_equipWeapon = NULL;

	m_pPlayerEye->LookDirection(m_pPlayerTrans->GetForward(), D3DXVECTOR3(0, 1, 0));

	//점프 처리
	if (KEY_MGR->IsOnceDown(VK_SPACE))
	{
		if (GetHeightGap() < 0.3f || !Air_Check)
		{
			m_bJump = true;
			m_bAir = true;
			Air_Check = true;
			m_fJumpCurPower = m_fJumpStartPower;
			if (m_ekPlayerState == PLAYER_STATE::RUN)
				m_ekPlayerState = PLAYER_STATE::WALK;
			SOUNDMANAGER->addSound("jump", "../../Resources/Sounds/jump.ogg", false, false);
			SOUNDMANAGER->play("jump", 1.0f);
		}
	}

	if (m_bAir && Air_Check)
	{
		m_fJumpCurPower -= m_fGravity;
		if (m_fJumpCurPower < 0)
			m_bJump = false;
		m_pPlayerTrans->MovePositionWorld(0, m_fJumpCurPower, 0);
	}


	m_fPosTerrainHeight = m_pTerrain->GetHeight(m_pPlayerTrans->GetWorldPosition().x, m_pPlayerTrans->GetWorldPosition().z);

	float RelativeHeight = m_fPosTerrainHeight;
	if (m_bOnBox) {

	}

	if (!FLOATEQUAL(m_pPlayerTrans->GetWorldPosition().y, RelativeHeight))
	{
		if (m_pPlayerTrans->GetWorldPosition().y > RelativeHeight)
			m_bAir = true;

		if (m_pPlayerTrans->GetWorldPosition().y < RelativeHeight)
		{
			D3DXVECTOR3 pos = m_pPlayerTrans->GetWorldPosition();
			m_pPlayerTrans->SetWorldPosition(pos.x, RelativeHeight, pos.z);
			m_fJumpCurPower = 0.f;
			m_bAir = false;
		}
	}

	if (!Air_Check) { m_bAir = false; }

	if (m_bReact) { m_pPlayerEye->ShakeUpdate(timDelta); }

	if (Camera_Check) {
		m_pPlayerTrans->FPSGameCameraControl();
		m_pPlayerEye->UpdateCamToDevice(Device);
	}

	this->m_pMeshAni->Update(timDelta);

	UpdateBullet(timDelta);
	//=================
	CollisionBox();

	ShiledUpdate(timDelta, 10, 1);

	m_pkRay->origin = m_pPlayerEye->GetWorldPosition() + m_pPlayerTrans->GetForward();
	m_pkRay->direction = m_pPlayerTrans->GetForward();


	m_stUiInfo.Bullet_Current = m_iBulletCount;
	m_stUiInfo.CurHp = m_fCurHp;
	m_stUiInfo.CurShield = m_fCurShield;
	m_stUiInfo.FullHp = m_fFullHp;
	m_stUiInfo.FullShield = m_fFullShield;
	m_stUiInfo.Grenade_Current = m_iGrenade;
	m_stUiInfo.Grenade_Max = m_iGrenadeMax;
	m_stUiInfo.Bullet_Reload_Max = m_iBulletMax;
	m_stUiInfo.Bullet_Total_Max = Bullet_Total_Max;
	m_stUiInfo.Exp_Current = m_fExpCur;
	m_stUiInfo.Exp_Max = m_fExpMax;

	m_pEffect->Scene_Update(timDelta);

	GrenadeUpdate(timDelta);

	if (m_vecGrenade.size() != 0)
	{
		m_pEffect->SetGrenadeLocation(m_vecGrenade[0].CurPos);
	}
}

void cPlayer::Render(cCamera* camera, cLight_Direction* Light_D)
{
	cXMesh_Skinned::SetCamera(camera);
	cXMesh_Skinned::SetBaseLight(Light_D);

	if (!FLOATEQUAL(m_pPlayerEye->fov, 20.f*ONE_RAD)) {
		cXMesh_Skinned::SetTechniqueName("ReciveShadowToon");
		this->m_pMeshAni->Render(m_pPlayerTrans);
	}

	if (!FLOATEQUAL(m_pPlayerEye->fov, 20.f*ONE_RAD))
	{
		cXMesh_Skinned::SetTechniqueName("ReciveShadowToonMask");
		cXMesh_Skinned::sSkinnedMeshEffect->SetVector("vColor1", &D3DXVECTOR4(1.7f, 1.3f, 1, 1));
		cXMesh_Skinned::sSkinnedMeshEffect->SetVector("vColor2", &D3DXVECTOR4(2.5f, 2.5f, 2.5f, 1));
		cXMesh_Skinned::sSkinnedMeshEffect->SetVector("vColor3", &D3DXVECTOR4(6, 6, 6, 1));
		cXMesh_Skinned::sSkinnedMeshEffect->SetTexture("base_Tex", RenderTexture);
		cXMesh_Skinned::sSkinnedMeshEffect->SetFloat("gTimes", TIME_MGR->GetTotalDeltaSec());

		m_pWeapon1->Render(m_pWeaponTrans1);
		m_pWeapon2->Render(m_pWeaponTrans1);
		m_pWeapon3->Render(m_pWeaponTrans1);
		m_pWeapon4->Render(m_pWeaponTrans1);
		m_pWeapon5->Render(m_pWeaponTrans1);
		m_pWeapon6->Render(m_pWeaponTrans1);
		m_pWeapon7->Render(m_pWeaponTrans1);
	}


	if (Text_Check) {

		RESOURCE_SKINNEDXMESH->GetResource("../../Resources/Player/PlayerArmSniper/PlayerArmSniper.X")->ShowAnimationName(0, 100);

		char tempString[1024];
		sprintf(tempString, "m_pPlayerEyePosition: %.2f, %.2f, %.2f", m_pPlayerEye->GetWorldPosition().x, m_pPlayerEye->GetWorldPosition().y, m_pPlayerEye->GetWorldPosition().z);
		DXFONT_MGR->PrintText(tempString, 0, WINSIZE_Y - 100, 0xffffffff);
		sprintf(tempString, "m_pPlayerEyeDirection: %.2f, %.2f, %.2f", m_pPlayerEye->GetForward().x, m_pPlayerEye->GetForward().y, m_pPlayerEye->GetForward().z);
		DXFONT_MGR->PrintText(tempString, 0, WINSIZE_Y - 70, 0xffffffff);
		sprintf(tempString, "m_pPlayerUpperDirection: %.2f, %.2f, %.2f", m_pPlayerUpperTrans->GetForward().x, m_pPlayerUpperTrans->GetForward().y, m_pPlayerUpperTrans->GetForward().z);
		DXFONT_MGR->PrintText(tempString, 0, WINSIZE_Y - 130, 0xffffffff);
	}

	if (!m_vecBullet.empty())
	{
		ShowBulletState();
	}

	D3DXMATRIXA16 identity;
	D3DXMatrixIdentity(&identity);

	//m_pPlayerEye->RenderGimozo();

	ShowPlayerMotionState();
	if (Text_Check) {
		GIZMO_MGR->WireSphere(m_pEnemy->GetWorldPosition(), 5, m_checkColor);
	}

	m_kCollisionBox.BoundRender(&m_pPlayerTrans->GetWorldPosition(), 0xffffffff);

	if (EnemySphere_Check) {
		playerSphere->RenderGizmo(m_pPlayerTrans);
		playerCoreSphere->RenderGizmo(m_pPlayerTrans);
	}

	if (m_bBlood) {
		m_pEffect->BloodEffectRender();
		m_fBloodCount -= 0.01;
		if (m_fBloodCount < 0) {
			m_fBloodCount = 0.3;
			m_bBlood = false;
		}
	}

	size_t size = m_vecGrenade.size();
	for (size_t i = 0; i < size; ++i) {
		if (m_vecGrenade[i].GrenadeState == GRENADE_STATE::ON_WAY) {

			m_vecGrenade[i].GrenadeObject->BaseObjectLight(camera, Light_D);
			cXMesh_Static::SetBaseLight(Light_D);
			cXMesh_Static::SetTechniqueName("Toon");
			cXMesh_Static::SetCamera(camera);

			cXMesh_Skinned::SetCamera(camera);
			cXMesh_Skinned::SetTechniqueName("ReciveShadowToon");
			cXMesh_Skinned::sSkinnedMeshEffect->SetTexture("Ramp_Tex", RESOURCE_TEXTURE->GetResource("../../Resources/Testures/T_Explosion_Flash.png"));
			cXMesh_Skinned::SetBaseLight(Light_D);
			m_vecGrenade[i].GrenadeObject->Render();
		}
		else if (m_vecGrenade[i].GrenadeState == GRENADE_STATE::EXPLODE) {
			m_pEffect->GrenadeFxRender();
		}
	}
}

void cPlayer::CollisionBox()
{
	bool bTempOnBox = false;

	if (m_pVecObjects != NULL) {
		for (int i = 0; i < m_pVecObjects->size(); ++i) {
			cBaseObject* Obj_Box = ((cBaseObject*)(*m_pVecObjects)[i]);
			if (PHYSICS_MGR->IsBlocking(m_pPlayerTrans, &m_kCollisionBox, Obj_Box->pTransform, &Obj_Box->BoundBox, 1)) {
				if (!Obj_Box->BoundBox.Air_Blocking_Check && Obj_Box->BoundBox.Warp_Check) { Warp_Check = true; return; }
				if (!Obj_Box->BoundBox.Air_Blocking_Check && !m_bJump) { bTempOnBox = true; }
			}
			else {
				Air_Check = true;
			}
		}
	}
	if (bTempOnBox == false) { Air_Check = true; }
	else { Air_Check = false; }
}
void cPlayer::W_IsStayDownUpdate(float timeDelta)
{
	if (!m_bAir)
		m_fCurSpeed = m_fWalkSpeed;

	switch (m_ekPlayerState)
	{
	case PLAYER_STATE::IDLE:
		m_ekPlayerState = PLAYER_STATE::WALK;
		break;
	case PLAYER_STATE::RUN:
		m_fCurSpeed = m_fRunSpeed;
		break;
	}
	m_pPlayerTrans->MovePositionFPS(0, m_fCurSpeed * timeDelta);

}
void cPlayer::S_IsStayDownUpdate(float timeDelta)
{
	if (!m_bAir)
		m_fCurSpeed = m_fWalkSpeed;

	switch (m_ekPlayerState)
	{
	case PLAYER_STATE::IDLE:
		m_ekPlayerState = PLAYER_STATE::WALK;
	case PLAYER_STATE::RUN:
		m_fCurSpeed = m_fRunSpeed;
		break;
	}

	m_pPlayerTrans->MovePositionFPS(0, -m_fCurSpeed * timeDelta);
}
void cPlayer::A_IsStayDownUpdate(float timeDelta)
{
	if (!m_bAir)
		m_fCurSpeed = m_fWalkSpeed;

	switch (m_ekPlayerState)
	{
	case PLAYER_STATE::IDLE:
		m_ekPlayerState = PLAYER_STATE::WALK;
	case PLAYER_STATE::RUN:
		m_fCurSpeed = m_fRunSpeed;
		break;
	}

	m_pPlayerTrans->MovePositionFPS(-m_fCurSpeed * timeDelta, 0);
}
void cPlayer::D_IsStayDownUpdate(float timeDelta)
{
	if (!m_bAir)
		m_fCurSpeed = m_fWalkSpeed;

	switch (m_ekPlayerState)
	{
	case PLAYER_STATE::IDLE:
		m_ekPlayerState = PLAYER_STATE::WALK;
	case PLAYER_STATE::RUN:
		m_fCurSpeed = m_fRunSpeed;
		break;
	}

	m_pPlayerTrans->MovePositionFPS(m_fCurSpeed * timeDelta, 0);
}
void cPlayer::G_IsOnceDownUpdate(float timeDelta)
{
	if (m_iGrenade <= 0)
		return;

	if (m_vecGrenade.size() <= 0)
		return;

	if (m_ekPlayerState == PLAYER_STATE::RELOAD
		|| m_ekPlayerState == PLAYER_STATE::DRAW
		|| m_ekPlayerState == PLAYER_STATE::AIM)
		return;

	m_fCurSpeed = m_fWalkSpeed;

	if (m_ekPlayerState == PLAYER_STATE::THROW)
		return;

	switch (m_ekPlayerState)
	{
	case PLAYER_STATE::IDLE:
		m_ekPlayerState = PLAYER_STATE::THROW;
		break;
	case PLAYER_STATE::WALK:
		m_ekPlayerState = PLAYER_STATE::THROW;
		break;
	case PLAYER_STATE::RUN:
		m_fCurSpeed = m_fRunSpeed;
		m_ekPlayerState = PLAYER_STATE::THROW;
		break;
	}

	--m_iGrenade;
}

void cPlayer::R_IsOnceDownUpdate(float timeDelta)
{
	m_fCurSpeed = m_fWalkSpeed;

	switch (m_ekPlayerState)
	{
	case PLAYER_STATE::IDLE:
		m_ekPlayerState = PLAYER_STATE::RELOAD;
		break;
	case PLAYER_STATE::WALK:
		m_ekPlayerState = PLAYER_STATE::RELOAD;
		break;
	case PLAYER_STATE::RUN:
		m_fCurSpeed = m_fRunSpeed;
		m_ekPlayerState = PLAYER_STATE::RELOAD;
		break;
	}
}

void cPlayer::SPACE_IsOnceDownUpdate(float timeDelta)
{
	if (m_bAir)
	{
		m_pPlayerTrans->MovePositionSelf(0, m_fJumpCurPower, 0);
		m_fGravity += 0.2;
		m_fJumpCurPower -= m_fGravity;

		if (m_pPlayerTrans->GetWorldPosition().y < m_fPosTerrainHeight)
		{
			D3DXVECTOR3 pos = m_pPlayerTrans->GetWorldPosition();
			m_pPlayerTrans->SetWorldPosition(pos.x, m_fPosTerrainHeight, pos.z);
		}
	}
}

void cPlayer::LBUTTON_IsOnceDownUpdate(float timeDelta)
{
	m_fCurSpeed = m_fWalkSpeed;

	m_bReact = true;

	switch (m_ekPlayerState)
	{
	case PLAYER_STATE::IDLE:
		--m_iBulletCount;
		m_ekPlayerState = PLAYER_STATE::FIRE;
		fire();
		break;
	case PLAYER_STATE::RUN:
		--m_iBulletCount;
		m_ekPlayerState = PLAYER_STATE::FIRE;
		fire();
		break;
	case PLAYER_STATE::WALK:
		--m_iBulletCount;
		m_ekPlayerState = PLAYER_STATE::FIRE;
		fire();
		break;
	case PLAYER_STATE::AIM:
		--m_iBulletCount;
		m_ekPlayerState = PLAYER_STATE::FIRE;
		fire();
		break;
	}
}
void cPlayer::LSHIFT_IsStayDownUpdate(float timeDelta)
{
	switch (m_ekPlayerState)
	{
	case PLAYER_STATE::WALK:
		if (!m_bAir)
		{
			m_fCurSpeed = m_fRunSpeed;
			m_ekPlayerState = PLAYER_STATE::RUN;
		}
		break;
	}
}

void cPlayer::LSHIFT_IsNotStayDownUpdate(float timeDelta)
{
	switch (m_ekPlayerState)
	{
	case PLAYER_STATE::RUN:
		if (!m_bAir)
		{
			m_fCurSpeed = m_fRunSpeed;
			m_ekPlayerState = PLAYER_STATE::IDLE;
		}

		break;
	}
}

void cPlayer::RBUTTON_IsStayDownUpdate(float timeDelta)
{
	switch (m_ekPlayerState)
	{
	case PLAYER_STATE::IDLE:
		m_ekPlayerState = PLAYER_STATE::AIM;
		break;
	case PLAYER_STATE::RUN:
		m_fCurSpeed = m_fWalkSpeed;
		m_ekPlayerState = PLAYER_STATE::AIM;
		break;
	case PLAYER_STATE::WALK:
		m_ekPlayerState = PLAYER_STATE::AIM;
		break;
	}
}

void cPlayer::RBUTTON_IsNotStayDownUpdate(float timeDelta)
{
	switch (m_ekPlayerState)
	{
	case PLAYER_STATE::AIM:
		m_ekPlayerState = PLAYER_STATE::IDLE;
		break;
	}
}

void cPlayer::ShowPlayerMotionState()
{
	string str = "PLAYER MOTION STATE : ";

	switch (m_ekPlayerState)
	{
	case PLAYER_STATE::IDLE:
		str.append("IDLE");
		break;
	case PLAYER_STATE::WALK:
		str.append("WALK");
		break;
	case PLAYER_STATE::RUN:
		str.append("RUN");
		break;
	case PLAYER_STATE::FIRE:
		str.append("FIRE");
		break;
	case PLAYER_STATE::DRAW:
		str.append("DRAW");
		break;
	case PLAYER_STATE::RELOAD:
		str.append("RELOAD");
		break;
	case PLAYER_STATE::AIM:
		str.append("AIM");
		break;
	case PLAYER_STATE::SETDOWN:
		str.append("SETDOWN");
		break;
	case PLAYER_STATE::SETUP:
		str.append("SETUP");
		break;
	case PLAYER_STATE::MELEE:
		str.append("MELEE");
		break;
	case PLAYER_STATE::THROW:
		str.append("THROW");
		break;
	}

	if (Text_Check) {
		DXFONT_MGR->PrintText(str, 0, 500, 0xffffffff);

		str.clear();
		char buf[64];
		sprintf(buf, "BULLET: %d", m_iBulletCount);

		DXFONT_MGR->PrintText(buf, 0, 530, 0xffffffff);

		if (m_bAir)
			DXFONT_MGR->PrintText("AIR", 0, 560, 0xffffffff);
		else
			DXFONT_MGR->PrintText("GROUND", 0, 560, 0xffffffff);

		sprintf(buf, "POSITION HEIGHT: %f", m_pPlayerTrans->GetWorldPosition().y);
		DXFONT_MGR->PrintText(buf, 0, 590, 0xffffffff);

		sprintf(buf, "CUR_SHIELD: %f, CUR_HP: %f", m_fCurShield, m_fCurHp);
		DXFONT_MGR->PrintText(buf, 0, 620, 0xffffffff);
	}

}

void cPlayer::ShowBulletState()
{
	string str = "BULLET[0] STATE : ";

	switch (m_vecBullet[0].bulletState)
	{
	case BULLET_STATE::ON_WAY:
		str.append("ON_WAY");
		break;
	case BULLET_STATE::HIT_ENEMY_NORMAL:
		str.append("HIT_ENEMY_NORMAL");
		break;
	case BULLET_STATE::HIT_ENEMY_CRITICAL:
		str.append("HIT_ENEMY_CRITICAL");
		break;
	case BULLET_STATE::HIT_TERRAIN:
		str.append("HIT_TERRAIN");
		break;
	case BULLET_STATE::REACHED_END:
		str.append("REACHED_END");
		break;
	}
}

float cPlayer::GetHeightGap()
{
	float TerrainHeight = m_pTerrain->GetHeight(m_pPlayerTrans->GetWorldPosition().x, m_pPlayerTrans->GetWorldPosition().z);
	float PlayerHeight = m_pPlayerTrans->GetWorldPosition().y;

	return fabs(TerrainHeight - PlayerHeight);
}

void cPlayer::UpdateBullet(float timeDelta)
{
	vector<BULLET>::iterator iter;
	iter = m_vecBullet.begin();
	for (; iter != m_vecBullet.end();)
	{
		switch (iter->bulletState)
		{
		case BULLET_STATE::ON_WAY:
			iter->prevPos = iter->curPos;
			iter->curPos += iter->direction * iter->velocity * timeDelta;
			//총알에도 터레인 정보가 있어야한다.
			if (iter->curPos.y <= m_pTerrain->GetHeight(iter->curPos.x, iter->curPos.z))
			{
				iter->bulletState = BULLET_STATE::HIT_TERRAIN;
			}
			/*
			에너미 매니져
			에너미 헤드 원,
			iter->bulletState = BULLET_STATE::HIT_ENEMY_CRITICAL;
			에너미 바디 원,
			iter->bulletState = BULLET_STATE::HIT_ENEMY_NORMAL;
			충돌체크
			*/
			size_t size;
			size = m_vecEnemy1->size();
			for (size_t i = 0; i < size; ++i) {
				for (size_t z = 0; z < (*m_vecEnemy1)[i]->GetBodyPosition().size(); z++) {
					if (LineSphereCollision(iter->prevPos, iter->curPos,
						(*m_vecEnemy1)[i]->GetHeadPosition(),
						(*m_vecEnemy1)[i]->GetEnemyRadius())) {
						iter->bulletState = BULLET_STATE::HIT_ENEMY_CRITICAL;
						Total_Damage = Player_Damage * 2;
						(*m_vecEnemy1)[i]->setMinusHp(Total_Damage);
						(*m_vecEnemy1)[i]->_Enemy_Hit = true;
						_cri = true;
						m_bBlood = true;
						m_pEffect->SetBloodLocation((*m_vecEnemy1)[i]->GetHeadPosition());
						break;
					}

					else if (LineSphereCollision(iter->prevPos, iter->curPos,
						(*m_vecEnemy1)[i]->GetBodyPosition()[z]->GetWorldPosition(),
						(*m_vecEnemy1)[i]->GetEnemyRadius())) {
						iter->bulletState = BULLET_STATE::HIT_ENEMY_NORMAL;
						Total_Damage = Player_Damage;
						(*m_vecEnemy1)[i]->setMinusHp(Total_Damage);
						(*m_vecEnemy1)[i]->_Enemy_Hit = true;
						_cri = false;
						m_bBlood = true;
						m_pEffect->SetBloodLocation((*m_vecEnemy1)[i]->GetBodyPosition()[z]->GetWorldPosition());
						break;
					}
				}
			}

			size = m_vecEnemy2->size();
			for (size_t i = 0; i < size; ++i) {
				for (size_t z = 0; z < (*m_vecEnemy2)[i]->GetBodyPosition().size(); z++) {
					if (LineSphereCollision(iter->prevPos, iter->curPos,
						(*m_vecEnemy2)[i]->GetHeadPosition(),
						(*m_vecEnemy2)[i]->GetEnemyRadius())) {
						iter->bulletState = BULLET_STATE::HIT_ENEMY_CRITICAL;
						Total_Damage = Player_Damage * 2;
						(*m_vecEnemy2)[i]->setMinusHp(Total_Damage);
						(*m_vecEnemy2)[i]->_Enemy_Hit = true;
						_cri = true;
						m_bBlood = true;
						m_pEffect->SetBloodLocation((*m_vecEnemy2)[i]->GetHeadPosition());
						break;
					}

					else if (LineSphereCollision(iter->prevPos, iter->curPos,
						(*m_vecEnemy2)[i]->GetBodyPosition()[z]->GetWorldPosition(),
						(*m_vecEnemy2)[i]->GetEnemyRadius())) {
						iter->bulletState = BULLET_STATE::HIT_ENEMY_NORMAL;
						Total_Damage = Player_Damage;
						(*m_vecEnemy2)[i]->setMinusHp(Total_Damage);
						(*m_vecEnemy2)[i]->_Enemy_Hit = true;
						_cri = false;
						m_bBlood = true;
						m_pEffect->SetBloodLocation((*m_vecEnemy2)[i]->GetBodyPosition()[z]->GetWorldPosition());
						break;
					}
				}
			}

			size = m_vecBoss->size();
			for (size_t i = 0; i < size; ++i) {
				for (size_t z = 0; z < (*m_vecBoss)[i]->GetBodyPosition().size(); z++) {
					if (LineSphereCollision(iter->prevPos, iter->curPos,
						(*m_vecBoss)[i]->GetHeadPosition(),
						(*m_vecBoss)[i]->GetEnemyRadius())) {
						iter->bulletState = BULLET_STATE::HIT_ENEMY_CRITICAL;
						Total_Damage = Player_Damage * 2;
						(*m_vecBoss)[i]->setMinusHp(Total_Damage);
						(*m_vecBoss)[i]->_Enemy_Hit = true;
						_cri = true;
						m_bBlood = true;
						m_pEffect->SetBloodLocation((*m_vecBoss)[i]->GetHeadPosition());
						break;
					}

					else if (LineSphereCollision(iter->prevPos, iter->curPos,
						(*m_vecBoss)[i]->GetBodyPosition()[z]->GetWorldPosition(),
						(*m_vecBoss)[i]->GetEnemyRadius())) {
						iter->bulletState = BULLET_STATE::HIT_ENEMY_NORMAL;
						Total_Damage = Player_Damage;
						(*m_vecBoss)[i]->setMinusHp(Total_Damage);
						(*m_vecBoss)[i]->_Enemy_Hit = true;
						_cri = false;
						m_bBlood = true;
						m_pEffect->SetBloodLocation((*m_vecBoss)[i]->GetBodyPosition()[z]->GetWorldPosition());
						break;
					}
				}
			}

			if (iter->distance <= D3DXVec3Length(&(iter->curPos - iter->startPos)))
			{
				iter->bulletState = BULLET_STATE::REACHED_END;
			}
			++iter;
			break;
		case BULLET_STATE::HIT_ENEMY_NORMAL:
			/*
			피 터지는 이펙트
			이펙트 종료 후 REACHED_END
			*/
			iter->bulletState = BULLET_STATE::REACHED_END;
			++iter;
			break;
		case BULLET_STATE::HIT_ENEMY_CRITICAL:
			/*
			피 터지는 이펙트
			크리티컬 메세지 UI
			이펙트 종료 후 REACHED_END
			*/
			iter->bulletState = BULLET_STATE::REACHED_END;
			++iter;
			break;
		case BULLET_STATE::HIT_TERRAIN:
			/*
			자국남기기
			총알 튕기는 파티클
			파티클 종료 후 REACHED_END
			*/
			iter->bulletState = BULLET_STATE::REACHED_END;
			++iter;
			break;
		case BULLET_STATE::REACHED_END:
			/*
			걍 사라짐
			*/
			/*
			iter = BULLET*
			*/
			iter = m_vecBullet.erase(iter);
			break;
		}
	}
}

void cPlayer::fire()
{
	BULLET* newBullet = new BULLET;
	newBullet->bulletState = BULLET_STATE::ON_WAY;
	newBullet->startPos = m_pPlayerEye->GetWorldPosition();
	newBullet->direction = m_pPlayerEye->GetForward();
	newBullet->curPos = newBullet->startPos;
	newBullet->prevPos = newBullet->curPos;
	newBullet->velocity = 600.f;
	newBullet->distance = 400.f;
	m_vecBullet.push_back(*newBullet);
	SOUNDMANAGER->addSound("Shot", "../../Resources/Sounds/Shot.ogg", false, false);
	SOUNDMANAGER->play("Shot", 0.5f);
	Ray ray;
}

bool cPlayer::LineSphereCollision(
	D3DXVECTOR3 lineStart, D3DXVECTOR3 lineEnd, D3DXVECTOR3 center, float radius)
{
	D3DXVECTOR3 temp1 = center - lineStart;
	D3DXVECTOR3 temp2 = lineEnd - lineStart;

	float length = D3DXVec3Length(&temp1);

	D3DXVECTOR3 normalTemp1;
	D3DXVECTOR3 normalTemp2;
	D3DXVec3Normalize(&normalTemp1, &temp1);
	D3DXVec3Normalize(&normalTemp2, &temp2);

	float cos = D3DXVec3Dot(&normalTemp1, &normalTemp2);

	if (D3DXVec3Length(&(normalTemp2 * (length * cos) - temp1)) < radius)
		return true;

	return false;
}

//데미지 입력
void cPlayer::AttackedByEnemy(float damage)
{
	float hpDamage = 0;

	if (m_fCurShield < damage) {
		hpDamage = damage - m_fCurShield;
		m_fCurShield = 0;
		if (m_fCurHp < hpDamage) {
			m_fCurHp = 0;
		}
		else {
			m_fCurHp -= hpDamage;
		}
	}
	else {
		m_fCurShield -= damage;
	}
	m_fShieldRegenStartCount = 0;
	m_fShieldRegenCount = 0;

}

bool cPlayer::IsPlayerAlive()
{
	if (0 < m_fCurHp)
		return true;
	else
		return false;
}

void cPlayer::ShiledUpdate(float timeDelta, float startSec, float regenSec)
{
	if (IsPlayerAlive())
	{
		if (startSec > m_fShieldRegenStartCount) {
			m_fShieldRegenStartCount += timeDelta;
		}
		else {
			if (regenSec > m_fShieldRegenCount) {
				m_fShieldRegenCount += timeDelta;
			}
			else {
				if (m_fCurShield < m_fFullShield) {
					m_fCurShield += 1.f;
					if (m_fCurShield > m_fFullShield) {
						m_fCurShield = m_fFullShield;
					}
					m_fShieldRegenCount = 0;
				}
			}
		}
	}
}

void cPlayer::SetEnemyVector(vector<enemyClass1*>* a, vector<enemyClass2*>* b, vector<bossClass*>* c)
{
	m_vecEnemy1 = a;
	m_vecEnemy2 = b;
	m_vecBoss = c;
}

void cPlayer::GrenadeUpdate(float timeDelta) {

	vector<GRENADE>::iterator iter;
	iter = m_vecGrenade.begin();
	for (; iter != m_vecGrenade.end(); ) {
		switch (iter->GrenadeState) {
		case GRENADE_STATE::READY:
			/*
			밖에서 상태를 바꿔줘야 한다.
			밖에서 ON_WAY로 상태를 바꿔주면
			그때부터 던져지는 것...
			*/
			++iter;
			break;
		case GRENADE_STATE::ON_WAY:

			if (0.01 <= iter->Power) {
				//앞으로 가는 힘 주는 것...
				iter->Power -= 0.01;
			}
			//추진력 만큼 방향대로 나간다.
			iter->CurPos = iter->CurPos + (60 * timeDelta * iter->Direction * iter->Power);

			//지도 위에 있다면
			if (iter->CurPos.y > m_pTerrain->GetHeight(iter->CurPos.x, iter->CurPos.z)) {
				//중력값 더해지는 것...
				iter->Gravity += 60 * timeDelta * 0.01;
				iter->CurPos.y -= iter->Gravity;
			}
			else {
				//지도 밑이라면 해당 지도 높이로
				iter->CurPos.y = m_pTerrain->GetHeight(iter->CurPos.x, iter->CurPos.z);
				if (0.02 <= iter->Power) {
					iter->Power -= 0.02;
				}
			}
			iter->GrenadeObject->pTransform->SetScale(0.05, 0.05, 0.05);
			iter->GrenadeObject->pTransform->SetWorldPosition(iter->CurPos);
			iter->GrenadeObject->ComputeBoundBox();

			int size;
			size = m_pVecObjects->size();
			for (int i = 0; i < size; ++i) {
				PHYSICS_MGR->IsBlocking(iter->GrenadeObject, (*m_pVecObjects)[i], 1);
			}

			//폭발 시간 카운트
			iter->TimeCount -= timeDelta;
			if (0 > iter->TimeCount) {
				iter->GrenadeState = GRENADE_STATE::EXPLODE;
			}
			++iter;
			break;
		case GRENADE_STATE::EXPLODE:
			/*
			폭탄 터질 때 이펙트나 스프라이트 애니메이션...
			끝날 때 REACHED_END로 상태를 바꿔준다.
			*/
			if (m_bExpStart) {
				size_t sizeT;
				sizeT = m_vecBoss->size();
				float range;
				range = 10.f;
				for (size_t i = 0; i < sizeT; ++i) {
					for (size_t j = 0; j < (*m_vecBoss)[i]->GetBodyPosition().size(); ++j) {
						float length = D3DXVec3Length(&((*m_vecBoss)[i]->GetBodyPosition()[j]->GetWorldPosition() - iter->CurPos));

						//(*m_vecBoss)[i]->setMinusHp(iter->Damage);

						if (range < length) {
							continue;
						}
						else {
							(*m_vecBoss)[i]->setMinusHp(
								iter->Damage * (float(range - length) / range));
						}
					}
				}
				sizeT = m_vecEnemy1->size();
				for (size_t i = 0; i < sizeT; ++i) {
					for (size_t j = 0; j < (*m_vecEnemy1)[i]->GetBodyPosition().size(); ++j) {
						float length = D3DXVec3Length(&((*m_vecEnemy1)[i]->GetBodyPosition()[j]->GetWorldPosition() - iter->CurPos));

						//(*m_vecBoss)[i]->setMinusHp(iter->Damage);

						if (range < length) {
							continue;
						}
						else {
							(*m_vecEnemy1)[i]->setMinusHp(
								iter->Damage * (float(range - length) / range));
						}
					}
				}
				sizeT = m_vecEnemy2->size();
				for (size_t i = 0; i < sizeT; ++i) {
					for (size_t j = 0; j < (*m_vecEnemy2)[i]->GetBodyPosition().size(); ++j) {
						float length = D3DXVec3Length(&((*m_vecEnemy2)[i]->GetBodyPosition()[j]->GetWorldPosition() - iter->CurPos));

						//(*m_vecBoss)[i]->setMinusHp(iter->Damage);

						if (range < length) {
							continue;
						}
						else {
							(*m_vecEnemy2)[i]->setMinusHp(
								iter->Damage * (float(range - length) / range));
						}
					}
				}
				m_bExpStart = false;
			}
			else {
				if (FLOATZERO(m_fGrenadeCount))
				{
					SOUNDMANAGER->addSound("Grenade_Explosion", "../../Resources/Sounds/Grenade_Explosion.ogg", false, false);
					SOUNDMANAGER->play("Grenade_Explosion", 1.0f);
				}
				m_fGrenadeCount += timeDelta;
				if (m_fGrenadeCount > 0.5f) {
					iter->GrenadeState = GRENADE_STATE::REACHED_END;
					m_bExpStart = true;
					m_fGrenadeCount = 0;
				}
			}
			++iter;
			break;
		case GRENADE_STATE::REACHED_END:
			iter = m_vecGrenade.erase(iter);
			break;
		}
	}

}