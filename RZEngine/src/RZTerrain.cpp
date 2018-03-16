#include <RZEnginePCH.h>

namespace rczEngine
{
	void Terrain::InitTerrain(Gfx::GfxCore* gfx, ResVault* res, ComputeAPI* Capi)
	{
		m_gfx = gfx;
		m_res = res;

		Material* mat = new Material;
		mat->InitMaterial(MAT_PBR_MetRough, gfx);

		m_Mesh.InitMeshPlane(64, 512.0f, Capi, true);
		m_Mesh.SetMaterial(res->InsertResource(mat));
		m_Mesh.GenerateMesh();
		
		m_Perlin.InitPerlinNoise(619);
		m_Mesh.HeightMap = m_Perlin.GetPerlinNoiseAsTextureR8UNORM(1024, 1024, 8, m_gfx, m_res);
		m_gfx = gfx;
		m_res = res;
		m_Capi = Capi;
	}

	void Terrain::GenerateTerrain()
	{
	}

	void Terrain::RenderTerrain()
	{
		m_Mesh.RenderMeshPlane();
	}

}