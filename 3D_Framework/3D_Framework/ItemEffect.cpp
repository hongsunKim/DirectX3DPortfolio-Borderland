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

	//��ƼŬ ������ ����
	Item_Particle->Init(
		500,            //�ִ� ��ƼŬ ��
		150.0f,            //�ʴ� ��ƼŬ �߻� ��
		1,               //�ϳ��� ��ƼŬ ���� ������ �ּҰ�
		1,               //�ϳ��� ��ƼŬ ���� ������ �ִ밪
		D3DXVECTOR3(0, 0, 0),   //��ƼŬ ���� �ӵ� �ּҰ� ( ���ñ��� )
		D3DXVECTOR3(0, 1, 0),   //��ƼŬ ���� �ӵ� �ִ밪 ( ���ñ��� )
		D3DXVECTOR3(0, 0, 0),   //��ƼŬ ���� ���� �ּҰ� ( ���ñ��� )
		D3DXVECTOR3(0, 1, 0), //��ƼŬ ���� ���� �ִ밪 ( ���ñ��� )
		colors,            //�÷� �迭
		scales,            //������ �迭
		1.1f,            //����ũ�� �ּҰ�
		1.1f,            //�ִ밪
		pTex,            //�ؽ���
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