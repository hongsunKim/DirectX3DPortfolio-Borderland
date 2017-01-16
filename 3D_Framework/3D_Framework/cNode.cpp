#include "stdafx.h"
#include "cNode.h"


cNode::cNode()
{
}


cNode::~cNode()
{
}

void cNode::Setup(int nIndex, eNodeType eType, D3DXVECTOR3 pos, int x, int z)
{
	//√ ±‚»≠
	m_fF = 0.0f;
	m_fG = 0.0f;
	m_fH = 0.0f;
	m_pParent = NULL;
	m_nX = nIndex % x;
	m_nZ = nIndex / z;
	m_eNodeType = eType;
	m_point = pos;
}

