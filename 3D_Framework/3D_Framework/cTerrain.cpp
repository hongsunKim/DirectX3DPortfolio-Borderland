#include "stdafx.h"
#include "cTerrain.h"
#include "cQuadTree.h"
#include "cCamera.h"
#include "cSetBoundObject.h"
#include "cLight_Direction.h"

cTerrain::cTerrain()
{
}


cTerrain::~cTerrain()
{
}

void cTerrain::makeQuadTreeSphere(LPQUADTREESPHERE quadTree, D3DXVECTOR3 centerPos, float VerNumX, float VerNumZ, float minX, float minZ, float maxX, float maxZ)
{

	//	D3DXVECTOR3 centerPos(startPos.x+ VerNumX,0, startPos.z+ VerNumZ);
	float divideX = abs(VerNumX / 2);
	float divideZ = abs(VerNumZ / 2);


	quadTree->sphere[0].centerPos = D3DXVECTOR3(centerPos.x - divideX, 0, centerPos.z + divideZ);
	quadTree->sphere[1].centerPos = D3DXVECTOR3(centerPos.x + divideX, 0, centerPos.z + divideZ);
	quadTree->sphere[2].centerPos = D3DXVECTOR3(centerPos.x - divideX, 0, centerPos.z - divideZ);
	quadTree->sphere[3].centerPos = D3DXVECTOR3(centerPos.x + divideX, 0, centerPos.z - divideZ);

	//	quadTree->_0.centerPos = D3DXVECTOR3(centerPos.x - divideX, 0, centerPos.z - divideZ);
	//	quadTree->_1.centerPos = D3DXVECTOR3(centerPos.x + divideX, 0, centerPos.z - divideZ);
	//	quadTree->_2.centerPos = D3DXVECTOR3(centerPos.x - divideX, 0, centerPos.z + divideZ);
	//	quadTree->_3.centerPos = D3DXVECTOR3(centerPos.x + divideX, 0, centerPos.z + divideZ);
	float temp = VerNumX*VerNumX + VerNumZ*VerNumZ;
	quadTree->sphere[0].radius = sqrt(temp);
	quadTree->sphere[1].radius = sqrt(temp);
	quadTree->sphere[2].radius = sqrt(temp);
	quadTree->sphere[3].radius = sqrt(temp);

	//	quadTree->minX=
	float tampX_1 = minX;
	float tampX_2 = (maxX - minX) / 2 + minX;
	float tampZ_1 = minZ;
	float tampZ_2 = (maxZ - minZ) / 2 + minZ;
	float midX = (maxX - minX) / 2;
	float midZ = (maxZ - minZ) / 2;

	quadTree->MinMax[0].minX = tampX_1;
	quadTree->MinMax[0].minZ = tampZ_1;
	quadTree->MinMax[0].maxX = tampX_2;
	quadTree->MinMax[0].maxZ = tampZ_2;

	quadTree->MinMax[1].minX = tampX_2;
	quadTree->MinMax[1].minZ = tampZ_1;
	quadTree->MinMax[1].maxX = tampX_2 + midX;
	quadTree->MinMax[1].maxZ = tampZ_1 + midZ;

	quadTree->MinMax[2].minX = tampX_1;
	quadTree->MinMax[2].minZ = tampZ_2;
	quadTree->MinMax[2].maxX = tampX_1 + midX;
	quadTree->MinMax[2].maxZ = tampZ_2 + midZ;

	quadTree->MinMax[3].minX = tampX_2;
	quadTree->MinMax[3].minZ = tampZ_2;
	quadTree->MinMax[3].maxX = tampX_2 + midX;
	quadTree->MinMax[3].maxZ = tampZ_2 + midZ;

	for (int i = 0; i < 4; ++i) {
		if (quadTree->MinMax[i].minX < 0)
			quadTree->MinMax[i].minX = 0;
		if (quadTree->MinMax[i].minZ < 0)
			quadTree->MinMax[i].minZ = 0;
		if (quadTree->MinMax[i].maxX > m_nCellNumX)
			quadTree->MinMax[i].maxX = m_nCellNumX - 1;
		if (quadTree->MinMax[i].maxZ > m_nCellNumZ)
			quadTree->MinMax[i].maxZ = m_nCellNumZ - 1;
	}
	if (divideX > 3) {

		QUADTREESPHERE temp[4];

		makeQuadTreeSphere(&temp[0], quadTree->sphere[0].centerPos, divideX, divideZ, tampX_1, tampZ_1, tampX_2, tampZ_2);
		makeQuadTreeSphere(&temp[1], quadTree->sphere[1].centerPos, divideX, divideZ, tampX_2, tampZ_1, tampX_2 + midX, tampZ_1 + midZ);
		makeQuadTreeSphere(&temp[2], quadTree->sphere[2].centerPos, divideX, divideZ, tampX_1, tampZ_2, tampX_1 + midX, tampZ_2 + midZ);
		makeQuadTreeSphere(&temp[3], quadTree->sphere[3].centerPos, divideX, divideZ, tampX_2, tampZ_2, tampX_2 + midX, tampZ_2 + midZ);

		quadTree->vec_0.push_back(temp[0]);
		quadTree->vec_0.push_back(temp[1]);
		quadTree->vec_0.push_back(temp[2]);
		quadTree->vec_0.push_back(temp[3]);
	}

	return;
}

void cTerrain::renderQuadTreeMap(LPQUADTREESPHERE quadTree)
{


	terrainCullingIb = NULL;
	(Device->CreateIndexBuffer(
		sizeof(TERRAINTRI) * this->m_nTotalTri,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX32,
		D3DPOOL_DEFAULT,
		&this->terrainCullingIb,
		NULL));

	TERRAINTRI index;
	LPTERRAINTRI lpIndex;
	if (SUCCEEDED(this->terrainCullingIb->Lock(0, 0, (void**)&lpIndex, 0)))
	{
		nCullingTotalTri = 0;
		while (!vecCulling.empty()) {
			int minZ = vecCulling.front().minZ;
			int maxZ = vecCulling.front().maxZ;
			int minX = vecCulling.front().minX;
			int maxX = vecCulling.front().maxX;
			vecCulling.pop();


			for (int z = minZ; z <= maxZ; z++)
			{
				for (int x = minX; x <= maxX; x++)
				{

					// lt-----rt
					//  |    /|
					//  |   / |
					//  |  /  |
					//  | /   |
					//  |/    |
					// lb-----rb

					//모서리 정점 인덱스
					DWORD lt = z * m_nVerNumX + x;
					DWORD rt = z * m_nVerNumX + x + 1;
					DWORD lb = ((z + 1)* m_nVerNumX) + x;
					DWORD rb = ((z + 1)* m_nVerNumX) + x + 1;



					//Cell 폴리곤 1
					//lb, lt, rt, 
					index.dw0 = lb;
					index.dw1 = lt;
					index.dw2 = rt;
					*lpIndex++ = index;
					++nCullingTotalTri;

					//Cell 폴리곤 1
					//lb, rt, rb, 
					index.dw0 = lb;
					index.dw1 = rt;
					index.dw2 = rb;
					*lpIndex++ = index;
					++nCullingTotalTri;

				}
			}
			int a = 0;
		}
		this->terrainCullingIb->Unlock();
	}
	//	vecCulling.clear();
	//	vecCulling.reserve(100);
}

