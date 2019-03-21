#include <RZEnginePCH.h>

namespace rczEngine
{
	class RacrozRenderer;

	void Planet::InitPlanet(int32 seed, float x, float y, float z, SpaceManager* spaceMng)
	{
		instance = this;

		QuadTreesNodes = 0;
		Connections = 0;

		m_SpaceMng = spaceMng;
		m_CurrentScene = SceneManager::Pointer()->GetActiveScene();
		m_GfxCore = Gfx::GfxCore::Pointer();

		LoadAndProcessModel();

		PlayerCamera = CameraManager::Pointer()->GetActiveCamera().lock();

		noise.InitPerlinNoise(seed);

		for (int i = 0; i < 6; ++i)
		{
			Quadtree[i].InitQuadTree(this, nullptr, &noise, Vector3(0, 0, 0), 0, 0, (eMeshPlaneOrientation)i);
		}

		m_HeightCameracb.CreateConstantBuffer(sizeof(Vector4), Gfx::USAGE_DEFAULT, m_GfxCore);

		//LoadAndProcessModel();
		//CreateMaterial();

		m_SpacePosition.Set(x, y, z);

		Vector3	gradients[12] = {
			Vector3(1,1,0),Vector3(-1,1,0),Vector3(1,-1,0),Vector3(-1,-1,0),
			Vector3(1,0,1),Vector3(-1,0,1),Vector3(1,0,-1),Vector3(-1,0,-1),
			Vector3(0,1,1),Vector3(0,-1,1),Vector3(0,1,-1),Vector3(0,-1,-1)
		};

		///Render the planet sphere.
		m_PlanetMatrix = Matrix4::Scale3D(1.0f, 1.0f, 1.0f)*Matrix4::Translate3D(m_SpacePosition.m_x, m_SpacePosition.m_y, m_SpacePosition.m_z);
		m_PlanetMatrix.Transpose();

		m_PosView.CreateConstantBuffer(sizeof(Vector4), Gfx::USAGE_DEFAULT, m_GfxCore);
		m_PosView.UpdateConstantBuffer(&PlanetPos, m_GfxCore);

		m_GradientsReal.CreateConstantBuffer(sizeof(Vector4) * 256, Gfx::USAGE_DEFAULT, m_GfxCore);
		m_GradientsReal.UpdateConstantBuffer(PermutationTable, m_GfxCore);

		m_GradientCB.CreateConstantBuffer(sizeof(Vector3) * 12, Gfx::USAGE_DEFAULT, m_GfxCore);
		m_GradientCB.UpdateConstantBuffer(gradients, m_GfxCore);

		m_HeightScale.m_x = 1.0f;

		m_HeightScale.Set(1.0f, 2.0f, 1.0f, 0.0f);
		m_ScaleCB.CreateConstantBuffer(sizeof(Vector4), Gfx::USAGE_DEFAULT, m_GfxCore);
		m_ScaleCB.UpdateConstantBuffer(&m_HeightScale, m_GfxCore);

		m_GradientCB.SetBufferInPS(10, m_GfxCore);
		m_GradientsReal.SetBufferInPS(11, m_GfxCore);

		m_ScaleCB.UpdateConstantBuffer(&m_HeightScale, m_GfxCore);
		m_ScaleCB.SetBufferInPS(12, m_GfxCore);
	}

