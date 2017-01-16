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

	//��Ʈ ��ü �����
	D3DXCreateFont(
		Device,				//����̽�
		30.f,				//��Ʈ�� ����
		0.f,				//��Ʈ�� ��(�� ���� 0�̸� �տ��� ������ ��Ʈ ���̿� ���)
		FW_BOLD,			//��Ʈ�� �β� ����(FW_BOLD ��Ʈ�� BOLD ��Ÿ�� ũ�⸦ ���´�)
		1,					//�Ӹ�(�׳� 1)
		FALSE,				//���ڸ� ����
		DEFAULT_CHARSET,	//ĳ���� set( �׳� DEFAULT_CHARSET )
		OUT_DEFAULT_PRECIS,	//�ܰ� ó����� ( �׳� OUT_DEFAULT_PRECIS )
		DEFAULT_QUALITY,	//����Ƽ ( �׳� DEFAULT_QUALITY )
		DEFAULT_PITCH | FF_DONTCARE,// �۾�ü �йи��� ����
		"Cambria",			//���� ��Ʈ �̸� 
		&pDxFont			//������ ��Ʈ��ü ���� ���� ������
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

	//��Ʈ ��ü �����
	D3DXCreateFont(
		Device,				//����̽�
		FontSize,				//��Ʈ�� ����
		0.f,				//��Ʈ�� ��(�� ���� 0�̸� �տ��� ������ ��Ʈ ���̿� ���)
		FW_BOLD,			//��Ʈ�� �β� ����(FW_BOLD ��Ʈ�� BOLD ��Ÿ�� ũ�⸦ ���´�)
		1,					//�Ӹ�(�׳� 1)
		FALSE,				//���ڸ� ����
		DEFAULT_CHARSET,	//ĳ���� set( �׳� DEFAULT_CHARSET )
		OUT_DEFAULT_PRECIS,	//�ܰ� ó����� ( �׳� OUT_DEFAULT_PRECIS )
		DEFAULT_QUALITY,	//����Ƽ ( �׳� DEFAULT_QUALITY )
		DEFAULT_PITCH | FF_DONTCARE,// �۾�ü �йи��� ����
		"Cambria",			//���� ��Ʈ �̸� 
		&DamegeFont			//������ ��Ʈ��ü ���� ���� ������
	);


	if (isCritical == true)
	{
		sprintf_s(str, "CRITICAL", (int)Damege);
		DXFONT_MGR->PrintText(str, x, y - 50, 0xffff0000);
	}
		sprintf_s(str, "%d", (int)Damege);
		DXFONT_MGR->PrintText(str, x, y, 0xff000000);


}
