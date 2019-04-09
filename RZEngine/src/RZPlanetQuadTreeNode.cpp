#include <RZEnginePCH.h>

namespace rczEngine
{
	void PlanetQuadTreeNode::InitQuadTree(Planet* planetRef, PlanetQuadTreeNode* parent, PerlinNoise3D* noise, Vector3 StartPos, int32 ChildNumber, int32 depth, eMeshPlaneOrientation side)
	{
		ProfilerObj obj("InitQuadTree", PROFILE_EVENTS::PROF_GAME);

		for (int i = 0; i < 4; ++i)
			SetChildrenReady(i, false);

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
		InitMeshPlane(Mesh_Res, planeSize, planeSize / 2.0, startPos, side, true, false);
		InitPlaneAndCorners();

		std::mutex buenMutex;
		buenMutex.lock();
		m_VertexBuffer.CreateVertexBuffer(Gfx::USAGE_DYNAMIC, false, Gfx::GfxCore::Pointer());
		buenMutex.unlock();

		m_Material = ResVault::Pointer()->FindResourceByName<Material>("lambert1").lock()->GetHandle();
	}

	void PlanetQuadTreeNode::Subdivide()
	{
		m_Subdivided = true;

	}

	void PlanetQuadTreeNode::Update(Vector3 playerPos)
	{
		ProfilerObj obj("PlanetQuadTreeNode::Update", PROFILE_EVENTS::PROF_GAME);

		CalculateLOD(playerPos);

		if (m_Subdivided)
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
		auto topright = GetVertex(Mesh_Row_Size, 0).VertexPosition;
		auto bottomright = GetVertex(Mesh_Row_Size, Mesh_Row_Size).VertexPosition;
		auto bottomleft = GetVertex(0, Mesh_Row_Size).VertexPosition;

		m_NodePlanes[0].ConstructFromPoints(topleft, topright, topleft * 2);
		m_NodePlanes[1].ConstructFromPoints(topright, bottomright, topright * 2);
		m_NodePlanes[2].ConstructFromPoints(bottomright, bottomleft, bottomright * 2);
		m_NodePlanes[3].ConstructFromPoints(bottomleft, topleft, bottomleft * 2);

		m_NodePlanes[4].ConstructFromPoints(topleft, topright, bottomright);

		NodeConnection node;

		node.Depth = m_QuadTreeDepth;
		node.node = this;

		node.Side = eSide::Up;
		node.Pos = GetVertex(Mesh_Row_Half, 0).VertexPosition;
		node.Hash = HashCorner(node.Pos);
		GraphicDebugger::Pointer()->AddPoint("Rand1 " + std::to_string(rand()), node.Pos, 1, Color(1, 0, 0));

		Connections.push_back(node);

		node.Side = eSide::Left;
		node.Pos = GetVertex(0, Mesh_Row_Half).VertexPosition;
		node.Hash = HashCorner(node.Pos);
		GraphicDebugger::Pointer()->AddPoint("Rand2 " + std::to_string(rand()), node.Pos, 1, Color(1, 0, 0));

		Connections.push_back(node);

		node.Side = eSide::Right;
		node.Pos = GetVertex(Mesh_Row_Size, Mesh_Row_Half).VertexPosition;
		node.Hash = HashCorner(node.Pos);
		GraphicDebugger::Pointer()->AddPoint("Rand3 " + std::to_string(rand()), node.Pos, 1, Color(1, 0, 0));

		Connections.push_back(node);

		node.Side = eSide::Down;
		node.Pos = GetVertex(Mesh_Row_Half, Mesh_Row_Size).VertexPosition;
		node.Hash = HashCorner(node.Pos);
		GraphicDebugger::Pointer()->AddPoint("Rand4 " + std::to_string(rand()), node.Pos, 1, Color(1, 0, 0));

		Connections.push_back(node);

		TerrainVertex* vertex;
		for (int i = 0; i < Size; ++i)
		{
			vertex = &GetVertex(i, 0);
			SideVertices[(int)eSide::Up].push_back(vertex);
		}

		for (int i = 0; i < Size; ++i)
		{
			vertex = &GetVertex(Mesh_Row_Size, i);
			SideVertices[(int)eSide::Right].push_back(vertex);
		}

		for (int i = 0; i < Size; ++i)
		{
			vertex = &GetVertex(i, Mesh_Row_Size);
			SideVertices[(int)eSide::Down].push_back(vertex);
		}

		for (int i = 0; i < Size; ++i)
		{
			vertex = &GetVertex(0, i);
			SideVertices[(int)eSide::Left].push_back(vertex);
		}
	}

