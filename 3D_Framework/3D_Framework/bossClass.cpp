#include "stdafx.h"
#include "bossClass.h"
#include "cParticleEmitter.h"

HRESULT bossClass::Init(float scale /*1.f*/, float followRange/*32.f*/, float airAttRange/*16.f*/, float enemyRange/*4.f*/, int SKAG_MODE, D3DXVECTOR3 EnemyPos,
	vector<cSetBoundObject*> pVecObjects, cPlayer* player, cTerrain* Terrain)
{
	screenPos.x = WINSIZE_X / 2;
	screenPos.y = WINSIZE_Y / 2 - 70;
	this->Cri.SetLocalPosition(D3DXVECTOR3(screenPos.x - 30, screenPos.y - 50, 0));
	this->Cri.ShakePos(D3DXVECTOR3(screenPos.x - 30, screenPos.y - 50, 0));

	t = 0.f;
	tempBoundS.SetBound(&D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(30, 30, 30));
	_cPlayer = new cPlayer;
	_cPlayer = player;

	_cTerrain = new cTerrain;
	_cTerrain = Terrain;
	_Enemy_Hit_Count = 0.f;
	_Enemy_Hit = false;
	_Enemy_Stat.F_ok = true;

	m_pVecObjects = pVecObjects;

	followRange *= 17.5;
	airAttRange *= 17.5;
	enemyRange *= 17.5;

	_enemyTrans.SetWorldPosition(EnemyPos);
	_enemyColisionTrans.SetWorldPosition(_enemyTrans.GetWorldPosition().x, _enemyTrans.GetWorldPosition().y + enemyRange / 4, _enemyTrans.GetWorldPosition().z + enemyRange / 8);
	_enemyTrans.SetScale(scale, scale, scale);
	_enemyAirAttTrans.SetWorldPosition(D3DXVECTOR3(_enemyTrans.GetWorldPosition().x, _enemyTrans.GetWorldPosition().y, _enemyTrans.GetWorldPosition().z));
	_enemyAirAttTrans.SetScale(scale, scale, scale);
	_enemyAttRangeSphere.SetBound(&D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(followRange, followRange, followRange));
	_enemyAirAttRangeSphere.SetBound(&D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(airAttRange, airAttRange, airAttRange));
	_enemySphere.SetBound(&D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(enemyRange, enemyRange, enemyRange));
	_enemyCoreSphere.SetBound(&D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(enemyRange / 2, enemyRange / 2, enemyRange / 2));

	_enemyAirAttTransReset.SetWorldPosition(_enemyTrans.GetWorldPosition().x, _enemyTrans.GetWorldPosition().y, _enemyTrans.GetWorldPosition().z + _enemyAirAttRangeSphere.radius);

	D3DXMATRIXA16 matCorrection;
	D3DXMatrixScaling(&matCorrection, 0.175f, 0.175f, 0.175f);

	cXMesh_Skinned* pSkinned = RESOURCE_SKINNEDXMESH->
		GetResource("../../Resources/Enemy/skag3.X",
			&matCorrection);

	cBaseObject* Enemy_obj;
	Enemy_obj = new cBaseObject();
	Enemy_obj->SetMesh(pSkinned);
	Enemy_obj->SetActive(true);
	vShadow_Enemy.push_back(Enemy_obj);

	vShadow_Enemy[0]->pTransform->RotateSelf(0, -D3DX_PI / 2, 0);

	//
	//this->pSkinnedTrans = new cTransform();
	this->_skinnedTrans.SetWorldPosition(_enemyTrans.GetWorldPosition().x, _enemyTrans.GetWorldPosition().y, _enemyTrans.GetWorldPosition().z);

	//this->pApplyTrans = new cTransform();
	vShadow_Enemy[0]->pSkinned->AddApplyTransform("Bip01", &_enemyTrans);

	vShadow_Enemy[0]->pSkinned->AddBoneTransform("Bone20", &_leftHeadTrans);
	vShadow_Enemy[0]->pSkinned->AddBoneTransform("Bone17", &_rightHeadTrans);
	vShadow_Enemy[0]->pSkinned->AddBoneTransform("Bone12", &_centerHeadTrans);
	vShadow_Enemy[0]->pSkinned->AddBoneTransform("Bip01_Head", &_centerBackHeadTrans);

	vShadow_Enemy[0]->pSkinned->AddBoneTransform("Bip01_Head", &_beamTrans);
	_beamTrans.SetWorldPosition(_centerBackHeadTrans.GetWorldPosition());
	_beamTrans.RotateLocal(0.0f, D3DX_PI / 4, 0.0f);
	_beamTrans.MovePositionSelf(0.0f, 0.0f, 0.5f);
	_beamTrans.AttachTo(&_centerBackHeadTrans);

	for (int i = 0; i < 26; i++)
	{
		cTransform* temp = new cTransform;
		_tempTrans.push_back(temp);
	}
	vShadow_Enemy[0]->pSkinned->AddBoneTransform("Bip01_Neck1", _tempTrans[0]);
	vShadow_Enemy[0]->pSkinned->AddBoneTransform("Bip01_R_Clavicle", _tempTrans[1]);
	vShadow_Enemy[0]->pSkinned->AddBoneTransform("Bip01_L_Clavicle", _tempTrans[2]);
	vShadow_Enemy[0]->pSkinned->AddBoneTransform("Bip01", _tempTrans[3]);
	vShadow_Enemy[0]->pSkinned->AddBoneTransform("Bip01_Spine1", _tempTrans[4]);
	vShadow_Enemy[0]->pSkinned->AddBoneTransform("Bip01_Spine2", _tempTrans[5]);

	vShadow_Enemy[0]->pSkinned->AddBoneTransform("Bip01_R_UpperArm", _tempTrans[6]);
	vShadow_Enemy[0]->pSkinned->AddBoneTransform("Bip01_L_UpperArm", _tempTrans[7]);
	vShadow_Enemy[0]->pSkinned->AddBoneTransform("Bip01_R_Forearm", _tempTrans[8]);
	vShadow_Enemy[0]->pSkinned->AddBoneTransform("Bip01_L_Forearm", _tempTrans[9]);
	vShadow_Enemy[0]->pSkinned->AddBoneTransform("Bip01_R_Thigh", _tempTrans[10]);
	vShadow_Enemy[0]->pSkinned->AddBoneTransform("Bip01_L_Thigh", _tempTrans[11]);
	vShadow_Enemy[0]->pSkinned->AddBoneTransform("Bip01_R_Calf", _tempTrans[12]);
	vShadow_Enemy[0]->pSkinned->AddBoneTransform("Bip01_L_Calf", _tempTrans[13]);
	vShadow_Enemy[0]->pSkinned->AddBoneTransform("Bip01_R_HorseLink", _tempTrans[14]);
	vShadow_Enemy[0]->pSkinned->AddBoneTransform("Bip01_L_HorseLink", _tempTrans[15]);
	vShadow_Enemy[0]->pSkinned->AddBoneTransform("Bip01_R_Foot", _tempTrans[16]);
	vShadow_Enemy[0]->pSkinned->AddBoneTransform("Bip01_L_Foot", _tempTrans[17]);
	vShadow_Enemy[0]->pSkinned->AddBoneTransform("Bip01_R_Sholder", _tempTrans[18]);
	vShadow_Enemy[0]->pSkinned->AddBoneTransform("Bip01_L_Sholder", _tempTrans[19]);
	vShadow_Enemy[0]->pSkinned->AddBoneTransform("Bip01_R_Toe0", _tempTrans[20]);
	vShadow_Enemy[0]->pSkinned->AddBoneTransform("Bip01_L_Toe0", _tempTrans[21]);
	vShadow_Enemy[0]->pSkinned->AddBoneTransform("Bip01_R_Hand", _tempTrans[22]);
	vShadow_Enemy[0]->pSkinned->AddBoneTransform("Bip01_L_Hand", _tempTrans[23]);
	vShadow_Enemy[0]->pSkinned->AddBoneTransform("Bip01_R_Finger0", _tempTrans[24]);
	vShadow_Enemy[0]->pSkinned->AddBoneTransform("Bip01_L_Finger0", _tempTrans[25]);

	vShadow_Enemy[0]->pSkinned->SetPlaySpeed(1.0f);

	//Anims_25 죽는 모션


	vShadow_Enemy[0]->pSkinned->Play("Anims_3", 0.3f);

	_skinnedTrans.RotateSelf(0, -D3DX_PI / 2, 0);

	D3DXVECTOR3 boundSizeVector(ENEMY_BOUND_SIZE, ENEMY_BOUND_SIZE, ENEMY_BOUND_SIZE);
	_boundBox.SetBound(&_centerBackHeadTrans.GetWorldPosition(), &boundSizeVector);


	_unitNomalTrans.SetWorldPosition(_enemyTrans.GetWorldPosition());			////////////////////////////////////////////////////////
	_startUnitVector3 = _enemyTrans.GetWorldPosition();

	m_pParticle = new cParticleEmitter();

	m_pParticle->SetActive(true);
	//배열을 2 개이상
	VEC_COLOR colors;
	colors.push_back(D3DXCOLOR(0.1f, 1.0f, 1.0f, 1.0f));
	colors.push_back(D3DXCOLOR(0.1f, 1.0f, 1.0f, 1.0f));
	colors.push_back(D3DXCOLOR(0.1f, 1.0f, 1.0f, 1.0f));

	VEC_SCALE scales;
	scales.push_back(1.0f);
	scales.push_back(1.0f);
	scales.push_back(1.0f);

	LPDIRECT3DTEXTURE9 pTex = RESOURCE_TEXTURE->GetResource(
		"../../Resources/Testures/particle_0.png");

	//파티클 랜더러 셋팅
	m_pParticle->Init(
		1000,				//최대 파티클 수
		200.0f,				//초당 파티클 발생 량
		5,					//하나의 파티클 입자 라이프 최소값
		5,					//하나의 파티클 입자 라이프 최대값
		D3DXVECTOR3(0, 0, 20),	//파티클 입자 속도 최소값 ( 로컬기준 )
		D3DXVECTOR3(0, 0, 20),	//파티클 입자 속도 최대값 ( 로컬기준 )
		D3DXVECTOR3(0, 0, 0),	//파티클 입자 가속 최소값 ( 로컬기준 )
		D3DXVECTOR3(0, 0, 0), //파티클 입자 가속 최대값 ( 로컬기준 )
		colors,				//컬러 배열
		scales,				//스케일 배열
		1.0f,				//입자크기 최소값
		1.0f,				//최대값
		pTex,				//텍스쳐
		true);

	m_pParticle->pTransform->RotateLocal(0.0f, D3DX_PI / 4, 0.0f);
	m_pParticle->pTransform->MovePositionSelf(0.0f, 0.0f, 0.5f);
	m_pParticle->pTransform->AttachTo(&_centerBackHeadTrans);

	SOUNDMANAGER->addSound("근접공격1", "../../Resources/Sounds/enemy/근접공격1.ogg", false, false);
	SOUNDMANAGER->addSound("근접공격2", "../../Resources/Sounds/enemy/근접공격2.ogg", false, false);
	SOUNDMANAGER->addSound("보스 레이저공격", "../../Resources/Sounds/enemy/보스 레이저공격.ogg", false, false);
	SOUNDMANAGER->addSound("보스 죽는소리", "../../Resources/Sounds/enemy/보스 죽는소리.ogg", false, false);
	SOUNDMANAGER->addSound("스캐그 죽는소리", "../../Resources/Sounds/enemy/스캐그 죽는소리.ogg", false, false);
	SOUNDMANAGER->addSound("입 벌릴때", "../../Resources/Sounds/enemy/입 벌릴때.ogg", false, false);
	SOUNDMANAGER->addSound("입벌리며 공격", "../../Resources/Sounds/enemy/입벌리며 공격.ogg", false, false);
	SOUNDMANAGER->addSound("투사체 공격소리", "../../Resources/Sounds/enemy/투사체 공격소리.ogg", false, false);
	SOUNDMANAGER->addSound("투사체공격 몹 죽는소리", "../../Resources/Sounds/enemy/투사체공격 몹 죽는소리.ogg", false, false);


	return S_OK;
}
void bossClass::Release()
{
	vShadow_Enemy[0]->pSkinned->Release();
}
void bossClass::Update(float timeDelta, cBoundSphere *playerSphere, cBoundSphere *playerCoreSphere, bool playerAir, D3DXVECTOR3 playerXYZ)
{


	if (_Enemy_Hit) _Enemy_Hit_Count += 0.1f;
	if (_Enemy_Hit_Count > 3) { _Enemy_Hit = false; _Enemy_Hit_Count = 0; }

	float get_y = _cTerrain->GetHeight(_enemyTrans.GetWorldPosition().x, _enemyTrans.GetWorldPosition().z);
	vShadow_Enemy[0]->pTransform->SetLocalPosition(_enemyTrans.GetWorldPosition().x, get_y, _enemyTrans.GetWorldPosition().z);
	vShadow_Enemy[0]->Update(timeDelta);

	cTransform tempPlayer = *_cPlayer->m_pPlayerTrans;

	

	for (int i = 0; i < m_pVecObjects.size(); i++)
	{
		PHYSICS_MGR->IsBlocking(&_enemyTrans, &_boundBox, m_pVecObjects[i]->pTransform, &m_pVecObjects[i]->BoundBox, 1);

	}

	if (_enemyMode == EYNEMY_MODE_BEAM_ATT)		//★★★★★★★★★★★★★★★★★★★★
	{
		m_pParticle->Update(timeDelta);
		m_pParticle->StartEmission();
	}											//★★★★★★★★★★★★★★★★★★★★

	if (_enemyMode == EYNEMY_MODE_BEAM_ATT && _prevEnemyMode != _enemyMode)
		SOUNDMANAGER->play("입벌리며 공격", 1.0F);
	//★★★★★★★★★★★★★★★★★★★★

	_prevEnemyMode = _enemyMode;

	vShadow_Enemy[0]->pSkinned->Update(timeDelta);

	if (!_Enemy_Stat.LiveOrDie)
	{
		_dieTimer++;

		if (_dieTimer >= REAL_DIE_TIMER)
		{
			_rearDie = true;
		}
	}

	if (_Enemy_Stat.Hp <= 0 && _Enemy_Stat.LiveOrDie)
	{
		_enemyMode = ENEMY_MODE_NULL;
		if (_prevEnemyMode != _enemyMode)
		{
			vShadow_Enemy[0]->pSkinned->PlayOneShotAfterHold("Anims_26", 0.3f);
		}
		//if (vShadow_Enemy[0]->pSkinned->getPlay())
		{
			_Enemy_Stat.LiveOrDie = false;
			SOUNDMANAGER->play("보스 죽는소리", 1.0F);
		}
		_Enemy_Stat.F_ok = false;
	}
	if (_Enemy_Stat.Hp > 0)
	{

		//공중 공격
		airAttSet(&tempPlayer, playerSphere, playerCoreSphere);

		//기본 따라가기
		if (_enemyMode == ENEMY_MODE_RUN_STAY || _enemyMode == ENEMY_MODE_MOVE)
			normalFollow(&tempPlayer, playerSphere);

		//빔 공격
		if (_beamAttTimer <= 1000)
			_beamAttTimer++;

		if (_beamAttTimer >= 1000 &&// vShadow_Enemy[0]->pSkinned->getPlay()) && 
			PHYSICS_MGR->IsOverlap(&tempPlayer, playerSphere, &_enemyTrans, &_enemyAirAttRangeSphere) &&
			PHYSICS_MGR->IsOverlap(&tempPlayer, playerSphere, &_enemyTrans, &_enemyAttRangeSphere) &&
			_enemyMode == ENEMY_MODE_RUN_STAY)
		{
			_enemyMode = EYNEMY_MODE_BEAM_ATT;
			_beamAttTimer = 0;
			vShadow_Enemy[0]->pSkinned->SetPlaySpeed(0.3f);
			vShadow_Enemy[0]->pSkinned->PlayOneShotAfterHold("Anims_1", 0.3f);

			for (int i = 0; i < 100; i++)
			{
				cTransform* tempTrans = new cTransform;
				*tempTrans = _beamTrans;

				float z1 = tempTrans->GetFinalMatrix()._31 * 0.5f * i;
				float z2 = tempTrans->GetFinalMatrix()._32 * 0.5f * i;
				float z3 = tempTrans->GetFinalMatrix()._33 * 0.5f * i;
				D3DXVECTOR3 tempVec = D3DXVECTOR3(z1, z2, z3);

				tempTrans->SetWorldPosition(tempTrans->GetWorldPosition() + tempVec);
				_beamAttTransVector.push_back(tempTrans);
				//_beamAttTransVector[i]->AttachTo(&_beamTrans);
			}
		}

		if (_enemyMode == EYNEMY_MODE_BEAM_ATT)
		{
			_beamDelayTimer++;
			if (_beamDelayTimer >= 10)
			{
				_beamDelayTimer = 0;
				_attCollFlag = true;
			}
		}

		if ((_prevEnemyMode == EYNEMY_MODE_BEAM_ATT && vShadow_Enemy[0]->pSkinned->getPlay()) || 
			_enemyMode != EYNEMY_MODE_BEAM_ATT && _beamAttTransVector.size() > 0)
		{
			_enemyMode = ENEMY_MODE_RUN_STAY;
			vShadow_Enemy[0]->pSkinned->SetPlaySpeed(1.f);
			vShadow_Enemy[0]->pSkinned->Play("Anims_5", 0.3f);
			for (int i = 0; i < _beamAttTransVector.size(); i++)
			{
				_beamAttTransVector.erase(_beamAttTransVector.begin() + i);
			}
			_beamAttTransVector.clear();
		}

		//공중 공격중이 아닐 때 보간처리
		if (_enemyAttFlag && !playerAir)
			interpolation(&tempPlayer);

		if (PHYSICS_MGR->IsOverlap(&tempPlayer, playerSphere, &_enemyTrans, &_enemySphere) && !_meleeAttFlag &&
			_enemyMode != ENEMY_MODE_NORMAL_ATT1 &&
			_enemyMode != ENEMY_MODE_NORMAL_ATT2 &&
			_enemyMode != ENEMY_MODE_NORMAL_ATT3)
		{
			_enemyMode = rand() % 3 + 6;		//근접 공격모드
			_movePoint.clear();
			_airAttCondition = false;
			_meleeAttFlag = true;
			/// _enemyMode를 근접 공격 애니메이션이 끝나면 한번 더 돌려준다
			/// 이 때 애니메이션이 끝났을 때 거리가 멀어진다면 다시 모드를 ENEMY_MODE_STATE로 변경시켜야한다
			switch (_enemyMode)
			{
			case ENEMY_MODE_NORMAL_ATT1:SOUNDMANAGER->play("근접공격1", 1.0F); break;
			case ENEMY_MODE_NORMAL_ATT2:SOUNDMANAGER->play("근접공격2", 1.0F); break;
			case ENEMY_MODE_NORMAL_ATT3:SOUNDMANAGER->play("입벌리며 공격", 1.0F); break;
			}
		}

		if (_meleeAttFlag &&  vShadow_Enemy[0]->pSkinned->getPlay())
		{
			_meleeAttFlag = false;
		}


		if (_prevEnemyMode != _enemyMode)
		{
			switch (_enemyMode)
			{
			case ENEMY_MODE_NULL:
				_enemyMode = ENEMY_MODE_RUN_STAY;
				break;
			case ENEMY_MODE_RUN_STAY:
				vShadow_Enemy[0]->pSkinned->SetPlaySpeed(1.0f);
				vShadow_Enemy[0]->pSkinned->Play("Anims_5", 0.3f);
				_attCollFlag = false;		//★
				_damageFlag = false;		//★
				break;
			case ENEMY_MODE_MOVE:
				vShadow_Enemy[0]->pSkinned->SetPlaySpeed(1.0f);
				vShadow_Enemy[0]->pSkinned->Play("Anims_3", 0.3f);
				_attCollFlag = false;		//★
				_damageFlag = false;		//★
				break;
			case ENEMY_MODE_STATE:
				vShadow_Enemy[0]->pSkinned->SetPlaySpeed(1.0f);
				vShadow_Enemy[0]->pSkinned->Play("Anims_2", 0.3f);
				_attCollFlag = false;		//★
				_damageFlag = false;		//★
				break;
			case ENEMY_MODE_JUMP_SUCCESS:
				vShadow_Enemy[0]->pSkinned->SetPlaySpeed(1.0f);
				vShadow_Enemy[0]->pSkinned->PlayOneShotAfterHold("Anims_13", 0.3f);		//공중
				_attCollFlag = true;		//★
				break;
			case ENEMY_MODE_JUMP_FAIL:
				vShadow_Enemy[0]->pSkinned->SetPlaySpeed(1.0f);
				vShadow_Enemy[0]->pSkinned->PlayOneShotAfterHold("Anims_14", 0.3f);
				_attCollFlag = true;		//★
				break;
			case ENEMY_MODE_CLAW1:
				vShadow_Enemy[0]->pSkinned->SetPlaySpeed(1.0f);
				vShadow_Enemy[0]->pSkinned->PlayOneShotAfterHold("Anims_8", 0.3f);		//근접
				_attCollFlag = true;		//★
				break;
			case ENEMY_MODE_CLAW2:
				vShadow_Enemy[0]->pSkinned->SetPlaySpeed(1.0f);
				vShadow_Enemy[0]->pSkinned->PlayOneShotAfterHold("Anims_9", 0.3f);
				_attCollFlag = true;		//★
				break;
			case ENEMY_MODE_NORMAL_ATT1:
				vShadow_Enemy[0]->pSkinned->SetPlaySpeed(1.0f);
				vShadow_Enemy[0]->pSkinned->PlayOneShotAfterHold("Anims_10", 0.3f);
				_attCollFlag = true;		//★
				break;
			case ENEMY_MODE_NORMAL_ATT2:
				vShadow_Enemy[0]->pSkinned->SetPlaySpeed(1.0f);
				vShadow_Enemy[0]->pSkinned->PlayOneShotAfterHold("Anims_11", 0.3f);
				_attCollFlag = true;		//★
				break;
			case ENEMY_MODE_NORMAL_ATT3:
				vShadow_Enemy[0]->pSkinned->SetPlaySpeed(1.0f);
				vShadow_Enemy[0]->pSkinned->PlayOneShotAfterHold("Anims_12", 0.3f);
				_attCollFlag = true;		//★
				break;
			}
		}

		if (_aniPlayFlag)
		{
			_movePoint.clear();
			switch (_enemyMode)
			{
			case ENEMY_MODE_JUMP_SUCCESS:
				_enemyMode = ENEMY_MODE_RUN_STAY;
				vShadow_Enemy[0]->pSkinned->SetPlaySpeed(1.0f);
				vShadow_Enemy[0]->pSkinned->Play("Anims_5", 0.3f);
				_airAttMode = false;
				_airAttCondition = false;
				_airAttDis = 0.0f;
				_attTrue = false;
				_enemyAirAttTrans.SetWorldPosition(D3DXVECTOR3(_enemyTrans.GetWorldPosition().x, _enemyTrans.GetWorldPosition().y, _enemyTrans.GetWorldPosition().z));
				_aniPlayFlag = false;
				_airAttDecisionFlag = false;
				_attCollFlag = false;		//★
				_damageFlag = false;		//★
				break;
			case ENEMY_MODE_JUMP_FAIL:
				_enemyMode = ENEMY_MODE_RUN_STAY;
				vShadow_Enemy[0]->pSkinned->SetPlaySpeed(1.0f);
				vShadow_Enemy[0]->pSkinned->Play("Anims_5", 0.3f);
				_airAttMode = false;
				_airAttCondition = false;
				_airAttDis = 0.0f;
				_attTrue = false;
				_enemyAirAttTrans.SetWorldPosition(D3DXVECTOR3(_enemyTrans.GetWorldPosition().x, _enemyTrans.GetWorldPosition().y, _enemyTrans.GetWorldPosition().z));
				_aniPlayFlag = false;
				_airAttDecisionFlag = false;
				_attCollFlag = false;		//★
				_damageFlag = false;		//★
				break;
			}
		}

		//공중공격 성공
		if (_enemyMode == ENEMY_MODE_JUMP_SUCCESS && vShadow_Enemy[0]->pSkinned->getPlay())
		{
			//_aniPlayFlag = true;
			_enemyMode = ENEMY_MODE_RUN_STAY;
			vShadow_Enemy[0]->pSkinned->SetPlaySpeed(1.0f);
			vShadow_Enemy[0]->pSkinned->Play("Anims_5", 0.3f);
			_enemyTrans.MovePositionSelf(0.f, 0.f, ENEMY_JUMP_ATT_SUCCESS);
			_enemyAirAttTrans.SetWorldPosition(D3DXVECTOR3(_enemyTrans.GetWorldPosition().x, _enemyTrans.GetWorldPosition().y, _enemyTrans.GetWorldPosition().z));
			vShadow_Enemy[0]->pSkinned->Play("Anims_3", 0.3f);
			_attCollFlag = true;		//★
		}
		//공중공격 실패
		if (_enemyMode == ENEMY_MODE_JUMP_FAIL && vShadow_Enemy[0]->pSkinned->getPlay())
		{
			//_aniPlayFlag = true;
			_enemyMode = ENEMY_MODE_RUN_STAY;
			vShadow_Enemy[0]->pSkinned->SetPlaySpeed(1.0f);
			vShadow_Enemy[0]->pSkinned->Play("Anims_3", 0.3f);
			_enemyTrans.MovePositionSelf(ENEMY_JUMP_ATT_FAIL_X, 0.f, ENEMY_JUMP_ATT_FAIL_Z);
			_enemyAirAttTrans.SetWorldPosition(D3DXVECTOR3(_enemyTrans.GetWorldPosition().x, _enemyTrans.GetWorldPosition().y, _enemyTrans.GetWorldPosition().z));
			vShadow_Enemy[0]->pSkinned->Play("Anims_5", 0.3f);
			_enemyTrans.RotateSelf(0.f, D3DX_PI, 0.f);
			_airAttDecisionFlag = false;
			_attCollFlag = false;		//★
		}

		//근접공격 이후 따라가기
		if ((_prevEnemyMode == ENEMY_MODE_CLAW1 || _prevEnemyMode == ENEMY_MODE_CLAW2 ||
			_prevEnemyMode == ENEMY_MODE_NORMAL_ATT1 || _prevEnemyMode == ENEMY_MODE_NORMAL_ATT2 ||
			_prevEnemyMode == ENEMY_MODE_NORMAL_ATT3) && !PHYSICS_MGR->IsOverlap(&tempPlayer, playerSphere, &_enemyTrans, &_enemySphere) &&
			_enemyMode != ENEMY_MODE_JUMP_FAIL && _enemyMode != ENEMY_MODE_JUMP_SUCCESS && _enemyMode != ENEMY_MODE_AIR_ATT &&
			vShadow_Enemy[0]->pSkinned->getPlay())
		{
			_enemyMode = ENEMY_MODE_RUN_STAY;
			//_aniPlayFlag = true;
			vShadow_Enemy[0]->pSkinned->SetPlaySpeed(1.0f);
			vShadow_Enemy[0]->pSkinned->Play("Anims_5", 0.3f);
			_meleeAttFlag = false;
			_airAttDecisionFlag = false;
			_airAttCondition = false;			////////////////////////////////////////////////////////////////////////////
			_attCollFlag = false;		//★
			_damageFlag = false;		//★
		}

		//빔 트랜스폼 좌표 생성


		//충돌용 트랜스폼 좌표 보정
		amendTrans();

		//★
		if (_attCollFlag)
		{
			cBoundSphere tempSphere;
			tempSphere.SetBound(&D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(_enemyCoreSphere.radius / 3, _enemyCoreSphere.radius / 3, _enemyCoreSphere.radius / 3));

			switch (_enemyMode)
			{
			case ENEMY_MODE_NORMAL_ATT1:			//머리
				if (PHYSICS_MGR->IsOverlap(&tempPlayer, playerSphere, &_centerHeadTrans, &_enemyCoreSphere))
				{
					_damageFlag = true;
					_attCollFlag = false;
				}
				break;
			case ENEMY_MODE_NORMAL_ATT2:			//손톱
			case ENEMY_MODE_NORMAL_ATT3:			//손톱
				if (PHYSICS_MGR->IsOverlap(&tempPlayer, playerSphere, _tempTrans[22], &_enemyCoreSphere))
				{
					_damageFlag = true;
					_attCollFlag = false;
				}
				break;
			case EYNEMY_MODE_BEAM_ATT:
				for (int i = 0; i < _beamAttTransVector.size(); i++)
				{
					if (PHYSICS_MGR->IsOverlap(&tempPlayer, playerSphere, _beamAttTransVector[i], &tempSphere))
					{
						_damageFlag = true;
						_attCollFlag = false;
					}
				}
				_attCollFlag = false;
				break;
			case ENEMY_MODE_JUMP_SUCCESS:
				_damageFlag = true;
				_attCollFlag = false;
				break;
			}
		}
	}

	this->Cri.ShakeUpdate(timeDelta);
}
void bossClass::Render(cCamera* pMainCamera, cLight_Direction* Light_Dt)
{
	if (_Enemy_Hit) {
		char str[128];
		if (_cPlayer->_cri == true)
		{
			this->Cri.ShakePos(5.0, 5.0);
			sprintf_s(str, "CRITICAL", _cPlayer->getTotal_Damage());
			DXFONT_MGR->PrintTextOutline(str, Cri.GetLocalPosition().x, Cri.GetLocalPosition().y, 0xffff0000, 0xff000000);
			sprintf(str, "%d", _cPlayer->getTotal_Damage());
			DXFONT_MGR->PrintTextOutline(str, screenPos.x, screenPos.y, 0xffff0000, 0xff000000);
		}
		else
			sprintf(str, "%d", _cPlayer->getTotal_Damage());
		DXFONT_MGR->PrintTextOutline(str, screenPos.x, screenPos.y, 0xffff0000, 0xff000000);
	}

	cXMesh_Static::SetBaseLight(Light_Dt);
	cXMesh_Static::SetTechniqueName("Toon");
	cXMesh_Static::SetCamera(pMainCamera);

	cXMesh_Skinned::SetCamera(pMainCamera);
	cXMesh_Skinned::SetTechniqueName("ReciveShadowToonMaskSkag");
	cXMesh_Skinned::sSkinnedMeshEffect->SetVector("vColor1", &D3DXVECTOR4(0.7f, 0.5f, 0.5f, 1));
	cXMesh_Skinned::sSkinnedMeshEffect->SetVector("vColor2", &D3DXVECTOR4(6.5f, 4.5f, 4.5f, 1));
	cXMesh_Skinned::sSkinnedMeshEffect->SetVector("vColor3", &D3DXVECTOR4(6, 6, 6, 1));
	cXMesh_Skinned::sSkinnedMeshEffect->SetTexture("Ramp_Tex", RESOURCE_TEXTURE->GetResource("../../Resources/Testures/Ramp_1.png"));
	cXMesh_Skinned::SetBaseLight(Light_Dt);

	vShadow_Enemy[0]->Render();

	if (EnemySphere_Check) {
		GIZMO_MGR->WireSphere(_enemyColisionTrans.GetLocalPosition(), _enemyAttRangeSphere.radius, 0xff0000ff);
		GIZMO_MGR->WireSphere(_enemyColisionTrans.GetLocalPosition(), _enemyAirAttRangeSphere.radius, 0xff00ff00);
		GIZMO_MGR->WireSphere(_enemyAirAttTrans.GetLocalPosition(), _enemyCoreSphere.radius, 0xffffff00);

		GIZMO_MGR->WireSphere(_enemyAirAttTransReset.GetLocalPosition(), _enemyCoreSphere.radius, 0xffffff00);


		GIZMO_MGR->WireSphere(_enemyTrans.GetLocalPosition(), _enemyCoreSphere.radius, 0xffffff);


		GIZMO_MGR->WireSphere(_leftHeadTrans.GetLocalPosition(), _enemyCoreSphere.radius / 3, 0xff00ff);
		GIZMO_MGR->WireSphere(_rightHeadTrans.GetLocalPosition(), _enemyCoreSphere.radius / 3, 0xff00ff);
		GIZMO_MGR->WireSphere(_centerHeadTrans.GetLocalPosition(), _enemyCoreSphere.radius / 2, 0xff00ff);
		GIZMO_MGR->WireSphere(_centerBackHeadTrans.GetLocalPosition(), _enemyCoreSphere.radius / 2, 0xff00ff);


		GIZMO_MGR->WireSphere(_tempTrans[0]->GetLocalPosition(), _enemyCoreSphere.radius / 3, 0x00ff00);
		GIZMO_MGR->WireSphere(_tempTrans[3]->GetLocalPosition(), _enemyCoreSphere.radius / 3, 0x00ff00);

		GIZMO_MGR->WireSphere(_tempTrans[1]->GetLocalPosition(), _enemyCoreSphere.radius, 0x00ff00);
		GIZMO_MGR->WireSphere(_tempTrans[2]->GetLocalPosition(), _enemyCoreSphere.radius, 0x00ff00);

		for (int i = 4; i < 26; i++)
		{
			GIZMO_MGR->WireSphere(_tempTrans[i]->GetLocalPosition(), _enemyCoreSphere.radius / 3, 0x00ff00);
		}
	}
	if(_enemyMode == EYNEMY_MODE_BEAM_ATT)
		m_pParticle->Render();

	char str[128];
	switch (_enemyMode)
	{
	case ENEMY_MODE_APPEARANCE:			  wsprintf(str, "ENEMY_MODE_APPEARANCE");			break;
	case ENEMY_MODE_STATE:				  wsprintf(str, "ENEMY_MODE_STATE");				break;
	case ENEMY_MODE_MOVE:				  wsprintf(str, "ENEMY_MODE_MOVE");					break;
	case ENEMY_MODE_RUN_READY:			  wsprintf(str, "ENEMY_MODE_RUN_READY");			break;
	case ENEMY_MODE_RUN_STAY:			  wsprintf(str, "ENEMY_MODE_RUN_STAY");				break;
	case ENEMY_MODE_RUN_END:			  wsprintf(str, "ENEMY_MODE_RUN_END");				break;
	case ENEMY_MODE_NORMAL_ATT1:		  wsprintf(str, "ENEMY_MODE_NORMAL_ATT1");			break;
	case ENEMY_MODE_NORMAL_ATT2:		  wsprintf(str, "ENEMY_MODE_NORMAL_ATT2");			break;
	case ENEMY_MODE_NORMAL_ATT3:		  wsprintf(str, "ENEMY_MODE_NORMAL_ATT3");			break;
	case ENEMY_MODE_CLAW1:				  wsprintf(str, "ENEMY_MODE_CLAW1");				break;
	case ENEMY_MODE_CLAW2:				  wsprintf(str, "ENEMY_MODE_CLAW2");				break;
	case ENEMY_MODE_JUMP_SUCCESS:		  wsprintf(str, "ENEMY_MODE_JUMP_SUCCESS");			break;
	case ENEMY_MODE_JUMP_FAIL:			  wsprintf(str, "ENEMY_MODE_JUMP_FAIL");			break;
	case ENEMY_MODE_SPIT1:				  wsprintf(str, "ENEMY_MODE_SPIT1");				break;
	case ENEMY_MODE_SPIT2:				  wsprintf(str, "ENEMY_MODE_SPIT2");				break;
	case ENEMY_MODE_HIT_IDLE_HEAD_HEAVY_L:wsprintf(str, "ENEMY_MODE_HIT_IDLE_HEAD_HEAVY_L"); break;
	case ENEMY_MODE_HIT_IDLE_HEAD_HEAVY_R:wsprintf(str, "ENEMY_MODE_HIT_IDLE_HEAD_HEAVY_R"); break;
	case ENEMY_MODE_HIT_IDLE_HEAD_LIGHT_L:wsprintf(str, "ENEMY_MODE_HIT_IDLE_HEAD_LIGHT_L"); break;
	case ENEMY_MODE_HIT_IDLE_HEAD_LIGHT_R:wsprintf(str, "ENEMY_MODE_HIT_IDLE_HEAD_LIGHT_R"); break;
	case ENEMY_MODE_HIT_RUN_HEAD_HEAVY_L: wsprintf(str, "ENEMY_MODE_HIT_RUN_HEAD_HEAVY_L");	break;
	case ENEMY_MODE_HIT_RUN_HEAD_HEAVY_R: wsprintf(str, "ENEMY_MODE_HIT_RUN_HEAD_HEAVY_R");	break;
	case ENEMY_MODE_HIT_RUN_HEAD_LIGHT_L: wsprintf(str, "ENEMY_MODE_HIT_RUN_HEAD_LIGHT_L");	break;
	case ENEMY_MODE_HIT_RUN_HEAD_LIGHT_R: wsprintf(str, "ENEMY_MODE_HIT_RUN_HEAD_LIGHT_R");	break;
	case ENEMY_MODE_AIR_ATT:			  wsprintf(str, "ENEMY_MODE_AIR_ATT");				break;
	case EYNEMY_MODE_BEAM_ATT:			  wsprintf(str, "EYNEMY_MODE_BEAM_ATT");			break;
	}
	if (Text_Check) {
		DXFONT_MGR->PrintStyleTextOutline("차차차", str, 200, 160, 0xff000000, 0xffff0000);
	}
	switch (_prevEnemyMode)
	{
	case ENEMY_MODE_APPEARANCE:			  wsprintf(str, "ENEMY_MODE_APPEARANCE");			break;
	case ENEMY_MODE_STATE:				  wsprintf(str, "ENEMY_MODE_STATE");				break;
	case ENEMY_MODE_MOVE:				  wsprintf(str, "ENEMY_MODE_MOVE");					break;
	case ENEMY_MODE_RUN_READY:			  wsprintf(str, "ENEMY_MODE_RUN_READY");			break;
	case ENEMY_MODE_RUN_STAY:			  wsprintf(str, "ENEMY_MODE_RUN_STAY");				break;
	case ENEMY_MODE_RUN_END:			  wsprintf(str, "ENEMY_MODE_RUN_END");				break;
	case ENEMY_MODE_NORMAL_ATT1:		  wsprintf(str, "ENEMY_MODE_NORMAL_ATT1");			break;
	case ENEMY_MODE_NORMAL_ATT2:		  wsprintf(str, "ENEMY_MODE_NORMAL_ATT2");			break;
	case ENEMY_MODE_NORMAL_ATT3:		  wsprintf(str, "ENEMY_MODE_NORMAL_ATT3");			break;
	case ENEMY_MODE_CLAW1:				  wsprintf(str, "ENEMY_MODE_CLAW1");				break;
	case ENEMY_MODE_CLAW2:				  wsprintf(str, "ENEMY_MODE_CLAW2");				break;
	case ENEMY_MODE_JUMP_SUCCESS:		  wsprintf(str, "ENEMY_MODE_JUMP_SUCCESS");			break;
	case ENEMY_MODE_JUMP_FAIL:			  wsprintf(str, "ENEMY_MODE_JUMP_FAIL");			break;
	case ENEMY_MODE_SPIT1:				  wsprintf(str, "ENEMY_MODE_SPIT1");				break;
	case ENEMY_MODE_SPIT2:				  wsprintf(str, "ENEMY_MODE_SPIT2");				break;
	case ENEMY_MODE_HIT_IDLE_HEAD_HEAVY_L:wsprintf(str, "ENEMY_MODE_HIT_IDLE_HEAD_HEAVY_L"); break;
	case ENEMY_MODE_HIT_IDLE_HEAD_HEAVY_R:wsprintf(str, "ENEMY_MODE_HIT_IDLE_HEAD_HEAVY_R"); break;
	case ENEMY_MODE_HIT_IDLE_HEAD_LIGHT_L:wsprintf(str, "ENEMY_MODE_HIT_IDLE_HEAD_LIGHT_L"); break;
	case ENEMY_MODE_HIT_IDLE_HEAD_LIGHT_R:wsprintf(str, "ENEMY_MODE_HIT_IDLE_HEAD_LIGHT_R"); break;
	case ENEMY_MODE_HIT_RUN_HEAD_HEAVY_L: wsprintf(str, "ENEMY_MODE_HIT_RUN_HEAD_HEAVY_L");	break;
	case ENEMY_MODE_HIT_RUN_HEAD_HEAVY_R: wsprintf(str, "ENEMY_MODE_HIT_RUN_HEAD_HEAVY_R");	break;
	case ENEMY_MODE_HIT_RUN_HEAD_LIGHT_L: wsprintf(str, "ENEMY_MODE_HIT_RUN_HEAD_LIGHT_L");	break;
	case ENEMY_MODE_HIT_RUN_HEAD_LIGHT_R: wsprintf(str, "ENEMY_MODE_HIT_RUN_HEAD_LIGHT_R");	break;
	case ENEMY_MODE_AIR_ATT:			  wsprintf(str, "ENEMY_MODE_AIR_ATT");				break;
	case EYNEMY_MODE_BEAM_ATT:			  wsprintf(str, "EYNEMY_MODE_BEAM_ATT");			break;
	}

	if (Text_Check) {
		DXFONT_MGR->PrintStyleTextOutline("차차차", str, 200, 100, 0xff000000, 0xff000000);

		wsprintf(str, "시간 %d", _beamAttTimer);
		DXFONT_MGR->PrintStyleTextOutline("차차차", str, 300, 400, 0xff000000, 0xff000000);

		if (_attTrue)
		{
			wsprintf(str, "공격판정");
			DXFONT_MGR->PrintStyleTextOutline("차차차", str, 400, 200, 0xff000000, 0xffff0000);
		}
		else
		{
			wsprintf(str, "비공격판정");
			DXFONT_MGR->PrintStyleTextOutline("차차차", str, 400, 200, 0xff000000, 0xffff0000);
		}
		if (_enemyAttFlag)
		{
			wsprintf(str, "★");
			DXFONT_MGR->PrintStyleTextOutline("차차차", str, 400, 400, 0xff000000, 0xffff0000);
		}
	}
	float z1 = _enemyColisionTrans.GetFinalMatrix()._31 * 3;
	float z2 = _enemyColisionTrans.GetFinalMatrix()._32 * 3;
	float z3 = _enemyColisionTrans.GetFinalMatrix()._33 * 3;

	D3DXVECTOR3 _enemyDir = D3DXVECTOR3(z1, z2, z3);

	GIZMO_MGR->Line(_enemyColisionTrans.GetWorldPosition(), _enemyColisionTrans.GetWorldPosition() + _enemyDir);

	//빔
	/*for (int i = 0; i < _beamAttTransVector.size(); i++)
	{
		GIZMO_MGR->WireSphere(_beamAttTransVector[i]->GetWorldPosition(), _enemyCoreSphere.radius / 3, 0xffffff);
	}

	GIZMO_MGR->WireSphere(_beamTrans.GetWorldPosition(), _enemyCoreSphere.radius, 0xffffff);*/
	_beamTrans.RenderGimozo(true);
}

