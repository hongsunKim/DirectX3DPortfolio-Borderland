#include "stdafx.h"
#include "AStarY.h"


AStarY::AStarY()
{
}


AStarY::~AStarY()
{
}


void AStarY::Setup(int tileX, int tileZ) {
	//���Ϳ� 8���� Ȯ���� ��� ��
	m_vecDirection.push_back(E_LEFT);
	m_vecDirection.push_back(E_RIGHT);
	m_vecDirection.push_back(E_UP);
	m_vecDirection.push_back(E_DOWN);
	m_vecDirection.push_back(E_LEFT | E_UP);
	m_vecDirection.push_back(E_LEFT | E_DOWN);
	m_vecDirection.push_back(E_RIGHT | E_UP);
	m_vecDirection.push_back(E_RIGHT | E_DOWN);

	//�� �������� �̵��� �ʿ��� �ڽ�Ʈ�� ������ ������ ��
	float fSqrt2 = sqrt(2.0f); //�밢��
	m_mapDistance[E_LEFT] = 1.0f;
	m_mapDistance[E_RIGHT] = 1.0f;
	m_mapDistance[E_UP] = 1.0f;
	m_mapDistance[E_DOWN] = 1.0f;
	m_mapDistance[E_LEFT | E_UP] = fSqrt2;
	m_mapDistance[E_LEFT | E_DOWN] = fSqrt2;
	m_mapDistance[E_RIGHT | E_UP] = fSqrt2;
	m_mapDistance[E_RIGHT | E_DOWN] = fSqrt2;

	asterTileX = tileX;
	asterTileZ = tileZ;
}

void AStarY::setMap(D3DXVECTOR3 map, bool b) {
	cNode* pNode = new cNode;
	if (b)
		pNode->Setup(m_vecNode.size(), cNode::E_WALL, map, asterTileX, asterTileZ);
	else
		pNode->Setup(m_vecNode.size(), cNode::E_EMPTY, map, asterTileX, asterTileZ);
	m_vecNode.push_back(pNode);
}

bool AStarY::FindPath() {
	//���� ���� �� ��带 ã�� ���ã�� �Լ� ����
	cNode* pStartNode = FindNode(cNode::E_START);
	cNode* pDestNode = FindNode(cNode::E_DEST);



	hight = pStartNode->m_point.y + 0.3f;
	//	if (pDestNode ->m_point.y >= hight+1)
	//		return false;


	//1. ���۳���� F, G, H ���� �����ְ� ���¸���Ʈ�� �ִ´�.
	pStartNode->m_fG = 0.0f;
	pStartNode->m_fH = CalcHeuristic(pStartNode, pDestNode); //�޸���ƽ�� ����ϴ� �Լ� ����
	pStartNode->m_fF = pStartNode->m_fG + pStartNode->m_fH;

	countHeap = 0;


	m_setOpenList.insert(pStartNode);
	minHeapCost temp;
	temp.cost = pStartNode->m_fF;
	temp.node = pStartNode;
	fMinF.push(temp);
	//	m_vecOpenList.push_back(pStartNode);
	//���¸���Ʈ���� F���� ���� ���� ��带 ã�Ƽ�
	//Ŭ�����Ʈ�� �ְ� 8�������� Ȯ��
	while (true)
	{
		cNode* pMinFNode = FindMinFNodeAtOpenList(); //���¸���Ʈ���� ���� ���� ���� ã���� �Լ� ����
		if (pMinFNode == NULL) //�� ���Ҵµ��� ���¸���Ʈ�� ��尡 ���ٸ� ���� ���ٴ� ��
		{
			//MarkNodeType(pDestNode); //����� ���ٸ� ���� ������. �Լ� ����
			//break; // �� ����
			
			returnStartDest();
			return false;
		}
		//ã�Ҵٸ� ���¸���Ʈ���� �����
		//Ŭ���� ����Ʈ�� ����
		m_setOpenList.erase(pMinFNode);
		m_setCloseList.insert(pMinFNode);

		//8���� Ȯ��. �Լ�����
		Extend(pMinFNode, pDestNode);

		//���� ���� ���� ��������
		if (pMinFNode == pDestNode)
		{
			MarkNodeType(pDestNode); //���� ������
									 //break; // �� ã��
									 //pStartNode->m_eNodeType = _startType;
									 //pDestNode->m_eNodeType = _destType;
			returnStartDest();
			return true;
		}
	}
}

cNode * AStarY::FindNode(cNode::eNodeType eType) {
	for each(cNode* pNode in m_vecNode)
	{
		if (pNode->m_eNodeType == eType)
			return pNode;
	}
	return NULL;
}

float AStarY::CalcHeuristic(cNode * pNode1, cNode * pNode2) {
	return fabs((float)pNode1->m_nX - pNode2->m_nX) +
		fabs((float)pNode1->m_nZ - pNode2->m_nZ);
}

