#pragma once
#include "AStarY.h"
class cQuadTree;
class cCamera;
class cLight_Direction;
class cSetBoundObject;

class cTerrain
{
public:
	//�ͷ��� ���� ����ü
	typedef struct tagTERRAINVERTEX{
		D3DXVECTOR3 pos;
		D3DXVECTOR3 normal;
		D3DXVECTOR3 binormal;
		D3DXVECTOR3 tangent;
		D3DXVECTOR2 baseUV;			//0~1 UV
		D3DXVECTOR2 tileUV;			//Ÿ�� UV 
		bool		Astar_check;
	}TERRAINVERTEX, *LPTERRAINVERTEX;

	//�ͷ��� �ε��� ����ü ( ������ �ϳ��� �ε��� )
	typedef struct tagTERRAINTRI{
		DWORD dw0;
		DWORD dw1;
		DWORD dw2;
	}TERRAINTRI, *LPTERRAINTRI;

	typedef struct VERTEX_Info
	{
		D3DXVECTOR3 Vertex_Astar_pos;
		bool Vertex_Astar_check;
	};

	typedef struct tagMAPSPHERE {
		D3DXVECTOR3 centerPos;
		float radius;
	}MAPSPHERE, *LPMAPSPHERE;

	typedef struct tagMinMax {
		int minX;
		int maxX;
		int minZ;
		int maxZ;
	}MinMax, *LPMinMax;

	typedef struct tagQUADTREESPHERE {
		MAPSPHERE sphere[4];
		vector<tagQUADTREESPHERE> vec_0;
		MinMax	MinMax[4];
	}QUADTREESPHERE, *LPQUADTREESPHERE;

	typedef struct tagFRUSTUMCULLING {
		D3DXMATRIXA16	cullingTrans;
		D3DXVECTOR3		vec[8];
	}FRUSTUMCULLING, ASTARCULLING, *LPFRUSTUMCULLING;

	typedef struct tagFRUSTUMCULLINGPLANE {
		D3DXPLANE planeNear;
		D3DXPLANE planeFar;
		D3DXPLANE planeLeft;
		D3DXPLANE planeRight;
		D3DXPLANE planeTop;
		D3DXPLANE planeBotton;
	}FRUSTUMCULLINGPLANE, ASTARCULLINGPLANE, *LPFRUSTUMCULLINGPLANE;
	bool								QuadTreeSphereRender;
	bool								QuadTreeMapRender;
	FRUSTUMCULLING						terrainCulling;
	bool setAstar(D3DXVECTOR3 start, D3DXVECTOR3 dest);
	deque<D3DXVECTOR3> getAstarPos() { return mapAster.getAsterMove2(); };
	void astarPop();
	int getAstarSize();
	void setMatInverseProj(D3DXMATRIXA16 *matInverseProj) {
		D3DXMatrixInverse(&terrainCulling.cullingTrans, NULL, matInverseProj);
	};
private:


	int									nCullingTotalTri;				//�ﰢ�� ����
	QUADTREESPHERE						mapQuadTreeSphere;

	FRUSTUMCULLINGPLANE					planeCulling;
	LPDIRECT3DINDEXBUFFER9				terrainCullingIb;
	queue<MinMax>						vecCulling;

	AStarY								mapAster;


	void makeQuadTreeSphere(LPQUADTREESPHERE quadTree, D3DXVECTOR3 centerPos, float VerNumX, float VerNumZ, float minX, float minZ, float maxX, float maxZ);
	void renderQuadTreeMap(LPQUADTREESPHERE quadTree = NULL);
	void renderQuadTreeSphere(LPQUADTREESPHERE quadTree);
	BOOL IsIn(D3DXVECTOR3* pv, float radius, int n, LPQUADTREESPHERE quadTree);

	//==================================================

