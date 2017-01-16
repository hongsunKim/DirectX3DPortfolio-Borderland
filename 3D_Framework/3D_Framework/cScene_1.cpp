#include "stdafx.h"
#include "cScene_1.h"
#include "cTerrain.h"
#include "cCamera.h"
#include "cLight_Direction.h"
#include "cSetBoundObject.h"
#include "cTransform.h"			

#include "enemyClass1.h"		
#include "enemyClass2.h"		
#include "bossClass.h"			
#include "item.h"

#include "cPlayer.h"			
#include "Ui_Bar.h"
#include "cParticleEmitter.h"
#include "sandEffect.h"
#include "evironmemtMesh.h"
#include "Water.h"
#include "ItemEffect.h"

cScene_1::cScene_1() : E1(0), E2(0), E3(0), isAstar(false)
{}
cScene_1::~cScene_1()
{}
HRESULT cScene_1::Scene_Init()
{
	bool Monster_Init = true;
	//맵=======================================================================
	m_pTerrain = new cTerrain;
	m_pTerrain->Init_Load("../../Resources/Save_File/맵/Map01.txt");
	m_pTerrain->Load_obj("../../Resources/Save_File/오브젝트/OBJ01.txt", &obj);
	m_pTerrain->Load_Bound("../../Resources/Save_File/바운드박스/Bound01.txt", &boundObjects);
	this->pSceneBaseDirectionLight->pTransform->SetRotateWorld(각도(110), 0, 0);
	boundObjects[61]->BoundBox.Warp_Check = true; // 맵이동 오브젝트박스 기동
	//플레이어 =======================================================================================================
	m_pkPlayer = new cPlayer;
	m_pkPlayer->Init(m_pTerrain);
	Player_Eye = m_pkPlayer->GetPlayerEye();
	m_pkPlayer->InitObjectSet(&boundObjects);
	m_pkPlayer->m_pPlayerTrans->SetRotateWorld(0.f, -각도(270), 0.f);
	//에너미=============================================================================================================
	Totalobj.clear();
	for (auto obj : obj) Totalobj.push_back(obj);
	enemy1.clear();
	for (auto Enemy1_Set : m_pTerrain->Enemy1_Start_Pos) {
		_enemyClass1 = new enemyClass1;
		_enemyClass1->Init(1.f, 9.f, 3.f, 0.4f, UNIT_NORMAL, Enemy1_Set, boundObjects, m_pkPlayer, m_pTerrain);
		_enemyClass1->vShadow_Enemy[0]->E1 = E1++;
		if (Monster_Init) {
			Totalobj.push_back(_enemyClass1->vShadow_Enemy[0]);
			enemy1.push_back(_enemyClass1);
		}
	}
	enemy2.clear();
	for (auto Enemy2_Set : m_pTerrain->Enemy2_Start_Pos) {
		_enemyClass2 = new enemyClass2;
		_enemyClass2->Init(1.f, 9.f, 3.f, 0.4f, UNIT_NORMAL, Enemy2_Set, boundObjects, m_pkPlayer, m_pTerrain);
		_enemyClass2->vShadow_Enemy[0]->E2 = E2++;
		if (Monster_Init) {
			Totalobj.push_back(_enemyClass2->vShadow_Enemy[0]);
			enemy2.push_back(_enemyClass2);
		}
	}
	Boss.clear();
	for (auto Boss_Set : m_pTerrain->Boss_Start_Pos) {
		_enemyBoss = new bossClass;
		_enemyBoss->Init(1.f, 9.f, 3.f, 0.4f, UNIT_NORMAL, Boss_Set, boundObjects, m_pkPlayer, m_pTerrain);
		_enemyBoss->vShadow_Enemy[0]->E3 = E3++;
		if (Monster_Init) {
		     Totalobj.push_back(_enemyBoss->vShadow_Enemy[0]);
		     Boss.push_back(_enemyBoss);
		}
	}
	//========================================================================================================================
	UiSet = new Ui_Bar;
	UiSet->Scene_Init();

	Sand = new sandEffect;
	Sand->Scene_Init();

	m_evironmemtMesh = new evironmemtMesh;
	m_evironmemtMesh->Init();
	m_pkPlayer->SetEnemyVector(&enemy1, &enemy2, &Boss);
	m_iSceneNum = 1;

	Water* water = new Water;
	D3DXMATRIXA16 matScaling;
	D3DXMatrixScaling(&matScaling, 0.02f, 0.02f, 0.02f);
	water->Init("../../Resources/Meshes/Water/water_plane_square.X", matScaling);
	m_water.push_back(water);

	SOUNDMANAGER->stop("Intro");
	SOUNDMANAGER->addSound("Map1", "../../Resources/Sounds/Map1.ogg", true, true);
	SOUNDMANAGER->play("Map1");
	SOUNDMANAGER->addSound("sand", "../../Resources/Sounds/sand.ogg", true, true);
	SOUNDMANAGER->play("sand");

	return S_OK;
}