cNode * AStarY::FindMinFNodeAtOpenList() {
	cNode* pMinFNode = NULL;
	minHeapCost temp;
	if (!fMinF.empty()) {
		temp = fMinF.top();
		fMinF.pop();
		//		pMinFNode = m_vecOpenList[temp.vecRenAccess];
	}

	return temp.node;
}

void AStarY::Extend(cNode * pCenterNode, cNode * pDestNode) {
	//8���� �˻�
	for each(eDirection d in m_vecDirection)
	{
		//Ȯ���� ������ ��带 ��� ��. Ȯ��� ��带 ã���� �Լ� ����
		cNode* pAdjNode = GetAdjNode(pCenterNode, d);

		if (pAdjNode == NULL) continue; //���̸� ��� Ž��

		if (IsInList(pAdjNode, m_setOpenList)) //Ȯ�尡���� ��尡 ���¸���Ʈ�� �ִٸ�. �Լ�����
		{
			if (pAdjNode->m_fG > pCenterNode->m_fG + m_mapDistance[d]) //G���� ���ؼ� �� �۴ٸ�
			{
				pAdjNode->m_pParent = pCenterNode; //�θ�� ������
				pAdjNode->m_fG = pCenterNode->m_fG + m_mapDistance[d];
				pAdjNode->m_fF = pAdjNode->m_fG + pAdjNode->m_fH; //G�� H�� ����ؼ� ����
			}
		}
		else //Ȯ�� ������ ��尡 ���� �ѹ��� Ž���� �������� ���� �����
		{
			pAdjNode->m_pParent = pCenterNode; //�θ�� ������� �����ְ�
			pAdjNode->m_fG = pCenterNode->m_fG + m_mapDistance[d];
			pAdjNode->m_fH = CalcHeuristic(pAdjNode, pDestNode);
			pAdjNode->m_fF = pAdjNode->m_fG + pAdjNode->m_fH; //FGH���� ���ؼ�
			m_setOpenList.insert(pAdjNode); //���¸���Ʈ�� �־� ��
			minHeapCost temp;
			temp.cost = pAdjNode->m_fF;
			temp.node = pAdjNode;
			fMinF.push(temp);
			//			m_vecOpenList.push_back(pAdjNode);
		}
	}
}

cNode * AStarY::GetAdjNode(cNode * pCenterNode, eDirection eDir) {
	//���� ����� ��� ���� ����
	int nAdjX = pCenterNode->m_nX;
	int nAdjZ = pCenterNode->m_nZ;
	int nIndex;
	if (eDir & E_LEFT) //���׿���
	{
		if (pCenterNode->m_nX == _asterMinMax.minX)
			return NULL; //0���̸� ������ �����Ƿ� �� ����
						 //0�� �ƴ�����
						 //		int nIndex = pCenterNode->m_nRow * asterTileX + nAdjCol - 1; //�������� ������ ���̸� ��(���̸� �밢������ �����ϱ�)
		nIndex = pCenterNode->m_nZ * asterTileX + pCenterNode->m_nX; //�������� ������ ���̸� ��(���̸� �밢������ �����ϱ�)
		if (m_vecNode.size() - 1 < nIndex || nIndex<0)
			return NULL;
		if (m_vecNode[nIndex]->m_eNodeType == cNode::E_WALL)
			return NULL;


		nAdjX -= 1;
	}

	if (eDir & E_RIGHT)
	{
		if (pCenterNode->m_nX >= _asterMinMax.maxX)
			return NULL; //Ÿ���� ���̸� �������� �����Ƿ� �� ����
						 //���� �ƴ�����
						 //		int nIndex = pCenterNode->m_nRow * asterTileX + pCenterNode->m_nCol + 1; //�������� �������� ���̸� ��(���̸� �밢������ �����ϱ�)
		nIndex = pCenterNode->m_nZ * asterTileX + pCenterNode->m_nX; //�������� ������ ���̸� ��(���̸� �밢������ �����ϱ�)
		if (m_vecNode.size() - 1 < nIndex || nIndex<0)
			return NULL;
		if (m_vecNode[nIndex]->m_eNodeType == cNode::E_WALL)
			return NULL;
		nAdjX += 1;
	}

	if (eDir & E_UP)
	{
		if (pCenterNode->m_nZ <= _asterMinMax.minZ)
			return NULL;
		//		int nIndex = (pCenterNode->m_nRow - 1) * asterTileX + pCenterNode->m_nCol;
		nIndex = pCenterNode->m_nZ * asterTileX + pCenterNode->m_nX; //�������� ������ ���̸� ��(���̸� �밢������ �����ϱ�)
		if (m_vecNode.size() - 1 < nIndex || nIndex<0)
			return NULL;
		if (m_vecNode[nIndex]->m_eNodeType == cNode::E_WALL)
			return NULL;

		nAdjZ -= 1;
	}

	if (eDir & E_DOWN)
	{
		if (pCenterNode->m_nZ >= _asterMinMax.maxZ)
			return NULL;
		//		int nIndex = (pCenterNode->m_nRow + 1) * asterTileX + pCenterNode->m_nCol;
		nIndex = pCenterNode->m_nZ  * asterTileX + pCenterNode->m_nX; //�������� ������ ���̸� ��(���̸� �밢������ �����ϱ�)
		if (m_vecNode.size() - 1 < nIndex || nIndex<0)
			return NULL;
		if (m_vecNode[nIndex]->m_eNodeType == cNode::E_WALL)
			return NULL;
		nAdjZ += 1;
	}

	//	int nIndex = nAdjRow * asterTileX + nAdjCol; //�������� ����ߴٸ� Ȯ�� ������ ����� �ε����� ����
	nIndex = nAdjZ * asterTileX + nAdjX; //�������� ����ߴٸ� Ȯ�� ������ ����� �ε����� ����
	if (m_vecNode.size() - 1 < nIndex || nIndex<0)
		return NULL;
	if (m_vecNode[nIndex]->m_eNodeType == cNode::E_WALL)
		return NULL; //Ȯ�� ������ ��尡 ���̸� �� ����
	if (IsInList(m_vecNode[nIndex], m_setCloseList))
		return NULL; //Ȯ�� ������ ��尡 Ŭ�����Ʈ�� �ִٸ� �� ����

	return m_vecNode[nIndex];
}

