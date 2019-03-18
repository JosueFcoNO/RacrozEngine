#include <RZEnginePCH.h>

namespace rczEngine
{
	void PlanetQuadTreeNode::InitQuadTree(Planet* planetRef, PlanetQuadTreeNode* parent, PerlinNoise3D* noise, Vector3 StartPos, int32 ChildNumber, int32 depth, eMeshPlaneOrientation side)
	{
		ProfilerObj obj("InitQuadTree", PROFILE_EVENTS::PROF_GAME);

		for (int i = 0; i < 4; ++i)
			SetChildrenReady(i, false);

		planetRef->QuadTreesNodes++;

		PlanetOwner = planetRef;

		m_QuadTreeDepth = depth;

		Parent = parent;

		m_Side = side;

		double planeSize = 2.0;
		double Offset = 0.0;

		if (depth > 0)
		{
			planeSize /= (m_QuadTreeDepth*2.0);
			Offset = (planeSize / 2.0);
		}

		Vector2 signs;
		signs.m_x = CastStatic<float>((ChildNumber < 2) ? -1 : 1);
		signs.m_y = CastStatic<float>((ChildNumber == 0 || ChildNumber == 2) ? -1 : 1);

		Vector3 startPos;

		switch (side)
		{
		case rczEngine::Ypos:
			startPos.Set(Offset*signs.m_x, 1, Offset*signs.m_y);
			startPos += StartPos;
			startPos.m_y = 1;
			break;
		case rczEngine::Yneg:
			startPos.Set(Offset*signs.m_x, -1, Offset*signs.m_y);
			startPos += StartPos;
			startPos.m_y = -1;
			break;
		case rczEngine::Xpos:
			startPos.Set(1, Offset*signs.m_x, Offset*signs.m_y);
			startPos += StartPos;
			startPos.m_x = 1;
			break;
		case rczEngine::Xneg:
			startPos.Set(-1, Offset*signs.m_x, Offset*signs.m_y);
			startPos += StartPos;
			startPos.m_x = -1;
			break;
		case rczEngine::Zpos:
			startPos.Set(Offset*signs.m_x, Offset*signs.m_y, 1);
			startPos += StartPos;
			startPos.m_z = 1;
			break;
		case rczEngine::Zneg:
			startPos.Set(Offset*signs.m_x, Offset*signs.m_y, -1);
			startPos += StartPos;
			startPos.m_z = -1;
			break;
		}

		Noise = noise;
		InitMeshPlane(MESH_RES, planeSize, planeSize / 2.0, startPos, side, true, false);
		InitPlaneAndCorners();

		std::mutex buenMutex;
		buenMutex.lock();
		m_VertexBuffer.CreateVertexBuffer(Gfx::USAGE_DYNAMIC, false, Gfx::GfxCore::Pointer());
		buenMutex.unlock();

		m_Material = ResVault::Pointer()->FindResourceByName<Material>("lambert1").lock()->GetHandle();
	}

	void PlanetQuadTreeNode::Update(Vector3 playerPos)
	{
		ProfilerObj obj("PlanetQuadTreeNode::Update", PROFILE_EVENTS::PROF_GAME);

		CalculateLOD(playerPos);

		if (m_Dirty)
			for (int i = 0; i < 4; ++i)
			{
				if (CheckChildrenReady())
					Children[i]->Update(playerPos);
			}
	}

	void PlanetQuadTreeNode::DestroyQuadTreeNode() noexcept
	{
		DestroyMeshPlane();
	}

