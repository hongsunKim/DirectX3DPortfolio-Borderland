#pragma once
#include "cSingletonBase.h"
#include "lib/inc/fmod.hpp"		//fomd.hpp �߰�
#pragma comment(lib, "lib/fmodex_vc.lib")	//lib ��ũ
using namespace FMOD;			//FMOD ���ӽ����̽� ���

//ä�ι���, �������
#define EXTRACHANNELBUFFER 5
#define SOUNDBUFFER 20
//�ѻ��� ���ۼ���: ���۰� �۾Ƽ� ������ ������ �ذ����ֱ� ����
#define TOTALSOUNDBUFFER SOUNDBUFFER + EXTRACHANNELBUFFER

class soundManager : public cSingletonBase <soundManager>
{
private:
	typedef map<string, Sound**> arrSounds;
	typedef map<string, Sound**>::iterator arrSoundIter;

private:
	System* _system;		//fmod�� �ý��� Ŭ����
	Sound** _sound;			//fmod�� ���� Ŭ����
	Channel** _channel;		//fmod�� ä�� Ŭ����

	arrSounds _mTotalSounds;//�ʿ� ��� ���� ���

public:
	HRESULT init(void);
	void release(void);
	void update(void);
	void render(void);

	//���� �߰�(Ű��, �����̸�, BGM?, ������?)
	void addSound(string keyName, string soundName, bool bgm = FALSE, bool loop = FALSE);

	//���� �÷���(Ű��, ����) 0.0f~ 1.0f
	void play(string keyName, float volume = 1.0f);
	//���� ����
	void stop(string keyName);
	//���� �Ͻ�����
	void pause(string keyName);
	//���� �ٽ� ���
	void resume(string keyName);

	//�÷��� ���̳�?
	bool isPlaySound(string keyName);
	//�Ͻ����� ���̳�?
	bool isPauseSound(string keyName);

	soundManager();
	~soundManager();
};

#define SOUNDMANAGER soundManager::GetInstance()
