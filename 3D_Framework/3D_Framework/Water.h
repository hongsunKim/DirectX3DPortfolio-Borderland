#pragma once

#include "cXMesh_Static.h"

class Water
{
private:
	cXMesh_Static*	m_Mesh;
public:
	Water();
	~Water();

	HRESULT Init(std::string filePath, void* pParam);
	void Render(const cTransform * Trans, bool Alphablend);
};