	void PlanetQuadTreeNode::InitPlaneAndCorners()
	{
		auto topleft = GetVertex(0, 0).VertexPosition;
		auto topright = GetVertex(MESH_ROW_SIZE, 0).VertexPosition;
		auto bottomright = GetVertex(MESH_ROW_SIZE, MESH_ROW_SIZE).VertexPosition;
		auto bottomleft = GetVertex(0, MESH_ROW_SIZE).VertexPosition;

		Planes[0].ConstructFromPoints(topleft, topright, topleft.GetNormalized() * 2);
		Planes[1].ConstructFromPoints(topright, bottomright, topright.GetNormalized() * 2);
		Planes[2].ConstructFromPoints(bottomright, bottomleft, bottomright.GetNormalized() * 2);
		Planes[3].ConstructFromPoints(bottomleft, topleft, bottomleft.GetNormalized() * 2);

		Planes[4].ConstructFromPoints(topleft, topright, bottomright);

		std::hash<String> hash;
		NodeConnection node;

		node.Depth = m_QuadTreeDepth;
		node.node = this;

		node.parentProxy = nullptr;
		node.Side = eSide::Up;
		node.Pos = GetVertex(MESH_ROW_HALF, 0).VertexPosition;
		node.Hash = HashCorner(node.Pos);
		GraphicDebugger::Pointer()->AddPoint("Rand1 " + std::to_string(rand()), node.Pos, 0.05f, Color(1, 0, 0));

		Connections.push_back(node);

		node.parentProxy = nullptr;
		node.Side = eSide::Left;
		node.Pos = GetVertex(0, MESH_ROW_HALF).VertexPosition;
		node.Hash = HashCorner(node.Pos);
		GraphicDebugger::Pointer()->AddPoint("Rand2 " + std::to_string(rand()), node.Pos, 0.05f, Color(1, 0, 0));

		Connections.push_back(node);

		node.parentProxy = nullptr;
		node.Side = eSide::Right;
		node.Pos = GetVertex(MESH_ROW_SIZE, MESH_ROW_HALF).VertexPosition;
		node.Hash = HashCorner(node.Pos);
		GraphicDebugger::Pointer()->AddPoint("Rand3 " + std::to_string(rand()), node.Pos, 0.05f, Color(1, 0, 0));

		Connections.push_back(node);

		node.parentProxy = nullptr;
		node.Side = eSide::Down;
		node.Pos = GetVertex(MESH_ROW_HALF, MESH_ROW_SIZE).VertexPosition;
		node.Hash = HashCorner(node.Pos);
		GraphicDebugger::Pointer()->AddPoint("Rand4 " + std::to_string(rand()), node.Pos, 0.05f, Color(1, 0, 0));

		Connections.push_back(node);

		if (Parent)
		{
			Connections.insert(Connections.end(), Parent->Connections.begin(), Parent->Connections.end());

			for (int i = 4; i < 8; ++i)
			{
				Connections[i].Depth = m_QuadTreeDepth;
				Connections[i].parentProxy = &Parent->Connections[i - 4];
			}
		}

		for (int i = 0; i < m_MeshBuffer.Size; ++i)
		{
			SideVertices[(int)eSide::Up].push_back(&GetVertex(i, 0));
		}

		for (int i = 0; i < m_MeshBuffer.Size; ++i)
		{
			SideVertices[(int)eSide::Right].push_back(&GetVertex(MESH_ROW_SIZE, i));
		}

		for (int i = 0; i < m_MeshBuffer.Size; ++i)
		{
			SideVertices[(int)eSide::Down].push_back(&GetVertex(i, MESH_ROW_SIZE));
		}

		for (int i = 0; i < m_MeshBuffer.Size; ++i)
		{
			SideVertices[(int)eSide::Left].push_back(&GetVertex(0, i));
		}
	}

	bool PlanetQuadTreeNode::TestIfInside(Vector3 pos)
	{
		return (Planes[0].SignedDistance(pos) < 0 &&
			Planes[1].SignedDistance(pos) < 0 &&
			Planes[2].SignedDistance(pos) < 0 &&
			Planes[3].SignedDistance(pos) < 0 &&
			Planes[4].SignedDistance(pos) > 0);
	}