	bool PlanetQuadTreeNode::TestIfInside(const Vector3& pos)
	{
		return (m_NodePlanes[0].SignedDistance(pos) < 0 &&
			m_NodePlanes[1].SignedDistance(pos) < 0 &&
			m_NodePlanes[2].SignedDistance(pos) < 0 &&
			m_NodePlanes[3].SignedDistance(pos) < 0 &&
			m_NodePlanes[4].SignedDistance(pos) > 0);
	}

	void PlanetQuadTreeNode::CalculateLOD(const Vector3& pos)
	{
		ProfilerObj obj("CalculateLOD", PROFILE_EVENTS::PROF_GAME);

		m_PlayerInside = TestIfInside(pos);

		if (m_PlayerInside)
		{
			auto mag = pos.Magnitude() - 100.0f;

			if ((1.0f / (m_QuadTreeDepth + 1)) 
				* 100.0f > mag)
			{
				m_Subdivided = true;
			}
			else
			{
				m_Subdivided = false;
				PlanetOwner->ActiveQuadTree = this;
			}
		}
		else
		{
			if (PlanetOwner->ActiveQuadTree)
			{
				if (PlanetOwner->ActiveQuadTree->GetQuadTreeDepth() <= m_QuadTreeDepth)
				{
					m_Subdivided = false;
				}
			}
		}

		if (m_Subdivided)
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

	void PlanetQuadTreeNode::TestVisibility(const Frustum & camFrustum, Vector<PlanetQuadTreeNode*>& nodesToDraw)
	{
		if (camFrustum.TestAABB(m_MeshAABB))
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
			return;
		}

	}

	void PlanetQuadTreeNode::ConnectNodesSameDepth(const NodeConnection & one, const NodeConnection & two)
	{
		auto& oneVertices = one.node->SideVertices[(int)one.Side];
		auto& twoVertices = two.node->SideVertices[(int)two.Side];

		if (oneVertices.size() == twoVertices.size())
		{
			if (Vector3::Distance(oneVertices[0]->VertexPosition, twoVertices[0]->VertexPosition) < 0.005f)
			{
				for (int i = 0; i < Mesh_Res; ++i)
				{
					*oneVertices[i] = *twoVertices[i];
				}
			}
			else
			{
				for (int i = 0; i < Mesh_Res; ++i)
				{
					*oneVertices[i] = *twoVertices[Mesh_Row_Size - i];
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
				for (int i = 0, k = 0; i < Mesh_Res; ++i, k+= 2)
				{
					*(*DeepSide)[k]  = *(*ShallowSide)[i];
				}
			}
			else
			{
				for (int i = 0, k = 0; i < Mesh_Res; ++i, k += 2)
				{
					*(*DeepSide)[Mesh_Res * 2 - 2 - k]  = *(*ShallowSide)[i];
				}
			}

			for (int i = 1; i < Mesh_Res*2-1; i+=2)
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

			if (dobleNode->node->CheckChildrenReady())
			{
				dobleNode->node->Children[0]->m_MeshDirty = true;
				dobleNode->node->Children[1]->m_MeshDirty = true;
				dobleNode->node->Children[2]->m_MeshDirty = true;
				dobleNode->node->Children[3]->m_MeshDirty = true;
			}
		}
	}

	void PlanetQuadTreeNode::Render()
	{
		const auto resPtr = ResVault::Pointer();
		const auto gfxPtr = Gfx::GfxCore::Pointer();

		if (!done)
		{
			Vector<Vector3> Points = m_MeshAABB.GetCorners();
			auto gDebug = GraphicDebugger::Pointer();
			
			Vector<uint32> Indices;
			Indices.push_back(0); Indices.push_back(1);
			Indices.push_back(0); Indices.push_back(2);
			Indices.push_back(1); Indices.push_back(3);
			Indices.push_back(2); Indices.push_back(3);
			
			Indices.push_back(0); Indices.push_back(4);
			Indices.push_back(1); Indices.push_back(5);
			Indices.push_back(2); Indices.push_back(6);
			Indices.push_back(3); Indices.push_back(7);
			
			Indices.push_back(4); Indices.push_back(5);
			Indices.push_back(4); Indices.push_back(6);
			Indices.push_back(5); Indices.push_back(7);
			Indices.push_back(6); Indices.push_back(7);
			
			AABB_Debug = gDebug->AddLineListIndex("_AABB" + std::to_string(rand()), Points, Indices, Color(1, 0, 1), -1.0f);
			done = true;
		}

		auto ptr = resPtr->GetResource<Material>(PlanetOwner->m_Materials).lock();

		if (m_PlayerInside)
			ptr->m_core.g_Albedo.Set(1.0f, 0.0f, 0.0f);
		else
			ptr->m_core.g_Albedo.Set(1.0f, 1.0f, 1.0f);

		ptr->SetThisMaterial(gfxPtr, resPtr);

		AABB_Debug.lock()->Active(!m_Subdivided);

		if (m_Subdivided)
		{
			if (CheckChildrenReady())
				return RenderChildren();
		}

		MeshPlane::Render();

	}

	Vector3 PlanetQuadTreeNode::CalculateVertexPos(const Vector3& pos, float& out_displacement)
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

		noise = pow(Noise->RidgedOctaveNoise(Pos128, 6, 0.4f), 2) *
			Noise->OctaveNoise(Pos24, 6, 0.5f) *
			Noise->OctaveNoise(NoisePos, 3, 0.5f) * 
			pow(Noise->RidgedOctaveNoise(Pos12, 3, 0.2f), 2);

		out_displacement = noise;

		auto finalPos = PosNormal + (PosNormal.GetNormalized())*noise*.03f;
		finalPos *= 100.0f;

		m_MeshAABB.AddPoint(finalPos);

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

		if (retValue && SideVertices[0].size() == Mesh_Res)
		{
			UpdateSideVertices();
		}

		BuenMutex.unlock();

		return retValue;
	}

