#pragma once
#include "AStarY.h"
class cQuadTree;
class cCamera;
class cLight_Direction;
class cSetBoundObject;

class cTerrain
{
public:
	//터레인 정점 구조체
	typedef struct tagTERRAINVERTEX{
		D3DXVECTOR3 pos;
		D3DXVECTOR3 normal;
		D3DXVECTOR3 binormal;
		D3DXVECTOR3 tangent;
		D3DXVECTOR2 baseUV;			//0~1 UV
		D3DXVECTOR2 tileUV;			//타일 UV 
		bool		Astar_check;
	}TERRAINVERTEX, *LPTERRAINVERTEX;

	//터레인 인덱스 구조체 ( 폴리곤 하나의 인덱싱 )
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


	int									nCullingTotalTri;				//삼각형 갯수
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

	LPDIRECT3DVERTEXBUFFER9				m_pTerrainVB;		//터레인 정점 버퍼
	LPDIRECT3DINDEXBUFFER9				m_pTerrainIB;		//터레인 인덱스 버퍼
	LPDIRECT3DVERTEXDECLARATION9		m_pTerrainDecl;		//터레인 정점 Decl

	float				m_fHeightScale;		//높이스케일 ( 픽셀컬러가 255 일때 높이 )
	float				m_fCellScale;		//셀하나의 크기

	int					m_nVerNumX;					//가로 정점수
	int					m_nVerNumZ;					//깊이 정점수
	int					m_nTotalVerNum;				//총 정점수

	int					m_nCellNumX;				//가로 셀수
	int					m_nCellNumZ;				//깊이 셀수
	int					m_nTotalCellNum;			//총 셀수
	int					m_nTotalTri;				//삼각형 갯수

	float				m_fTerrainSizeZ;			//터레인 가로 사이즈
	float				m_fTerrainSizeX;			//터레인 세로 사이즈

	//정확한 높이 계산을 위해 필요하다...
	float				m_fTerrainStartX;			//터레인 시작 위치X
	float				m_fTerrainStartZ;			//터레인 시작 위치Z
	float				m_fTerrainEndX;
	float				m_fTerrainEndZ;

	LPTERRAINVERTEX						m_pTerrainVertices;	//터레인 정점정보
	LPDIRECT3DTEXTURE9					m_pHeightMap;		//터레인 높이 맵

	cQuadTree*							m_pQuadTree;		//쿼드 트리


	LPDIRECT3DTEXTURE9	m_pTexTile_0;				//터레인 0 층 타일
	LPDIRECT3DTEXTURE9	m_pTexTile_1;				//터레인 1 층 타일
	LPDIRECT3DTEXTURE9	m_pTexTile_2;				//터레인 2 층 타일
	LPDIRECT3DTEXTURE9	m_pTexTile_3;				//터레인 3 층 타일
	LPDIRECT3DTEXTURE9  m_pTexSlat;					//스플렛팅 Texture

public:
	LPD3DXEFFECT		m_pTerrainEffect;			//터레인 Effect

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

	//초기화 
	HRESULT Init_Load(string Load_Terrain_str);
	//해제
	void Release();

	//랜더
	void Render(cCamera* pCam, cLight_Direction* pDirectionLight);
	void Render(cCamera* pCam, cLight_Direction* pDirectionLight, cCamera* pDirectionLightCamera);
	void RenderShadow(cCamera* pDirectionLightCam);

	//Ray 히트 위치를 얻는다.
	bool IsIntersectRay(D3DXVECTOR3* pOut, cCamera* cCam);

	//해당 X, Z 위치의 지형의 높이를 얻는다.
	float GetHeight(float x, float z);

	//해당 X, Z 위치의 경사 벡터를 얻는다.
	bool GetSlant(D3DXVECTOR3* pOut, float gravityPower, float x, float z);

	//======================================================================

	void Text_Load(string Map_Text, vector<string>* vtemp);
	void Map_Load(string Map_Text);
	void Load_obj(string Map_Text, vector<cBaseObject*> *temp);
	void Load_Bound(string Map_Text, vector<cSetBoundObject*> *temp);

private:

	//터레인 만들기
	HRESULT CreateTerrain();

	//터레인 스무싱
	void SmoothTerrain(int passed);

};

