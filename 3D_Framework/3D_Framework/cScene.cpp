#include "stdafx.h"
#include "cScene.h"
#include "cCamera.h"
#include "cLight_Direction.h"
#include "cTerrain.h"

cScene::cScene()
{
	this->pMainCamera = new cCamera();
	Player_Eye = new cCamera();
	Main_Eye = new cCamera();

	//�⺻ ���� ����
	this->pSceneBaseDirectionLight = new cLight_Direction();

	//���⼺ ������ ī�޶� �� �����Ѵ�.
	this->pDirectionLightCamera = new cCamera();

	//�⺻ ��
	this->pSceneBaseDirectionLight->Color = D3DXCOLOR(1, 1, 1, 1);
	this->pSceneBaseDirectionLight->Intensity = 1.0f;

	//�׸��� �Ÿ�
	shadowDistance = 100.0f;


	//ī�޶��� ��������� �ٲ۴�...
	this->pDirectionLightCamera->bOrtho = true;
	this->pDirectionLightCamera->camNear = 0.1f;
	this->pDirectionLightCamera->camFar = shadowDistance * 2.0f;
	this->pDirectionLightCamera->aspect = 1;
	this->pDirectionLightCamera->orthoSize = shadowDistance * 1.5f;	//����ũ��� �׸���ũ���...

	//���⼺���� ī�޶��� RenderToTexture �غ�
	this->pDirectionLightCamera->ReadyShadowTexture(4096);
}


cScene::~cScene()
{

	SAFE_DELETE(this->pMainCamera);
	SAFE_DELETE(Player_Eye);
	SAFE_DELETE(Main_Eye);

	SAFE_DELETE(this->pSceneBaseDirectionLight);
	SAFE_DELETE(this->pDirectionLightCamera);
}

HRESULT cScene::Init()
{
	//���� �ʱ�ȭ �̷������.
	if (FAILED(this->Scene_Init()))
		return E_FAIL;

	Main_Eye = pMainCamera;

	this->pMainCamera->SetWorldPosition(0, 9, -15);
	this->pMainCamera->LookPosition(d3v3(0, 0, 0));

	//����ī�޶� RenderToTexture �غ�
	this->pMainCamera->ReadyRenderToTexture(WINSIZE_X, WINSIZE_Y);

	//0----1
	//|   /|
	//|  / |
	//| /  |
	//|/   |
	//3----2

	this->scenePlaneVertex[0].pos = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);
	this->scenePlaneVertex[1].pos = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
	this->scenePlaneVertex[2].pos = D3DXVECTOR3(1.0f, -1.0f, 0.0f);
	this->scenePlaneVertex[3].pos = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);

	this->scenePlaneVertex[0].uv = D3DXVECTOR2(0.0f, 0.0f);
	this->scenePlaneVertex[1].uv = D3DXVECTOR2(1.0f, 0.0f);
	this->scenePlaneVertex[2].uv = D3DXVECTOR2(1.0f, 1.0f);
	this->scenePlaneVertex[3].uv = D3DXVECTOR2(0.0f, 1.0f);

	this->scenePlaneIndex[0] = 0;
	this->scenePlaneIndex[1] = 1;
	this->scenePlaneIndex[2] = 3;
	this->scenePlaneIndex[3] = 3;
	this->scenePlaneIndex[4] = 1;
	this->scenePlaneIndex[5] = 2;

	this->postEffect = RESOURCE_FX->GetResource("../../Resources/Shaders/PostEffect.fx");

	return S_OK;
}

void cScene::Release()
{
	//���� ������ �Ͼ��.
	//this->Scene_Release();
}

