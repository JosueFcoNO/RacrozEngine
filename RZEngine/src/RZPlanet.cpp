#include <RZEnginePCH.h>

namespace rczEngine
{
	void Planet::InitPlanet(int32 seed, float x, float y, float z, SpaceManager* spaceMng)
	{
		m_SpaceMng = spaceMng;
		m_CurrentScene = SceneManager::Pointer()->GetActiveScene();
		m_GfxCore = Gfx::GfxCore::Pointer();

		LoadAndProcessModel();

		m_SpacePosition.Set(x, y, z);

		CreateMaterial();

		Vector3	gradients[12] = {
			Vector3(1,1,0),Vector3(-1,1,0),Vector3(1,-1,0),Vector3(-1,-1,0),
			Vector3(1,0,1),Vector3(-1,0,1),Vector3(1,0,-1),Vector3(-1,0,-1),
			Vector3(0,1,1),Vector3(0,-1,1),Vector3(0,1,-1),Vector3(0,-1,-1)
		};

		for (int i = 0; i < PERMUTATION_TABLE_SIZE; ++i)
		{
			PermutationTable[i].m_x = rand() % 255;
		}

		m_GradientsReal.CreateConstantBuffer(sizeof(Vector4) * 256, Gfx::USAGE_DEFAULT, m_GfxCore);
		m_GradientsReal.UpdateConstantBuffer(PermutationTable, m_GfxCore);

		m_GradientCB.CreateConstantBuffer(sizeof(Vector3) * 12, Gfx::USAGE_DEFAULT, m_GfxCore);
		m_GradientCB.UpdateConstantBuffer(gradients, m_GfxCore);

		m_HeightScale.m_x = 1.0f;

		m_HeightScale.Set(1.0f, 2.0f, 1.0f, 0.0f);
		m_ScaleCB.CreateConstantBuffer(sizeof(Vector4), Gfx::USAGE_DEFAULT, m_GfxCore);
		m_ScaleCB.UpdateConstantBuffer(&m_HeightScale, m_GfxCore);
	}

	void Planet::RenderPlanet(float scale)
	{
		///Render the planet sphere.
		auto ScaleMatrix = Matrix4::Scale3D(scale, scale, scale)*Matrix4::Translate3D(m_SpacePosition.m_x, m_SpacePosition.m_y, m_SpacePosition.m_z);
		ScaleMatrix.Transpose();

		m_CurrentScene->m_WorldMatrix.UpdateConstantBuffer(&ScaleMatrix, m_GfxCore);
		m_CurrentScene->m_WorldMatrix.SetBufferInVS(2, m_GfxCore);
		m_CurrentScene->m_WorldMatrix.SetBufferInHS(1, m_GfxCore);
		m_CurrentScene->m_WorldMatrix.SetBufferInDS(2, m_GfxCore);

		m_GradientCB.SetBufferInDS(10, m_GfxCore);
		m_GradientsReal.SetBufferInDS(11, m_GfxCore);

		m_ScaleCB.UpdateConstantBuffer(&m_HeightScale, m_GfxCore);
		m_ScaleCB.SetBufferInDS(12, m_GfxCore);

		ResVault::Pointer()->GetResource<Material>(m_Materials).lock()->SetPlanetMaterial();

		static Gfx::RasterizerState wireframe;
		static bool Done = false;

		if (!Done)
		{
			Done = true;
			wireframe.Init(Gfx::FILL_WIREFRAME, Gfx::CULL_NONE);
			wireframe.CreateRasterizerState(m_GfxCore);
		}

		//wireframe.SetThisRasterizerState(m_GfxCore);
		m_Planet->DrawModel(m_GfxCore, ResVault::Pointer(), NULL);
		m_GfxCore->SetRSStateDefault();
	}

	void Planet::CreateMaterial()
	{
		StrPtr<Texture3D> tex3D = std::make_shared<Texture3D>();
		tex3D->LoadTexture3D("Models/PBR/grass/a.png", "Models/PBR/Cliff/a.png", "Models/PBR/crater/a.png", "Models/PBR/Slate/a.png", "TexA", true);
		
		StrPtr<Texture3D> tex3Dao = std::make_shared<Texture3D>();
		tex3Dao->LoadTexture3D("Models/PBR/grass/ao.png", "Models/PBR/Cliff/ao.png", "Models/PBR/crater/ao.png", "Models/PBR/Slate/ao.png", "TexAO", true);
		
		StrPtr<Texture3D> tex3Dr = std::make_shared<Texture3D>();
		tex3Dr->LoadTexture3D("Models/PBR/grass/r.png", "Models/PBR/Cliff/r.png", "Models/PBR/crater/r.png", "Models/PBR/Slate/r.png", "TexR", true);
		
		StrPtr<Texture3D> tex3Dm = std::make_shared<Texture3D>();
		tex3Dm->LoadTexture3D("Models/PBR/grass/m.png", "Models/PBR/Cliff/m.png", "Models/PBR/crater/m.png", "Models/PBR/Slate/m.png", "TexM", true);
		
		StrPtr<Texture3D> tex3Dn = std::make_shared<Texture3D>();
		tex3Dn->LoadTexture3D("Models/PBR/grass/n.png", "Models/PBR/Cliff/n.png", "Models/PBR/crater/n.png", "Models/PBR/Slate/n.png", "TexN", true);
		
		//StrPtr<Texture3D> tex3Dh = std::make_shared<Texture3D>();
		//tex3Dh->LoadTexture3D("Models/PBR/grass/h.png", "Models/PBR/Cliff/h.png", "Models/PBR/crater/h.png", "Models/PBR/Slate/h.png", "TexH", true);
		
		StrPtr<Material> mat = std::make_shared<Material>();
		mat->InitMaterial(MAT_PLANET, Gfx::GfxCore::Pointer());
		mat->SetFilePath("PlanetMaterialOne");
		ResVault* res = ResVault::Pointer();
		
		mat->m_TextureAlbedo = res->InsertResource(tex3D);
		mat->m_TextureAO = res->InsertResource(tex3Dao);
		mat->m_TextureRoughSmooth = res->InsertResource(tex3Dr);
		mat->m_TextureNormal = res->InsertResource(tex3Dn);
		mat->m_TextureMetSpec = res->InsertResource(tex3Dm);
		//mat->m_TextureH = res->InsertResource(tex3Dh);
		
		m_Materials = ResVault::Pointer()->InsertResource(mat);

		m_Planet->m_MaterialMap["DefaultMaterial"] = m_Materials;
	}

	Vector3 CubeToSphere(const Vector3 cubepoint)
	{
		float x2 = pow(cubepoint.m_x, 2);
		float y2 = pow(cubepoint.m_y, 2);
		float z2 = pow(cubepoint.m_z, 2);

		Vector3 posFinal;

		posFinal.m_x = cubepoint.m_x * sqrt(1.0f - y2 / 2.0f -
			z2 / 2.0f +
			y2 * z2 / 3.0f);

		posFinal.m_y = cubepoint.m_y * sqrt(1.0f - z2 / 2.0f -
			x2 / 2.0f +
			x2 * z2 / 3.0f);

		posFinal.m_z = cubepoint.m_z * sqrt(1.0f - x2 / 2.0f -
			y2 / 2.0f +
			y2 * x2 / 3.0f);

		return posFinal;
	}

	void Planet::LoadAndProcessModel()
	{
		m_Planet = std::make_shared<Model>();
		m_Planet->Load("Models/PlanetNew.dae", "PlanetModel");
	}
}