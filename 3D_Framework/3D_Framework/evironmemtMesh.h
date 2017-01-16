#pragma once

#include "cXMesh_Static.h"
#include "cCamera.h"

class evironmemtMesh
{
private:
	cXMesh_Static*						m_evironmemtMesh;		//È¯°æ ±¸


public:
	evironmemtMesh();
	~evironmemtMesh();

	void Init();
	void Render(cLight_Direction* BaseDirectionLight, cCamera* eye);

};