void cTerrain::renderQuadTreeSphere(LPQUADTREESPHERE quadTree)
{

	//	GIZMO_MGR->WireSphere(quadTree->sphere[0].centerPos, quadTree->sphere[0].radius);
	//	GIZMO_MGR->WireSphere(quadTree->sphere[1].centerPos, quadTree->sphere[1].radius);
	//	GIZMO_MGR->WireSphere(quadTree->sphere[2].centerPos, quadTree->sphere[2].radius);
	//	GIZMO_MGR->WireSphere(quadTree->sphere[3].centerPos, quadTree->sphere[3].radius);
	if (IsIn(&quadTree->sphere[0].centerPos, quadTree->sphere[0].radius, 0, quadTree)) {
		if (quadTree->vec_0.size() != 0)
			renderQuadTreeSphere(&quadTree->vec_0[0]);
	}

	if (IsIn(&quadTree->sphere[1].centerPos, quadTree->sphere[1].radius, 1, quadTree)) {
		if (quadTree->vec_0.size() != 0)
			renderQuadTreeSphere(&quadTree->vec_0[1]);
	}

	if (IsIn(&quadTree->sphere[2].centerPos, quadTree->sphere[2].radius, 2, quadTree)) {
		if (quadTree->vec_0.size() != 0)
			renderQuadTreeSphere(&quadTree->vec_0[2]);
	}

	if (IsIn(&quadTree->sphere[3].centerPos, quadTree->sphere[3].radius, 3, quadTree)) {
		if (quadTree->vec_0.size() != 0)
			renderQuadTreeSphere(&quadTree->vec_0[3]);
	}

	//		if (quadTree->vec_0.size() != 0) {
	//			renderQuadTreeSphere(&quadTree->vec_0[0]);
	//			renderQuadTreeSphere(&quadTree->vec_0[1]);
	//			renderQuadTreeSphere(&quadTree->vec_0[2]);
	//			renderQuadTreeSphere(&quadTree->vec_0[3]);
	//		}
	return;

}


void cTerrain::astarPop()
{
	while (mapAster.getAsterMove()->size())
		mapAster.getAsterMove()->pop_back();

	mapAster.returnStartDest();
}

int cTerrain::getAstarSize()
{
	return mapAster.getAsterMove()->size();
}


bool cTerrain::setAstar(D3DXVECTOR3 start, D3DXVECTOR3 dest)
{
	if (!mapAster.setStartDest(start, dest))
		return false;
	mapAster._asterMinMax.maxX = m_nVerNumX;
	mapAster._asterMinMax.maxZ = m_nVerNumZ;
	mapAster._asterMinMax.minX = 0;
	mapAster._asterMinMax.minZ = 0;
	return mapAster.FindPath();
}

BOOL cTerrain::IsIn(D3DXVECTOR3* pv, float radius, int n, LPQUADTREESPHERE quadTree)
{
	float fDist;
	// int i;

	// 현재는 left, right, far plane만 적용한다.
	// for( i = 0 ; i < 6 ; i++ )
	fDist = D3DXPlaneDotCoord(&planeCulling.planeNear, pv);
	if (fDist > radius) return false; // plane의 normal벡터가 far로 향하고 있으므로 양수이면 프러스텀의 바깥쪽
	fDist = D3DXPlaneDotCoord(&planeCulling.planeFar, pv);
	if (fDist > radius) return false; // plane의 normal벡터가 far로 향하고 있으므로 양수이면 프러스텀의 바깥쪽
	fDist = D3DXPlaneDotCoord(&planeCulling.planeLeft, pv);
	if (fDist > radius) return false; // plane의 normal벡터가 left로 향하고 있으므로 양수이면 프러스텀의 왼쪽
	fDist = D3DXPlaneDotCoord(&planeCulling.planeRight, pv);
	if (fDist > radius) return false; // plane의 normal벡터가 right로 향하고 있으므로 양수이면 프러스텀의 오른쪽

	if (radius < 6) {
		//		GIZMO_MGR->WireSphere(*pv, radius);
		MinMax temp;
		temp.minX = quadTree->MinMax[n].minX;
		temp.minZ = quadTree->MinMax[n].minZ;
		temp.maxX = quadTree->MinMax[n].maxX;
		temp.maxZ = quadTree->MinMax[n].maxZ;
		vecCulling.push(temp);
	}

	return true;
}

//초기화 

