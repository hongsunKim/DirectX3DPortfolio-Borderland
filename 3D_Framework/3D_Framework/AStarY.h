#pragma once
#include "cNode.h"
#include <set>
#include <stack>
#include <queue>
#include <deque>
#include <functional>

class AStarY
{
public:
	struct minHeapCost {
		float cost;
		cNode* node;
		bool operator >(const minHeapCost &heap) const { return cost > heap.cost; };
	};

	typedef struct tagMinMax {
		int minX;
		int maxX;
		int minZ;
		int maxZ;
	}MinMax, *LPMinMax;

	typedef struct tagSeveStartDest {
		cNode::eNodeType _startType;
		cNode::eNodeType _destType;
		int destPos;
		int startPos;
	}SeveStartDest, *LPSeveStartDest;

	MinMax			_asterMinMax;
private:

	//방향을 정하기 위한 비트연산
	enum
	{
		E_LEFT = 1 << 0,	// 00000001
		E_RIGHT = 1 << 1,	// 00000010
		E_UP = 1 << 2,	// 00000100
		E_DOWN = 1 << 3,	// 00001000
	};

	int asterTileX;
	int	asterTileZ;
	float hight;
	typedef int eDirection;

	vector<cNode*>				m_vecNode;
	vector<cNode*>::iterator	m_ivecNode;
	deque<D3DXVECTOR3>			m_point;

	//	vector<cNode*>			    m_vecOpenList;

	set<cNode*>			    m_setOpenList;
	set<cNode*>::iterator   m_isetOpenList;

	set<cNode*>			    m_setCloseList;
	set<cNode*>::iterator   m_isetCloseList;

	vector<eDirection>		m_vecDirection;
	map<eDirection, float>  m_mapDistance;

	SeveStartDest			seveStartDest;

	priority_queue<minHeapCost, vector<minHeapCost>, greater<minHeapCost>> fMinF;
	int countHeap;
public:
	AStarY();
	~AStarY();

	void	Setup(int tileX, int tileZ);
	void	setMap(D3DXVECTOR3 map, bool b);
	bool	FindPath();

	deque<D3DXVECTOR3>* getAsterMove() { return &m_point; };
	deque<D3DXVECTOR3> getAsterMove2() { return m_point; };
	//	void setMap(Map* mp) { _map = mp; }

	cNode*	FindNode(cNode::eNodeType eType);
	float	CalcHeuristic(cNode* pNode1, cNode* pNode2);
	cNode*	FindMinFNodeAtOpenList();
	void	Extend(cNode* pCenterNode, cNode* pDestNode);
	cNode*	GetAdjNode(cNode* pCenterNode, eDirection eDir);
	bool	IsInList(cNode* pNode, std::set<cNode*>& setList);
	void	MarkNodeType(cNode* pDestNode);
	bool	setStartDest(D3DXVECTOR3 start, D3DXVECTOR3 dest);

	void returnStartDest();

	//	bool	setDEST(tagTile1* _start, tagTile1* _dest);
	//	bool	setDEST2(tagTile1* _start, tagTile1* _dest);

	void mapSeting();
};