void bossClass::amendTrans()
{
	float y1 = _enemyColisionTrans.GetFinalMatrix()._31 * _enemySphere.radius / 2;
	float y2 = _enemyColisionTrans.GetFinalMatrix()._32 * _enemySphere.radius / 2;
	float y3 = _enemyColisionTrans.GetFinalMatrix()._33 * _enemySphere.radius / 2;
	D3DXVECTOR3 _enemyDirPosY = D3DXVECTOR3(y1, y2, y3);

	float z1 = _enemyColisionTrans.GetFinalMatrix()._31 * _enemySphere.radius / 3;
	float z2 = _enemyColisionTrans.GetFinalMatrix()._32 * _enemySphere.radius / 3;
	float z3 = _enemyColisionTrans.GetFinalMatrix()._33 * _enemySphere.radius / 3;
	D3DXVECTOR3 _enemyDirPosZ = D3DXVECTOR3(z1, z2, z3);

	_enemyColisionTrans = _enemyTrans;
	_enemyColisionTrans.SetWorldPosition(
		_enemyTrans.GetWorldPosition().x + _enemyDirPosY.x,
		_enemyTrans.GetWorldPosition().y + _enemyDirPosY.y,
		_enemyTrans.GetWorldPosition().z + _enemyDirPosY.z);

	_skinnedTrans.SetWorldPosition(
		_enemyTrans.GetWorldPosition().x - _enemyDirPosZ.x,
		_enemyTrans.GetWorldPosition().y - _enemyDirPosZ.y,
		_enemyTrans.GetWorldPosition().z - _enemyDirPosZ.z);
}