HRESULT cTerrain::Init_Load(string Load_Terrain_str)
{
	Load_Terrain_Text.clear();
	Map_Load(Load_Terrain_str.c_str());
	//스케일값 대입
	m_fHeightScale = 100;
	m_fCellScale = 1;

	int sqrt_num = sqrt(Load_Terrain.size());
	//가로세로 정점 수를 구한다.
	m_nVerNumX = sqrt_num;		//가로 정점갯수는 높이 맵에 가로 해상도 + 1 과 같다. ( 이유는 쿼드트리쓰려면 정점갯수가 2의N승 + 1 이여야 하기 때문에 )
	m_nVerNumZ = sqrt_num;		//세로 정점잿수는 높이 맵에 세로 해상도 + 1 과 같다. ( 이유는 쿼드트리쓰려면 정점갯수가 2의N승 + 1 이여야 하기 때문에 )
	m_nTotalVerNum = m_nVerNumX * m_nVerNumZ;		//총 정점 갯수


													//가로세로 셀수
	m_nCellNumX = m_nVerNumX - 1;
	m_nCellNumZ = m_nVerNumZ - 1;
	m_nTotalCellNum = m_nCellNumX * m_nCellNumZ;
	vertex_Index.resize(m_nTotalVerNum);

	//총 삼각형수는
	m_nTotalTri = m_nTotalCellNum * 2;

	m_pHeightMap = NULL;

	//터레인을 만든다.
	if (FAILED(CreateTerrain(
	)))
	{
		Release();
		return E_FAIL;
	}

	//터레인 크기
	m_fTerrainSizeX = m_nCellNumX * m_fCellScale;
	m_fTerrainSizeZ = m_nCellNumZ * m_fCellScale;

	//터레인 범위
	m_fTerrainStartX = m_pTerrainVertices[0].pos.x;
	m_fTerrainStartZ = m_pTerrainVertices[0].pos.z;
	m_fTerrainEndX = m_pTerrainVertices[m_nTotalVerNum - 1].pos.x;
	m_fTerrainEndZ = m_pTerrainVertices[m_nTotalVerNum - 1].pos.z;

	//쿼드트리를 만든다.
	m_pQuadTree = new cQuadTree;
	m_pQuadTree->Init(m_pTerrainVertices, m_nVerNumX);


	//터레인 Texture 로딩
	m_pTexTile_0 = RESOURCE_TEXTURE->GetResource(Load_Terrain_Text[0]);
	m_pTexTile_1 = RESOURCE_TEXTURE->GetResource(Load_Terrain_Text[1]);
	m_pTexTile_2 = RESOURCE_TEXTURE->GetResource(Load_Terrain_Text[2]);
	m_pTexTile_3 = RESOURCE_TEXTURE->GetResource(Load_Terrain_Text[3]);
	m_pTexSlat = RESOURCE_TEXTURE->GetResource(Load_Terrain_Text[4]);

	//Terrain Effect 로딩
	m_pTerrainEffect = RESOURCE_FX->GetResource("../../Resources/Shaders/TerrainBase.fx");

	//============================================================================
	nCullingTotalTri = 0;
	float startPosX = this->m_fTerrainSizeX / 2;
	float startPosZ = this->m_fTerrainSizeZ / 2;
	QuadTreeSphereRender = false;
	QuadTreeMapRender = false;
	makeQuadTreeSphere(&mapQuadTreeSphere, D3DXVECTOR3(0, 0, 0), startPosX, startPosZ, 0, 0, this->m_nCellNumX, this->m_nCellNumZ);
	terrainCulling.vec[0] = D3DXVECTOR3(-1.f, -1.f, 0.f);
	terrainCulling.vec[1] = D3DXVECTOR3(1.f, -1.f, 0.f);
	terrainCulling.vec[2] = D3DXVECTOR3(-1.f, 1.f, 0.f);
	terrainCulling.vec[3] = D3DXVECTOR3(1.f, 1.f, 0.f);
	terrainCulling.vec[4] = D3DXVECTOR3(-1.f, -1.f, 1.f);
	terrainCulling.vec[5] = D3DXVECTOR3(1.f, -1.f, 1.f);
	terrainCulling.vec[6] = D3DXVECTOR3(-1.f, 1.f, 1.f);
	terrainCulling.vec[7] = D3DXVECTOR3(1.f, 1.f, 1.f);

	D3DXPlaneFromPoints(&planeCulling.planeNear, &terrainCulling.vec[0], &terrainCulling.vec[2], &terrainCulling.vec[3]);
	D3DXPlaneFromPoints(&planeCulling.planeFar, &terrainCulling.vec[7], &terrainCulling.vec[6], &terrainCulling.vec[4]);
	D3DXPlaneFromPoints(&planeCulling.planeTop, &terrainCulling.vec[2], &terrainCulling.vec[6], &terrainCulling.vec[7]);
	D3DXPlaneFromPoints(&planeCulling.planeBotton, &terrainCulling.vec[5], &terrainCulling.vec[4], &terrainCulling.vec[0]);
	D3DXPlaneFromPoints(&planeCulling.planeRight, &terrainCulling.vec[3], &terrainCulling.vec[7], &terrainCulling.vec[5]);
	D3DXPlaneFromPoints(&planeCulling.planeLeft, &terrainCulling.vec[4], &terrainCulling.vec[6], &terrainCulling.vec[2]);

}
//해제
void cTerrain::Release()
{
	SAFE_RELEASE(m_pTerrainVB);
	SAFE_RELEASE(m_pTerrainIB);
	SAFE_RELEASE(m_pTerrainDecl);
	SAFE_DELETE_ARR(m_pTerrainVertices);
	SAFE_DELETE(m_pQuadTree);
}
//랜더
void cTerrain::Render(cCamera* pCam, cLight_Direction* pDirectionLight)
{
	//월드 행렬셋팅
	D3DXMATRIXA16 matInd;
	D3DXMatrixIdentity(&matInd);
	m_pTerrainEffect->SetMatrix("matWorld", &matInd);

	//뷰 행렬셋팅
	m_pTerrainEffect->SetMatrix("matViewProjection", &pCam->GetViewProjectionMatrix());

	//Texture 셋팅
	m_pTerrainEffect->SetTexture("Terrain0_Tex", m_pTexTile_0);
	m_pTerrainEffect->SetTexture("Terrain1_Tex", m_pTexTile_1);
	m_pTerrainEffect->SetTexture("Terrain2_Tex", m_pTexTile_2);
	m_pTerrainEffect->SetTexture("Terrain3_Tex", m_pTexTile_3);
	m_pTerrainEffect->SetTexture("TerrainControl_Tex", m_pTexSlat);

	//광원 셋팅
	D3DXVECTOR3 dirLight = pDirectionLight->pTransform->GetForward();
	m_pTerrainEffect->SetVector("worldLightDir", &D3DXVECTOR4(dirLight, 1));

	//추가
	m_pTerrainEffect->SetTechnique("Base");

	UINT passNum = 0;
	m_pTerrainEffect->Begin(&passNum, 0);

	for (UINT i = 0; i < passNum; i++) {
		m_pTerrainEffect->BeginPass(i);
		Device->SetStreamSource(0, m_pTerrainVB, 0, sizeof(TERRAINVERTEX));
		Device->SetVertexDeclaration(this->m_pTerrainDecl);
		Device->SetIndices(m_pTerrainIB);
		Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_nTotalVerNum, 0, m_nTotalTri);

		m_pTerrainEffect->EndPass();

	}
	m_pTerrainEffect->End();

}