	LPDIRECT3DVERTEXBUFFER9				m_pTerrainVB;		//�ͷ��� ���� ����
	LPDIRECT3DINDEXBUFFER9				m_pTerrainIB;		//�ͷ��� �ε��� ����
	LPDIRECT3DVERTEXDECLARATION9		m_pTerrainDecl;		//�ͷ��� ���� Decl

	float				m_fHeightScale;		//���̽����� ( �ȼ��÷��� 255 �϶� ���� )
	float				m_fCellScale;		//���ϳ��� ũ��

	int					m_nVerNumX;					//���� ������
	int					m_nVerNumZ;					//���� ������
	int					m_nTotalVerNum;				//�� ������

	int					m_nCellNumX;				//���� ����
	int					m_nCellNumZ;				//���� ����
	int					m_nTotalCellNum;			//�� ����
	int					m_nTotalTri;				//�ﰢ�� ����

	float				m_fTerrainSizeZ;			//�ͷ��� ���� ������
	float				m_fTerrainSizeX;			//�ͷ��� ���� ������

	//��Ȯ�� ���� ����� ���� �ʿ��ϴ�...
	float				m_fTerrainStartX;			//�ͷ��� ���� ��ġX
	float				m_fTerrainStartZ;			//�ͷ��� ���� ��ġZ
	float				m_fTerrainEndX;
	float				m_fTerrainEndZ;

	LPTERRAINVERTEX						m_pTerrainVertices;	//�ͷ��� ��������
	LPDIRECT3DTEXTURE9					m_pHeightMap;		//�ͷ��� ���� ��

	cQuadTree*							m_pQuadTree;		//���� Ʈ��


	LPDIRECT3DTEXTURE9	m_pTexTile_0;				//�ͷ��� 0 �� Ÿ��
	LPDIRECT3DTEXTURE9	m_pTexTile_1;				//�ͷ��� 1 �� Ÿ��
	LPDIRECT3DTEXTURE9	m_pTexTile_2;				//�ͷ��� 2 �� Ÿ��
	LPDIRECT3DTEXTURE9	m_pTexTile_3;				//�ͷ��� 3 �� Ÿ��
	LPDIRECT3DTEXTURE9  m_pTexSlat;					//���÷��� Texture

public:
	LPD3DXEFFECT		m_pTerrainEffect;			//�ͷ��� Effect

	vector<TERRAINVERTEX> Load_Terrain;
	vector<string>        Load_Terrain_Text;
	D3DXVECTOR3			  Player_Start_Pos;

	vector<D3DXVECTOR3>	      Enemy1_Start_Pos;
	vector<D3DXVECTOR3>	      Enemy2_Start_Pos;
	vector<D3DXVECTOR3>	      Boss_Start_Pos;

	vector<VERTEX_Info>	  vertex_Index;
public:
	cTerrain(void);
	~cTerrain(void);

	//�ʱ�ȭ 
	HRESULT Init_Load(string Load_Terrain_str);
	//����
	void Release();

	//����
	void Render(cCamera* pCam, cLight_Direction* pDirectionLight);
	void Render(cCamera* pCam, cLight_Direction* pDirectionLight, cCamera* pDirectionLightCamera);
	void RenderShadow(cCamera* pDirectionLightCam);

	//Ray ��Ʈ ��ġ�� ��´�.
	bool IsIntersectRay(D3DXVECTOR3* pOut, cCamera* cCam);

	//�ش� X, Z ��ġ�� ������ ���̸� ��´�.
	float GetHeight(float x, float z);

	//�ش� X, Z ��ġ�� ��� ���͸� ��´�.
	bool GetSlant(D3DXVECTOR3* pOut, float gravityPower, float x, float z);

	//======================================================================

	void Text_Load(string Map_Text, vector<string>* vtemp);
	void Map_Load(string Map_Text);
	void Load_obj(string Map_Text, vector<cBaseObject*> *temp);
	void Load_Bound(string Map_Text, vector<cSetBoundObject*> *temp);

private:

	//�ͷ��� �����
	HRESULT CreateTerrain();

	//�ͷ��� ������
	void SmoothTerrain(int passed);

};

