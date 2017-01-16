#include "stdafx.h"
#include "Water.h"


Water::Water()
{
}


Water::~Water()
{
}

HRESULT Water::Init(std::string filePath, void* pParam)
{
	m_Mesh = RESOURCE_STATICXMESH->GetResource(filePath, pParam);

	return S_OK;
}

void Water::Render(const cTransform * Trans, bool Alphablend)
{

	cXMesh_Static::sStaticMeshEffect->SetTexture("Diffuse_Tex", RESOURCE_TEXTURE->GetResource("../../Resources/Meshes/Water/tex_caustics_water_nmap.tga"));
	if (Alphablend)
	{
		Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
		Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		cXMesh_Static::sStaticMeshEffect->SetTexture("Diffuse_Tex", RESOURCE_TEXTURE->GetResource("../../Resources/Meshes/Water/tex_caustics_water_nmap1.tga"));
	}
	//텍스쳐 셋팅
	cXMesh_Static::sStaticMeshEffect->SetTexture("Normal_Tex", RESOURCE_TEXTURE->GetResource("../../Resources/Meshes/normalDefault.png"));
	cXMesh_Static::sStaticMeshEffect->SetTexture("Specular_Tex", RESOURCE_TEXTURE->GetResource("../../Resources/Meshes/specularDefault.png"));
	cXMesh_Static::sStaticMeshEffect->SetTexture("Emission_Tex", RESOURCE_TEXTURE->GetResource("../../Resources/Meshes/emissionDefault.png"));
	//스펙파워
	//sStaticMeshEffect->SetFloat("fSpecPower", this->vecMaterials[m].Power);
	cXMesh_Static::sStaticMeshEffect->SetTexture("tex_water_nm_Tex", RESOURCE_TEXTURE->GetResource("../../Resources/Meshes/Water/tex_water_nm.tga"));
	cXMesh_Static::sStaticMeshEffect->SetFloat("gTimes", static_cast<float>(TIME_MGR->GetTotalDeltaSec()));
	m_Mesh->RenderNT(Trans);
	if (Alphablend) Device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
}
