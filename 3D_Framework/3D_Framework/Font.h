#pragma once

#include "cPlayer.h"

class Font
{
private:

	LPD3DXFONT pDxFont; //Direct3D ÆùÆ® °´Ã¼
	LPD3DXFONT DamegeFont;
	UIINFO		_UIINFO;
	cPlayer		player;
	char str[1024];

public:
	Font();
	~Font();

	virtual void Init();
	virtual void Release();
	virtual void Update(float timeDelta);
	virtual void Render();

	virtual void HpFont(float x, float y);
	virtual void shieldFont(float x, float y);
	virtual void BulletFont(float x, float y);
	virtual void EnemyFont(char* EnemyName, float x, float y);
	virtual void DamageFont(bool isCritical,float Damege,float FontSize, float x, float y);
	void SetUiInfo(UIINFO& info) { _UIINFO = info; }

};

