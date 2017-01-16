#pragma once
class cNode
{
public:
	cNode();
	~cNode();
	//������ ����ü
	enum eNodeType
	{
		E_NONE,
		E_EMPTY,
		E_WALL,
		E_START,
		E_DEST,
		E_CLOSE,
		E_OPEN,
		E_PATH
	};

	//�ʿ��� ������	
	float		m_fF;
	float		m_fG;
	float		m_fH;
	cNode*		m_pParent;
	int			m_nX;
	int			m_nZ;
	eNodeType	m_eNodeType;
	D3DXVECTOR3	m_point;

	void Setup(int nIndex, eNodeType eType, D3DXVECTOR3 pos, int x, int z);
};