void bossClass::airAttSet(cTransform *playerTrans, cBoundSphere *playerSphere, cBoundSphere *playerCoreSphere)
{
	float z1 = _enemyColisionTrans.GetFinalMatrix()._31 * -(_enemyAirAttRangeSphere.radius + _enemyCoreSphere.radius * 2);
	float z2 = _enemyColisionTrans.GetFinalMatrix()._32 * -(_enemyAirAttRangeSphere.radius + _enemyCoreSphere.radius * 2);
	float z3 = _enemyColisionTrans.GetFinalMatrix()._33 * -(_enemyAirAttRangeSphere.radius + _enemyCoreSphere.radius * 2);
	D3DXVECTOR3 _enemyDirPosZ = D3DXVECTOR3(z1, z2, z3);

	_enemyAirAttTransReset.SetWorldPosition(
		_enemyTrans.GetWorldPosition().x - _enemyDirPosZ.x,
		_enemyTrans.GetWorldPosition().y - _enemyDirPosZ.y,
		_enemyTrans.GetWorldPosition().z - _enemyDirPosZ.z);

	//가까워서 공중 공격을 하지 않게 만든다
	if (!PHYSICS_MGR->IsOverlap(playerTrans, playerSphere, &_enemyTrans, &_enemyAirAttRangeSphere))
	{
		_airAttCondition = true;
	}

	//공중 공격
	if (((PHYSICS_MGR->IsOverlap(playerTrans, playerSphere, &_enemyTrans, &_enemyAirAttRangeSphere) ||
		_prevEnemyMode == ENEMY_MODE_AIR_ATT
		) && _airAttCondition) || _airAttDecisionFlag)
	{
		_airAttDecisionFlag = true;


		_enemyAirAttTrans.MovePositionSelf(0.f, 0.f, AIR_ATT_DECISION_SPEED);

		_accrueDis += AIR_ATT_DECISION_SPEED;
		if (_accrueDis >= _enemyAirAttRangeSphere.radius * 2)
		{
			_prevEnemyMode = ENEMY_MODE_RUN_STAY;
			_attTrue = false;
			_airAttDecisionFlag = false;
			_airAttCondition = false;
			_enemyAirAttTrans.SetWorldPosition(_enemyTrans.GetWorldPosition());
			_accrueDis = 0.0f;
		}

		if (PHYSICS_MGR->IsOverlap(playerTrans, playerCoreSphere, &_enemyAirAttTrans, &_enemyCoreSphere) && !_attTrue)
		{
			_attTrue = true;		//공중공격 성공
			_enemyMode = ENEMY_MODE_JUMP_SUCCESS;
			_airAttCondition = false;
			_airAttDecisionFlag = false;
			SOUNDMANAGER->play("입 벌릴때", 1.0F);
		}
		else if (PHYSICS_MGR->IsOverlap(&_enemyAirAttTransReset, &_enemyCoreSphere, &_enemyAirAttTrans, &_enemyCoreSphere))
		{
			_attTrue = false;		//공중공격 실패
			_enemyMode = ENEMY_MODE_JUMP_FAIL;
			_airAttDecisionFlag = false;
			_airAttCondition = false;
			SOUNDMANAGER->play("입 벌릴때", 1.0F);
		}
	}
	else
	{
		_enemyAirAttTrans = _enemyTrans;
	}

	if (_prevEnemyMode == ENEMY_MODE_JUMP_FAIL)
	{
		_airAttDecisionFlag = false;
	}
}

