#include "stdafx.h"
#include "item.h"
#include "ItemEffect.h"


HRESULT item::Init(float x, float y, float z, int type, bool enemyItem)
{
	_itemType = type;
	Item_Pic_ok = false;
	_enemyItem = enemyItem;
	D3DXMATRIXA16 matCorrection;
	_ItemEffect = new ItemEffect;
	_ItemEffect->Scene_Init(type);

	switch (type)
	{
	case ITEM_HP_POTION:
		D3DXMatrixScaling(&matCorrection, ITEM_SIZE, ITEM_SIZE, ITEM_SIZE);
		_skinned.Init("../../Resources/item/Hp/Health_Item.X", &matCorrection);
		break;
	case ITEM_SNIPER_RIFLE_AMMO:
		D3DXMatrixScaling(&matCorrection, ITEM_SIZE, ITEM_SIZE, ITEM_SIZE);
		_skinned.Init("../../Resources/item/Sniper_Rifle_Ammo/Sniper_Rifle_Ammo.X", &matCorrection);
		break;
	}

	_itemSphere.localCenter = D3DXVECTOR3(0, 0, 0);
	_itemSphere.SetBound(&D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(ITEM_RADIUS, ITEM_RADIUS, ITEM_RADIUS));
	_itemSphere.radius = ITEM_RADIUS;
	_skinnedTrans.SetWorldPosition(x, y, z);
	SOUNDMANAGER->addSound("Pickup_Ammo_01", "../../Resources/Sounds/Pickup_Ammo_01.ogg", false, false);
	SOUNDMANAGER->addSound("Pickup_Health", "../../Resources/Sounds/Pickup_Health.ogg", false, false);
	return S_OK;
}
void item::Release()
{

}
void item::Update(float timeDelta, cTransform* pos, bool eat, float minY)
{
	if (_enemyItem)
	{
		_itemNewFlag = true;
		_skinnedTrans.RotateSelf(0, ((D3DX_PI * 1000000000 / 10) * 0.000000001f  * (rand() % 10)), 0);
		_itemStartY = _skinnedTrans.GetWorldPosition().y;
		_enemyItem = false;

		_itemGravity += 0.025;
		_itemUpSpeed += 0.01;
		_skinnedTrans.MovePositionSelf(0, _itemUpSpeed - (_itemUpSpeed * _itemGravity), 0.03f);
	}

	float tempGra = _itemGravity + 0.025;
	float tempUpSpeed = _itemUpSpeed + 0.01;
	cTransform tempTrans = _skinnedTrans;
	tempTrans.MovePositionSelf(0, tempUpSpeed - (tempUpSpeed * tempGra), 0.03f);

	if (tempTrans.GetWorldPosition().y <= minY + 0.3f)
	{
		_itemNewFlag = false;
		_itemGravity = 0;
		_itemUpSpeed = 0;
	}
	if (_itemNewFlag)
	{
		_itemGravity += 0.025;
		_itemUpSpeed += 0.01;
		_skinnedTrans.MovePositionSelf(0, _itemUpSpeed - (_itemUpSpeed * _itemGravity), 0.03f);
	}
	if (Item_Pic_ok)
	{

		if (KEY_MGR->IsOnceDown('E')) {
			_itemMoveFlag = true;

			if(_itemType == ITEM_HP_POTION)
				SOUNDMANAGER->play("Pickup_Health",1.0f);
				
			else if (_itemType == ITEM_SNIPER_RIFLE_AMMO)
				SOUNDMANAGER->play("Pickup_Ammo_01",1.0f);
			
			
		}

	}
	if (_itemMoveFlag)
	{
		_skinnedTrans.LookDirection(pos->GetWorldPosition() - _skinnedTrans.GetWorldPosition(), _skinnedTrans.GetUp());
		_skinnedTrans.MovePositionSelf(0, 0, 0.1f);
	}
	if (PHYSICS_MGR->IsOverlap(&_skinnedTrans, &_itemSphere, pos, &_itemSphere))
	{
		_deleteItemFlag = true;
	}

	GIZMO_MGR->WireSphere(pos->GetLocalPosition(), _itemSphere.radius, 0xff0000);
	_ItemEffect->Scene_Update(timeDelta);
	D3DXVECTOR3 temp_pos = _skinnedTrans.GetWorldPosition();
	temp_pos.y = _skinnedTrans.GetWorldPosition().y + 0.1f;
	_ItemEffect->SetPos(&temp_pos);
}
void item::Render(cCamera* pMainCamera)
{
	cXMesh_Static::SetTechniqueName("ReciveShadowToonNoOutline");
	_skinned.Render(&_skinnedTrans);
	//GIZMO_MGR->WireSphere(_skinnedTrans.GetLocalPosition(), _itemSphere.radius, 0xff0000);
	cXMesh_Static::SetTechniqueName("ReciveShadowToon");
	_ItemEffect->Scene_Render1();
}