//추가
void cTerrain::Render(cCamera* pCam, cLight_Direction* pDirectionLight, cCamera* pDirectionLightCamera)
{
	//ShadowMap 셋팅
	m_pTerrainEffect->SetTexture("Shadow_Tex", pDirectionLightCamera->GetRenderTexture());

	//ShadowViewProjection 셋팅 
	m_pTerrainEffect->SetMatrix("matLightViewProjection", &pDirectionLightCamera->GetViewProjectionMatrix());

	//월드 행렬셋팅
	D3DXMATRIXA16 matInd;
	D3DXMatrixIdentity(&matInd);
	m_pTerrainEffect->SetMatrix("matWorld", &matInd);

	//뷰 행렬셋팅
	m_pTerrainEffect->SetMatrix("matViewProjection", &pCam->GetViewProjectionMatrix());

	//Texture 셋팅
	m_pTerrainEffect->SetTexture("Terrain0_Tex", m_pTexTile_0);
	m_pTerrainEffect->SetTexture("Terrain1_Tex", m_pTexTile_1);
	m_pTerrainEffect->SetTexture("Terrain2_Tex", m_pTexTile_2);
	m_pTerrainEffect->SetTexture("Terrain3_Tex", m_pTexTile_3);
	m_pTerrainEffect->SetTexture("TerrainControl_Tex", m_pTexSlat);

	//광원 셋팅
	D3DXVECTOR3 dirLight = pDirectionLight->pTransform->GetForward();
	m_pTerrainEffect->SetVector("worldLightDir", &D3DXVECTOR4(dirLight, 1));


	//광원 셋팅
	//m_pTerrainEffect->SetVector( "worldLightDir", &D3DXVECTOR4( dirLight, 1 ) );


	m_pTerrainEffect->SetTechnique("ReciveShadow");

	UINT passNum = 0;
	m_pTerrainEffect->Begin(&passNum, 0);

	terrainCulling.vec[0] = D3DXVECTOR3(-1.f, -1.f, 0.f);
	terrainCulling.vec[1] = D3DXVECTOR3(1.f, -1.f, 0.f);
	terrainCulling.vec[2] = D3DXVECTOR3(-1.f, 1.f, 0.f);
	terrainCulling.vec[3] = D3DXVECTOR3(1.f, 1.f, 0.f);
	terrainCulling.vec[4] = D3DXVECTOR3(-1.f, -1.f, 1.f);
	terrainCulling.vec[5] = D3DXVECTOR3(1.f, -1.f, 1.f);
	terrainCulling.vec[6] = D3DXVECTOR3(-1.f, 1.f, 1.f);
	terrainCulling.vec[7] = D3DXVECTOR3(1.f, 1.f, 1.f);

	D3DXVec3TransformCoord(&terrainCulling.vec[0], &terrainCulling.vec[0], &terrainCulling.cullingTrans);
	D3DXVec3TransformCoord(&terrainCulling.vec[1], &terrainCulling.vec[1], &terrainCulling.cullingTrans);
	D3DXVec3TransformCoord(&terrainCulling.vec[2], &terrainCulling.vec[2], &terrainCulling.cullingTrans);
	D3DXVec3TransformCoord(&terrainCulling.vec[3], &terrainCulling.vec[3], &terrainCulling.cullingTrans);
	D3DXVec3TransformCoord(&terrainCulling.vec[4], &terrainCulling.vec[4], &terrainCulling.cullingTrans);
	D3DXVec3TransformCoord(&terrainCulling.vec[5], &terrainCulling.vec[5], &terrainCulling.cullingTrans);
	D3DXVec3TransformCoord(&terrainCulling.vec[6], &terrainCulling.vec[6], &terrainCulling.cullingTrans);
	D3DXVec3TransformCoord(&terrainCulling.vec[7], &terrainCulling.vec[7], &terrainCulling.cullingTrans);

	D3DXPlaneFromPoints(&planeCulling.planeNear, &terrainCulling.vec[0], &terrainCulling.vec[2], &terrainCulling.vec[3]);
	D3DXPlaneFromPoints(&planeCulling.planeFar, &terrainCulling.vec[7], &terrainCulling.vec[6], &terrainCulling.vec[4]);
	D3DXPlaneFromPoints(&planeCulling.planeTop, &terrainCulling.vec[2], &terrainCulling.vec[6], &terrainCulling.vec[7]);
	D3DXPlaneFromPoints(&planeCulling.planeBotton, &terrainCulling.vec[5], &terrainCulling.vec[4], &terrainCulling.vec[0]);
	D3DXPlaneFromPoints(&planeCulling.planeRight, &terrainCulling.vec[3], &terrainCulling.vec[7], &terrainCulling.vec[5]);
	D3DXPlaneFromPoints(&planeCulling.planeLeft, &terrainCulling.vec[4], &terrainCulling.vec[6], &terrainCulling.vec[2]);

	GIZMO_MGR->Line(terrainCulling.vec[0], terrainCulling.vec[1], 0xffffff00);
	GIZMO_MGR->Line(terrainCulling.vec[0], terrainCulling.vec[2], 0xffffff00);
	GIZMO_MGR->Line(terrainCulling.vec[2], terrainCulling.vec[3], 0xffffff00);
	GIZMO_MGR->Line(terrainCulling.vec[3], terrainCulling.vec[1], 0xffffff00);


	GIZMO_MGR->Line(terrainCulling.vec[4], terrainCulling.vec[5], 0xffffff00);
	GIZMO_MGR->Line(terrainCulling.vec[4], terrainCulling.vec[6], 0xffffff00);
	GIZMO_MGR->Line(terrainCulling.vec[6], terrainCulling.vec[7], 0xffffff00);
	GIZMO_MGR->Line(terrainCulling.vec[7], terrainCulling.vec[5], 0xffffff00);

	GIZMO_MGR->Line(terrainCulling.vec[0], terrainCulling.vec[4], 0xffffff00);
	GIZMO_MGR->Line(terrainCulling.vec[1], terrainCulling.vec[5], 0xffffff00);
	GIZMO_MGR->Line(terrainCulling.vec[2], terrainCulling.vec[6], 0xffffff00);
	GIZMO_MGR->Line(terrainCulling.vec[3], terrainCulling.vec[7], 0xffffff00);


	if (QuadTreeSphereRender)
		renderQuadTreeSphere(&mapQuadTreeSphere);

	if (QuadTreeMapRender) {
		renderQuadTreeMap();
		for (UINT i = 0; i < passNum; i++) {

			m_pTerrainEffect->BeginPass(i);

			Device->SetStreamSource(0, m_pTerrainVB, 0, sizeof(TERRAINVERTEX));
			Device->SetVertexDeclaration(this->m_pTerrainDecl);
			Device->SetIndices(terrainCullingIb);
			Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_nTotalVerNum, 0, m_nTotalTri);

			m_pTerrainEffect->EndPass();

		}
	}
	else {
		for (UINT i = 0; i < passNum; i++) {

			m_pTerrainEffect->BeginPass(i);

			Device->SetStreamSource(0, m_pTerrainVB, 0, sizeof(TERRAINVERTEX));
			Device->SetVertexDeclaration(this->m_pTerrainDecl);
			Device->SetIndices(m_pTerrainIB);
			Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_nTotalVerNum, 0, m_nTotalTri);

			m_pTerrainEffect->EndPass();

		}
	}
	m_pTerrainEffect->End();
}

//추가
void cTerrain::RenderShadow(cCamera* pDirectionLightCam)
{
	//월드 행렬셋팅
	D3DXMATRIXA16 matInd;
	D3DXMatrixIdentity(&matInd);
	m_pTerrainEffect->SetMatrix("matWorld", &matInd);

	//뷰 행렬셋팅
	m_pTerrainEffect->SetMatrix("matViewProjection", &pDirectionLightCam->GetViewProjectionMatrix());

	m_pTerrainEffect->SetTechnique("CreateShadow");

	UINT passNum = 0;
	m_pTerrainEffect->Begin(&passNum, 0);

	for (UINT i = 0; i < passNum; i++) {

		m_pTerrainEffect->BeginPass(i);

		Device->SetStreamSource(0, m_pTerrainVB, 0, sizeof(TERRAINVERTEX));
		Device->SetVertexDeclaration(this->m_pTerrainDecl);
		Device->SetIndices(m_pTerrainIB);
		Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_nTotalVerNum, 0, m_nTotalTri);

		m_pTerrainEffect->EndPass();

	}
	m_pTerrainEffect->End();
}

//Ray 히트 위치를 얻는다.
bool cTerrain::IsIntersectRay(D3DXVECTOR3* pOut, cCamera* cCam)
{
	Ray ray;
	POINT ptMousePos = GetMousePos();
	D3DXVECTOR2 screenPos(ptMousePos.x, ptMousePos.y);
	cCam->ComputeRay(&ray, &screenPos);

	vector<D3DXVECTOR3> hits;

	//최상단 쿼드 트리부터 Ray Check 들어간다.
	this->m_pQuadTree->GetRayHits(&hits, &ray);

	//히트된게 없다
	if (hits.size() == 0)
		return false;

	//먼저 처음 위치와 처음 위치에대한 거리 대입
	D3DXVECTOR3 hitPos = hits[0];
	D3DXVECTOR3 dir = hitPos - ray.origin;
	float distSq = D3DXVec3LengthSq(&dir);

	for (int i = 1; i < hits.size(); ++i)
	{
		dir = hits[i] - ray.origin;
		float newDist = D3DXVec3LengthSq(&dir);

		if (newDist < distSq)
		{
			distSq = newDist;
			hitPos = hits[i];
		}
	}


	//여기까지온다면 제일 거리가 가까운 Hit 위치가 나온다
	*pOut = hitPos;

	return true;
}