	void Planet::RenderPlanet(float scale)
	{
		m_CurrentScene->m_WorldMatrix.UpdateConstantBuffer(&m_PlanetMatrix, m_GfxCore);
		m_CurrentScene->m_WorldMatrix.SetBufferInVS(2, m_GfxCore);
		m_CurrentScene->m_WorldMatrix.SetBufferInHS(1, m_GfxCore);
		m_CurrentScene->m_WorldMatrix.SetBufferInDS(2, m_GfxCore);

		//ResVault::Pointer()->GetResource<Material>(m_Materials).lock()->SetThisMaterial(Gfx::GfxCore::Pointer(), ResVault::Pointer());

		static Gfx::RasterizerState wireframe;
		static bool Done = false;

		if (!Done)
		{
			Done = true;
			wireframe.Init(Gfx::FILL_WIREFRAME, Gfx::CULL_BACK);
			wireframe.CreateRasterizerState(m_GfxCore);
		}

		if (GUIEditor::Pointer()->Wireframe)
			wireframe.SetThisRasterizerState(m_GfxCore);

		Vector3 PlayerPos = PlayerCamera->GetPosition();

		for (int i = 0; i < 6; ++i)
		{
			Quadtree[i].Update(PlayerPos);
		}

		Vector<PlanetQuadTreeNode*> nodesToDraw;
		for (int i = 0; i < 6; ++i)
		{
			Quadtree[i].TestVisibility(PlayerCamera->m_CameraCore.GetFrustum(), nodesToDraw);
		}

		Map<PlanetQuadTreeNode*, int> depthMap;

		for (auto nodes : nodesToDraw)
		{
			for (auto& c : nodes->Connections)
			{
				ProcessConnectionNode(c);

				if (depthMap.find(c.node) != depthMap.end())
				{
					auto depth = c.node->GetQuadTreeDepth();

					if (depthMap[c.node]*2 > depth)
					{
						depthMap[c.node] = depth;
						c.node->m_Dirty = true;
						continue;
					}
				}
				else
				{
					depthMap[c.node] = c.node->GetQuadTreeDepth();
				}
			}
		}

		for (auto nodes : nodesToDraw)
		{
			if (nodes->m_MeshDirty)
			{
				nodes->m_MeshDirty = false;
				nodes->m_VertexBuffer.UpdateVertexBuffer(m_GfxCore);
			}
		}

		for (auto nodes : nodesToDraw)
		{
			nodes->Render();
		}

		m_GfxCore->SetRSStateDefault();
	}

	void Planet::RenderPlanetWater(float scale)
	{
		ResVault::Pointer()->GetResource<Material>(Water).lock()->SetThisMaterial();
		m_Planet->DrawModel(NULL);
	}

	void Planet::RenderAtmosphere(float scale)
	{
		///Render the planet sphere.
		Matrix4 ScaleMatrix;
		ScaleMatrix = Matrix4::Scale3D(scale + 0.5f, scale + 0.5f, scale + 0.5f)*Matrix4::Translate3D(m_SpacePosition.m_x, m_SpacePosition.m_y, m_SpacePosition.m_z);
		ScaleMatrix.Transpose();

		Vector3 cPos = CameraManager::Pointer()->GetActiveCamera().lock()->GetPosition();

		m_HeightCamera.m_x = (m_SpacePosition - cPos).Magnitude();
		m_HeightCamera.m_y = pow(m_HeightCamera.m_x, 2.0f);

		m_HeightCameracb.UpdateConstantBuffer(&m_HeightCamera, m_GfxCore);
		m_HeightCameracb.SetBufferInVS(11, m_GfxCore);

		m_CurrentScene->m_WorldMatrix.UpdateConstantBuffer(&ScaleMatrix, m_GfxCore);
		m_CurrentScene->m_WorldMatrix.SetBufferInVS(2, m_GfxCore);

		m_Planet->DrawModel(NULL);
	}