void bossClass::normalFollow(cTransform *playerTrans, cBoundSphere *playerSphere)
{
	if (PHYSICS_MGR->IsOverlap(playerTrans, playerSphere, &_enemyTrans, &_enemyAttRangeSphere) || _enemyAttFlag)
	{
		if (!_enemyAttFlag)
		{
			_enemyMode = ENEMY_MODE_RUN_STAY;
			vShadow_Enemy[0]->pSkinned->SetPlaySpeed(0.5f);
			vShadow_Enemy[0]->pSkinned->Play("Anims_5", 0.3f);
		}
		//플레이어와 충돌하지 않았을 때
		if (!PHYSICS_MGR->IsOverlap(playerTrans, playerSphere, &_enemyTrans, &_enemySphere) && _enemyMode != ENEMY_MODE_AIR_ATT)
		{
			if (_movePoint.size() != 0) {
				t += 0.1f;
				cTransform tempTran;
				tempTran.SetWorldPosition(_movePoint.back());
				_enemyTrans.PositionLerp(_enemyTrans, tempTran, t);
				if (t >= 1) {
					_movePoint.pop_back();
					t = 0.f;
				}
			}
			else {
				_enemyTrans.MovePositionSelf(0.f, 0.f, ENEMY_MOVE_SPEED);
				_enemyAirAttTrans.MovePositionSelf(0.f, 0.f, ENEMY_MOVE_SPEED);
			}
		}
		_enemyAttFlag = true;
	}
	//따라가지 않는 상태일 때 기본 조건(몬스터가 플레이어를 발견하지 못해 지들끼리 움직일 때)
	else
	{
		if (!PHYSICS_MGR->IsOverlap(&_unitNomalTrans, playerSphere, &_enemyTrans, &_enemySphere))
		{
			_enemyTrans.MovePositionSelf(0.f, 0.f, ENEMY_MOVE_SPEED);
			_enemyAirAttTrans.MovePositionSelf(0.f, 0.f, ENEMY_MOVE_SPEED);
			//보간
			_enemyOriginTrans.SetWorldPosition(_enemyTrans.GetWorldPosition().x, _enemyTrans.GetWorldPosition().y, _enemyTrans.GetWorldPosition().z);
			_enemyOriginTrans.LookDirection(_unitNomalTrans.GetWorldPosition() - _enemyOriginTrans.GetWorldPosition(), _enemyOriginTrans.GetUp());
			_enemyTrans.RotateSlerp(_enemyTrans, _enemyOriginTrans, INTERPOLATION_SPEED);
			_enemyAirAttTrans.RotateSlerp(_enemyTrans, _enemyOriginTrans, INTERPOLATION_SPEED);
		}

		_normalMoveTimer++;
		if (_normalMoveTimer >= 100)
		{
			_normalMoveTimer = 0;
			float tempX = (rand() / ENEMY_NORMAL_MOVE_RANGE + 1) * ENEMY_NORMAL_MOVE_MIN;
			if (rand() % 2)
			{
				tempX *= -1;
			}
			tempX += _startUnitVector3.x;

			float tempZ = (rand() / ENEMY_NORMAL_MOVE_RANGE + 1) * ENEMY_NORMAL_MOVE_MIN;
			if (rand() % 2)
			{
				tempZ *= -1;
			}
			tempZ += _startUnitVector3.z;

			_unitNomalTrans.SetWorldPosition(tempX, _startUnitVector3.y, tempZ);
		}
	}
}