void cScene_1::Scene_Release()
{
	m_pkPlayer->Release();
	SAFE_DELETE(m_pkPlayer);
	m_pTerrain->Release();
	SAFE_DELETE(m_pTerrain);
	SAFE_DELETE(UiSet);
}

void cScene_1::Scene_Update(float timDelta)
{
	if (m_pkPlayer->getWarp_Check()) SCENE_MGR->ChangeScene("씬2", 1);
	this->ReadyShadowMap(&Totalobj, this->m_pTerrain);
	//오브젝트=====================================================
	for (auto obj : obj) obj->Update(timDelta);
	//===============================================================
	//조명===========================================================
	this->pSceneBaseDirectionLight->pTransform->DefaultControl2(timDelta);
	//에너미=============================================================================================================
	int temp = Item_Random;

	for (auto enemy1 : enemy1){
		if (enemy1->getEnemyMode() == ENEMY_MODE_RUN_STAY) {
			if (!enemy1->_movePoint.size()) {
				if (!PHYSICS_MGR->IsOverlap(m_pkPlayer->m_pPlayerTrans, m_pkPlayer->playerSphere, &enemy1->getEnemyTrans(), &enemy1->getEnemySphere())) {
					if (m_pTerrain->setAstar(enemy1->getEnemyTrans().GetWorldPosition(), m_pkPlayer->m_pPlayerTrans->GetWorldPosition())) {
						enemy1->_movePoint = m_pTerrain->getAstarPos();
						enemy1->terrainAstar.SetWorldPosition(enemy1->_movePoint.front());						
					}else
						enemy1->_movePoint.clear();
				}else
					enemy1->_movePoint.clear();
			}
			else if (!PHYSICS_MGR->IsOverlap(m_pkPlayer->m_pPlayerTrans, m_pkPlayer->playerSphere, &enemy1->terrainAstar, &enemy1->tempBoundS)) {
				enemy1->_movePoint.clear();
			}
		}
		enemy1->Update(timDelta, m_pkPlayer->playerSphere, m_pkPlayer->playerCoreSphere, m_pkPlayer->getAir(), D3DXVECTOR3(0, 0, 0));		//박준석
		enemy1->DamageFlag(1.f, 2.f, 3.f);
		if (enemy1->getEnemyMode() == ENEMY_MODE_NULL){
			enemy1->enemyFinish();
			item *tempItem1 = new item;
			tempItem1->Init(enemy1->getEnemyTrans().GetWorldPosition().x, enemy1->getEnemyTrans().GetWorldPosition().y + 0.3, enemy1->getEnemyTrans().GetWorldPosition().z,
				temp, true);	
			_item.push_back(tempItem1);
			break;
		}
	}
	for (auto enemy2 : enemy2) {
		if (enemy2->getEnemyMode() == ENEMY_MODE_RUN_STAY) {
			if (!enemy2->_movePoint.size()) {
				if (!PHYSICS_MGR->IsOverlap(m_pkPlayer->m_pPlayerTrans, m_pkPlayer->playerSphere, &enemy2->getEnemyTrans(), &enemy2->getEnemySphere())) {
					if (m_pTerrain->setAstar(enemy2->getEnemyTrans().GetWorldPosition(), m_pkPlayer->m_pPlayerTrans->GetWorldPosition())) {
						enemy2->_movePoint = m_pTerrain->getAstarPos();
						enemy2->terrainAstar.SetWorldPosition(enemy2->_movePoint.front());
					}else
						enemy2->_movePoint.clear();
				}else
					enemy2->_movePoint.clear();
			}
			else if (!PHYSICS_MGR->IsOverlap(m_pkPlayer->m_pPlayerTrans, m_pkPlayer->playerSphere, &enemy2->terrainAstar, &enemy2->tempBoundS)) {
				enemy2->_movePoint.clear();
			}

		}
		enemy2->Update(timDelta, m_pkPlayer->playerSphere, m_pkPlayer->playerCoreSphere, m_pkPlayer->getAir(), D3DXVECTOR3(0, 0, 0));		//박준석
		enemy2->DamageFlag(1.f, 2.f, 3.f);
		enemy2->setProjectileMinY(m_pTerrain);
		if (enemy2->getEnemyMode() == ENEMY_MODE_NULL){
			enemy2->enemyFinish();
			item *tempItem1 = new item;
			tempItem1->Init(enemy2->getEnemyTrans().GetWorldPosition().x, enemy2->getEnemyTrans().GetWorldPosition().y + 0.3, enemy2->getEnemyTrans().GetWorldPosition().z,
				temp, true);	
			_item.push_back(tempItem1);
			break;
		}
	}
	for (auto Boss : Boss) {
		if (Boss->getEnemyMode() == ENEMY_MODE_RUN_STAY) {
			if (!Boss->_movePoint.size()) {
				if (!PHYSICS_MGR->IsOverlap(m_pkPlayer->m_pPlayerTrans, m_pkPlayer->playerSphere, &Boss->getEnemyTrans(), &Boss->getEnemySphere())) {
					if (m_pTerrain->setAstar(Boss->getEnemyTrans().GetWorldPosition(), m_pkPlayer->m_pPlayerTrans->GetWorldPosition())) {
						Boss->_movePoint = m_pTerrain->getAstarPos();
						Boss->terrainAstar.SetWorldPosition(Boss->_movePoint.front());
					}else
						Boss->_movePoint.clear();
				}else
					Boss->_movePoint.clear();
			}
			else if (!PHYSICS_MGR->IsOverlap(m_pkPlayer->m_pPlayerTrans, m_pkPlayer->playerSphere, &Boss->terrainAstar, &Boss->tempBoundS)) {
				Boss->_movePoint.clear();
			}

		}
		Boss->Update(timDelta, m_pkPlayer->playerSphere, m_pkPlayer->playerCoreSphere, m_pkPlayer->getAir(), D3DXVECTOR3(0, 0, 0));		//박준석
		Boss->DamageFlag(1.f, 2.f, 3.f);
		if (Boss->getEnemyMode() == ENEMY_MODE_NULL){
			Boss->enemyFinish();
			item *tempItem1 = new item;
			tempItem1->Init(Boss->getEnemyTrans().GetWorldPosition().x, Boss->getEnemyTrans().GetWorldPosition().y + 0.3, Boss->getEnemyTrans().GetWorldPosition().z,
				ITEM_SNIPER_RIFLE_AMMO,true);	//true 부분은 플레이어가 아이템을 피킹했는가 여부
			_item.push_back(tempItem1);
			break;
		}
	}

	for (int i = 0; i < enemy1.size(); i++){
		if (enemy1[i]->getRealDie()){
			int enemy1_T_obj = obj.size();
			for (int z = enemy1_T_obj; z < Totalobj.size(); z++) {
				if (enemy1[i]->vShadow_Enemy[0]->E1 == Totalobj[z]->E1) {
					Totalobj.erase(Totalobj.begin() + z);
				}
			}
			enemy1.erase(enemy1.begin() + i);
			break;
		}
	}
	for (int i = 0; i < enemy2.size(); i++){
		if (enemy2[i]->getRealDie()){
			int enemy2_T_obj = obj.size() + enemy1.size();
			for (int z = enemy2_T_obj; z < Totalobj.size(); z++) {
				if (enemy2[i]->vShadow_Enemy[0]->E2 == Totalobj[z]->E2) {
					Totalobj.erase(Totalobj.begin() + z);
				}
			}
			enemy2.erase(enemy2.begin() + i);
			break;
		}
	}
	for (int i = 0; i < Boss.size(); i++){
		if (Boss[i]->getRealDie()){
			int Boss_T_obj = obj.size() + enemy1.size()+ enemy2.size();
			for (int z = Boss_T_obj; z < Totalobj.size(); z++) {
				if (Boss[i]->vShadow_Enemy[0]->E3 == Totalobj[z]->E3) {
					Totalobj.erase(Totalobj.begin() + z);
				}
			}
			Boss.erase(Boss.begin() + i);
			break;
		}
	}

	for (int i = 0; i < _item.size(); i++){
		_item[i]->Update(timDelta, m_pkPlayer->m_pPlayerTrans, true, m_pTerrain->GetHeight(_item[i]->getTrans().GetWorldPosition().x, _item[i]->getTrans().GetWorldPosition().z));
		if (_item[i]->getDeleteItemFlag()){
			switch (_item[i]->getItemInfo())
			{
			case ITEM_HP_POTION:
				m_pkPlayer->Set_Hp_Healing(50);
				break;
			case ITEM_SNIPER_RIFLE_AMMO:
				m_pkPlayer->Set_Bullet_Reload(5);
				break;
			}
			_item.erase(_item.begin() + i);
		}
	}
	//====================================================================================================================																																							
	m_pkPlayer->Update(timDelta);
	UiSet->Player_SetUp(*(m_pkPlayer->GetUiInfo()));
	UiSet->Scene_Update(timDelta);
	Sand->Scene_Update(timDelta);
	m_player_fov = m_pkPlayer->GetPlayerEye()->fov;
	m_pkPlayer->SetRenderTexture(pMainCamera->GetRenderTexture());
	//========================================

	m_pTerrain->setMatInverseProj(&m_pkPlayer->GetPlayerEye()->GetViewProjectionMatrix());
}

