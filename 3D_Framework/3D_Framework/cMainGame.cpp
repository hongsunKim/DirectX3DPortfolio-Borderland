#include "StdAfx.h"
#include "cMainGame.h"
#include "cImage.h"

#include "cStart_Scene.h"
#include "cScene_1.h"
#include "cScene_2.h"
#include "GameOver.h"
#include "Ending.h"

cMainGame::cMainGame( void )
{
}

cMainGame::~cMainGame( void )
{
}

//����
HRESULT cMainGame::Init( void )
{
	//�����õ�
	SetRandomSeed();

	//����̽� ����
	CreateDevice();

	//�Ŵ��� ����
	TIME_MGR->Init();
	DXFONT_MGR->Init(Device);
	GIZMO_MGR->Init(Device);
	SPRITE_MGR->Init(Device);
	SCENE_MGR->Init();
	SOUNDMANAGER->init();
	DXFONT_MGR->addStyle(Device, "������", "���긲", 30.f);

	//���ӿ� ���Ǵ� �� �߰�
	SCENE_MGR->AddScene("��ŸƮ��", new cStart_Scene());
	SCENE_MGR->AddScene("��1", new cScene_1());
	SCENE_MGR->AddScene("��2", new cScene_2());
	SCENE_MGR->AddScene("���ӿ���", new GameOver());
	SCENE_MGR->AddScene("����", new Ending());

	//���� ���۾�
	SCENE_MGR->ChangeScene("��ŸƮ��");

	return S_OK;
}

//����
void cMainGame::Release()
{

	//������
	m_pNowScene->Release();
	SAFE_DELETE( m_pNowScene );

	//�Ŵ��� ����
	TIME_MGR->Release();
	cTimeMgr::ReleaseInstance();
	KEY_MGR->Release();
	cKeyMgr::ReleaseInstance();
	GIZMO_MGR->Release();
	cGizmoManager::ReleaseInstance();
	DXFONT_MGR->Release();
	cDXFontManager::ReleaseInstance();
	SPRITE_MGR->Release();
	cSpriteManager::ReleaseInstance();
	SCENE_MGR->Release();
	cScene_Mgr::ReleaseInstance();


	cPhysicManager::ReleaseInstance();

	RESOURCE_TEXTURE->ClearResource();
	cResourceMgr_Texture::ReleaseInstance();
	RESOURCE_FX->ClearResource();
	cResourceMgr_ShaderFX::ReleaseInstance();
	RESOURCE_STATICXMESH->ClearResource();
	cResourceMgr_XStaticMesh::ReleaseInstance();
	RESOURCE_SKINNEDXMESH->ClearResource();
	cResourceMgr_XSkinnedMesh::ReleaseInstance();
	//���� ����, �̱��� ����
	SOUNDMANAGER->release();
	soundManager::ReleaseInstance();
	//����̽� ����
	ReleaseDevice();
}

//������Ʈ
void cMainGame::Update()
{
	//Ÿ�ӸŴ��� ������Ʈ
	TIME_MGR->UpdateTime( 60.0f );

	if (KEY_MGR->IsOnceDown(VK_F2)) (Camera_Check) ? Camera_Check = false : Camera_Check = true;
	if (KEY_MGR->IsOnceDown(VK_F9)) (Text_Check) ? Text_Check = false : Text_Check = true;
	if (KEY_MGR->IsOnceDown(VK_F11)) (Bound_Check) ? Bound_Check = false : Bound_Check = true;
	if (KEY_MGR->IsOnceDown(VK_F12)) (EnemySphere_Check) ? EnemySphere_Check = false : EnemySphere_Check = true;
	
	//�������� ���� �ð�
	double timeDelta = TIME_MGR->GetFrameDeltaSec();

	//��������Ʈ
	SCENE_MGR->Update(timeDelta);

	SOUNDMANAGER->update();
}

//��ο�
void cMainGame::Draw()
{
	//���۸� û���Ѵ�
	HRESULT result = Device->Clear(
		0,							//û���� ������ D3DRECT �迭 ����		( ��ü Ŭ���� 0 )
		NULL,						//û���� ������ D3DRECT �迭 ������		( ��ü Ŭ���� NULL )
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,		//û�ҵ� ���� �÷��� ( D3DCLEAR_TARGET �÷�����, D3DCLEAR_ZBUFFER ���̹���, D3DCLEAR_STENCIL ���ٽǹ���
		0xff000000,					//�÷����۸� û���ϰ� ä���� ����( 0xAARRGGBB )
		1.0f,						//���̹��۸� û���Ұ� ( 0 ~ 1, 0 �� ī�޶󿡼� ���ϰ���� 1 �� ī�޶󿡼� ���� �� )
		0							//���ٽ� ���۸� ä�ﰪ
	);

	//���� û�Ұ� �����ߴٸ�.......
	if ( SUCCEEDED( result ) )
	{
		//����̽� ������ ���� ���
		Device->BeginScene();

		//������ ���

		//������
		SCENE_MGR->Render();

#ifdef _DEBUG	//����� ��忡���� ����
		//����̽� ������ ���� ���
		//���� �׸���
		//GIZMO_MGR->WorldGrid(1, 10);

		//Ÿ������ ���
		//TIME_MGR->DrawTimeInfo();
#endif

		//����̽� ������ ���� ���
		Device->EndScene();

		//�������� ���۳����� ȭ�鿡 ����
		Device->Present( 0, 0, 0, 0 );
	}

}



//////////////////////////////////////////////////////////////////////////

//���� ���ν��� �Լ�
LRESULT cMainGame::MainProc( HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam )
{
	switch ( iMessage )
	{
	case WM_TIMER:
		Update();
		break;

	case WM_KEYDOWN:
		switch ( wParam )
		{
		case VK_ESCAPE:
			DestroyWindow( hWnd );
			break;
		}
		break;
	case WM_DESTROY:			//�����찡 �ı��ȴٸ�..
		PostQuitMessage( 0 );	//���α׷� ���� ��û ( �޽��� ������ ���������� �ȴ� )
		break;

	case WM_SETCURSOR:
		//SetCursor(NULL);
		//return true;
		ShowCursor(FALSE);
		break;
	}
	return DefWindowProc( hWnd, iMessage, wParam, lParam );
}

