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

					//�𼭸� ���� �ε���
					DWORD lt = z * m_nVerNumX + x;
					DWORD rt = z * m_nVerNumX + x + 1;
					DWORD lb = ((z + 1)* m_nVerNumX) + x;
					DWORD rb = ((z + 1)* m_nVerNumX) + x + 1;



					//Cell ������ 1
					//lb, lt, rt, 
					index.dw0 = lb;
					index.dw1 = lt;
					index.dw2 = rt;
					*lpIndex++ = index;
					++nCullingTotalTri;

					//Cell ������ 1
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

	// ����� left, right, far plane�� �����Ѵ�.
	// for( i = 0 ; i < 6 ; i++ )
	fDist = D3DXPlaneDotCoord(&planeCulling.planeNear, pv);
	if (fDist > radius) return false; // plane�� normal���Ͱ� far�� ���ϰ� �����Ƿ� ����̸� ���������� �ٱ���
	fDist = D3DXPlaneDotCoord(&planeCulling.planeFar, pv);
	if (fDist > radius) return false; // plane�� normal���Ͱ� far�� ���ϰ� �����Ƿ� ����̸� ���������� �ٱ���
	fDist = D3DXPlaneDotCoord(&planeCulling.planeLeft, pv);
	if (fDist > radius) return false; // plane�� normal���Ͱ� left�� ���ϰ� �����Ƿ� ����̸� ���������� ����
	fDist = D3DXPlaneDotCoord(&planeCulling.planeRight, pv);
	if (fDist > radius) return false; // plane�� normal���Ͱ� right�� ���ϰ� �����Ƿ� ����̸� ���������� ������

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

//�ʱ�ȭ 

