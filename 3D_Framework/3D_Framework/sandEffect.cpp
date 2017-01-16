#include "stdafx.h"
#include "sandEffect.h"
#include "cCamera.h"
#include "cParticleEmitter.h"

sandEffect::sandEffect()
{
}


sandEffect::~sandEffect()
{
}

HRESULT sandEffect::Scene_Init()
{
	Sand_Particle = new cParticleEmitter();
	Sand_Particle->SetActive(true);

	//�迭�� 2 ���̻� 
	VEC_COLOR colors;
	colors.push_back(D3DXCOLOR(1.0f, 0.6f, 0.3f, 1.0f));
	colors.push_back(D3DXCOLOR(1.0f, 0.6f, 0.3f, 1.0f));

	VEC_SCALE scales;
	scales.push_back(0.1f);
	scales.push_back(0.1f);


	LPDIRECT3DTEXTURE9 pTex = RESOURCE_TEXTURE->GetResource(
		"../../Resources/Testures/particle_0.png");

	//��ƼŬ ������ ����
	Sand_Particle->Init(
		5000,				//�ִ� ��ƼŬ ��
		3000.0f,				//�ʴ� ��ƼŬ �߻� ��
		7,					//�ϳ��� ��ƼŬ ���� ������ �ּҰ�
		10,					//�ϳ��� ��ƼŬ ���� ������ �ִ밪
		D3DXVECTOR3(7, -4.0f, 0),	//��ƼŬ ���� �ӵ� �ּҰ� ( ���ñ��� )
		D3DXVECTOR3(10, -6.0f, 0),	//��ƼŬ ���� �ӵ� �ִ밪 ( ���ñ��� )
		D3DXVECTOR3(7, 3, 1),	//��ƼŬ ���� ���� �ּҰ� ( ���ñ��� )
		D3DXVECTOR3(10, 5, 1), //��ƼŬ ���� ���� �ִ밪 ( ���ñ��� )
		colors,				//�÷� �迭
		scales,				//������ �迭
		0.3f,				//����ũ�� �ּҰ�
		0.5f,				//�ִ밪
		pTex,				//�ؽ���
		false);



	
//	Sand_Particle->EmissionType = PATICLE_EMISSION_TYPE::BOX;
//
//	Sand_Particle->MinEmissionRangeX = -10.0f;
//	Sand_Particle->MaxEmissionRangeX = 100.0f;
//
//	Sand_Particle->MinEmissionRangeY = 10.0f;
//	Sand_Particle->MaxEmissionRangeY = 50;
//
//	Sand_Particle->MinEmissionRangeZ = -10.0f;
//	Sand_Particle->MaxEmissionRangeZ = 10;
	

	Sand_Particle->EmissionType = PATICLE_EMISSION_TYPE::SPHERE;
	Sand_Particle->SphereEmissionRange = 10.0f;


	Sand_Particle->StartEmission();

	Sand_Particle->pTransform->SetLocalPosition(0, 5, 0);

	return S_OK;
}

void sandEffect::Scene_Release()
{
	Sand_Particle->Release();
	SAFE_DELETE(Sand_Particle);
}

void sandEffect::Scene_Update(float timDelta)
{
	Sand_Particle->Update(timDelta);
//	Sand_Particle->pTransform->DefaultControl2(timDelta);
}

void sandEffect::Scene_Render1()
{
	Sand_Particle->Render();
	//Sand_Particle->pTransform->RenderGimozo();
}

void sandEffect::Scene_RenderSprite()
{
}
