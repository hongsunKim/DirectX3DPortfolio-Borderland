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

	//배열을 2 개이상 
	VEC_COLOR colors;
	colors.push_back(D3DXCOLOR(1.0f, 0.6f, 0.3f, 1.0f));
	colors.push_back(D3DXCOLOR(1.0f, 0.6f, 0.3f, 1.0f));

	VEC_SCALE scales;
	scales.push_back(0.1f);
	scales.push_back(0.1f);


	LPDIRECT3DTEXTURE9 pTex = RESOURCE_TEXTURE->GetResource(
		"../../Resources/Testures/particle_0.png");

	//파티클 랜더러 셋팅
	Sand_Particle->Init(
		5000,				//최대 파티클 수
		3000.0f,				//초당 파티클 발생 량
		7,					//하나의 파티클 입자 라이프 최소값
		10,					//하나의 파티클 입자 라이프 최대값
		D3DXVECTOR3(7, -4.0f, 0),	//파티클 입자 속도 최소값 ( 로컬기준 )
		D3DXVECTOR3(10, -6.0f, 0),	//파티클 입자 속도 최대값 ( 로컬기준 )
		D3DXVECTOR3(7, 3, 1),	//파티클 입자 가속 최소값 ( 로컬기준 )
		D3DXVECTOR3(10, 5, 1), //파티클 입자 가속 최대값 ( 로컬기준 )
		colors,				//컬러 배열
		scales,				//스케일 배열
		0.3f,				//입자크기 최소값
		0.5f,				//최대값
		pTex,				//텍스쳐
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
