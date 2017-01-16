#include "stdafx.h"
#include "cScene_00.h"
#include "cLight_Direction.h"
#include "cXMesh_Static.h"
#include "cBaseObject.h"
#include "cTransform.h"
#include "cCamera.h"

cScene_00::cScene_00()
{
}


cScene_00::~cScene_00()
{
}

HRESULT cScene_00::Scene_Init()
{
	//원래 있던 매쉬에서 다음과 같은 행렬로 보정시켜 
	//모든 정보를 틀어버린다.
	D3DXMATRIXA16 matScale;
	D3DXMatrixScaling(&matScale, 0.01f, 0.01f, 0.01f);
	D3DXMATRIXA16 matRotate;
	D3DXMatrixRotationY(&matRotate, 180.0f * ONE_RAD);
	D3DXMATRIXA16 matCorrection = matScale * matRotate;

	//원본 메쉬 로딩
	cXMesh_Static* mesh =
		RESOURCE_STATICXMESH->GetResource(
		"../../Resources/Meshes/Fighter/Fighter.X", &matCorrection);


	cXMesh_Static* box =
		RESOURCE_STATICXMESH->GetResource(
		"../../Resources/Meshes/Disc.x");

	//오브젝트 생성
	cBaseObject* pNewObject1 = new cBaseObject();
	pNewObject1->SetMesh(mesh);
	pNewObject1->SetActive(true);

	cBaseObject* pNewObject2 = new cBaseObject();
	pNewObject2->SetMesh(box);
	pNewObject2->SetActive(true);
	pNewObject2->pTransform->SetWorldPosition(0, -10.0f, 0.0f);
	pNewObject2->IgnoreCreateShadow = true;		//그림자 안그린다.

	cBaseObject* pNewObject3 = new cBaseObject();
	pNewObject3->SetMesh(mesh);
	pNewObject3->SetActive(true);
	pNewObject3->pTransform->SetWorldPosition(0, -5.0f, 0.0f);

	//랱더 오브젝트 푸쉬
	this->renderObjects.push_back(pNewObject1);
	this->renderObjects.push_back(pNewObject2);
	this->renderObjects.push_back(pNewObject3);


	//라이트 위치
	this->pSceneBaseDirectionLight->pTransform->SetWorldPosition(0, 20, 0);
	this->pSceneBaseDirectionLight->pTransform->SetRotateWorld(90.0f * ONE_RAD, 0, 0);


	return S_OK;
}

void cScene_00::Scene_Release()
{
	//오브젝트 해재
	for (int i = 0; i < this->renderObjects.size(); i++)
		SAFE_DELETE(this->renderObjects[i]);
	this->renderObjects.clear();
	this->cullObjects.clear();
}

void cScene_00::Scene_Update(float timeDelta)
{
	if (KEY_MGR->IsOnceDown(VK_RETURN)){
		//SCENE_MGR->ChangeScene("씬투", 1); //둘 중에 하나만 해라. 같이 하면 이상
		SCENE_MGR->ChangeSceneWithLoading(
			"씬투",
			"로딩씬",
			1, 1);

	}

	this->pSceneBaseDirectionLight->pTransform->DefaultControl2(timeDelta);

	this->pMainCamera->UpdateFrustum();
	this->cullObjects.clear();
	for (int i = 0; i < this->renderObjects.size(); i++){

		this->renderObjects[i]->Update(timeDelta);

		//프러스텀 안에 있니?
		if (this->pMainCamera->Frustum.IsInFrustum(this->renderObjects[i]))
		{
			this->cullObjects.push_back(this->renderObjects[i]);
		}
	}

	//쉐도우맵 준비
	this->ReadyShadowMap(&this->renderObjects);  //랜더링될 오브젝트 전달한다 ( 반드시 Render 에서 하지말고 업데이트에서 해라 )
}

void cScene_00::Scene_Render1()
{
	cXMesh_Static::SetCamera(this->pMainCamera);
	cXMesh_Static::SetTechniqueName("ReciveShadow");		//쉐도우랑 같이 그릴려면 ReciveShadow 로 Technique 셋팅

	//베이스 라이팅 셋팅
	cXMesh_Static::SetBaseLight(this->pSceneBaseDirectionLight);

	for (int i = 0; i < this->cullObjects.size(); i++)
		this->cullObjects[i]->Render();

	//프러텀을 그려보장
	this->pDirectionLightCamera->Frustum.RenderGizmo();
	this->pSceneBaseDirectionLight->pTransform->RenderGimozo();
}

void cScene_00::Scene_RenderSprite()
{
	//쉐도우 맵을 그려보자....
	RECT rcSour = { 0, 0, 4096, 4096 };
	RECT rcDest = { 0, 0, 300, 300 };


	SPRITE_MGR->DrawArea(this->pDirectionLightCamera->GetRenderTexture(),	&rcSour, &rcDest);
}