//해당 X, Z 위치의 지형의 높이를 얻는다.
float cTerrain::GetHeight(float x, float z)
{
	//터레인 범위을 넘어가면 0.0 값을 리턴한다
	if (x < m_fTerrainStartX || x > m_fTerrainEndX ||
		z > m_fTerrainStartZ || z < m_fTerrainEndZ)
	{
		return 0.0f;
	}

	//Terrain 의 좌상단 0 을 기준으로 월드 Terrain 의 상태적 위치를 찾자
	float pX = x - m_fTerrainStartX;
	float pZ = -(z + m_fTerrainEndZ);

	//해당 위치가 어느 셀에 포함되는지 파악
	float invCell = 1.0f / m_fCellScale;
	pX *= invCell;
	pZ *= invCell;

	//해당 위치의 셀 인덱스
	int idxX = static_cast<int>(pX);
	int idxZ = static_cast<int>(pZ);

	//셀의 네군데 정점을 얻는다.
	// lt-----rt
	//  |    /|
	//  |   / |
	//  |  /  |
	//  | /   |
	//  |/    |
	// lb-----rb

	D3DXVECTOR3 lt = m_pTerrainVertices[idxZ * m_nVerNumX + idxX].pos;
	D3DXVECTOR3 rt = m_pTerrainVertices[idxZ * m_nVerNumX + idxX + 1].pos;
	D3DXVECTOR3 lb = m_pTerrainVertices[(idxZ + 1) * m_nVerNumX + idxX].pos;
	D3DXVECTOR3 rb = m_pTerrainVertices[(idxZ + 1) * m_nVerNumX + idxX + 1].pos;

	//해당 셸에서의 delta 량을 구한다.
	float dX = pX - static_cast<float>(idxX);
	float dZ = pZ - static_cast<float>(idxZ);

	float fHeight = 0.0f;

	//해당 점이 좌상단에 있니?
	if (dX < 1.0f - dZ)
	{
		float deltaU = rt.y - lt.y;	//lt 정점에서 rt 정점까지의 y 위치의 변위량
		float deltaV = lb.y - lt.y; //lt 정점에서 lb 정점까지의 y 위치의 변위량

		fHeight = lt.y + (deltaU * dX) + (deltaV * dZ);
	}

	//해당 점이 우하단이 있니?
	else
	{
		float deltaU = lb.y - rb.y;
		float deltaV = rt.y - rb.y;


		//우에서 좌로 하에서 상으로 보간방향이 바뀌었기 때문에
		//delta 량을 역수로 취한다.
		dX = 1.0f - dX;
		dZ = 1.0f - dZ;

		fHeight = rb.y + (deltaU * dX) + (deltaV * dZ);
	}

	return fHeight;
}

//해당 X, Z 위치의 경사 벡터를 얻는다.
bool cTerrain::GetSlant(D3DXVECTOR3* pOut, float gravityPower, float x, float z)
{
	//터레인 범위을 넘어가면 0.0 값을 리턴한다
	if (x < m_fTerrainStartX || x > m_fTerrainEndX ||
		z > m_fTerrainStartZ || z < m_fTerrainEndZ)
	{
		return false;
	}


	//Terrain 의 좌상단 0 을 기준으로 월드 Terrain 의 상태적 위치를 찾자
	float pX = x - m_fTerrainStartX;
	float pZ = -(z + m_fTerrainEndZ);


	//해당 위치가 어느 셀에 포함되는지 파악
	float invCell = 1.0f / m_fCellScale;
	pX *= invCell;
	pZ *= invCell;


	//해당 위치의 셀 인덱스
	int idxX = static_cast<int>(pX);
	int idxZ = static_cast<int>(pZ);

	//셀의 네군데 정점을 얻는다.
	// lt-----rt
	//  |    /|
	//  |   / |
	//  |  /  |
	//  | /   |
	//  |/    |
	// lb-----rb

	D3DXVECTOR3 lt = m_pTerrainVertices[idxZ * m_nVerNumX + idxX].pos;
	D3DXVECTOR3 rt = m_pTerrainVertices[idxZ * m_nVerNumX + idxX + 1].pos;
	D3DXVECTOR3 lb = m_pTerrainVertices[(idxZ + 1) * m_nVerNumX + idxX].pos;
	D3DXVECTOR3 rb = m_pTerrainVertices[(idxZ + 1) * m_nVerNumX + idxX + 1].pos;

	//해당 셸에서의 delta 량을 구한다.
	float dX = pX - static_cast<float>(idxX);
	float dZ = pZ - static_cast<float>(idxZ);


	float fHeight = 0.0f;

	//폴리곤의 노말 벡터
	D3DXVECTOR3 normal;

	//해당 점이 좌상단에 있니?
	if (dX < 1.0f - dZ)
	{
		//해당폴리곤의 법선 벡터를 구한다.
		D3DXVECTOR3 edge1 = rt - lt;
		D3DXVECTOR3 edge2 = lb - lt;
		D3DXVec3Cross(&normal, &edge1, &edge2);
	}

	//해당 점이 우하단이 있니?
	else
	{
		//해당폴리곤의 법선 벡터를 구한다.
		D3DXVECTOR3 edge1 = rt - lb;
		D3DXVECTOR3 edge2 = rb - lb;
		D3DXVec3Cross(&normal, &edge1, &edge2);
	}

	//노말은 정규화
	D3DXVec3Normalize(&normal, &normal);

	//중력 방향
	D3DXVECTOR3 gravity(0, -gravityPower, 0);

	//경사면의 우측
	D3DXVECTOR3 right;
	D3DXVec3Cross(&right, &normal, &gravity);

	//우측벡터에서 법선벡터를 외적한 결과가 경사면의 경사 방향이 된다.
	D3DXVec3Cross(pOut, &right, &normal);

	return true;
}