void cScene_1::Scene_Render1()
{
	m_evironmemtMesh->Render(pSceneBaseDirectionLight, Main_Eye);
	//에너미==================================================================================
	for (auto enemy1 : enemy1) { enemy1->Render(Main_Eye, pSceneBaseDirectionLight); }					
	for (auto Boss : Boss) { Boss->Render(Main_Eye, pSceneBaseDirectionLight); }					
	//========================================================================================
	//맵 및 오브젝트==================================================================================
	cXMesh_Static::SetTechniqueName("ReciveShadowToon");
	m_pTerrain->Render(this->Main_Eye, this->pSceneBaseDirectionLight, this->pDirectionLightCamera);
	for (auto obj : obj) { obj->Render(); }
	for (auto boundObjects : boundObjects) { boundObjects->Render(Bound_Check); }
	if (KEY_MGR->IsOnceDown('M')) isAstar = !isAstar;
	if (isAstar){
		for (auto Astar_Check : m_pTerrain->vertex_Index) {
			if (Astar_Check.Vertex_Astar_check)  GIZMO_MGR->WireSphere(Astar_Check.Vertex_Astar_pos, 0.1f, 0xff000000);
		}
	}
	//========================================================================================
	for (int i = 0; i < _item.size(); i++){
		_item[i]->Render(Main_Eye);
	}
	m_pkPlayer->Render(Main_Eye, this->pSceneBaseDirectionLight);
	Sand->Scene_Render1();
	for (auto enemy2 : enemy2) { enemy2->Render(Main_Eye, pSceneBaseDirectionLight); }			
	cTransform* Transform = new cTransform;
	Transform->SetWorldPosition(-9, -0.5f, 7);
	cXMesh_Static::SetTechniqueName("Water");
	m_water[0]->Render(Transform, true);
	cXMesh_Static::SetTechniqueName("ReciveShadowToon");
}
void cScene_1::Scene_RenderSprite()
{
	if (Camera_Check) UiSet->Total_Render(Main_Eye, m_pkPlayer, Boss, enemy1, enemy2,_item);
}
