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
	cXMesh_Static			_skinned;					//�Ž�
	cTransform				_skinnedTrans;				//�Ž� Ʈ����
	cBoundSphere			_itemSphere;				//�浹�� ���Ǿ�

	float					_itemGravity = 0.0f;		//������ �߷°�
	float					_itemUpSpeed = 0.0f;		//������ ��� �ӵ�
	float					_itemStartY;				//������ ��� ���� ��ġ
	bool					_itemNewFlag;				//������ �ӽ� ���� �÷���
	bool					_itemMoveFlag;				//������ �̵� �÷���
	bool					_enemyItem;					//���ʹ̿��� ���������� ���� �÷���
	int						_itemType;					//������ ����
	bool					_deleteItemFlag;			//�������� �������ߵǴ��� ���� �÷���
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
	//ȫ���� ���� ���� ���� ����~
	int getItemInfo()				//������ ����
	{
		return _itemType;
		//���� = 0
		//�Ѿ� = 1
	}
	cTransform getTrans()			//������ Ʈ������
	{
		return _skinnedTrans;
	}
	cBoundSphere getItemSphere()	//������ ������
	{
		return _itemSphere;
	}
	bool getDeleteItemFlag()		//�̰� true�� �������� ������
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