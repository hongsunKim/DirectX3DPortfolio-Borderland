#pragma once

class cQuadParticleEmitter;
class cParticleEmitter;

class Effect
{
private:
	cQuadParticleEmitter* BloodEffect;
	cParticleEmitter* PoisonEff;
	cQuadParticleEmitter* PoisonEffx;
	cParticleEmitter*	 GrenadeFx;
public:
	float time = 0;

	Effect();
	~Effect();

	virtual HRESULT Scene_Init();

	virtual void Scene_Release();

	virtual void Scene_Update(float timeDelta);

	virtual void Scene_Render1();

	void BloodEffectRender();

	void PoisonEffectRender();

	void PoisonEffxRender();

	void GrenadeFxRender();

	virtual void Scene_RenderSprite();

	void SetBloodLocation(D3DXVECTOR3 o);

	void SetGrenadeLocation(D3DXVECTOR3 o);

	void SetPoisonLocation(D3DXVECTOR3 o);
};
