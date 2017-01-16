#pragma once

#include "enemyH.h"
#include "cTransform.h"
#include "cXMesh_Skinned.h"
#include "cSkinnedAnimation.h"
#include "cBoundBox.h"
#include "cPlayer.h"
#include "cTerrain.h"
#include "cSetBoundObject.h"				/////\

class cParticleEmitter;						//�ڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡ�

#define GRAVITY_SPEED 0.025f				* 17.5
#define AIR_JUMP_SPEED 0.015				* 17.5
#define ENEMY_MOVE_SPEED 0.03f				* 17.5
#define ENEMY_AIR_ATT_MOVE_SPEED 0.13f		* 17.5
#define INTERPOLATION_SPEED 0.05f			//* 3.5
#define AIR_ATT_DECISION_SPEED 0.23f		* 17.5 //���� ���� �ӵ�(�÷��̾� Ʈ������ ������ 1/3)

#define ENEMY_JUMP_ATT_SUCCESS 6.07f		* 17.5f
#define ENEMY_JUMP_ATT_FAIL_Z 12.07f		* 17.5f
#define ENEMY_JUMP_ATT_FAIL_X 0.03f			* 17.5f

#define ENEMY_BOUND_SIZE 1.3

#define ENEMY_NORMAL_MOVE_RANGE 10			//�̵��Ÿ� �ִ밪
#define ENEMY_NORMAL_MOVE_MIN 0.1			//�̵� �ּҰŸ�(ENEMY_NORMAL_MOVE_RANGE�� ���Ұ�)

class bossClass
{
protected:
	int						_dieTimer;

	cPlayer*                 _cPlayer;
	cTerrain*                _cTerrain;
	typedef vector<cBaseObject*>	 _vShadow_Enemy;

	cParticleEmitter* m_pParticle;			//�ڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡ�

	cBoundBox				_boundBox;						//�ٿ��ڽ�

//	cSkinnedAnimation*		_skinned1;						//�Ž�
	cTransform				_skinnedTrans;					//�Ž� Ʈ����

															//��� �浹�� Ʈ����
	cTransform				_leftHeadTrans;					//�Թ��� �� ��弦 ���� Ʈ����
	cTransform				_rightHeadTrans;				//�Թ��� �� ��弦 ���� Ʈ����
	cTransform				_centerHeadTrans;				//��ҿ� �Ӹ� �� �� ������ ��弦 ���� Ʈ����
	cTransform				_centerBackHeadTrans;			//��ҿ� �Ӹ� ����

	cTransform				_beamTrans;						//�� Ʈ������

	typedef vector<cTransform*>		tempTrans;						//�ٵ� �浹�� Ʈ����

															//� Ʈ����

	cTransform				_enemyOriginTrans;				//������ Ʈ������

	cTransform				_enemyTrans;					//���ʹ� Ʈ������
	cTransform				_enemyAirAttTrans;				//���ʹ� ���� ���ݽ� ������ Ʈ������
	cTransform				_enemyAirAttTransReset;			//���ʹ� ���� ���ݽ� �浹 ���� ���� Ʈ������
	cTransform				_enemyColisionTrans;			//���ʹ� �浹�� Ʈ������
	cBoundSphere			_enemyAttRangeSphere;			//�÷��̾ ���󰡴� ����
	cBoundSphere			_enemyAirAttRangeSphere;		//���� ���� ����
	cBoundSphere			_enemySphere;					//���ʹ�
	cBoundSphere			_enemyCoreSphere;				//���ʹ� ���� ���� �ھ� ����
	bool					_enemyAttFlag;					//���ʹ��� ������ ���۵��� �˷���
	int						_enemyMode;						//���� ���ʹ� ����
	int						_prevEnemyMode;					//���� ���ʹ� ����
	bool					_airAttMode;					//���� ���ʹ̰� �������� �ߴ� ���̳�?
	bool					_airAttCondition;				//���� ������ �ص� �ǳ�?
	float					_airAttDis;						//���߰��� �̵��Ÿ�
	int						_airAttTimer;					//���� ���� ������ Ÿ�̸�


	bool					_attTrue;						//���������� ����

	bool					_aniPlayFlag;					//�ִϸ��̼� ���� �÷���

	bool					_meleeAttFlag;					//���� �����÷���

	bool					_airAttDecisionFlag;			//���߰��� ������ Ʈ������ ���� �÷���

	float					_accrueDis;						//���� �������� Ʈ������ �����Ÿ�

	int						_beamAttTimer;					//�� ���� Ÿ�̸�

	vector<cTransform*>		_beamAttTransVector;					//�� Ʈ������ ����


	bool					_attCollFlag;						//������ ���� ���� �÷���			//��
	bool					_damageFlag;						//���� ���� ���� �÷���				//��



	cTransform				_unitNomalTrans;					//������ �÷��̾ ���󰡱��� ����ٴ� Ʈ������
	int						_normalMoveTimer;					//�⺻ �̵� Ÿ�̸�
	D3DXVECTOR3				_startUnitVector3;					//������ ���� ��ǥ

	vector<cSetBoundObject*> m_pVecObjects;						//�ٿ��ڽ� ������Ʈ
	Enemy_Stat              _Enemy_Stat;						//���ʹ� ����
	tempTrans               _tempTrans;

	int						_beamDelayTimer;					//������ ���� Ÿ�̸�

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
	void Render(cCamera* pMainCamera,cLight_Direction* Light_Dt);



	void amendTrans();																							//�浹��, �޽� Ʈ���� ��ǥ ���� �Լ�
	void airAttSet(cTransform *playerTrans, cBoundSphere *playerSphere, cBoundSphere *playerCoreSphere);		//���߰��� ���� �Լ�
	void normalFollow(cTransform *playerTrans, cBoundSphere *playerSphere);										//�÷��̾� ���󰡴� �Լ�
	void interpolation(cTransform *playerTrans);//���� �Լ�

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
	bossClass()
	{
		_dieTimer = 0;
		_rearDie = false;

		_enemyMode = ENEMY_MODE_RUN_STAY;
		_prevEnemyMode = ENEMY_MODE_RUN_STAY;

		_accrueDis = 0.0f;

		_Enemy_Stat.Hp = Boss_MAX_HP;
		_Enemy_Stat.Max_Hp = Boss_MAX_HP;
		_Enemy_Stat.shield = Boss_SHIELD;
		_Enemy_Stat.Max_shield = Boss_SHIELD;
		_Enemy_Stat.Properties = Boss_PROPERTIES;

		_enemyAttFlag = false;
		_airAttMode = false;
		_airAttCondition = true;
		_Enemy_Stat.LiveOrDie = true;
		_attTrue = false;
		_meleeAttFlag = false;
		_aniPlayFlag = false;
		_airAttDecisionFlag = false;

		_beamAttTimer = 1000;
		_normalMoveTimer = 0;
		_beamDelayTimer = 0;

		_beamAttTransVector.clear();

		_attCollFlag = false;			//��
		_damageFlag = false;			//��
	}
	~bossClass()
	{

	}

	void setEnemyTransY(float y)
	{
		_enemyTrans.SetLocalPosition(_enemyTrans.GetWorldPosition().x, y, _enemyTrans.GetWorldPosition().z);
	}

	void falseDamageFlag()				//��		//�������� �ָ� ������ �ٽ� �ʱ�ȭ ���ִ� �Լ�
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

	bool getRealDie()
	{
		return _rearDie;
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

};