	void PlanetQuadTreeNode::UpdateSideVertices()
	{
		return;
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
		auto temp = (v + Vector3(1.0f, 1.0f, 1.0f)) * 100.0f;
		return Vector3::Hash(temp);
	}

	void MeshPlane::GenerateMesh(const Vector3& startPos)
	{
		ProfilerObj meshY("GenerateMesh", PROFILE_EVENTS::PROF_GAME);

		TerrainVertex* TempVertex;
		auto size = Size;

		double halfSize = HalfSize;

		double LastX = -halfSize - distVertex;
		double LastZ = -halfSize;

		for (int32 x = 0; x < size; ++x)
		{
			for (int32 y = 0; y < size; ++y)
			{
				TempVertex = &GetVertex(x, y);

				TempVertex->VertexPosition.m_x = LastX += distVertex;
				TempVertex->VertexPosition.m_y = 0.0f;
				TempVertex->VertexPosition.m_z = LastZ;

				TempVertex->VertexPosition += startPos;

				TempVertex->VertexPosition = CalculateVertexPos(TempVertex->VertexPosition, TempVertex->Displacement);

				TempVertex->TextureCoordinates.m_x = float(y)  * distVertex * 10;
				TempVertex->TextureCoordinates.m_y = float(x)  * distVertex * 10;
			}

			LastZ += distVertex;
			LastX = -halfSize - distVertex;
		}
	}

	void MeshPlane::GenerateMeshYNeg(const Vector3& startPos)
	{
		ProfilerObj meshY("GenerateMeshYNeg", PROFILE_EVENTS::PROF_GAME);

		TerrainVertex* TempVertex;
		auto size = Size;
		auto vertexSize = m_VertexBuffer.GetSize();

		double halfSize = HalfSize;

#pragma omp parallel for
		for (uint32 i = 0; i < vertexSize; ++i)
		{
			TempVertex = &m_VertexBuffer.GetVertex(i);

			int32 x = i / size;
			int32 y = i % size;

			TempVertex->VertexPosition.m_x = CastStatic<float>(((distVertex*(size - y)) - halfSize) - distVertex);
			TempVertex->VertexPosition.m_y = 0.0f;
			TempVertex->VertexPosition.m_z = CastStatic<float>(((distVertex*(size - x)) - halfSize) - distVertex);

			TempVertex->VertexPosition += startPos;

			TempVertex->VertexPosition = CalculateVertexPos(TempVertex->VertexPosition, TempVertex->Displacement);

			//m_MeshAABB.AddPoint(TempVertex->VertexPosition);

			TempVertex->TextureCoordinates.m_x = float(y)  * distVertex * 10;
			TempVertex->TextureCoordinates.m_y = float(x)  * distVertex * 10;
		}
	}

