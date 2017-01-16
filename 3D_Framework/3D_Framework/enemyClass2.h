#pragma once

#include "enemyH.h"
#include "cTransform.h"
#include "cXMesh_Skinned.h"
#include "cSkinnedAnimation.h"
#include "cPlayer.h"
#include "cTerrain.h"
#include "cSetBoundObject.h"				/////\

#include "Water.h"

#include "Effect.h"

#define GRAVITY_SPEED 0.025f
#define AIR_JUMP_SPEED 0.015
#define ENEMY_MOVE_SPEED 0.03f
#define ENEMY_AIR_ATT_MOVE_SPEED 0.13f
#define INTERPOLATION_SPEED 0.05f
#define AIR_ATT_DECISION_SPEED 0.23f		//공격 판정 속도(플레이어 트랜스폼 지름의 1/3)

#define ENEMY_JUMP_ATT_SUCCESS 6.07f
#define ENEMY_JUMP_ATT_FAIL_Z 12.07f
#define ENEMY_JUMP_ATT_FAIL_X 0.03f

#define ENEMY_BOUND_SIZE 0.5

#define ENEMY_NORMAL_MOVE_RANGE 10			//이동거리 최대값
#define ENEMY_NORMAL_MOVE_MIN 0.1			//이동 최소거리(ENEMY_NORMAL_MOVE_RANGE에 곱할값)
#define ENEMY_ATT_SKILL1 5
#define ENEMY_ATT_SKILL2 7

#define PROJECTILE_TIMER_MAX 25		//프레임 올라가면 50으로 수정

class enemyClass2
{
protected:
	cPlayer*                 _cPlayer;
	cTerrain*                _cTerrain;
	typedef vector<cBaseObject*>	 _vShadow_Enemy;

	cBoundBox				_boundBox;						//바운드박스

	cSkinnedAnimation*		_skinned1;						//매쉬
	cTransform				_skinnedTrans;					//매쉬 트랜스

															//헤드 충돌용 트랜스
	cTransform				_leftHeadTrans;					//입벌릴 때 헤드샷 판정 트린새
	cTransform				_rightHeadTrans;				//입벌릴 때 헤드샷 판정 트린새
	cTransform				_centerHeadTrans;				//평소와 머리 및 입 벌릴때 헤드샷 판정 트랜스
	cTransform				_centerBackHeadTrans;			//평소와 머리 뒤쪽



	int						_projectileTimer;				//

	vector<cTransform*>		_projectileTrans;				//투사체 트랜스 벡터
	cBoundSphere			_enemyAirRange;					//투사체 공격 범위
	vector<Water*>			m_Spit;
	vector<Effect*>			m_Effect;

	float					_projectileUpSpeed;				//투사체 위로 뜨는 속도
	float					_projectileGravity;				//투사체 중력값



	typedef vector<cTransform*>		tempTrans;						//바디 충돌용 트랜스

															//운영 트랜스

	cTransform				_enemyOriginTrans;				//보간용 트랜스폼

	cTransform				_enemyTrans;					//에너미 트랜스폼
	cTransform				_enemyAirAttTrans;				//에너미 공중 공격시 판정용 트랜스폼
	cTransform				_enemyAirAttTransReset;			//에너미 공중 공격시 충돌 여부 판정 트랜스폼
	cTransform				_enemyColisionTrans;			//에너미 충돌용 트랜스폼
	cBoundSphere			_enemyAttRangeSphere;			//플레이어를 따라가는 범위
	cBoundSphere			_enemyAirAttRangeSphere;		//공중 공격 범위
	cBoundSphere			_enemySphere;					//에너미
	cBoundSphere			_enemyCoreSphere;				//에너미 공중 공격 코어 범위
	bool					_enemyAttFlag;					//에너미의 공격이 시작됨을 알려줌
	int						_enemyMode;						//현재 에너미 상태
	int						_prevEnemyMode;					//이전 에너미 상태
	bool					_airAttMode;					//현재 에너미가 공중으로 뜨는 중이냐?
	bool					_airAttCondition;				//공중 공격을 해도 되냐?
	float					_airAttDis;						//공중공격 이동거리
	int						_airAttTimer;					//공중 공격 딜레이 타이머

	bool					_attTrue;						//공격판정이 들어갔다

	bool					_aniPlayFlag;					//애니메이션 시작 플래그

	bool					_meleeAttFlag;					//근접 공격플래그

	bool					_airAttDecisionFlag;			//공중공격 판정용 트랜스폼 전진 플래그

	float					_accrueDis;						//공중 공격판정 트랜스폼 누적거리

	bool					_projectileFlag;

	bool					_permissionProjectileFlag;

	cTransform				_unitNomalTrans;					//유닛이 플레이어를 따라가기전 따라다닐 트랜스폼
	int						_normalMoveTimer;					//기본 이동 타이머
	D3DXVECTOR3				_startUnitVector3;					//유닛의 시작 좌표


	vector<cSetBoundObject*> m_pVecObjects;						//바운드박스 오브젝트


	bool					_attCollFlag;						//공격이 실행 여부 플래그			//★
	bool					_damageFlag;						//공격 성공 여부 플래그				//★
	Enemy_Stat              _Enemy_Stat;						//에너미 스탯
	tempTrans               _tempTrans;