//터레인 정점 만들기
HRESULT cTerrain::CreateTerrain()
{
	//
	// 정점 위치 구한다. ------------------------------------------------------------
	//
	//타일링 갯수에 따른 간격 ( 정점당 uv 간격 )
	//float tileIntervalX = static_cast<float>(tileNum) / m_nCellNumX;
	//float tileIntervalY = static_cast<float>(tileNum) / m_nCellNumZ;

	float tileIntervalX = m_nCellNumX / m_nCellNumX;
	float tileIntervalY = m_nCellNumZ / m_nCellNumZ;

	//터레인 정점 가지고 있어야 한다.
	m_pTerrainVertices = new TERRAINVERTEX[m_nTotalVerNum];


	//텍스쳐의 pixel 정보를 얻기 위해 Texture 를 lock 한다.
	D3DLOCKED_RECT lockRect;
	if (m_pHeightMap != NULL) {
		m_pHeightMap->LockRect(
			0,					//lock 을 할 밉맵 레벨 0
			&lockRect,			//lock 으로 얻어온 D3DLOCKED_RECT 정보 구조체
			0,					//lock 을 일부 영역만 하고 싶은 경우 사각형RECT 구조체의 포인터를 주는데 전체 할꺼면 NULL
			0					//lock 플레그 일단 0
		);
	}
	//lockRect->Pitch	lock 을 한 영역 이미지의 가로 byte 크기 ( 얻어온 바이트크기는 다음을 성립한다 pitch % 4 == 0 ) < 3byte 컬러시 pitch byte 구하는 공식 ( 가로 픽셀수 * 3 + 3 ) & ~3 = pitch  >
	//lockRect->pBits	이미지데이터가 시작되는 포인터 주소

	//정정위치와 정점 UV 를 계산했음....
	for (int z = 0; z < m_nVerNumZ; z++) {
		for (int x = 0; x < m_nVerNumX; x++) {

			D3DXVECTOR3 pos;

			//정점의 x, z 위치 계산
			pos.x = (x - (m_nCellNumX * 0.5)) * m_fCellScale;
			pos.z = (-z + (m_nCellNumZ * 0.5)) * m_fCellScale;

			//가로마지막 라인이라면 ( 이전 왼쪽의 정점 Y 위치와 맞춘다 )
			if (x == m_nVerNumX - 1) {
				int idx = z * m_nVerNumX + x - 1;
				pos.y = m_pTerrainVertices[idx].pos.y;
			}

			//세로 마지막 라인이라면 ( 이전 위쪽의 정점 Y 위치와 맞춘다 )
			else if (z == m_nVerNumZ - 1) {
				int idx = (z - 1) * m_nVerNumX + x;
				pos.y = m_pTerrainVertices[idx].pos.y;
			}

			else
			{
				if (m_pHeightMap != NULL) {
					//해당 픽셀의 컬러 값을 얻는다.
					DWORD* pStart = (DWORD*)lockRect.pBits;	//(DWORD 형으로 형변환된 lock 된 이미지지의 시작 주소
					DWORD dwColor = *(pStart + (z * (lockRect.Pitch / 4) + x));

					//각 컬러 값을 0 ~ 1 사이의 실수로 나눈다.
					float inv = 1.0f / 255.0f;
					float r = ((dwColor & 0x00ff0000) >> 16) * inv;
					float g = ((dwColor & 0x0000ff00) >> 8) * inv;
					float b = ((dwColor & 0x000000ff)) * inv;

					//높이 맵 값
					float factor = (r + g + b) / 3.0f;

					//높이 값
					pos.y = factor * m_fHeightScale;
				}
				else {
					//높이 맵 값
					float factor = 0;

					//높이 값
					pos.y = factor * m_fHeightScale;
				}
			}


			//정점 UV 계산

			//Terrain Tile Splating UV
			D3DXVECTOR2 baseUV;
			baseUV.x = x / static_cast<float>(m_nVerNumX - 1);
			baseUV.y = z / static_cast<float>(m_nVerNumZ - 1);

			//Terrain Tile UV
			D3DXVECTOR2 tileUV;
			tileUV.x = x * tileIntervalX;
			tileUV.y = z * tileIntervalY;

			//버텍스 배열인덱스 계산
			int idx = z * m_nVerNumX + x;

			m_pTerrainVertices[idx].pos = pos;
			m_pTerrainVertices[idx].normal = D3DXVECTOR3(0, 0, 0);	//아래에서 정점 노말 구할때 더해지니 일단 0 벡터로 초기화
			m_pTerrainVertices[idx].baseUV = baseUV;
			m_pTerrainVertices[idx].tileUV = tileUV;
		}
	}

	//텍스쳐의 pixel 정보 Unlock
	if (m_pHeightMap != NULL) {
		m_pHeightMap->UnlockRect(0);
	}


	//터레인 스무싱 
	//SmoothTerrain();


	//
	// 정점 인덱스를 구한다.....
	//
	LPTERRAINTRI pIndices = new tagTERRAINTRI[m_nTotalTri];

	//인덱스 배열 인덱스
	int idx = 0;

	for (DWORD z = 0; z < m_nCellNumZ; z++)
	{
		for (DWORD x = 0; x < m_nCellNumX; x++) {

			// lt-----rt
			//  |    /|
			//  |   / |
			//  |  /  |
			//  | /   |
			//  |/    |
			// lb-----rb

			//해당 셀에 대한 정점 인덱스를 얻자
			DWORD lt = z * m_nVerNumX + x;
			DWORD rt = z * m_nVerNumX + x + 1;
			DWORD lb = ((z + 1)* m_nVerNumX) + x;
			DWORD rb = ((z + 1)* m_nVerNumX) + x + 1;

			//셀의 삼각형 하나
			pIndices[idx].dw0 = lt;
			pIndices[idx].dw1 = rt;
			pIndices[idx].dw2 = lb;
			idx++;

			//셀의 삼각형 하나
			pIndices[idx].dw0 = lb;
			pIndices[idx].dw1 = rt;
			pIndices[idx].dw2 = rb;
			idx++;
		}
	}

	//
	// 노말이랑 Binormal 이랑 Tangent 계산하자...
	//
	D3DXVECTOR3* poses = new D3DXVECTOR3[m_nTotalVerNum];		//정점위치
	D3DXVECTOR3* normals = new D3DXVECTOR3[m_nTotalVerNum];
	D3DXVECTOR3* tangents = new D3DXVECTOR3[m_nTotalVerNum];
	D3DXVECTOR3* binormals = new D3DXVECTOR3[m_nTotalVerNum];
	D3DXVECTOR2* uvs = new D3DXVECTOR2[m_nTotalVerNum];
	DWORD* indices = (DWORD*)pIndices;

	//정점위치 및 UV 대입
	for (int i = 0; i < this->m_nTotalVerNum; i++)
	{
		poses[i] = this->m_pTerrainVertices[i].pos;
		uvs[i] = this->m_pTerrainVertices[i].baseUV;
	}


	//노말계산
	ComputeNormal(normals, poses, m_nTotalVerNum, indices, m_nTotalTri * 3);

	//탄젠트 바이노말 계산
	ComputeTangentAngBinormal(
		tangents,
		binormals,
		poses,
		normals,
		uvs,
		indices,
		m_nTotalTri,
		m_nTotalVerNum);

	//계산된거 대입
	for (int i = 0; i < this->m_nTotalVerNum; i++)
	{
		this->m_pTerrainVertices[i].normal = normals[i];
		this->m_pTerrainVertices[i].binormal = binormals[i];
		this->m_pTerrainVertices[i].tangent = tangents[i];
	}

	mapAster.Setup(m_nVerNumX, m_nVerNumZ);

	if (!Load_Terrain.empty()) {
		for (int i = 0; i < Load_Terrain.size(); i++) {
			m_pTerrainVertices[i].pos = Load_Terrain[i].pos;
			m_pTerrainVertices[i].Astar_check = Load_Terrain[i].Astar_check;
			mapAster.setMap(this->m_pTerrainVertices[i].pos, Load_Terrain[i].Astar_check);
			vertex_Index[i].Vertex_Astar_check = m_pTerrainVertices[i].Astar_check;
			vertex_Index[i].Vertex_Astar_pos = m_pTerrainVertices[i].pos;
		}
	}
	Load_Terrain.clear();

	//
	// 버퍼 생성
	//
	//정점버퍼 만든다.
	Device->CreateVertexBuffer(
		sizeof(TERRAINVERTEX) * m_nTotalVerNum,
		D3DUSAGE_WRITEONLY,
		0,
		D3DPOOL_MANAGED,
		&m_pTerrainVB,
		0);

	//만들어진 정점 버퍼를 Lock 하여 터레인 정점 값을 쓴다.
	void* p = NULL;
	m_pTerrainVB->Lock(0, 0, &p, 0);
	memcpy(p, m_pTerrainVertices, sizeof(TERRAINVERTEX) * m_nTotalVerNum);
	m_pTerrainVB->Unlock();

	//인덱스 버퍼 만든다.
	Device->CreateIndexBuffer(
		sizeof(TERRAINTRI) * m_nTotalTri,
		D3DUSAGE_WRITEONLY,				//D3DUSAGE_WRITEONLY 예는 락안하고 고정정으로 버퍼내용이 유지될때 최고성능을 발휘, D3DUSAGE_DYNAMIC 락 을 자주하여 버퍼 내용이 바뀌는 경우
		D3DFMT_INDEX32,
		D3DPOOL_DEFAULT,				//D3DUSAGE_DYNAMIC 을 쓰려면 무조건 D3DPOOL_DEFAULT
		&m_pTerrainIB,
		0);

	//인덱스 버퍼에 값을 쓴다.
	m_pTerrainIB->Lock(0, 0, &p, 0);
	memcpy(p, pIndices, sizeof(TERRAINTRI) * m_nTotalTri);
	m_pTerrainIB->Unlock();


	//
	// 정점 선언부를 만든다.
	//

	//정점의 형태를 알려주는 배열
	D3DVERTEXELEMENT9 vertElement[7];			//배열을 정점정보 갯수 + 1

	/*

	//Terrain 정점 구조체
	typedef struct tagTERRAINVERTEX{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 normal;
	D3DXVECTOR3 binormal;
	D3DXVECTOR3 tangent;
	D3DXVECTOR2 baseUV;			//0~1 UV
	D3DXVECTOR2 tileUV;			//타일 UV
	}TERRAINVERTEX, *LPTERRAINVERTEX;
	*/


	//Position 
	vertElement[0].Stream = 0;							//Stream 은 0
	vertElement[0].Offset = 0;							//메모리의 시작 Byte 단위 0
	vertElement[0].Type = D3DDECLTYPE_FLOAT3;			//자료의 타입
	vertElement[0].Method = D3DDECLMETHOD_DEFAULT;		//일단 무조건  D3DDECLMETHOD_DEFAULT
	vertElement[0].Usage = D3DDECLUSAGE_POSITION;		//정보 타입
	vertElement[0].UsageIndex = 0;						//UsageIndex 일단 0


	//Normal
	vertElement[1].Stream = 0;					//Stream 은 0
	vertElement[1].Offset = 12;					//메모리의 시작 Byte 단위 0
	vertElement[1].Type = D3DDECLTYPE_FLOAT3;	//자료의 타입
	vertElement[1].Method = D3DDECLMETHOD_DEFAULT;		//일단 무조건  D3DDECLMETHOD_DEFAULT
	vertElement[1].Usage = D3DDECLUSAGE_NORMAL;		//정보 타입
	vertElement[1].UsageIndex = 0;						//UsageIndex 일단 0

	//BiNormal
	vertElement[2].Stream = 0;					//Stream 은 0
	vertElement[2].Offset = 24;					//메모리의 시작 Byte 단위 0
	vertElement[2].Type = D3DDECLTYPE_FLOAT3;	//자료의 타입
	vertElement[2].Method = D3DDECLMETHOD_DEFAULT;		//일단 무조건  D3DDECLMETHOD_DEFAULT
	vertElement[2].Usage = D3DDECLUSAGE_BINORMAL;		//정보 타입
	vertElement[2].UsageIndex = 0;						//UsageIndex 일단 0

	//Tangent
	vertElement[3].Stream = 0;					//Stream 은 0
	vertElement[3].Offset = 36;					//메모리의 시작 Byte 단위 0
	vertElement[3].Type = D3DDECLTYPE_FLOAT3;	//자료의 타입
	vertElement[3].Method = D3DDECLMETHOD_DEFAULT;		//일단 무조건  D3DDECLMETHOD_DEFAULT
	vertElement[3].Usage = D3DDECLUSAGE_TANGENT;		//정보 타입
	vertElement[3].UsageIndex = 0;						//UsageIndex 일단 0


	//BaseUV
	vertElement[4].Stream = 0;						//Stream 은 0
	vertElement[4].Offset = 48;						//메모리의 시작 Byte 단위 0
	vertElement[4].Type = D3DDECLTYPE_FLOAT2;		//자료의 타입
	vertElement[4].Method = D3DDECLMETHOD_DEFAULT;		//일단 무조건  D3DDECLMETHOD_DEFAULT
	vertElement[4].Usage = D3DDECLUSAGE_TEXCOORD;		//정보 타입
	vertElement[4].UsageIndex = 0;						//UsageIndex 일단 0


	//tileUV
	vertElement[5].Stream = 0;						//Stream 은 0
	vertElement[5].Offset = 56;						//메모리의 시작 Byte 단위 0
	vertElement[5].Type = D3DDECLTYPE_FLOAT2;		//자료의 타입
	vertElement[5].Method = D3DDECLMETHOD_DEFAULT;		//일단 무조건  D3DDECLMETHOD_DEFAULT
	vertElement[5].Usage = D3DDECLUSAGE_TEXCOORD;		//정보 타입
	vertElement[5].UsageIndex = 1;						//UsageIndex 두번째 UV 1 

	//더이상 없어
	D3DVERTEXELEMENT9 end = D3DDECL_END(); //{0xFF,0,D3DDECLTYPE_UNUSED,0,0,0}
	vertElement[6] = end;


	//
	// LPDIRECT3DVERTEXDECLARATION9 생성
	//
	Device->CreateVertexDeclaration(
		vertElement,			//앞에서 만든 D3DVERTEXELEMENT9 배열 포인터
		&m_pTerrainDecl			//얻어올 LPDIRECT3DVERTEXDECLARATION9 포인터
	);


	SAFE_DELETE_ARR(poses);
	SAFE_DELETE_ARR(normals);
	SAFE_DELETE_ARR(tangents);
	SAFE_DELETE_ARR(binormals);
	SAFE_DELETE_ARR(uvs);
	SAFE_DELETE_ARR(indices);

	return S_OK;
}