	void PlanetQuadTreeNode::CalculateLOD(Vector3 pos)
	{
		ProfilerObj obj("CalculateLOD", PROFILE_EVENTS::PROF_GAME);

		ActiveTouch = TestIfInside(pos);

		if (ActiveTouch)
		{
			auto mag = pos.Magnitude() - 1.0f;

			if (1.0f / (m_QuadTreeDepth + 1) > mag)
			{
				m_Dirty = true;
			}
			else
			{
				m_Dirty = false;
				PlanetOwner->ActiveQuadTree = this;
			}
		}
		else
		{
			if (PlanetOwner->ActiveQuadTree)
			{
				if (PlanetOwner->ActiveQuadTree->GetQuadTreeDepth() <= m_QuadTreeDepth)
				{
					m_Dirty = false;
				}
			}
		}

		if (m_Dirty)
		{
			DeathTimer.StartTimer();

			if (!m_ChildGenerated && m_QuadTreeDepth < 256)
			{
				m_ChildGenerated = true;
				GenerateChildren();
			};
		}
		else
		{
			if (m_ChildGenerated && m_QuadTreeDepth > 0)
			{
				m_TimeTillDeath -= CastStatic<float>(DeathTimer.GetFrameTime());
				if (m_TimeTillDeath <= 0.0f)
				{
					m_TimeTillDeath = 0.1f;
					DestroyChildren();
				}
			}
		}
	}

	void PlanetQuadTreeNode::TestVisibility(Frustum & camFrustum, Vector<PlanetQuadTreeNode*>& nodesToDraw)
	{
		if (camFrustum.TestAABB(aabb))
		{
			if (CheckChildrenReady())
			{
				for (int i = 0; i < 4; ++i)
				{
					Children[i]->TestVisibility(camFrustum, nodesToDraw);
				}
			}
			else
			{
				nodesToDraw.push_back(this);
			}

			return;
		}
		else
		{
			if (CheckChildrenReady())
			{
				for (int i = 0; i < 4; ++i)
				{
					Children[i]->TestVisibility(camFrustum, nodesToDraw);
				}
			}

			return;
		}

	}

	void PlanetQuadTreeNode::ConnectNodesSameDepth(const NodeConnection & one, const NodeConnection & two)
	{
		auto& oneVertices = one.node->SideVertices[(int)one.Side];
		auto& twoVertices = two.node->SideVertices[(int)two.Side];

		if (oneVertices.size() == twoVertices.size())
		{
			if (Vector3::Distance(oneVertices[0]->VertexPosition, twoVertices[0]->VertexPosition) < 0.05f)
			{
				for (int i = 0; i < MESH_RES; ++i)
				{
					*oneVertices[i] = *twoVertices[i];
				}
			}
			else
			{
				for (int i = 0; i < MESH_RES; ++i)
				{
					*oneVertices[i] = *twoVertices[MESH_ROW_SIZE - i];
				}
			}

			one.node->m_MeshDirty = true;
			two.node->m_MeshDirty = true;
		}
		else
		{
			if (one.Depth == two.Depth) return;

			const NodeConnection* dobleNode;
			if (one.Depth > two.Depth)
			{
				dobleNode = &one;
			}
			else if (one.Depth < two.Depth)
			{
				dobleNode = &two;
			}

			Vector<TerrainVertex*>* DeepSide = &oneVertices;
			Vector<TerrainVertex*>* ShallowSide = &twoVertices;

			if (oneVertices.size() > twoVertices.size())
			{
				DeepSide = &oneVertices;
				ShallowSide = &twoVertices;
			}
			else
			{
				DeepSide = &twoVertices;
				ShallowSide = &oneVertices;
			}

			if (Vector3::Distance(oneVertices[0]->VertexPosition, twoVertices[0]->VertexPosition) < 0.05f)
			{
				for (int i = 0, k = 0; i < MESH_RES; ++i, k+= 2)
				{
					*(*DeepSide)[k]  = *(*ShallowSide)[i];
				}
			}
			else
			{
				for (int i = 0, k = 0; i < MESH_RES; ++i, k += 2)
				{
					*(*DeepSide)[MESH_RES * 2 - 2 - k]  = *(*ShallowSide)[i];
				}
			}

			for (int i = 1; i < MESH_RES*2-1; i+=2)
			{
				auto correctedIndex = i;

				auto PrevDeep = (*DeepSide)[correctedIndex - 1];
				auto CurrentDeep = (*DeepSide)[correctedIndex];
				
				*CurrentDeep = *PrevDeep;
			}

			if (&one != dobleNode)
				one.node->m_MeshDirty = true;

			if (&two != dobleNode)
				two.node->m_MeshDirty = true;

			dobleNode->node->Children[0]->m_MeshDirty = true;
			dobleNode->node->Children[1]->m_MeshDirty = true;
			dobleNode->node->Children[2]->m_MeshDirty = true;
			dobleNode->node->Children[3]->m_MeshDirty = true;
		}
	}

