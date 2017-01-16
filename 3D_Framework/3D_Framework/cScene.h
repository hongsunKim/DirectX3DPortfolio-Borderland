#pragma once

class cCamera;
class cLight_Direction;
class cTerrain;

class cScene
{
private:
	typedef struct SCENE_VERTEX{
		D3DXVECTOR3 pos;
		D3DXVECTOR2 uv;
		enum { FVF = D3DFVF_XYZ | D3DFVF_TEX1 };
	}SCENE_VERTEX, *LPSCENE_VETEX;

protected:
	cCamera*						pMainCamera;

	SCENE_VERTEX					scenePlaneVertex[4];		//씬플랜 버텍스
	WORD							scenePlaneIndex[6];			//인덱스

	LPD3DXEFFECT					postEffect;					//PostEffect

	cLight_Direction*				pSceneBaseDirectionLight;	//씬에 배치된 기본라이팅
	cCamera*						pDirectionLightCamera;		//방향성 광원에 따른 카메라...

	float							shadowDistance;				//그림자 거리

	float							m_player_fov;

	cCamera*						Player_Eye;
	cCamera*						Main_Eye;

protected:

	int								m_iSceneNum = 0;

public:

	cScene( void );
	virtual ~cScene( void );

	HRESULT Init();
	void Release();
	void Update(float timeDelta);
	void Render();

	void ReadyShadowMap(std::vector<cBaseObject*>* renderObjects, cTerrain* pTerrain = NULL);

	//
	//추가
	//

	//메인 카메라의 RenderToTexture 만 업데이트한다.
	void RenderToMainCamTexture();

	//메인카메라의 랜더 Texture 를 얻는다.
	LPDIRECT3DTEXTURE9 GetTexture();

private:

	//씬에서 호출될 함수들을 추상함수로...
	virtual HRESULT Scene_Init() = 0;
	virtual void Scene_Release() = 0;
	virtual void Scene_Update(float timDelta) = 0;

	virtual void Scene_Render0(){}
	virtual void Scene_Render1() = 0;
	virtual void Scene_Render2(){}

	virtual void Scene_RenderSprite(){}

};