//터레인 스무싱
void cTerrain::SmoothTerrain(int passed)
{
	if (passed <= 0) return;


	float* smooth = new float[m_nTotalVerNum];

	while (passed > 0) {

		passed--;

		for (int z = 0; z < m_nVerNumZ; z++) {
			for (int x = 0; x < m_nVerNumX; x++) {

				int adjacentSections = 0;		//몇개의 정점과 평균값을 내니?
				float totalSections = 0.0f;		//주변의 정점 높이 총합은 얼마니?


				//왼쪽체크
				if ((x - 1) > 0) {
					totalSections += m_pTerrainVertices[(z * m_nVerNumX) + (x - 1)].pos.y;
					adjacentSections++;

					//왼쪽 상단
					if ((z - 1) > 0) {
						totalSections += m_pTerrainVertices[((z - 1) * m_nVerNumX) + (x - 1)].pos.y;
						adjacentSections++;
					}

					//왼쪽 하단
					if ((z + 1) < m_nVerNumZ) {
						totalSections += m_pTerrainVertices[((z + 1) * m_nVerNumX) + (x - 1)].pos.y;
						adjacentSections++;
					}
				}

				//오른쪽 체크
				if ((x + 1) < m_nVerNumX) {
					totalSections += m_pTerrainVertices[(z * m_nVerNumX) + (x + 1)].pos.y;
					adjacentSections++;

					//오른쪽 상단
					if ((z - 1) > 0) {
						totalSections += m_pTerrainVertices[((z - 1) * m_nVerNumX) + (x + 1)].pos.y;
						adjacentSections++;
					}

					//오른쪽 하단 
					if ((z + 1) < m_nVerNumZ) {
						totalSections += m_pTerrainVertices[((z + 1) * m_nVerNumX) + (x + 1)].pos.y;
						adjacentSections++;
					}
				}


				//상단
				if ((z - 1) > 0)
				{
					totalSections += m_pTerrainVertices[((z - 1) * m_nVerNumX) + x].pos.y;
					adjacentSections++;
				}

				//하단
				if ((z + 1) < m_nVerNumZ)
				{
					totalSections += m_pTerrainVertices[((z + 1) * m_nVerNumX) + x].pos.y;
					adjacentSections++;
				}
				smooth[(z * m_nVerNumX) + x] = (
					m_pTerrainVertices[(z * m_nVerNumX) + x].pos.y +
					(totalSections / adjacentSections)) * 0.5f;
			}
		}


		//위에서 계산된 y 스무싱 적용
		for (int i = 0; i < m_nTotalVerNum; i++) {
			m_pTerrainVertices[i].pos.y = smooth[i];
		}
	}

	SAFE_DELETE_ARR(smooth);
}
//맵로드
void cTerrain::Text_Load(string Map_Text, vector<string>* vtemp)
{
	fstream file;
	file.open(Map_Text, fstream::in);
	while (!file.eof()) {
		string line;
		file >> line;
		vtemp->push_back(line);
	}
	file.close();
}
void cTerrain::Map_Load(string Map_Text)
{
	int c = 0, Temp_Index = 0;
	vector<string> strLine;
	Text_Load(Map_Text, &strLine);
	D3DXVECTOR3 temp_pos(0.f, 0.f, 0.f);

	char cStr[2048];
	char *text = NULL, *context = NULL;
	for (int i = 0; i < strLine.size(); i++) {
		if (strLine[i].size() == 0) continue;
		strcpy(cStr, strLine[i].c_str());
		if (i == 0) { Load_Terrain.resize(atoi(cStr)); continue; }
		if (i == 1) {
			text = strtok_s(cStr, "[,]", &context);
			for (int i = 0; i < 3; i++) {
				Player_Start_Pos[i] = atof(text);
				text = strtok_s(NULL, "[,]\n", &context);
			}
			continue;
		}
		if (cStr[0] == '$') {
			text = strtok_s(cStr, "$[,]", &context);
			for (int i = 0; i < 3; i++) {
				temp_pos[i] = atof(text);
				text = strtok_s(NULL, "[,]\n", &context);
			}
			Enemy1_Start_Pos.push_back(temp_pos);
			continue;
		}
		if (cStr[0] == '%') {
			text = strtok_s(cStr, "%[,]", &context);
			for (int i = 0; i < 3; i++) {
				temp_pos[i] = atof(text);
				text = strtok_s(NULL, "[,]\n", &context);
			}
			Enemy2_Start_Pos.push_back(temp_pos);
			continue;
		}
		if (cStr[0] == '&') {
			text = strtok_s(cStr, "&[,]", &context);
			for (int i = 0; i < 3; i++) {
				temp_pos[i] = atof(text);
				text = strtok_s(NULL, "[,]\n", &context);
			}
			Boss_Start_Pos.push_back(temp_pos);
			continue;
		}
		if (cStr[0] == '/') continue;
		if (cStr[0] == '#') {
			text = strtok_s(cStr, "#[,]", &context);
			Load_Terrain_Text.push_back(text); continue;
		}
		else {
			text = strtok_s(cStr, "[,]", &context);
			for (int i = 0; i < 3; i++) {
				Load_Terrain[Temp_Index].pos[i] = atof(text);
				text = strtok_s(NULL, "[,]\n", &context);
			}
			Load_Terrain[Temp_Index].Astar_check = atof(text);
			Temp_Index++;
		}
	}
}