void cScene::Update(float timeDelta)
{
	if (!Camera_Check) {
		pMainCamera->DefaultControl(timeDelta);
	}

		pMainCamera->UpdateCamToDevice(Device);
		pMainCamera->UpdateFrustum();


		//����ī�޶� DirectionLight �� ������ ������ü ����ٴϰ�....

		//���� ��ġ
		D3DXVECTOR3 camPos = pMainCamera->GetWorldPosition();		//����ī�޶��� ��ġ
		D3DXVECTOR3 camFront = pMainCamera->GetForward();			//����ī�޶��� ����
		D3DXVECTOR3 lightDir = pSceneBaseDirectionLight->pTransform->GetForward();	//���⼺ ������ ����

		D3DXVECTOR3 lightPos = camPos +
			(camFront * (shadowDistance * 0.5f)) +
			(-lightDir * shadowDistance);

		this->pDirectionLightCamera->SetWorldPosition(lightPos.x, lightPos.y, lightPos.z);
		this->pDirectionLightCamera->LookDirection(lightDir);

	//���� , ����ī�޶� ����
	(Camera_Check) ? Main_Eye = Player_Eye : Main_Eye = pMainCamera;

	//���� ������Ʈ�� �Ͼ��.
	this->Scene_Update(timeDelta);
}

void cScene::Render()
{
	this->pMainCamera->RenderTextureBegin(0x00000000);


	//�����ȴ�.
	this->Scene_Render0();
	this->Scene_Render1();
	this->Scene_Render2();

#ifdef _DEBUG		//����� ��忡���� ����
	//����̽� ������ ���� ���
	//���� �׸���
	//GIZMO_MGR->WorldGrid(1, 10);
#endif

	this->pMainCamera->RenderTextureEnd();

	//Scene ����
	this->postEffect->SetTexture("screenTex", this->pMainCamera->GetRenderTexture());
	this->postEffect->SetTexture("normalTex", this->pMainCamera->GetRenderTexture());
	if ((m_player_fov > 20.f*ONE_RAD))
	{
		this->postEffect->SetTechnique("Base");
	}
	else
	{
		if (Camera_Check && 1 == m_iSceneNum) {
			this->postEffect->SetTechnique("ScopeOutLine");
			this->postEffect->SetTexture("scopeTex", RESOURCE_TEXTURE->GetResource("../../Resources/Testures/ScopeReticuleMask.tga"));
			this->postEffect->SetTexture("ScopeGlassTex", RESOURCE_TEXTURE->GetResource("../../Resources/Testures/texture_Scope_GlassReflection4.tga"));
			this->postEffect->SetTexture("ScopesDifTex", RESOURCE_TEXTURE->GetResource("../../Resources/Testures/Scopes_Dif_GP.tga"));
			this->postEffect->SetTexture("ScopesMasksTex", RESOURCE_TEXTURE->GetResource("../../Resources/Testures/Scopes_Masks_GP.tga"));
			this->postEffect->SetFloat("gTime", TIME_MGR->GetTotalDeltaSec()*2.f);
			this->postEffect->SetFloat("WINSIZE_X", WINSIZE_X);
			this->postEffect->SetFloat("WINSIZE_Y", WINSIZE_Y);
		}
	}
	float pixelU = 1.0f / WINSIZE_X;
	float pixelV = 1.0f / WINSIZE_Y;
	this->postEffect->SetFloat("pixelSizeU", pixelU);
	this->postEffect->SetFloat("pixelSizeV", pixelV);
	this->postEffect->SetFloat("pixelHalfSizeU", pixelU * 0.5f);
	this->postEffect->SetFloat("pixelHalfSizeV", pixelV * 0.5f);


	UINT iPass;
	this->postEffect->Begin(&iPass, 0);
	for (UINT i = 0; i < iPass; i++) {
		this->postEffect->BeginPass(i);

		Device->SetFVF(SCENE_VERTEX::FVF);
		Device->DrawIndexedPrimitiveUP(
			D3DPT_TRIANGLELIST,
			0,
			4,
			2,
			this->scenePlaneIndex,
			D3DFMT_INDEX16,
			this->scenePlaneVertex,
			sizeof(SCENE_VERTEX));


		this->postEffect->EndPass();
	}
	this->postEffect->End();


	//���ö���Ʈ ����
	SPRITE_MGR->BeginSpriteRender();
	this->Scene_RenderSprite();
	SPRITE_MGR->EndSpriteRender();
}

