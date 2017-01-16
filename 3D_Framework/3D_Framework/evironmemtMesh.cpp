#include "stdafx.h"
#include "evironmemtMesh.h"


evironmemtMesh::evironmemtMesh()
{
}


evironmemtMesh::~evironmemtMesh()
{
}

void evironmemtMesh::Init()
{
	D3DXMATRIXA16 mat;
	D3DXMatrixScaling(&mat, 0.02f, 0.02f, 0.02f);
	this->m_evironmemtMesh = RESOURCE_STATICXMESH->GetResource("../../Resources/Meshes/SKY_DOME.X", &mat);
}

void evironmemtMesh::Render(cLight_Direction* BaseDirectionLight, cCamera* eye)
{
	cXMesh_Static::SetBaseLight(BaseDirectionLight);
	cXMesh_Static::SetCamera(eye);
	cXMesh_Static::SetTechniqueName("Evironment");
	cXMesh_Static::sStaticMeshEffect->SetTexture("TransitionTex", RESOURCE_TEXTURE->GetResource("../../Resources/Meshes/Transition.tga"));
	cXMesh_Static::sStaticMeshEffect->SetTexture("MultiTex", RESOURCE_TEXTURE->GetResource("../../Resources/Meshes/Sky_Multi.tga"));
	cXMesh_Static::sStaticMeshEffect->SetFloat("gTimes", TIME_MGR->GetTotalDeltaSec());
	this->m_evironmemtMesh->RenderEV(eye);
}
