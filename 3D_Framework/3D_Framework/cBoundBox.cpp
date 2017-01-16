#include "stdafx.h"
#include "cBoundBox.h"
#include "cTransform.h"

cBoundBox::cBoundBox() : Air_Blocking_Check(true) , Warp_Check(false)
{
}
cBoundBox::~cBoundBox()
{
}
void cBoundBox::GetWorldBox(const cTransform* cTrans, D3DXVECTOR3* outBoxPos)
{
	//   5-------6
	//  /|      /|
	// 1-------2 |
	// | 4-----|-7
	// |/      |/
	// 0-------3 

	//로컬 8 개의 정점을 구한다
	D3DXVECTOR3 Vertices[8];
	Vertices[0] = D3DXVECTOR3(this->localMinPos.x, this->localMinPos.y, this->localMinPos.z);
	Vertices[1] = D3DXVECTOR3(this->localMinPos.x, this->localMaxPos.y, this->localMinPos.z);
	Vertices[2] = D3DXVECTOR3(this->localMaxPos.x, this->localMaxPos.y, this->localMinPos.z);
	Vertices[3] = D3DXVECTOR3(this->localMaxPos.x, this->localMinPos.y, this->localMinPos.z);
	Vertices[4] = D3DXVECTOR3(this->localMinPos.x, this->localMinPos.y, this->localMaxPos.z);
	Vertices[5] = D3DXVECTOR3(this->localMinPos.x, this->localMaxPos.y, this->localMaxPos.z);
	Vertices[6] = D3DXVECTOR3(this->localMaxPos.x, this->localMaxPos.y, this->localMaxPos.z);
	Vertices[7] = D3DXVECTOR3(this->localMaxPos.x, this->localMinPos.y, this->localMaxPos.z);

	D3DXMATRIXA16 matFinal = cTrans->GetFinalMatrix();
	for (int i = 0; i < 8; i++)
	{
		D3DXVec3TransformCoord(outBoxPos + i, Vertices + i, &matFinal);
	}

}

void cBoundBox::GetWorldAABBMinMax(const cTransform* cTrans, D3DXVECTOR3* min, D3DXVECTOR3* max)
{
	//변환된 박스의 월듸 위치 8개를 얻는다.
	D3DXVECTOR3 worldPos[8];
	GetWorldBox(cTrans, worldPos);

	//얻은 8개의 최대 최소를 구하자
	*min = worldPos[0];
	*max = worldPos[0];

	for (int i = 1; i < 8; i++){

		if (min->x > worldPos[i].x) {
			min->x = worldPos[i].x;
		}
		else if (max->x < worldPos[i].x){
			max->x = worldPos[i].x;
		}

		if (min->y > worldPos[i].y){
			min->y = worldPos[i].y;
		}
		else if (max->y < worldPos[i].y){
			max->y = worldPos[i].y;
		}

		if (min->z > worldPos[i].z){
			min->z = worldPos[i].z;
		}
		else if (max->z < worldPos[i].z){
			max->z = worldPos[i].z;
		}
	}
}

//월드 단위로 그려준다.
void cBoundBox::RenderGizmo(const cTransform* trans)
{
	//변환된 박스의 월드위치 8개 얻는다.
	D3DXVECTOR3 worldPos[8];
	GetWorldBox(trans, worldPos);

	//   5-------6
	//  /|      /|
	// 1-------2 |
	// | 4-----|-7
	// |/      |/
	// 0-------3 
	//OBB 박스 그린다

	if (Air_Blocking_Check && !Warp_Check) BoundRender(worldPos, 0xffff0000);
	else if (!Air_Blocking_Check && Warp_Check) BoundRender(worldPos, 0xffffff00);
	else BoundRender(worldPos, 0xffffffff);

	D3DXVECTOR3 min;
	D3DXVECTOR3 max;

	//얻은 8 개의 최대 최소를 구하자.
	min = worldPos[0];
	max = worldPos[0];

	for (int i = 1; i < 8; i++){

		if (min.x > worldPos[i].x){
			min.x = worldPos[i].x;
		}
		else if (max.x < worldPos[i].x){
			max.x = worldPos[i].x;
		}

		if (min.y > worldPos[i].y){
			min.y = worldPos[i].y;
		}
		else if (max.y < worldPos[i].y){
			max.y = worldPos[i].y;
		}

		if (min.z > worldPos[i].z){
			min.z = worldPos[i].z;
		}
		else if (max.z < worldPos[i].z){
			max.z = worldPos[i].z;
		}
	}

	//AABB 그린다.
	//GIZMO_MGR->AABBBox(min, max, 0xff008800);
}
void cBoundBox::BoundRender(D3DXVECTOR3* Pos,DWORD color)
{
	GIZMO_MGR->Line(Pos[0], Pos[1], color);
	GIZMO_MGR->Line(Pos[1], Pos[2], color);
	GIZMO_MGR->Line(Pos[2], Pos[3], color);
	GIZMO_MGR->Line(Pos[3], Pos[0], color);

	GIZMO_MGR->Line(Pos[4], Pos[5], color);
	GIZMO_MGR->Line(Pos[5], Pos[6], color);
	GIZMO_MGR->Line(Pos[6], Pos[7], color);
	GIZMO_MGR->Line(Pos[7], Pos[4], color);

	GIZMO_MGR->Line(Pos[0], Pos[4], color);
	GIZMO_MGR->Line(Pos[1], Pos[5], color);
	GIZMO_MGR->Line(Pos[2], Pos[6], color);
	GIZMO_MGR->Line(Pos[3], Pos[7], color);
}

void cBoundBox::SetBound(const D3DXVECTOR3* pCenter, const D3DXVECTOR3* pHalfSize)
{
	cBoundSphere::SetBound(pCenter, pHalfSize);

	this->localMinPos = this->localCenter - this->halfSize;
	this->localMaxPos = this->localCenter + this->halfSize;
}