void cScene::ReadyShadowMap(std::vector<cBaseObject*>* renderObjects, cTerrain* pTerrain)
{
	//���⼺������ ���� ī�޶��� Frustum ������Ʈ
	this->pDirectionLightCamera->UpdateMatrix();
	this->pDirectionLightCamera->UpdateFrustum();


	//���̷��Ƕ����� ī�޶� ������ �ֵ鸸 �׸���...
	static std::vector<cBaseObject*>		shadowCullObject;
	shadowCullObject.clear();

	for (int i = 0; i < renderObjects->size(); i++) {

		//�������� �ȿ� �ִ�?
		if (this->pDirectionLightCamera->Frustum.IsInFrustum((*renderObjects)[i]))
		{
			shadowCullObject.push_back((*renderObjects)[i]);
		}
	}


	//������ �� �׸���.
	this->pDirectionLightCamera->RenderTextureBegin(0xffffffff);

	cXMesh_Static::SetCamera(this->pDirectionLightCamera);
	cXMesh_Static::SetTechniqueName("CreateShadow");

	cXMesh_Skinned::SetCamera(this->pDirectionLightCamera);
	cXMesh_Skinned::SetTechniqueName("CreateShadow");



	for (int i = 0; i < shadowCullObject.size(); i++) {
		if (shadowCullObject[i]->IgnoreCreateShadow == false)
			shadowCullObject[i]->Render();
	}

	//���� Terrain �� ������ ���� �׷����Ѵٸ�...
	if (pTerrain != NULL)
	{
		pTerrain->RenderShadow(this->pDirectionLightCamera);
	}


	this->pDirectionLightCamera->RenderTextureEnd();


	//���� Terrain �� ������ ���� �����Ѵٸ�...
	if (pTerrain != NULL)
	{
		pTerrain->m_pTerrainEffect->SetTexture("Shadow_Tex",
			this->pDirectionLightCamera->GetRenderTexture());

		pTerrain->m_pTerrainEffect->SetMatrix("matLightViewProjection",
			&this->pDirectionLightCamera->GetViewProjectionMatrix());
	}



	//������ Texture
	cXMesh_Static::sStaticMeshEffect->SetTexture("Shadow_Tex",
		this->pDirectionLightCamera->GetRenderTexture());

	//������ ���
	cXMesh_Static::sStaticMeshEffect->SetMatrix("matLightViewProjection",
		&this->pDirectionLightCamera->GetViewProjectionMatrix());


	//������ Texture
	cXMesh_Skinned::sSkinnedMeshEffect->SetTexture("Shadow_Tex",
		this->pDirectionLightCamera->GetRenderTexture());

	//������ ���
	cXMesh_Skinned::sSkinnedMeshEffect->SetMatrix("matLightViewProjection",
		&this->pDirectionLightCamera->GetViewProjectionMatrix());
}



//���� ī�޶��� RenderToTexture �� ������Ʈ�Ѵ�.
void cScene::RenderToMainCamTexture()
{
	this->pMainCamera->RenderTextureBegin(0x00101010);

	//�����ȴ�.
	this->Scene_Render0();
	this->Scene_Render1();
	this->Scene_Render2();


#ifdef _DEBUG		//����� ��忡���� ����
	//����̽� ������ ���� ���
	//���� �׸���
	//GIZMO_MGR->WorldGrid(1, 10);
#endif

	this->pMainCamera->RenderTextureEnd();

}

//����ī�޶��� ���� Texture �� ��´�.
LPDIRECT3DTEXTURE9 cScene::GetTexture()
{
	return this->pMainCamera->GetRenderTexture();
}