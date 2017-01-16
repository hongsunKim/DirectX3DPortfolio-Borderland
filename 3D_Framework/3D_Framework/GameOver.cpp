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

	//��Ʈ ��ü �����
	D3DXCreateFont(
		Device,				//����̽�
		200.f,				//��Ʈ�� ����
		0.f,				//��Ʈ�� ��(�� ���� 0�̸� �տ��� ������ ��Ʈ ���̿� ���)
		FW_BOLD,			//��Ʈ�� �β� ����(FW_BOLD ��Ʈ�� BOLD ��Ÿ�� ũ�⸦ ���´�)
		1,					//�Ӹ�(�׳� 1)
		TRUE,				//���ڸ� ����
		DEFAULT_CHARSET,	//ĳ���� set( �׳� DEFAULT_CHARSET )
		OUT_DEFAULT_PRECIS,	//�ܰ� ó����� ( �׳� OUT_DEFAULT_PRECIS )
		DEFAULT_QUALITY,	//����Ƽ ( �׳� DEFAULT_QUALITY )
		DEFAULT_PITCH | FF_DONTCARE,// �۾�ü �йи��� ����
		"Cambria",			//���� ��Ʈ �̸� 
		&Font			//������ ��Ʈ��ü ���� ���� ������
	);
	SOUNDMANAGER->stop("BossMap");
	return S_OK;
}

void GameOver::Scene_Release()
{
}

void GameOver::Scene_Update(float timDelta)
{
	if (KEY_MGR->IsOnceDown(VK_RETURN)) { SCENE_MGR->ChangeScene("��ŸƮ��", 1); }
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
		NULL,				//��������Ʈ�� �׸��� ��� ��������Ʈ ��ü
		"Game Over",			//�� ���ڿ�
		-1,					//��� ���ڿ� ���� -1�̸� �� ����
		&rcText2,			//��� ����
		DT_LEFT | DT_NOCLIP,//��������, DT_NOCLIP�ϸ� ������ �����ϰ� �� ���
		0xffffffff			//���ڿ� ����
	);
}
