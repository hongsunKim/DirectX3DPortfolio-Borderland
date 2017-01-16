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
	//���� �ִ� �Ž����� ������ ���� ��ķ� �������� 
	//��� ������ Ʋ�������.
	D3DXMATRIXA16 matScale;
	D3DXMatrixScaling(&matScale, 0.01f, 0.01f, 0.01f);
	D3DXMATRIXA16 matRotate;
	D3DXMatrixRotationY(&matRotate, 180.0f * ONE_RAD);
	D3DXMATRIXA16 matCorrection = matScale * matRotate;

	//���� �޽� �ε�
	cXMesh_Static* mesh =
		RESOURCE_STATICXMESH->GetResource(
		"../../Resources/Meshes/Fighter/Fighter.X", &matCorrection);


	cXMesh_Static* box =
		RESOURCE_STATICXMESH->GetResource(
		"../../Resources/Meshes/Disc.x");

	//������Ʈ ����
	cBaseObject* pNewObject1 = new cBaseObject();
	pNewObject1->SetMesh(mesh);
	pNewObject1->SetActive(true);

	cBaseObject* pNewObject2 = new cBaseObject();
	pNewObject2->SetMesh(box);
	pNewObject2->SetActive(true);
	pNewObject2->pTransform->SetWorldPosition(0, -10.0f, 0.0f);
	pNewObject2->IgnoreCreateShadow = true;		//�׸��� �ȱ׸���.

	cBaseObject* pNewObject3 = new cBaseObject();
	pNewObject3->SetMesh(mesh);
	pNewObject3->SetActive(true);
	pNewObject3->pTransform->SetWorldPosition(0, -5.0f, 0.0f);

	//�M�� ������Ʈ Ǫ��
	this->renderObjects.push_back(pNewObject1);
	this->renderObjects.push_back(pNewObject2);
	this->renderObjects.push_back(pNewObject3);


	//����Ʈ ��ġ
	this->pSceneBaseDirectionLight->pTransform->SetWorldPosition(0, 20, 0);
	this->pSceneBaseDirectionLight->pTransform->SetRotateWorld(90.0f * ONE_RAD, 0, 0);


	return S_OK;
}

void cScene_00::Scene_Release()
{
	//������Ʈ ����
	for (int i = 0; i < this->renderObjects.size(); i++)
		SAFE_DELETE(this->renderObjects[i]);
	this->renderObjects.clear();
	this->cullObjects.clear();
}

void cScene_00::Scene_Update(float timeDelta)
{
	if (KEY_MGR->IsOnceDown(VK_RETURN)){
		//SCENE_MGR->ChangeScene("����", 1); //�� �߿� �ϳ��� �ض�. ���� �ϸ� �̻�
		SCENE_MGR->ChangeSceneWithLoading(
			"����",
			"�ε���",
			1, 1);

	}

	this->pSceneBaseDirectionLight->pTransform->DefaultControl2(timeDelta);

	this->pMainCamera->UpdateFrustum();
	this->cullObjects.clear();
	for (int i = 0; i < this->renderObjects.size(); i++){

		this->renderObjects[i]->Update(timeDelta);

		//�������� �ȿ� �ִ�?
		if (this->pMainCamera->Frustum.IsInFrustum(this->renderObjects[i]))
		{
			this->cullObjects.push_back(this->renderObjects[i]);
		}
	}

	//������� �غ�
	this->ReadyShadowMap(&this->renderObjects);  //�������� ������Ʈ �����Ѵ� ( �ݵ�� Render ���� �������� ������Ʈ���� �ض� )
}

void cScene_00::Scene_Render1()
{
	cXMesh_Static::SetCamera(this->pMainCamera);
	cXMesh_Static::SetTechniqueName("ReciveShadow");		//������� ���� �׸����� ReciveShadow �� Technique ����

	//���̽� ������ ����
	cXMesh_Static::SetBaseLight(this->pSceneBaseDirectionLight);

	for (int i = 0; i < this->cullObjects.size(); i++)
		this->cullObjects[i]->Render();

	//�������� �׷�����
	this->pDirectionLightCamera->Frustum.RenderGizmo();
	this->pSceneBaseDirectionLight->pTransform->RenderGimozo();
}

void cScene_00::Scene_RenderSprite()
{
	//������ ���� �׷�����....
	RECT rcSour = { 0, 0, 4096, 4096 };
	RECT rcDest = { 0, 0, 300, 300 };


	SPRITE_MGR->DrawArea(this->pDirectionLightCamera->GetRenderTexture(),	&rcSour, &rcDest);
}