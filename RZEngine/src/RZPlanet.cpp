#include <RZEnginePCH.h>

namespace rczEngine
{
	class RacrozRenderer;

	void Planet::InitPlanet(int32 seed, float x, float y, float z, SpaceManager* spaceMng)
	{
		m_SpaceMng = spaceMng;
		m_CurrentScene = SceneManager::Pointer()->GetActiveScene();
		m_GfxCore = Gfx::GfxCore::Pointer();

		LoadAndProcessModel();
		auto patchSize = PlanetQuadTreeNode::Mesh_Res * PlanetQuadTreeNode::Mesh_Res;

		m_VertexVector.resize(PATCH_NUMBER);
		m_VertexBuffer.ResizeVertexVector(PATCH_NUMBER * patchSize);

		int index = 0;
		for (auto& verticesVector : m_VertexVector)
		{
			verticesVector.resize(patchSize);

			for (auto i = 0; i < patchSize; ++i)
			{
				verticesVector[i] = &m_VertexBuffer.GetVertex(i + index*patchSize);
			}

			++index;
		}

		m_VertexBuffer.CreateVertexBuffer(Gfx::USAGE_DYNAMIC, false, m_GfxCore);

		PlayerCamera = CameraManager::Pointer()->GetActiveCamera().lock();

		noise.InitPerlinNoise(seed);

		for (int i = 0; i < 6; ++i)
		{
			m_QTreeRoots[i].InitQuadTree(this, nullptr, &noise, Vector3(0, 0, 0), 0, 0, (eMeshPlaneOrientation)i);
		}

		m_HeightCameracb.CreateConstantBuffer(sizeof(Vector4), Gfx::USAGE_DEFAULT, m_GfxCore);

		CreateMaterial();

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

		if (ImGUIEditor::Pointer()->Wireframe)
			wireframe.SetThisRasterizerState(m_GfxCore);

		Vector3 PlayerPos = PlayerCamera->GetPosition();

		for (int i = 0; i < 6; ++i)
		{
			m_QTreeRoots[i].Update(PlayerPos);
		}

		auto& frustum = PlayerCamera->m_CameraCore.GetFrustum();

		///Get all the nodes visibles by their AABB.
		Vector<PlanetQuadTreeNode*> nodesToDraw;
		for (int i = 0; i < 6; ++i)
		{
			m_QTreeRoots[i].TestVisibility(frustum, nodesToDraw);
		}

		///Of all the nodes visible, get their connection points that are visible using the frustum and put them in the adyacent node info list.
		for (auto nodes : nodesToDraw)
		{ 
			for (auto& c : nodes->Connections)
			{
				//if (frustum.TestPoint(c.Pos))
				//{
					ProcessConnectionNode(c);
				//}
			}
		}

		Set<PlanetQuadTreeNode*> dividers;

		for (auto cNode : m_PatchInfo)
		{
			int maxDepth = 0;
			for (auto connect : cNode.second.Connectors)
			{
				if (connect->Depth > maxDepth)
				{
					maxDepth = connect->Depth;
				}
			}

			for (auto connect : cNode.second.Connectors)
			{
				if (connect->Depth < maxDepth/2)
				{
					dividers.insert(connect->node);
				}
			}
		}

		for (auto divs : dividers)
		{
			if (frustum.TestAABB(divs->GetAABB()))
			{
				divs->SubdivideNode();
			}
		}

		m_PatchInfo.clear();

		for (auto nodes : nodesToDraw)
		{
			//if (nodes->m_MeshDirty)
			//{
				//nodes->m_MeshDirty = false;
				//nodes->m_VertexBuffer.UpdateVertexBuffer(m_GfxCore);
			//}
		}

		m_SpaceMng->m_AtmosValues.UpdateConstantBuffer(&m_Atmosphere, m_GfxCore);
		m_SpaceMng->m_AtmosValues.SetBufferInPS(10, m_GfxCore);
		m_SpaceMng->m_AtmosValues.SetBufferInVS(10, m_GfxCore);

		Vector3 cPos = CameraManager::Pointer()->GetActiveCamera().lock()->GetPosition();

		m_HeightCamera.m_x = (m_SpacePosition - cPos).Magnitude();
		m_HeightCamera.m_y = pow(m_HeightCamera.m_x, 2.0f);

		m_HeightCameracb.UpdateConstantBuffer(&m_HeightCamera, m_GfxCore);
		m_HeightCameracb.SetBufferInVS(11, m_GfxCore);
		m_HeightCameracb.SetBufferInPS(11, m_GfxCore);

		m_VertexBuffer.UpdateVertexBuffer(m_GfxCore);
		m_VertexBuffer.SetThisVertexBuffer(m_GfxCore, 0);

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

	void Planet::RenderAtmosphere()
	{
		m_SpaceMng->m_AtmosValues.UpdateConstantBuffer(&m_Atmosphere, m_GfxCore);
		m_SpaceMng->m_AtmosValues.SetBufferInPS(10, m_GfxCore);
		m_SpaceMng->m_AtmosValues.SetBufferInVS(10, m_GfxCore);

		const auto scale = m_Atmosphere.OuterRadius;

		///Render the planet sphere.
		Matrix4 ScaleMatrix;
		ScaleMatrix = Matrix4::Scale3D(scale, scale, scale)*Matrix4::Translate3D(m_SpacePosition.m_x, m_SpacePosition.m_y, m_SpacePosition.m_z);
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
		mat->InitMaterial(eMaterialType::PlanetSurface);
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

	PatchIndex Planet::CreateNewNode(Vector<TerrainVertex*>*& vertexVector)
	{
		int chosenPatch = -1;
		for (;chosenPatch < PATCH_NUMBER;)
		{
			++chosenPatch;
			if (!m_ActivePatches[chosenPatch]) break;
		}

		vertexVector = &m_VertexVector[chosenPatch];
		m_ActivePatches[chosenPatch] = true;
		return chosenPatch;
	}

	void Planet::ReleaseNode(PatchIndex index)
	{
		m_ActivePatches[index] = false;
	}

	void Planet::LoadAndProcessModel()
	{
		m_Planet = std::make_shared<Model>();
		m_Planet->Load("RacrozEngineAssets/SphereHigh.fbx", "PlanetModel");
	}

	void Planet::ProcessConnectionNode(NodeConnection & node)
	{
		m_PatchInfo[node.Hash].Connectors.push_back(&node);
	}
}