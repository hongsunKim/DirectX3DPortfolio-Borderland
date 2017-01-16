#include "stdafx.h"
#include "cScene_01.h"
#include "cLight_Direction.h"
#include "cXMesh_Static.h"
#include "cBaseObject.h"
#include "cTransform.h"
#include "cCamera.h"
#include "cTerrain.h"
#include "cSkinnedAnimation.h"

cScene_01::cScene_01()
{
}


cScene_01::~cScene_01()
{
}

HRESULT cScene_01::Scene_Init()
{
	//진행률
	SCENE_MGR->fProgress = 0.0f;
	SCENE_MGR->fString = "개벌찟 중";


	double startRealTime = TIME_MGR->GetNowRealTimeSec();

	while (true)
	{
		double nowReadTime = TIME_MGR->GetNowRealTimeSec();
		if (nowReadTime > startRealTime + 3.5)
			break;
	}

	LOG_MGR->AddLog("fffff");


	//진행률
	SCENE_MGR->fProgress = 35.0f;
	SCENE_MGR->fString = "개벌찟2 중";


	startRealTime = TIME_MGR->GetNowRealTimeSec();

	while (true)
	{
		double nowReadTime = TIME_MGR->GetNowRealTimeSec();
		if (nowReadTime > startRealTime + 3.5)
			break;
	}

	LOG_MGR->AddLog("fffff");



	//진행률
	SCENE_MGR->fProgress = 70.0f;
	SCENE_MGR->fString = "지형로딩 중";

	m_pTerrain = new cTerrain();
	m_pTerrain->Init_Load("../../Resources/Save_File/Map2.txt");

	m_pTerrainShadow = new cTerrain();
	m_pTerrainShadow->Init_Load("../../Resources/Save_File/Map3.txt");


	//진행률
	SCENE_MGR->fProgress = 90.0f;
	SCENE_MGR->fString = "오브젝트 로딩 중";


	//원래 있던 매쉬에서 다음과 같은 행렬로 보정시켜 
	//모든 정보를 틀어버린다.
	D3DXMATRIXA16 matScale;
	D3DXMatrixScaling(&matScale, 0.007f, 0.007f, 0.007f);
	D3DXMATRIXA16 matCorrection = matScale;

	//원본 메쉬 로딩
	cXMesh_Static* mesh =
		RESOURCE_STATICXMESH->GetResource(
		"../../Resources/Meshes/Tree/PineTree_01.X", &matCorrection);

	cXMesh_Static* Disc =
		RESOURCE_STATICXMESH->GetResource(
		"../../Resources/Meshes/Disc.x");

	//오브젝트 생성
	for (int i = 0; i < OBJECT_NUM; i++)
	{
		for (int j = 0; j < OBJECT_NUM; j++)
		{
			cBaseObject* pNewObject = new cBaseObject();
			pNewObject->SetMesh(mesh);
			pNewObject->SetActive(true);

			D3DXVECTOR3 pos(i * 10, 30.0f, j * 20);

			pos.y = this->m_pTerrain->GetHeight(pos.x, pos.z);

			//위치 셋팅
			pNewObject->pTransform->SetWorldPosition(pos);


			//랱더 오브젝트 푸쉬
			this->renderObjects.push_back(pNewObject);
		}
	}

	//디스크 추가
	cBaseObject* pNewObject2 = new cBaseObject();
	pNewObject2->SetMesh(Disc);
	pNewObject2->SetActive(true);
	pNewObject2->pTransform->SetWorldPosition(0, -10.0f, 0.0f);
	pNewObject2->IgnoreCreateShadow = true;		//그림자 안그린다.
	this->renderObjects.push_back(pNewObject2);

	//기본라이트 방향
	this->pSceneBaseDirectionLight->pTransform->RotateWorld(
		90.0f * ONE_RAD, 0, 0);

	this->SetEnvironment("../../Resources/TextureCUBE/SuperKanjiCube.dds");



	D3DXMATRIXA16 matCorrection2;
	D3DXMatrixScaling(&matCorrection2, 0.1f, 0.1f, 0.1f);
	cXMesh_Skinned* pSkinned = RESOURCE_SKINNEDXMESH->GetResource("../../Resources/Meshes/Female/Femail.X", &matCorrection2);

	cBaseObject* pChar;
	pChar = new cBaseObject();
	pChar->SetMesh(pSkinned);
	pChar->SetActive(true);

	D3DXVECTOR3 pos2(13, 0, 26);
	pos2.y = this->m_pTerrain->GetHeight(pos2.x, pos2.z);
	pChar->pTransform->SetWorldPosition(pos2);

	this->renderObjects.push_back(pChar);



	//진행률
	SCENE_MGR->fProgress = 100.0f;
	SCENE_MGR->fString = "로딩 끝 이새끼야....";

	return S_OK;
}

void cScene_01::Scene_Release()
{
	//오브젝트 해재
	for (int i = 0; i < this->renderObjects.size(); i++)
		SAFE_DELETE(this->renderObjects[i]);
	this->renderObjects.clear();
	this->cullObjects.clear();


	SAFE_DELETE(this->m_pTerrain);
	SAFE_DELETE(this->m_pTerrainShadow);
}

void cScene_01::Scene_Update(float timeDelta)
{
	if (KEY_MGR->IsOnceDown(VK_RETURN))
		SCENE_MGR->ChangeScene("테스트씬", 2);

	this->pSceneBaseDirectionLight->pTransform->DefaultControl2(timeDelta);


	for (int i = 0; i < this->renderObjects.size(); i++)
		this->renderObjects[i]->Update(timeDelta);

	//쉐도우맵 준비
	this->ReadyShadowMap(&this->renderObjects, this->m_pTerrain);
}

void cScene_01::Scene_Render1()
{
	//카메라에 컬링된거만....
	this->cullObjects.clear();
	for (int i = 0; i < this->renderObjects.size(); i++)
	{
		//프러스텀 안에 있니?
		if (this->pMainCamera->Frustum.IsInFrustum(this->renderObjects[i]))
			this->cullObjects.push_back(this->renderObjects[i]);
	}



	cXMesh_Static::SetCamera(this->pMainCamera);
	cXMesh_Static::SetTechniqueName("BaseTree");		//쉐도우랑 같이 그릴려면 ReciveShadow 로 Technique 셋팅
	cXMesh_Static::SetBaseLight(this->pSceneBaseDirectionLight);

	cXMesh_Skinned::SetCamera(this->pMainCamera);
	//cXMesh_Skinned::SetTechniqueName("ReciveShadow");
	cXMesh_Skinned::SetTechniqueName("Toon");
	cXMesh_Skinned::sSkinnedMeshEffect->SetTexture("Ramp_Tex", RESOURCE_TEXTURE->GetResource("../../Resources/Testures/Ramp_1.png"));
	cXMesh_Skinned::SetBaseLight(this->pSceneBaseDirectionLight);

	for (int i = 0; i < this->cullObjects.size(); i++)
		this->cullObjects[i]->Render();

	m_pTerrain->Render(
		this->pMainCamera,
		this->pSceneBaseDirectionLight,
		this->pDirectionLightCamera);
}

void cScene_01::Scene_RenderSprite()
{

}