HRESULT cTerrain::Init_Load(string Load_Terrain_str)
{
	Load_Terrain_Text.clear();
	Map_Load(Load_Terrain_str.c_str());
	//�����ϰ� ����
	m_fHeightScale = 100;
	m_fCellScale = 1;

	int sqrt_num = sqrt(Load_Terrain.size());
	//���μ��� ���� ���� ���Ѵ�.
	m_nVerNumX = sqrt_num;		//���� ���������� ���� �ʿ� ���� �ػ� + 1 �� ����. ( ������ ����Ʈ�������� ���������� 2��N�� + 1 �̿��� �ϱ� ������ )
	m_nVerNumZ = sqrt_num;		//���� ��������� ���� �ʿ� ���� �ػ� + 1 �� ����. ( ������ ����Ʈ�������� ���������� 2��N�� + 1 �̿��� �ϱ� ������ )
	m_nTotalVerNum = m_nVerNumX * m_nVerNumZ;		//�� ���� ����


													//���μ��� ����
	m_nCellNumX = m_nVerNumX - 1;
	m_nCellNumZ = m_nVerNumZ - 1;
	m_nTotalCellNum = m_nCellNumX * m_nCellNumZ;
	vertex_Index.resize(m_nTotalVerNum);

	//�� �ﰢ������
	m_nTotalTri = m_nTotalCellNum * 2;

	m_pHeightMap = NULL;

	//�ͷ����� �����.
	if (FAILED(CreateTerrain(
	)))
	{
		Release();
		return E_FAIL;
	}

	//�ͷ��� ũ��
	m_fTerrainSizeX = m_nCellNumX * m_fCellScale;
	m_fTerrainSizeZ = m_nCellNumZ * m_fCellScale;

	//�ͷ��� ����
	m_fTerrainStartX = m_pTerrainVertices[0].pos.x;
	m_fTerrainStartZ = m_pTerrainVertices[0].pos.z;
	m_fTerrainEndX = m_pTerrainVertices[m_nTotalVerNum - 1].pos.x;
	m_fTerrainEndZ = m_pTerrainVertices[m_nTotalVerNum - 1].pos.z;

	//����Ʈ���� �����.
	m_pQuadTree = new cQuadTree;
	m_pQuadTree->Init(m_pTerrainVertices, m_nVerNumX);


	//�ͷ��� Texture �ε�
	m_pTexTile_0 = RESOURCE_TEXTURE->GetResource(Load_Terrain_Text[0]);
	m_pTexTile_1 = RESOURCE_TEXTURE->GetResource(Load_Terrain_Text[1]);
	m_pTexTile_2 = RESOURCE_TEXTURE->GetResource(Load_Terrain_Text[2]);
	m_pTexTile_3 = RESOURCE_TEXTURE->GetResource(Load_Terrain_Text[3]);
	m_pTexSlat = RESOURCE_TEXTURE->GetResource(Load_Terrain_Text[4]);

	//Terrain Effect �ε�
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
//����
void cTerrain::Release()
{
	SAFE_RELEASE(m_pTerrainVB);
	SAFE_RELEASE(m_pTerrainIB);
	SAFE_RELEASE(m_pTerrainDecl);
	SAFE_DELETE_ARR(m_pTerrainVertices);
	SAFE_DELETE(m_pQuadTree);
}
//����
void cTerrain::Render(cCamera* pCam, cLight_Direction* pDirectionLight)
{
	//���� ��ļ���
	D3DXMATRIXA16 matInd;
	D3DXMatrixIdentity(&matInd);
	m_pTerrainEffect->SetMatrix("matWorld", &matInd);

	//�� ��ļ���
	m_pTerrainEffect->SetMatrix("matViewProjection", &pCam->GetViewProjectionMatrix());

	//Texture ����
	m_pTerrainEffect->SetTexture("Terrain0_Tex", m_pTexTile_0);
	m_pTerrainEffect->SetTexture("Terrain1_Tex", m_pTexTile_1);
	m_pTerrainEffect->SetTexture("Terrain2_Tex", m_pTexTile_2);
	m_pTerrainEffect->SetTexture("Terrain3_Tex", m_pTexTile_3);
	m_pTerrainEffect->SetTexture("TerrainControl_Tex", m_pTexSlat);

	//���� ����
	D3DXVECTOR3 dirLight = pDirectionLight->pTransform->GetForward();
	m_pTerrainEffect->SetVector("worldLightDir", &D3DXVECTOR4(dirLight, 1));

	//�߰�
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

//�߰�
void cTerrain::Render(cCamera* pCam, cLight_Direction* pDirectionLight, cCamera* pDirectionLightCamera)
{
	//ShadowMap ����
	m_pTerrainEffect->SetTexture("Shadow_Tex", pDirectionLightCamera->GetRenderTexture());

	//ShadowViewProjection ���� 
	m_pTerrainEffect->SetMatrix("matLightViewProjection", &pDirectionLightCamera->GetViewProjectionMatrix());

	//���� ��ļ���
	D3DXMATRIXA16 matInd;
	D3DXMatrixIdentity(&matInd);
	m_pTerrainEffect->SetMatrix("matWorld", &matInd);

	//�� ��ļ���
	m_pTerrainEffect->SetMatrix("matViewProjection", &pCam->GetViewProjectionMatrix());

	//Texture ����
	m_pTerrainEffect->SetTexture("Terrain0_Tex", m_pTexTile_0);
	m_pTerrainEffect->SetTexture("Terrain1_Tex", m_pTexTile_1);
	m_pTerrainEffect->SetTexture("Terrain2_Tex", m_pTexTile_2);
	m_pTerrainEffect->SetTexture("Terrain3_Tex", m_pTexTile_3);
	m_pTerrainEffect->SetTexture("TerrainControl_Tex", m_pTexSlat);

	//���� ����
	D3DXVECTOR3 dirLight = pDirectionLight->pTransform->GetForward();
	m_pTerrainEffect->SetVector("worldLightDir", &D3DXVECTOR4(dirLight, 1));


	//���� ����
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

//�߰�
void cTerrain::RenderShadow(cCamera* pDirectionLightCam)
{
	//���� ��ļ���
	D3DXMATRIXA16 matInd;
	D3DXMatrixIdentity(&matInd);
	m_pTerrainEffect->SetMatrix("matWorld", &matInd);

	//�� ��ļ���
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

//Ray ��Ʈ ��ġ�� ��´�.
bool cTerrain::IsIntersectRay(D3DXVECTOR3* pOut, cCamera* cCam)
{
	Ray ray;
	POINT ptMousePos = GetMousePos();
	D3DXVECTOR2 screenPos(ptMousePos.x, ptMousePos.y);
	cCam->ComputeRay(&ray, &screenPos);

	vector<D3DXVECTOR3> hits;

	//�ֻ�� ���� Ʈ������ Ray Check ����.
	this->m_pQuadTree->GetRayHits(&hits, &ray);

	//��Ʈ�Ȱ� ����
	if (hits.size() == 0)
		return false;

	//���� ó�� ��ġ�� ó�� ��ġ������ �Ÿ� ����
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


	//��������´ٸ� ���� �Ÿ��� ����� Hit ��ġ�� ���´�
	*pOut = hitPos;

	return true;
}

//�ش� X, Z ��ġ�� ������ ���̸� ��´�.
float cTerrain::GetHeight(float x, float z)
{
	//�ͷ��� ������ �Ѿ�� 0.0 ���� �����Ѵ�
	if (x < m_fTerrainStartX || x > m_fTerrainEndX ||
		z > m_fTerrainStartZ || z < m_fTerrainEndZ)
	{
		return 0.0f;
	}

	//Terrain �� �»�� 0 �� �������� ���� Terrain �� ������ ��ġ�� ã��
	float pX = x - m_fTerrainStartX;
	float pZ = -(z + m_fTerrainEndZ);

	//�ش� ��ġ�� ��� ���� ���ԵǴ��� �ľ�
	float invCell = 1.0f / m_fCellScale;
	pX *= invCell;
	pZ *= invCell;

	//�ش� ��ġ�� �� �ε���
	int idxX = static_cast<int>(pX);
	int idxZ = static_cast<int>(pZ);

	//���� �ױ��� ������ ��´�.
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

	//�ش� �п����� delta ���� ���Ѵ�.
	float dX = pX - static_cast<float>(idxX);
	float dZ = pZ - static_cast<float>(idxZ);

	float fHeight = 0.0f;

	//�ش� ���� �»�ܿ� �ִ�?
	if (dX < 1.0f - dZ)
	{
		float deltaU = rt.y - lt.y;	//lt �������� rt ���������� y ��ġ�� ������
		float deltaV = lb.y - lt.y; //lt �������� lb ���������� y ��ġ�� ������

		fHeight = lt.y + (deltaU * dX) + (deltaV * dZ);
	}

	//�ش� ���� ���ϴ��� �ִ�?
	else
	{
		float deltaU = lb.y - rb.y;
		float deltaV = rt.y - rb.y;


		//�쿡�� �·� �Ͽ��� ������ ���������� �ٲ���� ������
		//delta ���� ������ ���Ѵ�.
		dX = 1.0f - dX;
		dZ = 1.0f - dZ;

		fHeight = rb.y + (deltaU * dX) + (deltaV * dZ);
	}

	return fHeight;
}

//�ش� X, Z ��ġ�� ��� ���͸� ��´�.
bool cTerrain::GetSlant(D3DXVECTOR3* pOut, float gravityPower, float x, float z)
{
	//�ͷ��� ������ �Ѿ�� 0.0 ���� �����Ѵ�
	if (x < m_fTerrainStartX || x > m_fTerrainEndX ||
		z > m_fTerrainStartZ || z < m_fTerrainEndZ)
	{
		return false;
	}


	//Terrain �� �»�� 0 �� �������� ���� Terrain �� ������ ��ġ�� ã��
	float pX = x - m_fTerrainStartX;
	float pZ = -(z + m_fTerrainEndZ);


	//�ش� ��ġ�� ��� ���� ���ԵǴ��� �ľ�
	float invCell = 1.0f / m_fCellScale;
	pX *= invCell;
	pZ *= invCell;


	//�ش� ��ġ�� �� �ε���
	int idxX = static_cast<int>(pX);
	int idxZ = static_cast<int>(pZ);

	//���� �ױ��� ������ ��´�.
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

	//�ش� �п����� delta ���� ���Ѵ�.
	float dX = pX - static_cast<float>(idxX);
	float dZ = pZ - static_cast<float>(idxZ);


	float fHeight = 0.0f;

	//�������� �븻 ����
	D3DXVECTOR3 normal;

	//�ش� ���� �»�ܿ� �ִ�?
	if (dX < 1.0f - dZ)
	{
		//�ش��������� ���� ���͸� ���Ѵ�.
		D3DXVECTOR3 edge1 = rt - lt;
		D3DXVECTOR3 edge2 = lb - lt;
		D3DXVec3Cross(&normal, &edge1, &edge2);
	}

	//�ش� ���� ���ϴ��� �ִ�?
	else
	{
		//�ش��������� ���� ���͸� ���Ѵ�.
		D3DXVECTOR3 edge1 = rt - lb;
		D3DXVECTOR3 edge2 = rb - lb;
		D3DXVec3Cross(&normal, &edge1, &edge2);
	}

	//�븻�� ����ȭ
	D3DXVec3Normalize(&normal, &normal);

	//�߷� ����
	D3DXVECTOR3 gravity(0, -gravityPower, 0);

	//������ ����
	D3DXVECTOR3 right;
	D3DXVec3Cross(&right, &normal, &gravity);

	//�������Ϳ��� �������͸� ������ ����� ������ ��� ������ �ȴ�.
	D3DXVec3Cross(pOut, &right, &normal);

	return true;
}

//�ͷ��� ���� �����
HRESULT cTerrain::CreateTerrain()
{
	//
	// ���� ��ġ ���Ѵ�. ------------------------------------------------------------
	//
	//Ÿ�ϸ� ������ ���� ���� ( ������ uv ���� )
	//float tileIntervalX = static_cast<float>(tileNum) / m_nCellNumX;
	//float tileIntervalY = static_cast<float>(tileNum) / m_nCellNumZ;

	float tileIntervalX = m_nCellNumX / m_nCellNumX;
	float tileIntervalY = m_nCellNumZ / m_nCellNumZ;

	//�ͷ��� ���� ������ �־�� �Ѵ�.
	m_pTerrainVertices = new TERRAINVERTEX[m_nTotalVerNum];


	//�ؽ����� pixel ������ ��� ���� Texture �� lock �Ѵ�.
	D3DLOCKED_RECT lockRect;
	if (m_pHeightMap != NULL) {
		m_pHeightMap->LockRect(
			0,					//lock �� �� �Ӹ� ���� 0
			&lockRect,			//lock ���� ���� D3DLOCKED_RECT ���� ����ü
			0,					//lock �� �Ϻ� ������ �ϰ� ���� ��� �簢��RECT ����ü�� �����͸� �ִµ� ��ü �Ҳ��� NULL
			0					//lock �÷��� �ϴ� 0
		);
	}
	//lockRect->Pitch	lock �� �� ���� �̹����� ���� byte ũ�� ( ���� ����Ʈũ��� ������ �����Ѵ� pitch % 4 == 0 ) < 3byte �÷��� pitch byte ���ϴ� ���� ( ���� �ȼ��� * 3 + 3 ) & ~3 = pitch  >
	//lockRect->pBits	�̹��������Ͱ� ���۵Ǵ� ������ �ּ�

	//������ġ�� ���� UV �� �������....
	for (int z = 0; z < m_nVerNumZ; z++) {
		for (int x = 0; x < m_nVerNumX; x++) {

			D3DXVECTOR3 pos;

			//������ x, z ��ġ ���
			pos.x = (x - (m_nCellNumX * 0.5)) * m_fCellScale;
			pos.z = (-z + (m_nCellNumZ * 0.5)) * m_fCellScale;

			//���θ����� �����̶�� ( ���� ������ ���� Y ��ġ�� ����� )
			if (x == m_nVerNumX - 1) {
				int idx = z * m_nVerNumX + x - 1;
				pos.y = m_pTerrainVertices[idx].pos.y;
			}

			//���� ������ �����̶�� ( ���� ������ ���� Y ��ġ�� ����� )
			else if (z == m_nVerNumZ - 1) {
				int idx = (z - 1) * m_nVerNumX + x;
				pos.y = m_pTerrainVertices[idx].pos.y;
			}

			else
			{
				if (m_pHeightMap != NULL) {
					//�ش� �ȼ��� �÷� ���� ��´�.
					DWORD* pStart = (DWORD*)lockRect.pBits;	//(DWORD ������ ����ȯ�� lock �� �̹������� ���� �ּ�
					DWORD dwColor = *(pStart + (z * (lockRect.Pitch / 4) + x));

					//�� �÷� ���� 0 ~ 1 ������ �Ǽ��� ������.
					float inv = 1.0f / 255.0f;
					float r = ((dwColor & 0x00ff0000) >> 16) * inv;
					float g = ((dwColor & 0x0000ff00) >> 8) * inv;
					float b = ((dwColor & 0x000000ff)) * inv;

					//���� �� ��
					float factor = (r + g + b) / 3.0f;

					//���� ��
					pos.y = factor * m_fHeightScale;
				}
				else {
					//���� �� ��
					float factor = 0;

					//���� ��
					pos.y = factor * m_fHeightScale;
				}
			}


			//���� UV ���

			//Terrain Tile Splating UV
			D3DXVECTOR2 baseUV;
			baseUV.x = x / static_cast<float>(m_nVerNumX - 1);
			baseUV.y = z / static_cast<float>(m_nVerNumZ - 1);

			//Terrain Tile UV
			D3DXVECTOR2 tileUV;
			tileUV.x = x * tileIntervalX;
			tileUV.y = z * tileIntervalY;

			//���ؽ� �迭�ε��� ���
			int idx = z * m_nVerNumX + x;

			m_pTerrainVertices[idx].pos = pos;
			m_pTerrainVertices[idx].normal = D3DXVECTOR3(0, 0, 0);	//�Ʒ����� ���� �븻 ���Ҷ� �������� �ϴ� 0 ���ͷ� �ʱ�ȭ
			m_pTerrainVertices[idx].baseUV = baseUV;
			m_pTerrainVertices[idx].tileUV = tileUV;
		}
	}

	//�ؽ����� pixel ���� Unlock
	if (m_pHeightMap != NULL) {
		m_pHeightMap->UnlockRect(0);
	}


	//�ͷ��� ������ 
	//SmoothTerrain();


	//
	// ���� �ε����� ���Ѵ�.....
	//
	LPTERRAINTRI pIndices = new tagTERRAINTRI[m_nTotalTri];

	//�ε��� �迭 �ε���
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

			//�ش� ���� ���� ���� �ε����� ����
			DWORD lt = z * m_nVerNumX + x;
			DWORD rt = z * m_nVerNumX + x + 1;
			DWORD lb = ((z + 1)* m_nVerNumX) + x;
			DWORD rb = ((z + 1)* m_nVerNumX) + x + 1;

			//���� �ﰢ�� �ϳ�
			pIndices[idx].dw0 = lt;
			pIndices[idx].dw1 = rt;
			pIndices[idx].dw2 = lb;
			idx++;

			//���� �ﰢ�� �ϳ�
			pIndices[idx].dw0 = lb;
			pIndices[idx].dw1 = rt;
			pIndices[idx].dw2 = rb;
			idx++;
		}
	}

	//
	// �븻�̶� Binormal �̶� Tangent �������...
	//
	D3DXVECTOR3* poses = new D3DXVECTOR3[m_nTotalVerNum];		//������ġ
	D3DXVECTOR3* normals = new D3DXVECTOR3[m_nTotalVerNum];
	D3DXVECTOR3* tangents = new D3DXVECTOR3[m_nTotalVerNum];
	D3DXVECTOR3* binormals = new D3DXVECTOR3[m_nTotalVerNum];
	D3DXVECTOR2* uvs = new D3DXVECTOR2[m_nTotalVerNum];
	DWORD* indices = (DWORD*)pIndices;

	//������ġ �� UV ����
	for (int i = 0; i < this->m_nTotalVerNum; i++)
	{
		poses[i] = this->m_pTerrainVertices[i].pos;
		uvs[i] = this->m_pTerrainVertices[i].baseUV;
	}


	//�븻���
	ComputeNormal(normals, poses, m_nTotalVerNum, indices, m_nTotalTri * 3);

	//ź��Ʈ ���̳븻 ���
	ComputeTangentAngBinormal(
		tangents,
		binormals,
		poses,
		normals,
		uvs,
		indices,
		m_nTotalTri,
		m_nTotalVerNum);

	//���Ȱ� ����
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
	// ���� ����
	//
	//�������� �����.
	Device->CreateVertexBuffer(
		sizeof(TERRAINVERTEX) * m_nTotalVerNum,
		D3DUSAGE_WRITEONLY,
		0,
		D3DPOOL_MANAGED,
		&m_pTerrainVB,
		0);

	//������� ���� ���۸� Lock �Ͽ� �ͷ��� ���� ���� ����.
	void* p = NULL;
	m_pTerrainVB->Lock(0, 0, &p, 0);
	memcpy(p, m_pTerrainVertices, sizeof(TERRAINVERTEX) * m_nTotalVerNum);
	m_pTerrainVB->Unlock();

	//�ε��� ���� �����.
	Device->CreateIndexBuffer(
		sizeof(TERRAINTRI) * m_nTotalTri,
		D3DUSAGE_WRITEONLY,				//D3DUSAGE_WRITEONLY ���� �����ϰ� ���������� ���۳����� �����ɶ� �ְ����� ����, D3DUSAGE_DYNAMIC �� �� �����Ͽ� ���� ������ �ٲ�� ���
		D3DFMT_INDEX32,
		D3DPOOL_DEFAULT,				//D3DUSAGE_DYNAMIC �� ������ ������ D3DPOOL_DEFAULT
		&m_pTerrainIB,
		0);

	//�ε��� ���ۿ� ���� ����.
	m_pTerrainIB->Lock(0, 0, &p, 0);
	memcpy(p, pIndices, sizeof(TERRAINTRI) * m_nTotalTri);
	m_pTerrainIB->Unlock();


	//
	// ���� ����θ� �����.
	//

	//������ ���¸� �˷��ִ� �迭
	D3DVERTEXELEMENT9 vertElement[7];			//�迭�� �������� ���� + 1

	/*

	//Terrain ���� ����ü
	typedef struct tagTERRAINVERTEX{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 normal;
	D3DXVECTOR3 binormal;
	D3DXVECTOR3 tangent;
	D3DXVECTOR2 baseUV;			//0~1 UV
	D3DXVECTOR2 tileUV;			//Ÿ�� UV
	}TERRAINVERTEX, *LPTERRAINVERTEX;
	*/


	//Position 
	vertElement[0].Stream = 0;							//Stream �� 0
	vertElement[0].Offset = 0;							//�޸��� ���� Byte ���� 0
	vertElement[0].Type = D3DDECLTYPE_FLOAT3;			//�ڷ��� Ÿ��
	vertElement[0].Method = D3DDECLMETHOD_DEFAULT;		//�ϴ� ������  D3DDECLMETHOD_DEFAULT
	vertElement[0].Usage = D3DDECLUSAGE_POSITION;		//���� Ÿ��
	vertElement[0].UsageIndex = 0;						//UsageIndex �ϴ� 0


	//Normal
	vertElement[1].Stream = 0;					//Stream �� 0
	vertElement[1].Offset = 12;					//�޸��� ���� Byte ���� 0
	vertElement[1].Type = D3DDECLTYPE_FLOAT3;	//�ڷ��� Ÿ��
	vertElement[1].Method = D3DDECLMETHOD_DEFAULT;		//�ϴ� ������  D3DDECLMETHOD_DEFAULT
	vertElement[1].Usage = D3DDECLUSAGE_NORMAL;		//���� Ÿ��
	vertElement[1].UsageIndex = 0;						//UsageIndex �ϴ� 0

	//BiNormal
	vertElement[2].Stream = 0;					//Stream �� 0
	vertElement[2].Offset = 24;					//�޸��� ���� Byte ���� 0
	vertElement[2].Type = D3DDECLTYPE_FLOAT3;	//�ڷ��� Ÿ��
	vertElement[2].Method = D3DDECLMETHOD_DEFAULT;		//�ϴ� ������  D3DDECLMETHOD_DEFAULT
	vertElement[2].Usage = D3DDECLUSAGE_BINORMAL;		//���� Ÿ��
	vertElement[2].UsageIndex = 0;						//UsageIndex �ϴ� 0

	//Tangent
	vertElement[3].Stream = 0;					//Stream �� 0
	vertElement[3].Offset = 36;					//�޸��� ���� Byte ���� 0
	vertElement[3].Type = D3DDECLTYPE_FLOAT3;	//�ڷ��� Ÿ��
	vertElement[3].Method = D3DDECLMETHOD_DEFAULT;		//�ϴ� ������  D3DDECLMETHOD_DEFAULT
	vertElement[3].Usage = D3DDECLUSAGE_TANGENT;		//���� Ÿ��
	vertElement[3].UsageIndex = 0;						//UsageIndex �ϴ� 0


	//BaseUV
	vertElement[4].Stream = 0;						//Stream �� 0
	vertElement[4].Offset = 48;						//�޸��� ���� Byte ���� 0
	vertElement[4].Type = D3DDECLTYPE_FLOAT2;		//�ڷ��� Ÿ��
	vertElement[4].Method = D3DDECLMETHOD_DEFAULT;		//�ϴ� ������  D3DDECLMETHOD_DEFAULT
	vertElement[4].Usage = D3DDECLUSAGE_TEXCOORD;		//���� Ÿ��
	vertElement[4].UsageIndex = 0;						//UsageIndex �ϴ� 0


	//tileUV
	vertElement[5].Stream = 0;						//Stream �� 0
	vertElement[5].Offset = 56;						//�޸��� ���� Byte ���� 0
	vertElement[5].Type = D3DDECLTYPE_FLOAT2;		//�ڷ��� Ÿ��
	vertElement[5].Method = D3DDECLMETHOD_DEFAULT;		//�ϴ� ������  D3DDECLMETHOD_DEFAULT
	vertElement[5].Usage = D3DDECLUSAGE_TEXCOORD;		//���� Ÿ��
	vertElement[5].UsageIndex = 1;						//UsageIndex �ι�° UV 1 

	//���̻� ����
	D3DVERTEXELEMENT9 end = D3DDECL_END(); //{0xFF,0,D3DDECLTYPE_UNUSED,0,0,0}
	vertElement[6] = end;


	//
	// LPDIRECT3DVERTEXDECLARATION9 ����
	//
	Device->CreateVertexDeclaration(
		vertElement,			//�տ��� ���� D3DVERTEXELEMENT9 �迭 ������
		&m_pTerrainDecl			//���� LPDIRECT3DVERTEXDECLARATION9 ������
	);


	SAFE_DELETE_ARR(poses);
	SAFE_DELETE_ARR(normals);
	SAFE_DELETE_ARR(tangents);
	SAFE_DELETE_ARR(binormals);
	SAFE_DELETE_ARR(uvs);
	SAFE_DELETE_ARR(indices);

	return S_OK;
}

//�ͷ��� ������
void cTerrain::SmoothTerrain(int passed)
{
	if (passed <= 0) return;


	float* smooth = new float[m_nTotalVerNum];

	while (passed > 0) {

		passed--;

		for (int z = 0; z < m_nVerNumZ; z++) {
			for (int x = 0; x < m_nVerNumX; x++) {

				int adjacentSections = 0;		//��� ������ ��հ��� ����?
				float totalSections = 0.0f;		//�ֺ��� ���� ���� ������ �󸶴�?


				//����üũ
				if ((x - 1) > 0) {
					totalSections += m_pTerrainVertices[(z * m_nVerNumX) + (x - 1)].pos.y;
					adjacentSections++;

					//���� ���
					if ((z - 1) > 0) {
						totalSections += m_pTerrainVertices[((z - 1) * m_nVerNumX) + (x - 1)].pos.y;
						adjacentSections++;
					}

					//���� �ϴ�
					if ((z + 1) < m_nVerNumZ) {
						totalSections += m_pTerrainVertices[((z + 1) * m_nVerNumX) + (x - 1)].pos.y;
						adjacentSections++;
					}
				}

				//������ üũ
				if ((x + 1) < m_nVerNumX) {
					totalSections += m_pTerrainVertices[(z * m_nVerNumX) + (x + 1)].pos.y;
					adjacentSections++;

					//������ ���
					if ((z - 1) > 0) {
						totalSections += m_pTerrainVertices[((z - 1) * m_nVerNumX) + (x + 1)].pos.y;
						adjacentSections++;
					}

					//������ �ϴ� 
					if ((z + 1) < m_nVerNumZ) {
						totalSections += m_pTerrainVertices[((z + 1) * m_nVerNumX) + (x + 1)].pos.y;
						adjacentSections++;
					}
				}


				//���
				if ((z - 1) > 0)
				{
					totalSections += m_pTerrainVertices[((z - 1) * m_nVerNumX) + x].pos.y;
					adjacentSections++;
				}

				//�ϴ�
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


		//������ ���� y ������ ����
		for (int i = 0; i < m_nTotalVerNum; i++) {
			m_pTerrainVertices[i].pos.y = smooth[i];
		}
	}

	SAFE_DELETE_ARR(smooth);
}
//�ʷε�
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

		//������
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