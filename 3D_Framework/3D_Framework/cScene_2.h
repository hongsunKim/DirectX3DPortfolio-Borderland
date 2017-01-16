#pragma once
#include "cScene.h"

class cTerrain;
class cTransform;
class cLight_Direction;
class cSetBoundObject;
class enemyClass1;		
class enemyClass2;		
class bossClass;		
class item;
class cPlayer;			
class Ui_Bar;
class sandEffect;
class evironmemtMesh;

class cScene_2 : public cScene
{
private:
	//¸Ê==========================================================
	cTerrain*							m_pTerrain;
	vector<cBaseObject*>				obj;
	vector<cSetBoundObject*>			boundObjects;
	vector<cBaseObject*>				Totalobj;
	//============================================================
	enemyClass1*						_enemyClass1;				
	enemyClass2*						_enemyClass2;				
	bossClass*							_enemyBoss;				

	vector<enemyClass1*>				 enemy1;                
	vector<enemyClass2*>				 enemy2;
	vector<bossClass*>					 Boss;
	vector<item*>						 _item;

	cPlayer*							 m_pkPlayer;
	Ui_Bar*								 UiSet;
	sandEffect*							 Sand;
	evironmemtMesh*						 m_evironmemtMesh;

	int									 E1, E2, E3;

public:
	cScene_2();
	~cScene_2();

	virtual HRESULT Scene_Init();
	virtual void Scene_Release();
	virtual void Scene_Update(float timDelta);
	virtual void Scene_Render1();
	virtual void Scene_RenderSprite();
};