void bossClass::interpolation(cTransform *playerTrans)
{
	if (_enemyMode != ENEMY_MODE_AIR_ATT &&
		_enemyMode != ENEMY_MODE_JUMP_SUCCESS &&
		_enemyMode != ENEMY_MODE_JUMP_FAIL &&
		!_airAttDecisionFlag)
	{
		cTransform tempUpTrans;
		cTransform tempDirTrans;
		tempUpTrans = _enemyTrans;
		tempDirTrans = _enemyOriginTrans;

		tempDirTrans.SetWorldPosition(_enemyTrans.GetWorldPosition().x, _enemyTrans.GetWorldPosition().y, _enemyTrans.GetWorldPosition().z);
		tempDirTrans.LookDirection(playerTrans->GetWorldPosition() - tempDirTrans.GetWorldPosition(), tempDirTrans.GetUp());

		tempUpTrans.RotateSlerp(tempUpTrans, tempDirTrans, 1);

		if (tempUpTrans.GetUp().y >= 0.5f)
		{
			//보간
			_enemyOriginTrans.SetWorldPosition(_enemyTrans.GetWorldPosition().x, _enemyTrans.GetWorldPosition().y, _enemyTrans.GetWorldPosition().z);
			_enemyOriginTrans.LookDirection(playerTrans->GetWorldPosition() - _enemyOriginTrans.GetWorldPosition(), _enemyOriginTrans.GetUp());
			_enemyTrans.RotateSlerp(_enemyTrans, _enemyOriginTrans, INTERPOLATION_SPEED);

			_enemyAirAttTrans.RotateSlerp(_enemyTrans, _enemyOriginTrans, INTERPOLATION_SPEED);
		}
	}
}

void bossClass::DamageFlag(float Attak1, float Attak2, float Attak3)
{
	if (_damageFlag)
	{
		falseDamageFlag();
		switch (_enemyMode)
		{
		case ENEMY_MODE_NORMAL_ATT1:
			_cPlayer->AttackedByEnemy(Attak1);
			break;
		case ENEMY_MODE_NORMAL_ATT2:
			_cPlayer->AttackedByEnemy(Attak2);
			break;
		case ENEMY_MODE_NORMAL_ATT3:
			_cPlayer->AttackedByEnemy(Attak3);
			break;
		case EYNEMY_MODE_BEAM_ATT:
			_cPlayer->AttackedByEnemy(Attak3);
			break;
		}

		if (_prevEnemyMode == ENEMY_MODE_JUMP_SUCCESS)
		{
			_cPlayer->AttackedByEnemy(Attak3);
		}
	}
}