void cTerrain::Load_obj(string Map_Text, vector<cBaseObject*> *temp)
{
	temp->clear();
	int c = 0; float s = 0.07f;
	vector<string> strLine;
	Text_Load(Map_Text, &strLine);

	char cStr[2048];
	char *text = NULL, *context = NULL;
	for (int i = 0; i < strLine.size(); i++) {
		if (strLine[i].size() == 0) continue;
		strcpy(cStr, strLine[i].c_str());
		text = strtok_s(cStr, "[,]", &context);

		string Save_Obj_name = text;
		text = strtok_s(NULL, "[,]", &context);

		D3DXVECTOR3 pos;
		for (int i = 0; i < 3; i++) {
			pos[i] = atof(text);
			text = strtok_s(NULL, "[,]\n", &context);
		}

		D3DXQUATERNION quat;
		for (int i = 0; i < 4; i++) {
			quat[i] = atof(text);
			text = strtok_s(NULL, "[,]\n", &context);
		}

		//스케일
		D3DXVECTOR3 scale;
		for (int i = 0; i < 3; i++) {
			scale[i] = atof(text);
			text = strtok_s(NULL, "[,]\n", &context);
		}

		cXMesh_Static* mesh =
			RESOURCE_STATICXMESH->GetResource(
				Save_Obj_name);

		cBaseObject* pNewObject = new cBaseObject();
		pNewObject->SetMesh(mesh);
		pNewObject->SetActive(true);

		pNewObject->Obj_Name = Save_Obj_name;
		pNewObject->pTransform->SetWorldPosition(pos);
		pNewObject->pTransform->SetRotateWorld(quat);
		pNewObject->pTransform->SetScale(scale);
		temp->push_back(pNewObject);
	}
}
void cTerrain::Load_Bound(string Map_Text, vector<cSetBoundObject*> *temp)
{
	temp->clear();
	int c = 0; float s = 0.07f;
	vector<string> strLine;
	Text_Load(Map_Text, &strLine);

	char cStr[2048];
	char *text = NULL, *context = NULL;
	for (int i = 0; i < strLine.size(); i++) {
		if (strLine[i].size() == 0) continue;
		if (strLine[i] == "") continue;
		strcpy(cStr, strLine[i].c_str());
		text = strtok_s(cStr, "[,]", &context);
		D3DXVECTOR3 pos;
		for (int i = 0; i < 3; i++) {
			pos[i] = atof(text);
			text = strtok_s(NULL, "[,]\n", &context);
		}

		D3DXQUATERNION quat;
		for (int i = 0; i < 4; i++) {
			quat[i] = atof(text);
			text = strtok_s(NULL, "[,]\n", &context);
		}

		D3DXVECTOR3 scale;
		for (int i = 0; i < 3; i++) {
			scale[i] = atof(text);
			text = strtok_s(NULL, "[,]\n", &context);
		}
		D3DXVECTOR3 boundCenter;
		for (int i = 0; i < 3; i++) {
			boundCenter[i] = atof(text);
			text = strtok_s(NULL, "[,]\n", &context);
		}
		D3DXVECTOR3 boundHalf;
		for (int i = 0; i < 3; i++) {
			boundHalf[i] = atof(text);
			text = strtok_s(NULL, "[,]\n", &context);
		}
		bool Check;
		Check = atof(text);

		cSetBoundObject* NewBound = new cSetBoundObject();
		NewBound->SetActive(true);
		NewBound->Update(TIME_MGR->GetFrameDeltaSec());
		NewBound->pTransform->SetWorldPosition(pos);
		NewBound->pTransform->SetRotateWorld(quat);
		NewBound->pTransform->SetScale(scale);
		NewBound->BoundBox.SetBound(&boundCenter, &boundHalf);
		NewBound->BoundBox.Air_Blocking_Check = Check;

		temp->push_back(NewBound);
	}
}