#include "stdafx.h"
#include "cBaseObject.h"
#include "cTransform.h"
#include "cSkinnedAnimation.h"
#include "cCamera.h"
#include "cLight_Direction.h"

cBaseObject::cBaseObject()
	: bActive(false), pMesh(NULL), pSkinned(NULL), Bound_Active(false)
{
	this->pTransform = new cTransform();
	IgnoreCreateShadow = false;
	cam = new cCamera;
	Light_D = new cLight_Direction;
	E1 = 0;
	E2 = 0;
	E3 = 0;
}

cBaseObject::~cBaseObject()
{
	SAFE_DELETE(this->pTransform);
	if (this->pSkinned != NULL)
	{
		this->pSkinned->Release();
		SAFE_DELETE(this->pSkinned);
	}
}

void cBaseObject::Update(float timeDelta)
{
	if (this->bActive)
	{
		this->BaseObjectUpdate(timeDelta);

		if (this->pSkinned != NULL)
			this->pSkinned->Update(timeDelta);
	}
	else
		this->BaseObjectNoActiveUpdate(timeDelta);
}

//활성화 셋팅
void cBaseObject::SetActive(bool bActive)
{
	//활성화 되어있는 상태에서 비활성화 셋팅시
	if (bActive == false && this->bActive)
	{
		this->bActive = false;
		this->BaseObjectDisable();
	}

	//비활성화 되어있는 상태에서 활성화 셋팅시
	else if (bActive && this->bActive == false)
	{
		this->bActive = true;
		this->BaseObjectEnable();
	}
}

void cBaseObject::SetTransform(cTransform * Transform)
{
	this->pTransform = Transform;
}

//셋팅된 메쉬에 따라 바운드박스를 재계산 한다.
void cBaseObject::ComputeBoundBox()
{
	if (this->pMesh != NULL)
	{
		//셋팅된 메쉬가 cXMesh_Static 라면...
		cXMesh_Static* pStaticMesh = dynamic_cast< cXMesh_Static* >(this->pMesh);
		if (pStaticMesh != NULL)
		{
			this->BoundBox.localCenter = pStaticMesh->Bound_Center;
			this->BoundBox.halfSize = pStaticMesh->Bound_HalfSize;
			this->BoundBox.localMinPos = pStaticMesh->Bound_Min;
			this->BoundBox.localMaxPos = pStaticMesh->Bound_Max;
			this->BoundBox.radius = pStaticMesh->Bound_Radius;
		}

		//셋팅된 메쉬가 cXMesh_Skinned 라면... ( 임시로 바운드 박스 )
		else
		{
			this->BoundBox.SetBound(&D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(10.0f, 10.0f, 10.0f));
		}
	}
}
void cBaseObject::BaseObjectRender()
{
	if (this->pMesh != NULL)
	{
		if (this->pSkinned)
			this->pSkinned->Render(this->pTransform);

		else
			this->pMesh->Render(this->pTransform);
	}

	if (this->Bound_Active)
		this->BoundBox.RenderGizmo(this->pTransform);
}

//랜더링 될 메쉬를 셋팅한다.
void  cBaseObject::SetMesh(cXMesh*	pMesh)
{
	cXMesh_Skinned* pSkin = dynamic_cast<cXMesh_Skinned*>(pMesh);
	this->pMesh = pMesh;
	this->ComputeBoundBox();

	//StaticMesh 라면...
	if (pSkin == NULL)
	{
		if (this->pSkinned != NULL)
		{
			this->pSkinned->Release();
			SAFE_DELETE(this->pSkinned);
		}

		pSkinned = NULL;
	}


	//SkinnedMesh 라면...
	else
	{
		this->pSkinned = new cSkinnedAnimation();
		this->pSkinned->Init(pSkin);
	}

}

void cBaseObject::BaseObjectLight(cCamera* camt, cLight_Direction* Light_Dt)
{
	Light_D = Light_Dt;
	cam = camt;
}