	void Planet::CreateMaterial()
	{
		StrPtr<Texture3D> tex3D = std::make_shared<Texture3D>();
		tex3D->LoadTexture3D("Models/PBR/Sand/a.png", "Models/PBR/grass/a.png", "Models/PBR/Slate/a.png", "Models/PBR/Snow/a.jpg", "TexA", true);

		StrPtr<Texture3D> tex3Dao = std::make_shared<Texture3D>();
		tex3Dao->LoadTexture3D("Models/PBR/Sand/ao.png", "Models/PBR/grass/ao.png", "Models/PBR/Slate/ao.png", "Models/PBR/Snow/ao.jpg", "TexAO", true);

		StrPtr<Texture3D> tex3Dr = std::make_shared<Texture3D>();
		tex3Dr->LoadTexture3D("Models/PBR/Sand/r.png", "Models/PBR/grass/r.png", "Models/PBR/Slate/r.png", "Models/PBR/Snow/r.jpg", "TexR", true);

		StrPtr<Texture3D> tex3Dm = std::make_shared<Texture3D>();
		tex3Dm->LoadTexture3D("Models/PBR/Sand/m.png", "Models/PBR/grass/m.png", "Models/PBR/Slate/m.png", "Models/PBR/Snow/m.png", "TexM", true);

		StrPtr<Texture3D> tex3Dn = std::make_shared<Texture3D>();
		tex3Dn->LoadTexture3D("Models/PBR/Sand/n.png", "Models/PBR/grass/n.png", "Models/PBR/Slate/n.png", "Models/PBR/Snow/n.jpg", "TexN", true);

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

		//mat = std::make_shared<Material>();
		//mat->InitMaterial(MAT_PBR_MetRough, Gfx::GfxCore::Pointer());
		//mat->SetFilePath("PlanetMaterialWater");
		//Water = res->InsertResource(mat);
	}

	void Planet::LoadAndProcessModel()
	{
		m_Planet = std::make_shared<Model>();
		m_Planet->Load("RacrozEngineAssets/EsferaLowPoly.fbx", "PlanetModel");
	}

	void Planet::ProcessConnectionNode(NodeConnection & node)
	{
		if (m_PatchInfo.find(node.Hash) != m_PatchInfo.end())
		{
			auto& patch = m_PatchInfo[node.Hash];

			///TODO: Check if i'm not doing this twice everytime.
			if (patch.Connected)
			{
				if (node.parentProxy == patch.ConnectorOne)
				{
					if (node.Depth == patch.OneConnectedDepth)
					{
						return;
					}
					else
					{
						patch.ConnectorOne = &node;
						patch.OneConnectedDepth = node.Depth;
						PlanetQuadTreeNode::ConnectNodesSameDepth(*patch.ConnectorOne, *patch.ConnectorTwo);
					}
				}

				if (node.parentProxy == patch.ConnectorTwo)
				{
					if (node.Depth == patch.TwoConnectedDepth)
					{
						return;
					}
					else
					{
						patch.ConnectorTwo = &node;
						patch.TwoConnectedDepth = node.Depth;
						PlanetQuadTreeNode::ConnectNodesSameDepth(*patch.ConnectorOne, *patch.ConnectorTwo);
					}
				}
			}

			if (patch.ConnectorOne == nullptr || patch.ConnectorOne == &node)
			{
				patch.ConnectorOne = &node;
			}
			else
			{

				if (!patch.Connected)
				{
					patch.ConnectorTwo = &node;

					if (patch.ConnectorOne->Depth > patch.ConnectorTwo->Depth * 2)
					{
						patch.ConnectorOne->node->m_Dirty = true;
						patch.ConnectorOne = nullptr;
						return;
					}

					if (patch.ConnectorTwo->Depth > patch.ConnectorOne->Depth * 2)
					{
						patch.ConnectorTwo->node->m_Dirty = true;
						patch.ConnectorTwo = nullptr;
						return;
					}

					patch.Connected = true;
					patch.OneConnectedDepth = patch.ConnectorOne->Depth;
					patch.TwoConnectedDepth = node.Depth;

					PlanetQuadTreeNode::ConnectNodesSameDepth(*patch.ConnectorOne, *patch.ConnectorTwo);
				}
			}
		}
		else
		{
			PatchData newData;
			newData.ConnectorOne = &node;
			newData.Connected = false;

			m_PatchInfo[node.Hash] = newData;
		}
	}
}