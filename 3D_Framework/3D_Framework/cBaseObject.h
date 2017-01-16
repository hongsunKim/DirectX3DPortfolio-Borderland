#pragma once

class cXMesh;
class cTransform;
class cSkinnedAnimation;
#include "cCamera.h"
#include "cLight_Direction.h"

class cBaseObject
{
public:
	cTransform*			pTransform;
	cXMesh*				pMesh;
	cBoundBox			BoundBox;
	bool				IgnoreCreateShadow;			//������� ���鶧 ���õǴ�
	string				Obj_Name;
	bool				Bound_Active;
	cCamera*			cam;
	cLight_Direction*	Light_D;

public:
	bool				bActive;	//Ȱ��ȭ ����
	cSkinnedAnimation*	pSkinned;	//��Ų�� Animtion
	int               E1;
	int               E2;
	int               E3;
public:
	cBaseObject();
	~cBaseObject();

	void Update(float timeDelta);

	void Render(bool check = false) {
		Bound_Active = check;
		if (this->bActive)
			this->BaseObjectRender();
	}

	//������ �� �޽��� �����Ѵ�.
	void SetMesh(cXMesh*	pMesh);

	//Ȱ��ȭ ����
	void SetActive(bool bActive);

	//Ȱ��ȭ ���θ� ��´�.
	bool IsActive() {
		return this->bActive;
	}

	void SetTransform(cTransform* Transform);


	//���õ� �޽��� ���� �ٿ��ڽ��� ���� �Ѵ�.
	void ComputeBoundBox();

	void BaseObjectLight(cCamera* camt, cLight_Direction* Light_Dt);
protected:

	//override �ؼ� ���ÿ�....
	virtual void BaseObjectEnable() {}							//BaseObject �� Ȱ��ȭ �ɶ� ����
	virtual void BaseObjectDisable(){}							//BaseObject �� ��Ȱ��ȭ �ɶ� ����
	virtual void BaseObjectUpdate(float timeDelta) {}			//BaseObject �� Update �� ����....
	virtual void BaseObjectNoActiveUpdate(float timeDelte){}	//BaseObject �� ��Ȱ��ȭ�� ������Ʈ ����....

	virtual void BaseObjectRender();							//BaseObject �� �׸��� ����
};