	float					_projectileMinY;					//투사체 트랜스 최소 y값

	int						_dieTimer;
	bool					_rearDie;
	cTransform				Cri;
	D3DXVECTOR2				screenPos;
public:
	deque<D3DXVECTOR3>			_movePoint;
	cTransform					terrainAstar;
	cBoundSphere					tempBoundS;
	float						t;
	_vShadow_Enemy			vShadow_Enemy;
	bool					_Enemy_Hit;
	float                   _Enemy_Hit_Count;

	HRESULT Init(float scale /*1.f*/, float followRange/*32.f*/, float airAttRange/*16.f*/, float enemyRange/*4.f*/, int SKAG_MODE, D3DXVECTOR3 EnemyPos,
		vector<cSetBoundObject*> pVecObjects, cPlayer* player, cTerrain* Terrain);
	void Release();
	void Update(float timeDelta, cBoundSphere *playerSphere, cBoundSphere *playerCoreSphere, bool playerAir, D3DXVECTOR3 playerXYZ);
	void Render(cCamera* pMainCamera, cLight_Direction* Light_Dt);



	void amendTrans();																							//충돌용, 메쉬 트랜스 좌표 수정 함수
	void airAttSet(cTransform *playerTrans, cBoundSphere *playerSphere, cBoundSphere *playerCoreSphere);		//공중공격 설정 함수
	void normalFollow(cTransform *playerTrans, cBoundSphere *playerSphere);										//플레이어 따라가는 함수
	void interpolation(cTransform *playerTrans);																//보간 함수
	_vShadow_Enemy get_vShadow_Enemy() { return vShadow_Enemy; }
	cTransform getEnemyTrans()
	{
		return _enemyTrans;
	}
	cBoundSphere getEnemySphere()
	{
		return _enemySphere;
	}
	Enemy_Stat getEnemy_Stat()
	{
		return _Enemy_Stat;
	}

	bool getRealDie()
	{
		return _rearDie;
	}

	enemyClass2()
	{
		_rearDie = false;

		_dieTimer = 0;

		_enemyMode = ENEMY_MODE_RUN_STAY;
		_prevEnemyMode = ENEMY_MODE_RUN_STAY;

		_projectileFlag = false;

		_permissionProjectileFlag = false;

		_accrueDis = 0.0f;
		_projectileTimer = 0;
		_normalMoveTimer = 0;
		_projectileMinY = 0;

		_Enemy_Stat.Hp = ENEMY2_MAX_HP;
		_Enemy_Stat.Max_Hp = ENEMY2_MAX_HP;
		_Enemy_Stat.shield = ENEMY2_SHIELD;
		_Enemy_Stat.Max_shield = ENEMY2_SHIELD;
		_Enemy_Stat.Properties = ENEMY2_PROPERTIES;

		_enemyAttFlag = false;
		_airAttMode = false;
		_airAttCondition = true;
		_Enemy_Stat.LiveOrDie = true;
		_attTrue = false;
		_meleeAttFlag = false;
		_aniPlayFlag = false;
		_airAttDecisionFlag = false;

		_projectileUpSpeed = 0.0f;
		_projectileGravity = 0.0f;

		_attCollFlag = false;			//★
		_damageFlag = false;			//★
	}
	~enemyClass2()
	{

	}

	void setEnemyTransY(float y)
	{
		_enemyTrans.SetLocalPosition(_enemyTrans.GetWorldPosition().x, y, _enemyTrans.GetWorldPosition().z);
	}

	void falseDamageFlag()				//★		//데미지를 주면 변수를 다시 초기화 해주는 함수
	{
		_damageFlag = false;
	}

	int getEnemyMode()
	{
		return _enemyMode;
	}

	bool getDamageFlag()
	{
		return _damageFlag;
	}

	D3DXVECTOR3 GetHeadPosition()
	{
		return _centerHeadTrans.GetWorldPosition();
	}
	tempTrans GetBodyPosition()
	{
		return _tempTrans;
	}
	float GetEnemyRadius()
	{
		return _enemyCoreSphere.radius;
	}

	bool GetHeadShot()
	{
		if (_enemyMode == ENEMY_MODE_JUMP_SUCCESS ||
			_enemyMode == ENEMY_MODE_JUMP_FAIL ||
			_enemyMode == EYNEMY_MODE_BEAM_ATT)
		{
			return true;
		}
		return false;
	}

	void setMinusHp(int hp)
	{
		_Enemy_Stat.Hp -= hp;
	}

	void DamageFlag(float Attak1, float Attak2, float Attak3);

	void enemyFinish()
	{
		_enemyMode = ENEMY_MODE_FINISH;
	}
	//▼▼▼▼▼▼▼
	vector<cTransform*> getProjectileTrans()
	{
		return _projectileTrans;
	}
	//▲▲▲▲▲▲▲

	//▼▼▼▼▼▼▼
	void setProjectileMinY(cTerrain* terr)
	{
		if (_projectileTrans.size() > 0)
		{
			_projectileMinY = terr->GetHeight(_projectileTrans[0]->GetWorldPosition().x, _projectileTrans[0]->GetWorldPosition().z);
		}
	}
	//▲▲▲▲▲▲▲
};

