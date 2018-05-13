#include <RZEnginePCH.h>

namespace rczEngine
{
	void Planet::InitPlanet(int32 seed, float x, float y, float z, SpaceManager* spaceMng)
	{
		///Save the spaceManager pointer
		m_Space = spaceMng;

		///Init the perlin noise.
		m_Perlin.InitPerlinNoise(seed);

		auto compute = spaceMng->m_capi;
		auto res = spaceMng->m_res;

		///Create the compute buffers

		///Buffer to hold the config data for the mesh being generated.
		m_CLDataBuffer.CreateMemBuffer(sizeof(MeshPlaneBuffer), eMEMORY_FLAGS::MEM_READ_ONLY, compute);
		///Buffer to hold the gradient index.
		m_CLGradIndexBuffer.CreateMemBuffer(MAX_GRAD_X * MAX_GRAD_Y, eMEMORY_FLAGS::MEM_READ_ONLY, compute);
		///Buffer to hold the 4 gradient vectors.
		m_CLGradBuffer.CreateMemBuffer(sizeof(Vector2) * 4, eMEMORY_FLAGS::MEM_READ_ONLY, compute);

		m_CLGradIndexBuffer.EnqueueWriteBuffer(MAX_GRAD_X * MAX_GRAD_Y, &m_Perlin.m_Core.GradientGrid, spaceMng->m_capi);
		m_CLGradBuffer.EnqueueWriteBuffer(4 * sizeof(Vector2), &m_Perlin.m_Core.GradientVectors, spaceMng->m_capi);

		m_PlanetModel.Load("Models/HighPolyCube.fbx", "PlanetBig", false);

		auto PlanetMat = res->GetResource<Material>(m_PlanetModel.m_MaterialMap.begin()->second).lock();
		PlanetMat->m_TextureAlbedo = res->LoadResource("Models/PBR/agua.bmp", "plante");
		PlanetMat->m_TextureRoughSmooth = res->m_BlackTex;

		m_SpacePosition.Set(x, y, z);

		CreateMaterial();
	}

	void Planet::RenderPlanet(float scale)
	{
		///Render the planet sphere.
		auto ScaleMatrix = Matrix4::Scale3D(RADIO*scale, RADIO*scale, RADIO*scale)*Matrix4::Translate3D(m_SpacePosition.m_x, m_SpacePosition.m_y, m_SpacePosition.m_z);
		ScaleMatrix.Transpose();
		m_Space->m_scene->m_WorldMatrix.UpdateConstantBuffer(&ScaleMatrix, m_Space->m_gfx);
		m_Space->m_scene->m_WorldMatrix.SetBufferInVS(2, m_Space->m_gfx);
		m_Space->m_scene->m_WorldMatrix.SetBufferInHS(1, m_Space->m_gfx);
		m_Space->m_scene->m_WorldMatrix.SetBufferInDS(2, m_Space->m_gfx);
		m_PlanetModel.DrawModel(m_Space->m_gfx, m_Space->m_res, NULL);
	}

	void Planet::CreateMaterial()
	{
		Texture3D* tex3D = new Texture3D;
		tex3D->LoadTexture3D("Models/PBR/grass/a.png", "Models/PBR/Cliff/a.png", "Models/PBR/crater/a.png", "Models/PBR/Slate/a.png", "TexA", true);

		Texture3D* tex3Dao = new Texture3D;
		tex3Dao->LoadTexture3D("Models/PBR/grass/ao.png", "Models/PBR/Cliff/ao.png", "Models/PBR/crater/ao.png", "Models/PBR/Slate/ao.png", "TexAO", true);

		Texture3D* tex3Dr = new Texture3D;
		tex3Dr->LoadTexture3D("Models/PBR/grass/r.png", "Models/PBR/Cliff/r.png", "Models/PBR/crater/r.png", "Models/PBR/Slate/r.png", "TexR", true);

		Texture3D* tex3Dm = new Texture3D;
		tex3Dm->LoadTexture3D("Models/PBR/grass/m.png", "Models/PBR/Cliff/m.png", "Models/PBR/crater/m.png", "Models/PBR/Slate/m.png", "TexM", true);

		Texture3D* tex3Dn = new Texture3D;
		tex3Dn->LoadTexture3D("Models/PBR/grass/n.png", "Models/PBR/Cliff/n.png", "Models/PBR/crater/n.png", "Models/PBR/Slate/n.png", "TexN", true);

		Texture3D* tex3Dh = new Texture3D;
		tex3Dh->LoadTexture3D("Models/PBR/grass/h.png", "Models/PBR/Cliff/h.png", "Models/PBR/crater/h.png", "Models/PBR/Slate/h.png", "TexH", true);

		Material* mat = new Material;
		mat->InitMaterial(MAT_PBR_TESS, Gfx::GfxCore::Pointer());

		ResVault* res = ResVault::Pointer();

		mat->m_TextureAlbedo = res->InsertResource(tex3D);
		mat->m_TextureAO = res->InsertResource(tex3Dao);
		mat->m_TextureRoughSmooth = res->InsertResource(tex3Dr);
		mat->m_TextureNormal = res->InsertResource(tex3Dn);
		mat->m_TextureMetSpec = res->InsertResource(tex3Dm);
		mat->m_TextureH = res->InsertResource(tex3Dh);

		m_Materials = res->InsertResource(mat);
	}
}