	void PlanetQuadTreeNode::Render()
	{
		const auto resPtr = ResVault::Pointer();
		const auto gfxPtr = Gfx::GfxCore::Pointer();

		if (!done)
		{
			//Vector<Vector3> Points = aabb.GetCorners();
			//auto gDebug = GraphicDebugger::Pointer();
			//
			//Vector<uint32> Indices;
			//Indices.push_back(0); Indices.push_back(1);
			//Indices.push_back(0); Indices.push_back(2);
			//Indices.push_back(1); Indices.push_back(3);
			//Indices.push_back(2); Indices.push_back(3);
			//
			//Indices.push_back(0); Indices.push_back(4);
			//Indices.push_back(1); Indices.push_back(5);
			//Indices.push_back(2); Indices.push_back(6);
			//Indices.push_back(3); Indices.push_back(7);
			//
			//Indices.push_back(4); Indices.push_back(5);
			//Indices.push_back(4); Indices.push_back(6);
			//Indices.push_back(5); Indices.push_back(7);
			//Indices.push_back(6); Indices.push_back(7);
			//
			//AABB_Debug = gDebug->AddLineListIndex("AABB" + std::to_string(rand()), Points, Indices, Color(1, 0, 1), -1.0f);

			done = true;
		}

		auto ptr = resPtr->GetResource<Material>(m_Material).lock();

		if (ActiveTouch)
			ptr->m_core.g_Albedo.Set(1.0f, 0.0f, 0.0f);
		else
			ptr->m_core.g_Albedo.Set(1.0f, 1.0f, 1.0f);

		ptr->SetThisMaterial(gfxPtr, resPtr);

		//AABB_Debug.lock()->Active(!m_Dirty);

		if (m_Dirty)
		{
			if (CheckChildrenReady())
				return RenderChildren();
		}

		MeshPlane::Render();

	}

	Vector3 PlanetQuadTreeNode::CalculateVertexPos(Vector3 pos)
	{
		ProfilerObj obj("CalculateVertexPos", PROFILE_EVENTS::PROF_GAME);

		Vector3 PosNormal = Math::CubeToSphere(pos);
		Vector3 NoisePos = PosNormal + Vector3(1.0f, 1.0f, 1.0f);
		NoisePos *= 0.5f;

		double x, y, z;
		x = NoisePos.m_x;
		y = NoisePos.m_y;
		z = NoisePos.m_z;
		Vector3 Pos128 = { gsl::narrow_cast<float>(x*128.0), gsl::narrow_cast<float>(y*128.0), gsl::narrow_cast<float>(z*128.0) };
		Vector3 Pos12 = { gsl::narrow_cast<float>(x*12.0), gsl::narrow_cast<float>(y*12.0), gsl::narrow_cast<float>(z*12.0) };
		Vector3 Pos24 = { gsl::narrow_cast<float>(x*24.0), gsl::narrow_cast<float>(y*24.0), gsl::narrow_cast<float>(z*24.0) };

		float noise;

		noise = //pow(Noise->RidgedOctaveNoise(Pos128, 6, 0.4f), 2);//  *
			//Noise->OctaveNoise(Pos24, 6, 0.5f) *
			//Noise->OctaveNoise(NoisePos, 3, 0.5f);
			pow(Noise->RidgedOctaveNoise(Pos12, 3, 0.2f), 2);

		auto finalPos = PosNormal + (PosNormal.GetNormalized())*noise*.02f;
		//finalPos *= 30.0f;

		aabb.AddPoint(finalPos);

		return finalPos;
	}

