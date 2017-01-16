#pragma once
#include "enemyH.h"
#include "cTransform.h"
#include "cXMesh_Skinned.h"
#include "cXMesh_Static.h"
#include "cBoundBox.h"
#include "cSetBoundObject.h"

class ItemEffect;

#define ITEM_RADIUS 0.3F
#define ITEM_SIZE 0.025F

enum ITEM_CODE
{
	ITEM_HP_POTION = 0,
	ITEM_SNIPER_RIFLE_AMMO
};

class item
{
private:
	cXMesh_Static			_skinned;					//매쉬
	cTransform				_skinnedTrans;				//매쉬 트랜스
	cBoundSphere			_itemSphere;				//충돌용 스피어

	float					_itemGravity = 0.0f;		//아이템 중력값
	float					_itemUpSpeed = 0.0f;		//아이템 상승 속도
	float					_itemStartY;				//아이템 상승 시작 위치
	bool					_itemNewFlag;				//아이템 임시 생성 플래그
	bool					_itemMoveFlag;				//아이템 이동 플래그
	bool					_enemyItem;					//에너미에서 떨어졌는지 판정 플래그
	int						_itemType;					//아이템 정보
	bool					_deleteItemFlag;			//아이템이 지워저야되는지 판정 플래그
	bool                    Item_Pic_ok;

	int						Item0;
	int						Item1;

public:
	ItemEffect* _ItemEffect;

	item()
	{
		_itemNewFlag = false;
		_itemMoveFlag = false;
		_enemyItem = false;
		_deleteItemFlag = false;
	}
	~item()
	{

	}

	HRESULT Init(float x, float y, float z, int type, bool enemyItem);
	void Release();
	void Update(float timeDelta, cTransform* pos, bool eat, float minY);
	void Render(cCamera* pMainCamera);



	/////////////////////////////////////////////////////////////
	//홍순이 형을 위한 선물 데헷~
	int getItemInfo()				//아이템 종류
	{
		return _itemType;
		//포션 = 0
		//총알 = 1
	}
	cTransform getTrans()			//아이템 트랜스폼
	{
		return _skinnedTrans;
	}
	cBoundSphere getItemSphere()	//아이템 반지름
	{
		return _itemSphere;
	}
	bool getDeleteItemFlag()		//이게 true면 아이템이 지워짐
	{
		return _deleteItemFlag;
	}
	bool get_Item_Pic_ok(){
		return Item_Pic_ok;
	}
	void set_Item_Pic_ok(bool ok){
		Item_Pic_ok = ok;
	}
};