#include "stdafx.h"
#include "cScene_02.h"
#include "cLight_Direction.h"
#include "cXMesh_Static.h"
#include "cBaseObject.h"
#include "cTransform.h"
#include "cCamera.h"
#include "cTerrain.h"
#include "cSkinnedAnimation.h"
#include "cLight_Point.h"

cScene_02::cScene_02()
{
}


cScene_02::~cScene_02()
{
}


HRESULT cScene_02::Scene_Init()
{
	//원래 있던 매쉬에서 다음과 같은 행렬로 보정시켜 
	//모든 정보를 틀어버린다.
	D3DXMATRIXA16 matScale;
	D3DXMatrixScaling(&matScale, 0.007f, 0.007f, 0.007f);
	D3DXMATRIXA16 matCorrection = matScale;

	//원본 메쉬 로딩
	cXMesh_Static* mesh =
		RESOURCE_STATICXMESH->GetResource(
		"../../Resources/Meshes/Tree/PineTree_01.X", &matCorrection);

	this->object = new cBaseObject();
	this->object->SetActive(true);
	this->object->SetMesh(mesh);

	//기본라이트 방향
	this->pSceneBaseDirectionLight->pTransform->RotateWorld(
		90.0f * ONE_RAD, 0, 0);


	this->pMainCamera->SetWorldPosition(0, 5, -10);


	return S_OK;
}

void cScene_02::Scene_Release()
{
	//오브젝트 해재
	SAFE_DELETE(this->object);
}

void cScene_02::Scene_Update(float timeDelta)
{
	this->object->pTransform->RotateWorld(0, 90 * ONE_RAD * timeDelta, 0);
	this->object->Update(timeDelta);
}

void cScene_02::Scene_Render1()
{

	cXMesh_Static::SetCamera(this->pMainCamera);
	cXMesh_Static::SetTechniqueName("BaseTree");  //쉐도우랑 같이 그릴려면 ReciveShadow 로 Technique 셋팅
	cXMesh_Static::SetBaseLight(this->pSceneBaseDirectionLight);;


	cXMesh_Skinned::SetCamera(this->pMainCamera);
	cXMesh_Skinned::SetTechniqueName("ReciveShadow");
	cXMesh_Skinned::sSkinnedMeshEffect->SetTexture("Ramp_Tex", RESOURCE_TEXTURE->GetResource("../../Resources/Testures/Ramp_1.png"));
	cXMesh_Skinned::SetBaseLight(this->pSceneBaseDirectionLight);

	this->object->Render();
}

void cScene_02::Scene_RenderSprite()
{
	DXFONT_MGR->PrintText(SCENE_MGR->fString, 100, 100, 0xffffff00);
	char szTemp[128];
	sprintf(szTemp, "%.2f 퍼센트 로딩중", SCENE_MGR->fProgress);
	DXFONT_MGR->PrintText(szTemp, 100, 130, 0xffffff00);
}