	void PlanetQuadTreeNode::GenerateChildren()
	{
		DeathTimer.StartTimer();

		int32 depth;

		if (m_QuadTreeDepth == 0)
			depth = 1;
		else if (m_QuadTreeDepth >= 1)
			depth = m_QuadTreeDepth * 2;

		for (int i = 0; i < 4; ++i)
		{
			m_ChildThread[i] = std::thread(&PlanetQuadTreeNode::GenerateChild, this, i, depth);
			m_ChildThread[i].detach();
		}

	}

	void PlanetQuadTreeNode::DestroyChildren()
	{
		if (m_ChildGenerated)
		{
			if (CheckChildrenReady())
			{
				for (int i = 0; i < 4; ++i)
				{
					Children[i]->DestroyChildren();
					Children[i]->DestroyQuadTreeNode();
					//delete Children[i];
					Children[i] = nullptr;
					SetChildrenReady(i, false);
				}

				m_ChildGenerated = false;
			}

		}
	}

	void PlanetQuadTreeNode::RenderChildren()
	{
		for (int i = 0; i < 4; ++i)
		{
			Children[i]->Render();
		}
	}

	void PlanetQuadTreeNode::GenerateChild(int index, int depth)
	{
		Children[index] = new PlanetQuadTreeNode();
		Children[index]->InitQuadTree(PlanetOwner, this, Noise, m_StartPos, index, depth, m_Side);
		SetChildrenReady(index, true);
	}

	bool PlanetQuadTreeNode::CheckChildrenReady()
	{
		std::mutex BuenMutex;
		bool retValue;

		BuenMutex.lock();
		retValue =
			ChildrenReady[0] &&
			ChildrenReady[1] &&
			ChildrenReady[2] &&
			ChildrenReady[3];

		if (retValue && SideVertices[0].size() == MESH_RES)
		{
			UpdateSideVertices();
		}

		BuenMutex.unlock();

		return retValue;
	}

	void PlanetQuadTreeNode::UpdateSideVertices()
	{
		SideVertices[0].clear();
		SideVertices[1].clear();
		SideVertices[2].clear();
		SideVertices[3].clear();

		auto side = (int)eSide::Up;
		SideVertices[side].insert(SideVertices[side].end(), Children[0]->SideVertices[side].begin(), Children[0]->SideVertices[side].end());
		SideVertices[side].pop_back();
		SideVertices[side].insert(SideVertices[side].end(), Children[2]->SideVertices[side].begin(), Children[2]->SideVertices[side].end());

		side = (int)eSide::Right;
		SideVertices[side].insert(SideVertices[side].end(), Children[2]->SideVertices[side].begin(), Children[2]->SideVertices[side].end());
		SideVertices[side].pop_back();
		SideVertices[side].insert(SideVertices[side].end(), Children[3]->SideVertices[side].begin(), Children[3]->SideVertices[side].end());

		side = (int)eSide::Left;
		SideVertices[side].insert(SideVertices[side].end(), Children[0]->SideVertices[side].begin(), Children[0]->SideVertices[side].end());
		SideVertices[side].pop_back();
		SideVertices[side].insert(SideVertices[side].end(), Children[1]->SideVertices[side].begin(), Children[1]->SideVertices[side].end());

		side = (int)eSide::Down;
		SideVertices[side].insert(SideVertices[side].end(), Children[1]->SideVertices[side].begin(), Children[1]->SideVertices[side].end());
		SideVertices[side].pop_back();
		SideVertices[side].insert(SideVertices[side].end(), Children[3]->SideVertices[side].begin(), Children[3]->SideVertices[side].end());
	}

	void PlanetQuadTreeNode::SetChildrenReady(int indexOfChild, bool value)
	{
		std::mutex BuenMutex;

		BuenMutex.lock();
		ChildrenReady[indexOfChild] = value;
		BuenMutex.unlock();
	}

	uint32 PlanetQuadTreeNode::HashCorner(Vector3 v)
	{
		auto temp = (v + Vector3(1.0f, 1.0f, 1.0f)) * 10.0f;
		return Vector3::Hash(temp);
	}
}
