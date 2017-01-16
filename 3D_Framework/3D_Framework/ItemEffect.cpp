#include "stdafx.h"
#include "ItemEffect.h"
#include "cCamera.h"
#include "cParticleEmitter.h"

ItemEffect::ItemEffect()
{
}


ItemEffect::~ItemEffect()
{
}


HRESULT ItemEffect::Scene_Init(int Item_Type)
{
	Item_Particle = new cParticleEmitter();
	Item_Particle->SetActive(true);

	VEC_COLOR colors;
	if (Item_Type == 0) {
		colors.push_back(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		colors.push_back(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		colors.push_back(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		colors.push_back(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		colors.push_back(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		colors.push_back(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	}
	else if (Item_Type == 1) {
		colors.push_back(D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f));
		colors.push_back(D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f));
		colors.push_back(D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f));
		colors.push_back(D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f));
		colors.push_back(D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f));
		colors.push_back(D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f));
	}


	VEC_SCALE scales;
	scales.push_back(0.1f);
	scales.push_back(0.1f);
	scales.push_back(0.1f);
	scales.push_back(0.1f);
	scales.push_back(0.1f);

	LPDIRECT3DTEXTURE9 pTex = RESOURCE_TEXTURE->GetResource(
		"../../Resources/Testures/FireExplosionBlurred.png");

	//파티클 랜더러 셋팅
	Item_Particle->Init(
		500,            //최대 파티클 수
		150.0f,            //초당 파티클 발생 량
		1,               //하나의 파티클 입자 라이프 최소값
		1,               //하나의 파티클 입자 라이프 최대값
		D3DXVECTOR3(0, 0, 0),   //파티클 입자 속도 최소값 ( 로컬기준 )
		D3DXVECTOR3(0, 1, 0),   //파티클 입자 속도 최대값 ( 로컬기준 )
		D3DXVECTOR3(0, 0, 0),   //파티클 입자 가속 최소값 ( 로컬기준 )
		D3DXVECTOR3(0, 1, 0), //파티클 입자 가속 최대값 ( 로컬기준 )
		colors,            //컬러 배열
		scales,            //스케일 배열
		1.1f,            //입자크기 최소값
		1.1f,            //최대값
		pTex,            //텍스쳐
		true);

	return S_OK;
}
void ItemEffect::Scene_Release()
{
	Item_Particle->Release();
	SAFE_DELETE(Item_Particle);
}
void ItemEffect::Scene_Update(float timeDelta)
{
	Item_Particle->Update(timeDelta);
	Item_Particle->StartEmission();
}
void ItemEffect::Scene_Render1()
{
	Item_Particle->Render();
}
void ItemEffect::Scene_RenderSprite()
{

}
void ItemEffect::SetPos(D3DXVECTOR3* setpos)
{
	Item_Particle->pTransform->SetWorldPosition(*setpos);
}