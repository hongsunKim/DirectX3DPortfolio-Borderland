#include "stdafx.h"
#include "enemyClass2.h"

HRESULT enemyClass2::Init(float scale /*1.f*/, float followRange/*32.f*/, float airAttRange/*16.f*/, float enemyRange/*4.f*/, int SKAG_MODE, D3DXVECTOR3 EnemyPos,
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
	_enemyTrans.SetWorldPosition(EnemyPos);
	_enemyColisionTrans.SetWorldPosition(_enemyTrans.GetWorldPosition().x, _enemyTrans.GetWorldPosition().y + enemyRange / 4, _enemyTrans.GetWorldPosition().z + enemyRange / 8);
	_enemyTrans.SetScale(scale, scale, scale);
	_enemyAirAttTrans.SetWorldPosition(D3DXVECTOR3(_enemyTrans.GetWorldPosition().x, _enemyTrans.GetWorldPosition().y, _enemyTrans.GetWorldPosition().z));
	_enemyAirAttTrans.SetScale(scale, scale, scale);
	_enemyAttRangeSphere.SetBound(&D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(followRange, followRange, followRange));
	_enemyAirAttRangeSphere.SetBound(&D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(airAttRange, airAttRange, airAttRange));
	_enemySphere.SetBound(&D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(enemyRange, enemyRange, enemyRange));
	_enemyCoreSphere.SetBound(&D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(enemyRange / 2, enemyRange / 2, enemyRange / 2));
	_enemyAirRange.radius = airAttRange;

	_enemyAirAttTransReset.SetWorldPosition(_enemyTrans.GetWorldPosition().x, _enemyTrans.GetWorldPosition().y, _enemyTrans.GetWorldPosition().z + _enemyAirAttRangeSphere.radius);

	D3DXMATRIXA16 matCorrection;
	D3DXMatrixScaling(&matCorrection, 0.03f, 0.03f, 0.03f);

	cXMesh_Skinned* pSkinned = RESOURCE_SKINNEDXMESH->
		GetResource("../../Resources/Enemy/skag2.X",
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

	this->vShadow_Enemy[0]->pSkinned->AddBoneTransform("Bone20", &_leftHeadTrans);
	this->vShadow_Enemy[0]->pSkinned->AddBoneTransform("Bone17", &_rightHeadTrans);
	this->vShadow_Enemy[0]->pSkinned->AddBoneTransform("Bone12", &_centerHeadTrans);
	this->vShadow_Enemy[0]->pSkinned->AddBoneTransform("Bip01_Head", &_centerBackHeadTrans);

	for (int i = 0; i < 26; i++)
	{
		cTransform* temp = new cTransform;
		_tempTrans.push_back(temp);
	}
	this->vShadow_Enemy[0]->pSkinned->AddBoneTransform("Bip01_Neck1", _tempTrans[0]);
	this->vShadow_Enemy[0]->pSkinned->AddBoneTransform("Bip01_R_Clavicle", _tempTrans[1]);
	this->vShadow_Enemy[0]->pSkinned->AddBoneTransform("Bip01_L_Clavicle", _tempTrans[2]);
	this->vShadow_Enemy[0]->pSkinned->AddBoneTransform("Bip01", _tempTrans[3]);
	this->vShadow_Enemy[0]->pSkinned->AddBoneTransform("Bip01_Spine1", _tempTrans[4]);
	this->vShadow_Enemy[0]->pSkinned->AddBoneTransform("Bip01_Spine2", _tempTrans[5]);

	this->vShadow_Enemy[0]->pSkinned->AddBoneTransform("Bip01_R_UpperArm", _tempTrans[6]);
	this->vShadow_Enemy[0]->pSkinned->AddBoneTransform("Bip01_L_UpperArm", _tempTrans[7]);
	this->vShadow_Enemy[0]->pSkinned->AddBoneTransform("Bip01_R_Forearm", _tempTrans[8]);
	this->vShadow_Enemy[0]->pSkinned->AddBoneTransform("Bip01_L_Forearm", _tempTrans[9]);
	this->vShadow_Enemy[0]->pSkinned->AddBoneTransform("Bip01_R_Thigh", _tempTrans[10]);
	this->vShadow_Enemy[0]->pSkinned->AddBoneTransform("Bip01_L_Thigh", _tempTrans[11]);
	this->vShadow_Enemy[0]->pSkinned->AddBoneTransform("Bip01_R_Calf", _tempTrans[12]);
	this->vShadow_Enemy[0]->pSkinned->AddBoneTransform("Bip01_L_Calf", _tempTrans[13]);
	this->vShadow_Enemy[0]->pSkinned->AddBoneTransform("Bip01_R_HorseLink", _tempTrans[14]);
	this->vShadow_Enemy[0]->pSkinned->AddBoneTransform("Bip01_L_HorseLink", _tempTrans[15]);
	this->vShadow_Enemy[0]->pSkinned->AddBoneTransform("Bip01_R_Foot", _tempTrans[16]);
	this->vShadow_Enemy[0]->pSkinned->AddBoneTransform("Bip01_L_Foot", _tempTrans[17]);
	this->vShadow_Enemy[0]->pSkinned->AddBoneTransform("Bip01_R_Sholder", _tempTrans[18]);
	this->vShadow_Enemy[0]->pSkinned->AddBoneTransform("Bip01_L_Sholder", _tempTrans[19]);
	this->vShadow_Enemy[0]->pSkinned->AddBoneTransform("Bip01_R_Toe0", _tempTrans[20]);
	this->vShadow_Enemy[0]->pSkinned->AddBoneTransform("Bip01_L_Toe0", _tempTrans[21]);
	this->vShadow_Enemy[0]->pSkinned->AddBoneTransform("Bip01_R_Hand", _tempTrans[22]);
	this->vShadow_Enemy[0]->pSkinned->AddBoneTransform("Bip01_L_Hand", _tempTrans[23]);
	this->vShadow_Enemy[0]->pSkinned->AddBoneTransform("Bip01_R_Finger0", _tempTrans[24]);
	this->vShadow_Enemy[0]->pSkinned->AddBoneTransform("Bip01_L_Finger0", _tempTrans[25]);


	//Anims_25 죽는 모션


	this->vShadow_Enemy[0]->pSkinned->Play("Anims_3", 0.3f);

	_skinnedTrans.RotateSelf(0, -D3DX_PI / 2, 0);

	D3DXVECTOR3 boundSizeVector(ENEMY_BOUND_SIZE, ENEMY_BOUND_SIZE, ENEMY_BOUND_SIZE);
	_boundBox.SetBound(&_centerBackHeadTrans.GetWorldPosition(), &boundSizeVector);



	_unitNomalTrans.SetWorldPosition(_enemyTrans.GetWorldPosition());			////////////////////////////////////////////////////////
	_startUnitVector3 = _enemyTrans.GetWorldPosition();

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
void enemyClass2::Release()
{
	this->vShadow_Enemy[0]->pSkinned->Release();
}
void enemyClass2::Update(float timeDelta, cBoundSphere *playerSphere, cBoundSphere *playerCoreSphere, bool playerAir, D3DXVECTOR3 playerXYZ)
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

	_prevEnemyMode = _enemyMode;

	this->vShadow_Enemy[0]->pSkinned->Update(timeDelta);


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
			this->vShadow_Enemy[0]->pSkinned->PlayOneShotAfterHold("Anims_26", 0.3f);
		}
		//if (vShadow_Enemy[0]->pSkinned->getPlay())
		{
			_Enemy_Stat.LiveOrDie = false;
			SOUNDMANAGER->play("투사체공격 몹 죽는소리", 1.0F);
		}
		_Enemy_Stat.F_ok = false;
	}
	if (_Enemy_Stat.Hp > 0)
	{
		//공중 공격
		//airAttSet(playerTrans, playerSphere, playerCoreSphere);

		if (!PHYSICS_MGR->IsOverlap(&tempPlayer, playerSphere, &_enemyTrans, &_enemyAirAttRangeSphere))
		{
			_permissionProjectileFlag = true;
		}

		if ((PHYSICS_MGR->IsOverlap(&tempPlayer, playerSphere, &_enemyTrans, &_enemyAirAttRangeSphere) || _projectileFlag) && _permissionProjectileFlag)
		{
			_projectileTimer++;

			if (!_projectileFlag)
			{
				_projectileFlag = true;
				vShadow_Enemy[0]->pSkinned->PlayOneShotAfterHold("Anims_15", 0.3f);
				_enemyMode = ENEMY_MODE_SPIT;
				_movePoint.clear();
			}
			if (_projectileTimer == PROJECTILE_TIMER_MAX)
			{
				_permissionProjectileFlag = false;
				cTransform *tempTrans = new cTransform;
				*tempTrans = _centerBackHeadTrans;

				tempTrans->LookDirection(tempPlayer.GetWorldPosition() - tempTrans->GetWorldPosition(), tempTrans->GetUp());

				_projectileTrans.push_back(tempTrans);

				Water* Spit = new Water;
				D3DXMATRIXA16 matScale;
				D3DXMatrixScaling(&matScale, 0.015f, 0.015f, 0.015f);
				Spit->Init("../../Resources/Meshes/Water/Sphere.X", &matScale);
				m_Spit.push_back(Spit);

				SOUNDMANAGER->play("투사체 공격소리", 1.0F);
			}
		}

		if (_enemyMode == ENEMY_MODE_SPIT && vShadow_Enemy[0]->pSkinned->getPlay())
		{
			_enemyMode = ENEMY_MODE_RUN_STAY;
			vShadow_Enemy[0]->pSkinned->Play("Anims_5", 0.3f);
			_projectileFlag = false;
			_projectileTimer = 0;
		}

		for (int i = 0; i < _projectileTrans.size(); i++)
		{
			//float z1 = _projectileTrans[i]->GetFinalMatrix()._31 * 0.001;
			//float z2 = _projectileTrans[i]->GetFinalMatrix()._32 * 0.001;
			//float z3 = _projectileTrans[i]->GetFinalMatrix()._33 * 0.001;
			//_projectileTrans[i]->SetWorldPosition(z1, z2, z3);
			_projectileTrans[i]->MovePositionSelf(_projectileUpSpeed - _projectileUpSpeed * _projectileGravity, 0.0f, 0.1);
			_projectileUpSpeed += 0.01;
			_projectileGravity += 0.02;



			if (_projectileMinY >= _projectileTrans[0]->GetWorldPosition().y)
			{
				Effect* effect = new Effect;
				effect->Scene_Init();
				effect->SetPoisonLocation(_projectileTrans[0]->GetWorldPosition());
				m_Effect.push_back(effect);

				m_Spit.erase(m_Spit.begin() + 0);
				m_Spit.clear();

				_projectileTrans.erase(_projectileTrans.begin() + 0);
				_projectileTrans.clear();
				_projectileUpSpeed = 0.f;
				_projectileGravity = 0.f;
				break;
			}
			cBoundSphere tempSphere;
			tempSphere.SetBound(&D3DXVECTOR3(0.f, 0.f, 0.f), &D3DXVECTOR3(_enemyCoreSphere.radius / 3, _enemyCoreSphere.radius / 3, _enemyCoreSphere.radius / 3));
			if (PHYSICS_MGR->IsOverlap(&tempPlayer, playerSphere, _projectileTrans[0], &tempSphere))
			{
				Effect* effect = new Effect;
				effect->Scene_Init();
				effect->SetPoisonLocation(_projectileTrans[0]->GetWorldPosition());
				m_Effect.push_back(effect);

				m_Spit.erase(m_Spit.begin() + 0);
				m_Spit.clear();

				_damageFlag = true;
				_projectileTrans.erase(_projectileTrans.begin() + 0);
				_projectileTrans.clear();
				_projectileUpSpeed = 0.f;
				_projectileGravity = 0.f;
				break;
			}
		}
		if (_enemyMode == ENEMY_MODE_SPIT && vShadow_Enemy[0]->pSkinned->getPlay())
		{
			_enemyMode = ENEMY_MODE_RUN_STAY;
			vShadow_Enemy[0]->pSkinned->Play("Anims_5", 0.3f);
			_projectileFlag = false;
			_projectileTimer = 0;
		}

		//기본 따라가기
		if (_enemyMode == ENEMY_MODE_RUN_STAY || _enemyMode == ENEMY_MODE_MOVE)
			normalFollow(&tempPlayer, playerSphere);

		//공중 공격중이 아닐 때 보간처리
		if (_enemyAttFlag && !playerAir)
			interpolation(&tempPlayer);

		if (PHYSICS_MGR->IsOverlap(&tempPlayer, playerSphere, &_enemyTrans, &_enemySphere) && !_meleeAttFlag)
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

		if (_meleeAttFlag && vShadow_Enemy[0]->pSkinned->getPlay())
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
				vShadow_Enemy[0]->pSkinned->Play("Anims_5", 0.3f);
				_attCollFlag = false;		//★
				_damageFlag = false;		//★
				break;
			case ENEMY_MODE_MOVE:
				vShadow_Enemy[0]->pSkinned->Play("Anims_3", 0.3f);
				_attCollFlag = false;		//★
				_damageFlag = false;		//★
				break;
			case ENEMY_MODE_STATE:
				vShadow_Enemy[0]->pSkinned->Play("Anims_2", 0.3f);
				_attCollFlag = false;		//★
				_damageFlag = false;		//★
				break;
			case ENEMY_MODE_JUMP_SUCCESS:
				vShadow_Enemy[0]->pSkinned->PlayOneShotAfterHold("Anims_13", 0.3f);		//공중
				_attCollFlag = true;		//★
				break;
			case ENEMY_MODE_JUMP_FAIL:
				vShadow_Enemy[0]->pSkinned->PlayOneShotAfterHold("Anims_14", 0.3f);
				_attCollFlag = true;		//★
				break;
			case ENEMY_MODE_CLAW1:
				vShadow_Enemy[0]->pSkinned->PlayOneShotAfterHold("Anims_8", 0.3f);		//근접
				_attCollFlag = true;		//★
				break;
			case ENEMY_MODE_CLAW2:
				vShadow_Enemy[0]->pSkinned->PlayOneShotAfterHold("Anims_9", 0.3f);
				_attCollFlag = true;		//★
				break;
			case ENEMY_MODE_NORMAL_ATT1:
				vShadow_Enemy[0]->pSkinned->PlayOneShotAfterHold("Anims_10", 0.3f);
				_attCollFlag = true;		//★
				break;
			case ENEMY_MODE_NORMAL_ATT2:
				vShadow_Enemy[0]->pSkinned->PlayOneShotAfterHold("Anims_11", 0.3f);
				_attCollFlag = true;		//★
				break;
			case ENEMY_MODE_NORMAL_ATT3:
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
			vShadow_Enemy[0]->pSkinned->Play("Anims_5", 0.3f);
			_enemyTrans.MovePositionSelf(0.f, 0.f, ENEMY_JUMP_ATT_SUCCESS);
			_enemyAirAttTrans.SetWorldPosition(D3DXVECTOR3(_enemyTrans.GetWorldPosition().x, _enemyTrans.GetWorldPosition().y, _enemyTrans.GetWorldPosition().z));
			vShadow_Enemy[0]->pSkinned->Play("Anims_5", 0.3f);
			_attCollFlag = true;		//★
		}
		//공중공격 실패
		if (_enemyMode == ENEMY_MODE_JUMP_FAIL && vShadow_Enemy[0]->pSkinned->getPlay())
		{
			//_aniPlayFlag = true;
			_enemyMode = ENEMY_MODE_RUN_STAY;
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
			vShadow_Enemy[0]->pSkinned->Play("Anims_5", 0.3f);
			_meleeAttFlag = false;
			_airAttDecisionFlag = false;
			_airAttCondition = false;
			_attCollFlag = false;		//★
			_damageFlag = false;		//★
		}

		//충돌용 트랜스폼 좌표 보정
		amendTrans();

		//★
		if (_attCollFlag)
		{
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
			case ENEMY_MODE_JUMP_SUCCESS:
				_damageFlag = true;
				_attCollFlag = false;
				break;
			}
		}
	}
	for (int i = 0; i < m_Effect.size(); i++)
	{
		m_Effect[i]->Scene_Update(timeDelta);
	}
	this->Cri.ShakeUpdate(timeDelta);
}
void enemyClass2::Render(cCamera* pMainCamera, cLight_Direction* Light_Dt)
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
	cXMesh_Skinned::sSkinnedMeshEffect->SetVector("vColor1", &D3DXVECTOR4(1.7f, 1.3f, 1, 1));
	cXMesh_Skinned::sSkinnedMeshEffect->SetVector("vColor2", &D3DXVECTOR4(2.5f, 2.5f, 2.5f, 1));
	cXMesh_Skinned::sSkinnedMeshEffect->SetVector("vColor3", &D3DXVECTOR4(6, 6, 6, 1));
	cXMesh_Skinned::sSkinnedMeshEffect->SetTexture("Ramp_Tex", RESOURCE_TEXTURE->GetResource("../../Resources/Testures/Ramp_1.png"));
	cXMesh_Skinned::SetBaseLight(Light_Dt);

	vShadow_Enemy[0]->Render();

	if (EnemySphere_Check) {

		GIZMO_MGR->WireSphere(_enemyColisionTrans.GetLocalPosition(), _enemyAttRangeSphere.radius, 0xff0000ff);
		GIZMO_MGR->WireSphere(_enemyColisionTrans.GetLocalPosition(), _enemyAirAttRangeSphere.radius, 0xff00ff00);
		GIZMO_MGR->WireSphere(_enemyAirAttTrans.GetLocalPosition(), _enemyCoreSphere.radius, 0xff0000);

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
		GIZMO_MGR->WireSphere(_tempTrans[1]->GetLocalPosition(), _enemyAirRange.radius, 0x00ff00);
	}

	cXMesh_Static::SetTechniqueName("SkagSpit");
	for (int i = 0; i < _projectileTrans.size(); i++)
	{
		//GIZMO_MGR->WireSphere(_projectileTrans[i]->GetWorldPosition(), _enemyCoreSphere.radius / 3, 0xff00ff);
		m_Spit[i]->Render(_projectileTrans[i], false);
	}
	cXMesh_Static::SetTechniqueName("ReciveShadowToon");

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
	case ENEMY_MODE_SPIT:					 wsprintf(str, "ENEMY_MODE_SPIT"); break;
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
	case ENEMY_MODE_SPIT:					 wsprintf(str, "ENEMY_MODE_SPIT"); break;
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
	}

	if (Text_Check) {
		DXFONT_MGR->PrintStyleTextOutline("차차차", str, 200, 100, 0xff000000, 0xff00ff00);

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
		wsprintf(str, "%d", _normalMoveTimer);
		DXFONT_MGR->PrintStyleTextOutline("차차차", str, 400, 400, 0xff000000, 0xffff0000);
	}

	float z1 = _enemyColisionTrans.GetFinalMatrix()._31 * 3;
	float z2 = _enemyColisionTrans.GetFinalMatrix()._32 * 3;
	float z3 = _enemyColisionTrans.GetFinalMatrix()._33 * 3;

	D3DXVECTOR3 _enemyDir = D3DXVECTOR3(z1, z2, z3);
	if (EnemySphere_Check) {
		GIZMO_MGR->Line(_enemyColisionTrans.GetWorldPosition(), _enemyColisionTrans.GetWorldPosition() + _enemyDir);

		GIZMO_MGR->WireSphere(_unitNomalTrans.GetLocalPosition(), _enemyCoreSphere.radius / 3, 0xff0000);			////////////////////////////////////////////////////////
	}

	for (int i = 0; i < m_Effect.size(); i++)
	{
		m_Effect[i]->PoisonEffectRender();
		m_Effect[i]->PoisonEffxRender();
		if (m_Effect[i]->time > 1.f)
		{
			m_Effect.erase(m_Effect.begin()+i);
			m_Effect.clear();
		}
	}
}

