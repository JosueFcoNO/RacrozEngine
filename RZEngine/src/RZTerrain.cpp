#include <RZEnginePCH.h>

namespace rczEngine
{
	void Terrain::InitTerrain(Gfx::GfxCore* gfx, ResVault* res, ComputeAPI* Capi)
	{
		m_gfx = gfx;
		m_res = res;

		StrPtr<Material> mat = std::make_shared<Material>();
		mat->InitMaterial(eMaterialType::PBR_MetRough);

		//m_Mesh.InitMeshPlane(64, 512.0f, Vector3(0,0,0), Ypos);
		//m_Mesh.SetMaterial(res->InsertResource(mat));
		//m_Mesh.GenerateMesh();
		
		m_Perlin.InitPerlinNoise(619);

		m_gfx = gfx;
		m_res = res;
		m_Capi = Capi;
	}

	void Terrain::GenerateTerrain()
	{
	}

	void Terrain::RenderTerrain()
	{
		//m_Mesh.Render();
	}

}