bool AStarY::IsInList(cNode * pNode, std::set<cNode*>& setList) {
	return setList.find(pNode) != setList.end();
}

void AStarY::MarkNodeType(cNode * pDestNode) {
	//������忡�� �θ� ���� �Ž��� �ö󰡸� ��θ� ����
	cNode* pTemp = pDestNode;
	while (pTemp)
	{
		m_point.push_back(pTemp->m_point);
		pTemp = pTemp->m_pParent;
	}
}

bool AStarY::setStartDest(D3DXVECTOR3 start, D3DXVECTOR3 dest) {
	for (m_isetOpenList = m_setOpenList.begin(); m_isetOpenList != m_setOpenList.end(); ++m_isetOpenList) {
		(*m_isetOpenList)->m_fF = 0.0f;
		(*m_isetOpenList)->m_fG = 0.0f;
		(*m_isetOpenList)->m_fH = 0.0f;
		(*m_isetOpenList)->m_pParent = NULL;
	}
	for (m_isetCloseList = m_setCloseList.begin(); m_isetCloseList != m_setCloseList.end(); ++m_isetCloseList) {
		(*m_isetCloseList)->m_fF = 0.0f;
		(*m_isetCloseList)->m_fG = 0.0f;
		(*m_isetCloseList)->m_fH = 0.0f;
		(*m_isetCloseList)->m_pParent = NULL;
	}
	m_setOpenList.clear();
	m_setCloseList.clear();
	while (!fMinF.empty())
		fMinF.pop();
	m_point.clear();


	int xHalf = asterTileX / 2;
	int zHalf = asterTileZ / 2;



	int x = dest.x + xHalf;
	int z = zHalf - dest.z;


	seveStartDest.destPos = z*asterTileX + x;


	x = start.x + xHalf;
	z = zHalf - start.z;

	seveStartDest.startPos = z*asterTileX + x;

	if(seveStartDest.destPos== seveStartDest.startPos)
		return false;
	
	if (m_vecNode[seveStartDest.destPos]->m_eNodeType == cNode::E_WALL)
		return false;


	seveStartDest._destType = m_vecNode[seveStartDest.destPos]->m_eNodeType;
	m_vecNode[seveStartDest.destPos]->m_eNodeType = cNode::E_DEST;


	seveStartDest._startType = m_vecNode[seveStartDest.startPos]->m_eNodeType;
	m_vecNode[seveStartDest.startPos]->m_eNodeType = cNode::E_START;



	return true;
}

void AStarY::returnStartDest() {
	m_vecNode[seveStartDest.startPos]->m_eNodeType = seveStartDest._startType;
	m_vecNode[seveStartDest.destPos]->m_eNodeType = seveStartDest._destType;
}

void AStarY::mapSeting() {
	map<char, cNode::eNodeType> mapNodeType;
	mapNodeType['E'] = cNode::E_EMPTY;
	mapNodeType['B'] = cNode::E_WALL;
	mapNodeType['S'] = cNode::E_START;
	mapNodeType['D'] = cNode::E_DEST;
	mapNodeType['L'] = cNode::E_CLOSE;
	char c;
	int temp;


	for (int i = 0; i < asterTileZ; ++i) {
		for (int j = 0; j < asterTileX; ++j) {

			temp = i*asterTileX + j;
			m_vecNode[temp]->m_eNodeType = mapNodeType[c];
		}
	}
}