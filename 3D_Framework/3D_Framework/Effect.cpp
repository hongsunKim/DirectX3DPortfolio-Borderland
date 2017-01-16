#include "stdafx.h"
#include "Effect.h"
#include "cQuadParticleEmitter.h"
#include "cCamera.h"
#include "cParticleEmitter.h"
Effect::Effect()
{
}


Effect::~Effect()
{
}

HRESULT Effect::Scene_Init()
{
	BloodEffect = new cQuadParticleEmitter();

	BloodEffect->SetActive(true);

	VEC_COLOR colors;
	colors.push_back(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
	colors.push_back(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	colors.push_back(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));

	VEC_SCALE scales;
	scales.push_back(1.5f);
	scales.push_back(1.5f);

	BloodEffect->Init(
		10,
		3.0f,
		1.5f,
		1.0f,
		D3DXVECTOR3(0, 0.5, 0),
		D3DXVECTOR3(0, 1, 0),
		D3DXVECTOR3(0, 0, 0),
		D3DXVECTOR3(0, 0, 0),
		D3DXVECTOR3(0, 0, 0),		//�ʱ���۽� ȸ��Min
		D3DXVECTOR3(0, 0, 0),     //�ʱ���۽� ȸ��Max
		D3DXVECTOR3(0, 0, 0),				//�ʴ� ȸ���� ȸ�� �� Min
		D3DXVECTOR3(0, 0, 0),				//�ʴ� ȸ���� ȭ�� �� Max
		D3DXVECTOR3(0, 0, 0),				//�ʴ� ȸ�� ���� Min
		D3DXVECTOR3(0, 0, 0),				//�ʴ� ȸ�� ���� Max
		colors, scales,
		2.0f, 2.0f,
		RESOURCE_TEXTURE->GetResource("../../Resources/Testures/Effect/tex_bloodSplater_256_3.tga"),
		true);

	BloodEffect->StartEmission();
	BloodEffect->pTransform->SetLocalPosition(0, 5, 0);
	//ī�޶� ����
	//	BloodEffect->SetCameraSort(this->pMainCamera, true);
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	PoisonEff = new  cParticleEmitter();

	PoisonEff->SetActive(true);



	//�迭�� 2 ���̻�
	VEC_COLOR Poison_colors;
	Poison_colors.push_back(D3DXCOLOR(0.3f, 1.0f, 0.3f, 1.0f));
	Poison_colors.push_back(D3DXCOLOR(0.0f, 0.2f, 0.0f, 1.0f));
	Poison_colors.push_back(D3DXCOLOR(0.0f, 0.2f, 0.0f, 1.0f));
	Poison_colors.push_back(D3DXCOLOR(0.0f, 0.2f, 0.0f, 1.0f));
	Poison_colors.push_back(D3DXCOLOR(0.0f, 0.2f, 0.0f, 1.0f));
	Poison_colors.push_back(D3DXCOLOR(0.0f, 0.2f, 0.0f, 1.0f));

	VEC_SCALE Poison_scales;
	Poison_scales.push_back(1.0f);
	Poison_scales.push_back(3.0f);
	Poison_scales.push_back(3.5f);
	Poison_scales.push_back(4.0f);
	Poison_scales.push_back(4.1f);

	LPDIRECT3DTEXTURE9 PpTex = RESOURCE_TEXTURE->GetResource(
		"../../Resources/Testures/Paticle_Snow.tga");

	//��ƼŬ ������ ����
	PoisonEff->Init(
		1000,				//�ִ� ��ƼŬ ��
		500.0f,				//�ʴ� ��ƼŬ �߻� ��
		3,					//�ϳ��� ��ƼŬ ���� ������ �ּҰ�
		5,					//�ϳ��� ��ƼŬ ���� ������ �ִ밪
		D3DXVECTOR3(0, 0, 0),	//��ƼŬ ���� �ӵ� �ּҰ� ( ���ñ��� )
		D3DXVECTOR3(0, 0, 0),	//��ƼŬ ���� �ӵ� �ִ밪 ( ���ñ��� )
		D3DXVECTOR3(1, 1, 0),	//��ƼŬ ���� ���� �ּҰ� ( ���ñ��� )
		D3DXVECTOR3(-1, 0, 0), //��ƼŬ ���� ���� �ִ밪 ( ���ñ��� )
		Poison_colors,				//�÷� �迭
		Poison_scales,				//������ �迭
		0.1f,				//����ũ�� �ּҰ�
		0.2f,				//�ִ밪
		PpTex,				//�ؽ���
		false);
	/////////////////////////////////////////////////////////////////////////////////////
	PoisonEffx = new cQuadParticleEmitter();

	PoisonEffx->SetActive(true);

	VEC_COLOR FXcolors;
	FXcolors.push_back(D3DXCOLOR(0.5f, 0.0f, 0.0f, 0.0f));
	FXcolors.push_back(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
	FXcolors.push_back(D3DXCOLOR(0.0f, 0.0f, 0.5f, 0.0f));

	VEC_SCALE FXscales;
	FXscales.push_back(1.0f);
	FXscales.push_back(2.0f);


	//��ƼŬ ������ ����
	PoisonEffx->Init(
		100,
		10.0f,
		0.5f,
		1.0f,
		D3DXVECTOR3(-1, 1, -1),
		D3DXVECTOR3(1, 2, 1),
		D3DXVECTOR3(0, 0, 0),
		D3DXVECTOR3(0, 0, 0),
		D3DXVECTOR3(0, 0, 0),		//�ʱ���۽� ȸ��Min
		D3DXVECTOR3(0, 0, 0),     //�ʱ���۽� ȸ��Max
		D3DXVECTOR3(0, 0, 0),				//�ʴ� ȸ���� ȸ�� �� Min
		D3DXVECTOR3(0, 0, 0),				//�ʴ� ȸ���� ȭ�� �� Max
		D3DXVECTOR3(0, 0, 0),				//�ʴ� ȸ�� ���� Min
		D3DXVECTOR3(0, 0, 0),				//�ʴ� ȸ�� ���� Max
		FXcolors, FXscales,
		1.0f, 1.0f,
		RESOURCE_TEXTURE->GetResource("../../Resources/Testures/T_Explosion_Flash.tga"),
		true);

	///////////////////////////////////////////////////////////////////////////////////////////
	GrenadeFx = new cParticleEmitter();

	GrenadeFx->SetActive(true);

	VEC_COLOR Gcolors;
	Gcolors.push_back(D3DXCOLOR(1.0f, 0.6f, 0.2f, 1.0f));
	Gcolors.push_back(D3DXCOLOR(1.0f, 0.6f, 0.2f, 1.0f));
	Gcolors.push_back(D3DXCOLOR(1.0f, 0.6f, 0.2f, 1.0f));
	Gcolors.push_back(D3DXCOLOR(1.0f, 0.6f, 0.2f, 1.0f));
	Gcolors.push_back(D3DXCOLOR(1.0f, 0.6f, 0.2f, 1.0f));
	Gcolors.push_back(D3DXCOLOR(1.0f, 0.6f, 0.2f, 1.0f));

	VEC_SCALE Gscales;
	Gscales.push_back(0.5f);
	Gscales.push_back(2.5f);
	Gscales.push_back(5.0f);
	Gscales.push_back(7.5f);
	Gscales.push_back(10.5f);

	LPDIRECT3DTEXTURE9 GpTex = RESOURCE_TEXTURE->GetResource(
		"../../Resources/Testures/T_Explosion_Flash.tga");

	//��ƼŬ ������ ����
	GrenadeFx->Init(
		500,				//�ִ� ��ƼŬ ��
		200.0f,				//�ʴ� ��ƼŬ �߻� ��
		3,					//�ϳ��� ��ƼŬ ���� ������ �ּҰ�
		5,					//�ϳ��� ��ƼŬ ���� ������ �ִ밪
		D3DXVECTOR3(0, 0, 0),	//��ƼŬ ���� �ӵ� �ּҰ� ( ���ñ��� )
		D3DXVECTOR3(0, 0, 0),	//��ƼŬ ���� �ӵ� �ִ밪 ( ���ñ��� )
		D3DXVECTOR3(0, 0, 0),	//��ƼŬ ���� ���� �ּҰ� ( ���ñ��� )
		D3DXVECTOR3(0, 1, 0), //��ƼŬ ���� ���� �ִ밪 ( ���ñ��� )
		Gcolors,				//�÷� �迭
		Gscales,				//������ �迭
		1.1f,				//��sss��ũ�� �ּҰ�
		2.2f,				//�ִ밪
		GpTex,				//�ؽ���
		false);


	return S_OK;
}

void Effect::Scene_Release()
{
	BloodEffect->Release();
	SAFE_DELETE(BloodEffect);

	PoisonEff->Release();
	SAFE_DELETE(PoisonEff);

	PoisonEffx->Release();
	SAFE_DELETE(PoisonEffx);

	GrenadeFx->Release();
	SAFE_DELETE(GrenadeFx);
}

void Effect::Scene_Update(float timeDelta)
{
	BloodEffect->Update(timeDelta);
	//	BloodEffect->pTransform->DefaultControl2(timeDelta);
	BloodEffect->StartEmission();
	PoisonEffx->Update(timeDelta);
	PoisonEffx->StartEmission();
	PoisonEff->Update(timeDelta);
	PoisonEff->Burst(100, 1.4f, 2.7f, 0.1f, 0.5f);
	GrenadeFx->Update(timeDelta);
	GrenadeFx->Burst(100, 1.4f, 2.7f, 0.1, 0.5);
	time += timeDelta;
}

void Effect::Scene_Render1()
{

}

void Effect::BloodEffectRender()
{
	BloodEffect->Render();
	//	BloodEffect->pTransform->RenderGimozo(Device);
}

void Effect::PoisonEffectRender()
{
	PoisonEff->Render();
}

void Effect::PoisonEffxRender()
{
	PoisonEffx->StartEmission();
	PoisonEffx->Render();
}

void Effect::GrenadeFxRender()
{
	GrenadeFx->Render();
}


void Effect::Scene_RenderSprite()
{
}

void Effect::SetBloodLocation(D3DXVECTOR3 o)
{
	BloodEffect->pTransform->SetLocalPosition(o);
}

void Effect::SetGrenadeLocation(D3DXVECTOR3 o)
{
	GrenadeFx->pTransform->SetLocalPosition(o + D3DXVECTOR3(0, 0.2, 0));
}

void Effect::SetPoisonLocation(D3DXVECTOR3 o)
{
	PoisonEff->pTransform->SetWorldPosition(o);
	PoisonEffx->pTransform->SetWorldPosition(o);
}