	void MeshPlane::GenerateMeshXPos(const Vector3& startPos)
	{
		TerrainVertex* TempVertex;
		auto size = Size;
		auto vertexSize = m_VertexBuffer.GetSize();

		double halfSize = HalfSize;

#pragma omp parallel for
		for (uint32 i = 0; i < vertexSize; ++i)
		{
			TempVertex = &m_VertexBuffer.GetVertex(i);

			int32 x = i / size;
			int32 y = i % size;

			TempVertex->VertexPosition.m_x = 0.0f;
			TempVertex->VertexPosition.m_y = CastStatic<float>((distVertex*y) - halfSize);
			TempVertex->VertexPosition.m_z = CastStatic<float>((distVertex*x) - halfSize);

			TempVertex->VertexPosition += startPos;

			TempVertex->VertexPosition = CalculateVertexPos(TempVertex->VertexPosition, TempVertex->Displacement);

			//m_MeshAABB.AddPoint(TempVertex->VertexPosition);

			TempVertex->TextureCoordinates.m_x = float(y)  * distVertex * 10;
			TempVertex->TextureCoordinates.m_y = float(x)  * distVertex * 10;
		}
	}

	void MeshPlane::GenerateMeshXNeg(const Vector3& startPos)
	{
		TerrainVertex* TempVertex;
		auto size = Size;
		auto vertexSize = m_VertexBuffer.GetSize();

		double Size = size * distVertex;
		double halfSize = HalfSize;

#pragma omp parallel for
		for (uint32 i = 0; i < vertexSize; ++i)
		{
			TempVertex = &m_VertexBuffer.GetVertex(i);

			int32 x = i / size;
			int32 y = i % size;

			TempVertex->VertexPosition.m_x = 0.0f;
			TempVertex->VertexPosition.m_y = CastStatic<float>(((distVertex*(size - x)) - halfSize) - distVertex);
			TempVertex->VertexPosition.m_z = CastStatic<float>(((distVertex*(size - y)) - halfSize) - distVertex);

			TempVertex->VertexPosition += startPos;

			TempVertex->VertexPosition = CalculateVertexPos(TempVertex->VertexPosition, TempVertex->Displacement);

			//m_MeshAABB.AddPoint(TempVertex->VertexPosition);

			TempVertex->TextureCoordinates.m_x = float(y)  * distVertex * 10;
			TempVertex->TextureCoordinates.m_y = float(x)  * distVertex * 10;
		}
	}

	void MeshPlane::GenerateMeshZPos(const Vector3& startPos)
	{
		TerrainVertex* TempVertex;
		auto size = Size;
		auto vertexSize = m_VertexBuffer.GetSize();

		double Size = size * distVertex;
		double halfSize = HalfSize;

#pragma omp parallel for
		for (uint32 i = 0; i < vertexSize; ++i)
		{
			TempVertex = &m_VertexBuffer.GetVertex(i);

			int32 x = i / size;
			int32 y = i % size;

			TempVertex->VertexPosition.m_x = CastStatic<float>((distVertex*y) - halfSize);
			TempVertex->VertexPosition.m_y = CastStatic<float>((distVertex*x) - halfSize);
			TempVertex->VertexPosition.m_z = 0.0f;

			TempVertex->VertexPosition += startPos;

			TempVertex->VertexPosition = CalculateVertexPos(TempVertex->VertexPosition, TempVertex->Displacement);

			//m_MeshAABB.AddPoint(TempVertex->VertexPosition);

			TempVertex->TextureCoordinates.m_x = float(y)  * distVertex * 10;
			TempVertex->TextureCoordinates.m_y = float(x)  * distVertex * 10;
		}
	}

	void MeshPlane::GenerateMeshZNeg(const Vector3& startPos)
	{
		TerrainVertex* TempVertex;
		auto size = Size;
		auto vertexSize = m_VertexBuffer.GetSize();

		double Size = size * distVertex;
		double halfSize = HalfSize;

#pragma omp parallel for
		for (uint32 i = 0; i < vertexSize; ++i)
		{
			TempVertex = &m_VertexBuffer.GetVertex(i);

			int32 x = i / size;
			int32 y = i % size;

			TempVertex->VertexPosition.m_x = CastStatic<float>(((distVertex*(size - x)) - halfSize) - distVertex);
			TempVertex->VertexPosition.m_y = CastStatic<float>(((distVertex*(size - y)) - halfSize) - distVertex);
			TempVertex->VertexPosition.m_z = 0.0f;

			TempVertex->VertexPosition += startPos;

			TempVertex->VertexPosition = CalculateVertexPos(TempVertex->VertexPosition, TempVertex->Displacement);

			//m_MeshAABB.AddPoint(TempVertex->VertexPosition);

			TempVertex->TextureCoordinates.m_x = float(y)  * distVertex * 10;
			TempVertex->TextureCoordinates.m_y = float(x)  * distVertex * 10;
		}
	}
}
