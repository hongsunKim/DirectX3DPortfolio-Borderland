#include "stdafx.h"
#include "GameOver.h"


GameOver::GameOver()
{
}


GameOver::~GameOver()
{
}

HRESULT GameOver::Scene_Init()
{
	this->GameOverScene = RESOURCE_TEXTURE->GetResource("../../Resources/Testures/Player_Etc/start.tga");
	this->GameOverScene_Back = RESOURCE_TEXTURE->GetResource("../../Resources/Testures/Player_Etc/gameover.jpg");

	//폰트 객체 만든다
	D3DXCreateFont(
		Device,				//디바이스
		200.f,				//폰트의 높이
		0.f,				//폰트의 폭(이 값이 0이면 앞에서 설정한 폰트 높이에 비례)
		FW_BOLD,			//폰트의 두께 설정(FW_BOLD 폰트의 BOLD 스타일 크기를 갖는다)
		1,					//밉맵(그냥 1)
		TRUE,				//이텔릭 여부
		DEFAULT_CHARSET,	//캐릭터 set( 그냥 DEFAULT_CHARSET )
		OUT_DEFAULT_PRECIS,	//외각 처리방식 ( 그냥 OUT_DEFAULT_PRECIS )
		DEFAULT_QUALITY,	//퀄리티 ( 그냥 DEFAULT_QUALITY )
		DEFAULT_PITCH | FF_DONTCARE,// 글씨체 패밀리셋 설정
		"Cambria",			//사용될 폰트 이름 
		&Font			//생성된 폰트객체 받을 더블 포인터
	);
	SOUNDMANAGER->stop("BossMap");
	return S_OK;
}

void GameOver::Scene_Release()
{
}

void GameOver::Scene_Update(float timDelta)
{
	if (KEY_MGR->IsOnceDown(VK_RETURN)) { SCENE_MGR->ChangeScene("스타트씬", 1); }
}

void GameOver::Scene_Render1()
{
}

void GameOver::Scene_RenderSprite()
{
	RECT Go_rc;
	SetRect(&Go_rc, 0, 0, WINSIZE_X, WINSIZE_Y);
	SPRITE_MGR->DrawTexture(
		this->GameOverScene_Back,
		&Go_rc,
		0, 0,
		2.5, 3.15,
		0,
		0xffffffff,
		&D3DXVECTOR3(0, 0, 0));

	RECT rcText2 = {  WINSIZE_X / 2 -450, WINSIZE_Y / 2 - 100,  WINSIZE_X / 2, WINSIZE_Y / 2 };

	Font->DrawText(
		NULL,				//스프라이트에 그리는 경우 스프라이트 객체
		"Game Over",			//들어갈 문자열
		-1,					//출력 문자열 갯수 -1이면 다 찍어라
		&rcText2,			//출력 영역
		DT_LEFT | DT_NOCLIP,//왼쪽정렬, DT_NOCLIP하면 영역을 무시하고 다 출력
		0xffffffff			//문자열 색상
	);
}
