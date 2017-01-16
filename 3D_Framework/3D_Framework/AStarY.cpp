#include "stdafx.h"
#include "AStarY.h"


AStarY::AStarY()
{
}


AStarY::~AStarY()
{
}


void AStarY::Setup(int tileX, int tileZ) {
	//벡터에 8방향 확장을 담아 둠
	m_vecDirection.push_back(E_LEFT);
	m_vecDirection.push_back(E_RIGHT);
	m_vecDirection.push_back(E_UP);
	m_vecDirection.push_back(E_DOWN);
	m_vecDirection.push_back(E_LEFT | E_UP);
	m_vecDirection.push_back(E_LEFT | E_DOWN);
	m_vecDirection.push_back(E_RIGHT | E_UP);
	m_vecDirection.push_back(E_RIGHT | E_DOWN);

	//각 방향으로 이동시 필요한 코스트를 맵으로 저장해 둠
	float fSqrt2 = sqrt(2.0f); //대각선
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
	//시작 노드와 끝 노드를 찾자 노드찾기 함수 구현
	cNode* pStartNode = FindNode(cNode::E_START);
	cNode* pDestNode = FindNode(cNode::E_DEST);



	hight = pStartNode->m_point.y + 0.3f;
	//	if (pDestNode ->m_point.y >= hight+1)
	//		return false;


	//1. 시작노드의 F, G, H 값을 정해주고 오픈리스트에 넣는다.
	pStartNode->m_fG = 0.0f;
	pStartNode->m_fH = CalcHeuristic(pStartNode, pDestNode); //휴리스틱을 계산하는 함수 구현
	pStartNode->m_fF = pStartNode->m_fG + pStartNode->m_fH;

	countHeap = 0;


	m_setOpenList.insert(pStartNode);
	minHeapCost temp;
	temp.cost = pStartNode->m_fF;
	temp.node = pStartNode;
	fMinF.push(temp);
	//	m_vecOpenList.push_back(pStartNode);
	//오픈리스트에서 F값이 가장 작은 노드를 찾아서
	//클로즈리스트에 넣고 8방향으로 확장
	while (true)
	{
		cNode* pMinFNode = FindMinFNodeAtOpenList(); //오픈리스트에서 가장 작은 값을 찾아줄 함수 구현
		if (pMinFNode == NULL) //다 돌았는데도 오픈리스트에 노드가 없다면 길이 없다는 뜻
		{
			//MarkNodeType(pDestNode); //결론이 났다면 색을 정해줌. 함수 구현
			//break; // 길 없음
			
			returnStartDest();
			return false;
		}
		//찾았다면 오픈리스트에서 지우고
		//클로즈 리스트에 담음
		m_setOpenList.erase(pMinFNode);
		m_setCloseList.insert(pMinFNode);

		//8방향 확장. 함수구현
		Extend(pMinFNode, pDestNode);

		//가장 작은 값이 도착노드면
		if (pMinFNode == pDestNode)
		{
			MarkNodeType(pDestNode); //색을 정해줌
									 //break; // 길 찾음
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
	//8방향 검색
	for each(eDirection d in m_vecDirection)
	{
		//확장이 가능한 노드를 골라 냄. 확장될 노드를 찾아줄 함수 구현
		cNode* pAdjNode = GetAdjNode(pCenterNode, d);

		if (pAdjNode == NULL) continue; //널이면 계속 탐색

		if (IsInList(pAdjNode, m_setOpenList)) //확장가능한 노드가 오픈리스트에 있다면. 함수구현
		{
			if (pAdjNode->m_fG > pCenterNode->m_fG + m_mapDistance[d]) //G값을 비교해서 더 작다면
			{
				pAdjNode->m_pParent = pCenterNode; //부모는 현재노드
				pAdjNode->m_fG = pCenterNode->m_fG + m_mapDistance[d];
				pAdjNode->m_fF = pAdjNode->m_fG + pAdjNode->m_fH; //G와 H를 계산해서 갱신
			}
		}
		else //확장 가능한 노드가 아직 한번도 탐색에 참여하지 않은 노드라면
		{
			pAdjNode->m_pParent = pCenterNode; //부모는 현재노드로 정해주고
			pAdjNode->m_fG = pCenterNode->m_fG + m_mapDistance[d];
			pAdjNode->m_fH = CalcHeuristic(pAdjNode, pDestNode);
			pAdjNode->m_fF = pAdjNode->m_fG + pAdjNode->m_fH; //FGH값을 정해서
			m_setOpenList.insert(pAdjNode); //오픈리스트에 넣어 줌
			minHeapCost temp;
			temp.cost = pAdjNode->m_fF;
			temp.node = pAdjNode;
			fMinF.push(temp);
			//			m_vecOpenList.push_back(pAdjNode);
		}
	}
}

cNode * AStarY::GetAdjNode(cNode * pCenterNode, eDirection eDir) {
	//현재 노드의 행과 열을 구함
	int nAdjX = pCenterNode->m_nX;
	int nAdjZ = pCenterNode->m_nZ;
	int nIndex;
	if (eDir & E_LEFT) //삼항연산
	{
		if (pCenterNode->m_nX == _asterMinMax.minX)
			return NULL; //0열이면 왼쪽이 없으므로 널 리턴
						 //0이 아니지만
						 //		int nIndex = pCenterNode->m_nRow * asterTileX + nAdjCol - 1; //현재노드의 왼쪽이 벽이면 널(벽이면 대각선으로 못가니까)
		nIndex = pCenterNode->m_nZ * asterTileX + pCenterNode->m_nX; //현재노드의 왼쪽이 벽이면 널(벽이면 대각선으로 못가니까)
		if (m_vecNode.size() - 1 < nIndex || nIndex<0)
			return NULL;
		if (m_vecNode[nIndex]->m_eNodeType == cNode::E_WALL)
			return NULL;


		nAdjX -= 1;
	}

	if (eDir & E_RIGHT)
	{
		if (pCenterNode->m_nX >= _asterMinMax.maxX)
			return NULL; //타일의 끝이면 오른쪽이 없으므로 널 리턴
						 //끝이 아니지만
						 //		int nIndex = pCenterNode->m_nRow * asterTileX + pCenterNode->m_nCol + 1; //현재노드의 오른쪽이 벽이면 널(벽이면 대각선으로 못가니까)
		nIndex = pCenterNode->m_nZ * asterTileX + pCenterNode->m_nX; //현재노드의 왼쪽이 벽이면 널(벽이면 대각선으로 못가니까)
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
		nIndex = pCenterNode->m_nZ * asterTileX + pCenterNode->m_nX; //현재노드의 왼쪽이 벽이면 널(벽이면 대각선으로 못가니까)
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
		nIndex = pCenterNode->m_nZ  * asterTileX + pCenterNode->m_nX; //현재노드의 왼쪽이 벽이면 널(벽이면 대각선으로 못가니까)
		if (m_vecNode.size() - 1 < nIndex || nIndex<0)
			return NULL;
		if (m_vecNode[nIndex]->m_eNodeType == cNode::E_WALL)
			return NULL;
		nAdjZ += 1;
	}

	//	int nIndex = nAdjRow * asterTileX + nAdjCol; //이프문을 통과했다면 확장 가능한 노드의 인덱스가 나옴
	nIndex = nAdjZ * asterTileX + nAdjX; //이프문을 통과했다면 확장 가능한 노드의 인덱스가 나옴
	if (m_vecNode.size() - 1 < nIndex || nIndex<0)
		return NULL;
	if (m_vecNode[nIndex]->m_eNodeType == cNode::E_WALL)
		return NULL; //확장 가능한 노드가 벽이면 널 리턴
	if (IsInList(m_vecNode[nIndex], m_setCloseList))
		return NULL; //확장 가능한 노드가 클로즈리스트에 있다면 널 리턴

	return m_vecNode[nIndex];
}

bool AStarY::IsInList(cNode * pNode, std::set<cNode*>& setList) {
	return setList.find(pNode) != setList.end();
}

void AStarY::MarkNodeType(cNode * pDestNode) {
	//도착노드에서 부모를 따라 거슬러 올라가며 경로를 지정
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