void enemyClass2::amendTrans()
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

void enemyClass2::airAttSet(cTransform *playerTrans, cBoundSphere *playerSphere, cBoundSphere *playerCoreSphere)
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

		//if (_enemyMode != ENEMY_MODE_JUMP_SUCCESS || _enemyMode != ENEMY_MODE_JUMP_FAIL)
		//	_enemyMode = ENEMY_MODE_AIR_ATT;

		if (PHYSICS_MGR->IsOverlap(playerTrans, playerCoreSphere, &_enemyAirAttTrans, &_enemyCoreSphere) && !_attTrue)
		{
			_attTrue = true;		//공중공격 성공
			_enemyMode = ENEMY_MODE_JUMP_SUCCESS;
			_airAttCondition = false;
			_airAttDecisionFlag = false;
		}
		else if (PHYSICS_MGR->IsOverlap(&_enemyAirAttTransReset, &_enemyCoreSphere, &_enemyAirAttTrans, &_enemyCoreSphere))
		{
			_attTrue = false;		//공중공격 실패
			_enemyMode = ENEMY_MODE_JUMP_FAIL;
			_airAttDecisionFlag = false;
			_airAttCondition = false;
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

void enemyClass2::normalFollow(cTransform *playerTrans, cBoundSphere *playerSphere)
{
	if (PHYSICS_MGR->IsOverlap(playerTrans, playerSphere, &_enemyTrans, &_enemyAttRangeSphere) || _enemyAttFlag)
	{
		if (!_enemyAttFlag)
		{
			_enemyMode = ENEMY_MODE_RUN_STAY;
			this->vShadow_Enemy[0]->pSkinned->Play("Anims_5", 0.3f);
		}
		//플레이어와 충돌하지 않았을 때
		if (!PHYSICS_MGR->IsOverlap(playerTrans, playerSphere, &_enemyTrans, &_enemySphere) && _enemyMode != ENEMY_MODE_AIR_ATT)
		{
			if (_movePoint.size() != 0) {
				t += 0.125f;
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
	{		////////////////////////////////////////////////////////
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

	if (KEY_MGR->IsStayDown(VK_SPACE))
	{
		//_unitNomalTrans.DefaultControl2(_tempTimeDelta);
	}
}

void enemyClass2::interpolation(cTransform *playerTrans)
{
	if (_enemyMode != ENEMY_MODE_AIR_ATT &&
		_enemyMode != ENEMY_MODE_JUMP_SUCCESS &&
		_enemyMode != ENEMY_MODE_JUMP_FAIL &&
		_enemyMode != ENEMY_MODE_SPIT &&
		!_airAttDecisionFlag)
	{
		cTransform tempUpTrans;
		cTransform tempDirTrans;
		tempUpTrans = _enemyTrans;
		tempDirTrans = _enemyOriginTrans;

		tempDirTrans.SetWorldPosition(_enemyTrans.GetWorldPosition().x, _enemyTrans.GetWorldPosition().y, _enemyTrans.GetWorldPosition().z);
		tempDirTrans.LookDirection(playerTrans->GetWorldPosition() - tempDirTrans.GetWorldPosition(), tempDirTrans.GetUp());

		tempUpTrans.RotateSlerp(tempUpTrans, tempDirTrans, INTERPOLATION_SPEED);

		if (tempUpTrans.GetUp().y >= 0.5f)
		{
			//보간
			_enemyOriginTrans.SetWorldPosition(_enemyTrans.GetWorldPosition().x, _enemyTrans.GetWorldPosition().y, _enemyTrans.GetWorldPosition().z);
			_enemyOriginTrans.LookDirection(playerTrans->GetWorldPosition() - _enemyOriginTrans.GetWorldPosition(), _enemyOriginTrans.GetUp());
			_enemyAirAttTrans.RotateSlerp(_enemyTrans, _enemyOriginTrans, INTERPOLATION_SPEED);
			_enemyTrans.RotateSlerp(_enemyTrans, _enemyOriginTrans, INTERPOLATION_SPEED);
		}
	}
}
void enemyClass2::DamageFlag(float Attak1, float Attak2, float Attak3)
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
		case ENEMY_MODE_SPIT:
			_cPlayer->AttackedByEnemy(Attak3);
			break;
		}
		if (_prevEnemyMode == ENEMY_MODE_JUMP_SUCCESS)
		{
			_cPlayer->AttackedByEnemy(Attak3);
		}
	}
}
