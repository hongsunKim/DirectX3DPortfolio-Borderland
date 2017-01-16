#include "stdafx.h"
#include "Font.h"


Font::Font()
{
}


Font::~Font()
{
}

void Font::Init()
{

	//폰트 객체 만든다
	D3DXCreateFont(
		Device,				//디바이스
		30.f,				//폰트의 높이
		0.f,				//폰트의 폭(이 값이 0이면 앞에서 설정한 폰트 높이에 비례)
		FW_BOLD,			//폰트의 두께 설정(FW_BOLD 폰트의 BOLD 스타일 크기를 갖는다)
		1,					//밉맵(그냥 1)
		FALSE,				//이텔릭 여부
		DEFAULT_CHARSET,	//캐릭터 set( 그냥 DEFAULT_CHARSET )
		OUT_DEFAULT_PRECIS,	//외각 처리방식 ( 그냥 OUT_DEFAULT_PRECIS )
		DEFAULT_QUALITY,	//퀄리티 ( 그냥 DEFAULT_QUALITY )
		DEFAULT_PITCH | FF_DONTCARE,// 글씨체 패밀리셋 설정
		"Cambria",			//사용될 폰트 이름 
		&pDxFont			//생성된 폰트객체 받을 더블 포인터
	);

	
}

void Font::Release()
{
	SAFE_RELEASE(pDxFont);
}

void Font::Update(float timeDelta)
{
	
}

void Font::Render()
{
}

void Font::HpFont(float x,float y)
{
	sprintf_s(str,"%d / %d", (int)_UIINFO.CurHp, (int)_UIINFO.FullHp);
	DXFONT_MGR->PrintText(str,x,y,0xffffffff);
}

void Font::shieldFont(float x, float y)
{
	sprintf_s(str, "%d / %d", (int)_UIINFO.CurShield, (int)_UIINFO.FullShield);
	DXFONT_MGR->PrintText(str, x, y, 0xffffffff);
}

void Font::BulletFont(float x, float y)
{
	sprintf_s(str, "%d / %d      Total[%d]", (int)_UIINFO.Bullet_Current, (int)_UIINFO.Bullet_Reload_Max, (int)_UIINFO.Bullet_Total_Max);
	DXFONT_MGR->PrintText(str, x, y, 0xffffffff);
}

void Font::EnemyFont(char* EnemyName, float x, float y)
{
	char str1[1024];
	sprintf_s(str1, "%d", EnemyName);
	DXFONT_MGR->PrintText(str1, x, y, 0xff000000);
}

void Font::DamageFont(bool isCritical,float Damege, float FontSize, float x, float y)
{
	if (FontSize < 30)
	{
		FontSize = 30;
	}
	else if (FontSize > 10)
	{
		FontSize = 10;
	}

	//폰트 객체 만든다
	D3DXCreateFont(
		Device,				//디바이스
		FontSize,				//폰트의 높이
		0.f,				//폰트의 폭(이 값이 0이면 앞에서 설정한 폰트 높이에 비례)
		FW_BOLD,			//폰트의 두께 설정(FW_BOLD 폰트의 BOLD 스타일 크기를 갖는다)
		1,					//밉맵(그냥 1)
		FALSE,				//이텔릭 여부
		DEFAULT_CHARSET,	//캐릭터 set( 그냥 DEFAULT_CHARSET )
		OUT_DEFAULT_PRECIS,	//외각 처리방식 ( 그냥 OUT_DEFAULT_PRECIS )
		DEFAULT_QUALITY,	//퀄리티 ( 그냥 DEFAULT_QUALITY )
		DEFAULT_PITCH | FF_DONTCARE,// 글씨체 패밀리셋 설정
		"Cambria",			//사용될 폰트 이름 
		&DamegeFont			//생성된 폰트객체 받을 더블 포인터
	);


	if (isCritical == true)
	{
		sprintf_s(str, "CRITICAL", (int)Damege);
		DXFONT_MGR->PrintText(str, x, y - 50, 0xffff0000);
	}
		sprintf_s(str, "%d", (int)Damege);
		DXFONT_MGR->PrintText(str, x, y, 0xff000000);


}
