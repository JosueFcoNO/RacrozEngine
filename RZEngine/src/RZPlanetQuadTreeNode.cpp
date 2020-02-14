#include <RZEnginePCH.h>

namespace rczEngine
{
	void PlanetQuadTreeNode::InitQuadTree(Planet* planetRef, PlanetQuadTreeNode* parent, PerlinNoise3D* noise, Vector3 StartPos, int32 ChildNumber, int32 depth, eMeshPlaneOrientation side)
	{
#ifdef RZ_PROFILING
		ProfilerObj obj("InitQuadTree", PROFILE_EVENTS::PROF_GAME);
#endif

		for (int i = 0; i < QUAD_DIVS; ++i)
			SetChildrenReady(i, false);

		PlanetOwner = planetRef;

		planetRef->NewQuad();

		m_QuadTreeDepth = depth;

		Parent = parent;

		m_Side = side;

		double planeSize = 2.0;
		double Offset = 0.0;

		if (depth > 0)
		{
			planeSize /= (m_QuadTreeDepth);
			Offset = (planeSize);
		}

		Vector2 signs;

		switch (ChildNumber)
		{
		case 0:
		case 3:
		case 6:
			signs.m_x = -1;
			break;
			
		case 1:
		case 4:
		case 7:
			signs.m_x = 0;
			break;

		case 2:
		case 5:
		case 8:
			signs.m_x = 1;
			break;
		}

		switch (ChildNumber)
		{
		case 0:
		case 1:
		case 2:
			signs.m_y = -1;
			break;

		case 3:
		case 4:
		case 5:
			signs.m_y = 0;
			break;

		case 6:
		case 7:
		case 8:
			signs.m_y = 1;
			break;
		}
		
		m_ChildNumber = ChildNumber;

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

		m_StartPos = startPos;
		Noise = noise;

		Size = Mesh_Res;
		distVertex = planeSize / double(Mesh_Res - 1);
		HalfSize = planeSize / 2.0;

		MeshPlane::GenerateIndices(Mesh_Res, m_IndexBuffer);
		
		m_Index = PlanetOwner->CreateNewNode(m_CurrentVertices);

		GenerateMesh(side);
		GenerateNormals();

		ConstructPlanesAndCorners();

		m_Material = ResVault::Pointer()->FindResourceByName<Material>("lambert1").lock()->GetHandle();
	}

	void PlanetQuadTreeNode::SubdivideNode()
	{
		m_Subdivided = true;
	}

	void PlanetQuadTreeNode::Update(Vector3 playerPos)
	{
#ifdef RZ_PROFILING
		ProfilerObj obj("PlanetQuadTreeNode::Update", PROFILE_EVENTS::PROF_GAME);
#endif

		CalculateLOD(playerPos);

		if (m_Subdivided)
			for (int i = 0; i < QUAD_DIVS; ++i)
			{
				if (CheckChildrenReady())
					Children[i]->Update(playerPos);
			}
	}

	void PlanetQuadTreeNode::DestroyQuadTreeNode() noexcept
	{
		PlanetOwner->ReleaseNode(m_Index);

		m_Index = -1;
		m_CurrentVertices = nullptr;
	}

	void PlanetQuadTreeNode::GenerateNormals()
	{
#ifdef RZ_PROFILING
		ProfilerObj obj("GenerateNormals", PROFILE_EVENTS::PROF_GAME);
#endif

		auto size = Size;
		auto indexSize = m_IndexBuffer.GetSize();

		//Le saco normales a todo.
#pragma omp parallel for
		for (uint32 i = 0; i < indexSize; i += 3)
		{
			auto index1 = m_IndexBuffer.GetIndex(i);
			auto index2 = m_IndexBuffer.GetIndex(i + 1);
			auto index3 = m_IndexBuffer.GetIndex(i + 2);

			TerrainVertex* Vert1 = m_CurrentVertices->at(index1);
			TerrainVertex* Vert2 = m_CurrentVertices->at(index2);
			TerrainVertex* Vert3 = m_CurrentVertices->at(index3);

			Vector3 V1 = (Vert1->VertexPosition - Vert2->VertexPosition).GetNormalized();
			Vector3 V2 = (Vert1->VertexPosition - Vert3->VertexPosition).GetNormalized();

			Vert1->VertexNormals = Vert2->VertexNormals = Vert3->VertexNormals = (V1^V2).GetNormalized();
			Vert1->Tangents = Vert2->Tangents = Vert3->Tangents = (V1);
			Vert1->BiNormals = Vert2->BiNormals = Vert3->BiNormals = (V2);
		}

		auto index1 = m_IndexBuffer.GetIndex(indexSize - 1);
		auto index2 = m_IndexBuffer.GetIndex(indexSize - 2);
		auto index3 = m_IndexBuffer.GetIndex(indexSize - 3);

		TerrainVertex* Vert1 = m_CurrentVertices->at(index1);
		TerrainVertex* Vert2 = m_CurrentVertices->at(index2);
		TerrainVertex* Vert3 = m_CurrentVertices->at(index3);

		Vector3 V1 = (Vert1->VertexPosition - Vert2->VertexPosition).GetNormalized();
		Vector3 V2 = (Vert1->VertexPosition - Vert3->VertexPosition).GetNormalized();

		Vert1->VertexNormals = Vert2->VertexNormals = Vert3->VertexNormals = (V1^V2).GetNormalized();
		Vert1->Tangents = Vert2->Tangents = Vert3->Tangents = (V1);
		Vert1->BiNormals = Vert2->BiNormals = Vert3->BiNormals = (V2);
	}

	void PlanetQuadTreeNode::GenerateSmoothNormals()
	{
#ifdef RZ_PROFILING
		ProfilerObj obj("GenerateSmoothNormals", PROFILE_EVENTS::PROF_GAME);
#endif

		auto size = Size;
		auto bufferSize = m_CurrentVertices->size();

		//Le saco normales suaves ya a todo.
		for (uint32 i = 0; i < bufferSize; ++i)
		{
			TerrainVertex* ThisVertex = m_CurrentVertices->at(i);

			int32 x = i % size;
			int32 y = i / size;

			TerrainVertex* NearbyVertices[8];
			int32 VerticesUsed = 0;

			if (i + size > bufferSize)
			{
				continue;
			}

			if (x > 0)
			{
				NearbyVertices[VerticesUsed] = m_CurrentVertices->at(i - 1);
				++VerticesUsed;
			}

			if (x < size - 1)
			{
				NearbyVertices[VerticesUsed] = m_CurrentVertices->at(i + 1);
				++VerticesUsed;
			}

			if (y > 0)
			{
				if (x > 0)
				{
					NearbyVertices[VerticesUsed] = m_CurrentVertices->at(i - 1 - size);
					++VerticesUsed;
				}

				NearbyVertices[VerticesUsed] = m_CurrentVertices->at(i - size);
				++VerticesUsed;

				if (x < size - 1)
				{
					NearbyVertices[VerticesUsed] = m_CurrentVertices->at(i + 1 - size);
					++VerticesUsed;
				}
			}

			if (y < size - 1)
			{
				if (x > 0)
				{
					NearbyVertices[VerticesUsed] = m_CurrentVertices->at(i - 1 + size);
					++VerticesUsed;
				}

				NearbyVertices[VerticesUsed] = m_CurrentVertices->at(i + size);
				++VerticesUsed;

				if (x < size - 1)
				{
					NearbyVertices[VerticesUsed] = m_CurrentVertices->at(i + 1 + size);
					++VerticesUsed;
				}
			}

			Vector3 normalAvg;
			Vector3 binormalAvg;
			Vector3 TangentAvg;

			for (int32 k = 0; k < VerticesUsed; ++k)
			{
				normalAvg += NearbyVertices[k]->VertexNormals;
				binormalAvg += NearbyVertices[k]->BiNormals;
				TangentAvg += NearbyVertices[k]->Tangents;
			}

			normalAvg /= float(VerticesUsed + 1);
			binormalAvg /= float(VerticesUsed + 1);
			TangentAvg /= float(VerticesUsed + 1);
			//
			ThisVertex->VertexNormals = normalAvg.GetNormalized();
			ThisVertex->BiNormals = binormalAvg.GetNormalized();
			ThisVertex->Tangents = TangentAvg.GetNormalized();
		}
	}

	void PlanetQuadTreeNode::ConstructPlanesAndCorners()
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
		node.Hash = Vector3::Hash(node.Pos.GetNormalized());

		Connections.push_back(node);

		node.Side = eSide::Left;
		node.Pos = GetVertex(0, Mesh_Row_Half).VertexPosition;
		node.Hash = Vector3::Hash(node.Pos.GetNormalized());

		Connections.push_back(node);

		node.Side = eSide::Right;
		node.Pos = GetVertex(Mesh_Row_Size, Mesh_Row_Half).VertexPosition;
		node.Hash = Vector3::Hash(node.Pos.GetNormalized());

		Connections.push_back(node);

		node.Side = eSide::Down;
		node.Pos = GetVertex(Mesh_Row_Half, Mesh_Row_Size).VertexPosition;
		node.Hash = Vector3::Hash(node.Pos.GetNormalized());

		Connections.push_back(node);
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
#ifdef RZ_PROFILING
		ProfilerObj obj("CalculateLOD", PROFILE_EVENTS::PROF_GAME);
#endif
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

			if (!m_ChildGenerated && m_QuadTreeDepth < 80)
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
				for (int i = 0; i < QUAD_DIVS; ++i)
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

	void PlanetQuadTreeNode::Render()
	{
		const auto resPtr = ResVault::Pointer();
		const auto gfxPtr = Gfx::GfxCore::Pointer();

		//if (!done)
		//{
		//	Vector<Vector3> Points = m_MeshAABB.GetCorners();
		//	auto gDebug = GraphicDebugger::Pointer();
		//
		//	Vector<uint32> Indices;
		//	Indices.push_back(0); Indices.push_back(1);
		//	Indices.push_back(0); Indices.push_back(2);
		//	Indices.push_back(1); Indices.push_back(3);
		//	Indices.push_back(2); Indices.push_back(3);
		//
		//	Indices.push_back(0); Indices.push_back(4);
		//	Indices.push_back(1); Indices.push_back(5);
		//	Indices.push_back(2); Indices.push_back(6);
		//	Indices.push_back(3); Indices.push_back(7);
		//
		//	Indices.push_back(4); Indices.push_back(5);
		//	Indices.push_back(4); Indices.push_back(6);
		//	Indices.push_back(5); Indices.push_back(7);
		//	Indices.push_back(6); Indices.push_back(7);
		// 
		//	AABB_Debug = gDebug->AddLineListIndex("_AABB" + std::to_string(rand()), Points, Indices, Color(1, 0, 1), -1.0f);
		//	done = true;
		//}

		auto ptr = resPtr->GetResource<Material>(PlanetOwner->m_Materials).lock();

		if (m_PlayerInside)
			ptr->m_core.g_Albedo.Set(1.0f, 0.0f, 0.0f);
		else
			ptr->m_core.g_Albedo.Set(1.0f, 1.0f, 1.0f);

		ptr->SetThisMaterial(gfxPtr, resPtr);

		//AABB_Debug.lock()->Active(!m_Subdivided);

		if (m_Subdivided)
		{
			if (CheckChildrenReady())
				return RenderChildren();
		}

		int32 vertexSize;

		m_IndexBuffer.SetThisIndexBuffer(gfxPtr);
		vertexSize = m_IndexBuffer.m_IndexSize;

		gfxPtr->DrawIndexed(vertexSize, 0, m_Index * Math::Pow(PlanetQuadTreeNode::Mesh_Row_Size, 2));

	}

	Vector3 PlanetQuadTreeNode::CalculateVertexPos(const Vector3& pos, float& out_displacement)
	{
#ifdef RZ_PROFILING
		ProfilerObj obj("CalculateVertexPos", PROFILE_EVENTS::PROF_GAME);
#endif

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
			//Noise->OctaveNoise(NoisePos, 3, 0.5f) *
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
			depth = 3;
		else if (m_QuadTreeDepth >= 3)
			depth = m_QuadTreeDepth * 3;

		for (int i = 0; i < QUAD_DIVS; ++i)
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
				for (int i = 0; i < QUAD_DIVS; ++i)
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

	void PlanetQuadTreeNode::GenerateMesh(eMeshPlaneOrientation side)
	{
		switch (side)
		{
		case rczEngine::Ypos:
			GenerateMeshYPos(m_StartPos);
			break;
		case rczEngine::Yneg:
			GenerateMeshYNeg(m_StartPos);
			break;
		case rczEngine::Xpos:
			GenerateMeshXPos(m_StartPos);
			break;
		case rczEngine::Xneg:
			GenerateMeshXNeg(m_StartPos);
			break;
		case rczEngine::Zpos:
			GenerateMeshZPos(m_StartPos);
			break;
		case rczEngine::Zneg:
			GenerateMeshZNeg(m_StartPos);
			break;
		default:
			break;
		}
	}

	void PlanetQuadTreeNode::RenderChildren()
	{
		for (int i = 0; i < QUAD_DIVS; ++i)
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

		BuenMutex.unlock();

		return retValue;
	}

	void PlanetQuadTreeNode::SetChildrenReady(int indexOfChild, bool value)
	{
		std::mutex BuenMutex;

		BuenMutex.lock();
		ChildrenReady[indexOfChild] = value;
		BuenMutex.unlock();
	}

	void PlanetQuadTreeNode::GenerateMeshYPos(const Vector3& startPos)
	{
#ifdef RZ_PROFILING
		ProfilerObj meshY("GenerateMesh", PROFILE_EVENTS::PROF_GAME);
#endif

		TerrainVertex* TempVertex;
		auto vertexSize = m_CurrentVertices->size();

		double halfSize = HalfSize;

#pragma omp parallel for
		for (uint32 i = 0; i < vertexSize; ++i)
		{
			TempVertex = m_CurrentVertices->at(i);

			int32 x = i / Size;
			int32 y = i % Size;

			TempVertex->VertexPosition.m_x = CastStatic<float>(((distVertex*x) - halfSize));
			TempVertex->VertexPosition.m_y = 0.0f;
			TempVertex->VertexPosition.m_z = CastStatic<float>(((distVertex*y) - halfSize));

			TempVertex->VertexPosition += startPos;

			TempVertex->VertexPosition = CalculateVertexPos(TempVertex->VertexPosition, TempVertex->Displacement);

			//m_MeshAABB.AddPoint(TempVertex->VertexPosition);

			TempVertex->TextureCoordinates.m_x = float(y)  * distVertex * 10;
			TempVertex->TextureCoordinates.m_y = float(x)  * distVertex * 10;
		}
	}

	void PlanetQuadTreeNode::GenerateMeshYNeg(const Vector3& startPos)
	{
#ifdef RZ_PROFILING
		ProfilerObj meshY("GenerateMeshYNeg", PROFILE_EVENTS::PROF_GAME);
#endif

		TerrainVertex* TempVertex;
		auto size = Size;
		auto vertexSize = m_CurrentVertices->size();

		double halfSize = HalfSize;

#pragma omp parallel for
		for (uint32 i = 0; i < vertexSize; ++i)
		{
			TempVertex = m_CurrentVertices->at(i);

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

	void PlanetQuadTreeNode::GenerateMeshXPos(const Vector3& startPos)
	{
		TerrainVertex* TempVertex;
		auto size = Size;
		auto vertexSize = m_CurrentVertices->size();

		double halfSize = HalfSize;

#pragma omp parallel for
		for (uint32 i = 0; i < vertexSize; ++i)
		{
			TempVertex = m_CurrentVertices->at(i);

			int32 x = i / size;
			int32 y = i % size;

			TempVertex->VertexPosition.m_x = 0.0f;
			TempVertex->VertexPosition.m_y = CastStatic<float>((distVertex*y) - halfSize);
			TempVertex->VertexPosition.m_z = CastStatic<float>((distVertex*x) - halfSize);

			TempVertex->VertexPosition += startPos;

			TempVertex->VertexPosition = CalculateVertexPos(TempVertex->VertexPosition, TempVertex->Displacement);

			TempVertex->TextureCoordinates.m_x = float(y)  * distVertex * 10;
			TempVertex->TextureCoordinates.m_y = float(x)  * distVertex * 10;
		}

	}

	void PlanetQuadTreeNode::GenerateMeshXNeg(const Vector3& startPos)
	{
		TerrainVertex* TempVertex;
		auto size = Size;
		auto vertexSize = m_CurrentVertices->size();

		double halfSize = HalfSize;

#pragma omp parallel for
		for (uint32 i = 0; i < vertexSize; ++i)
		{
			TempVertex = m_CurrentVertices->at(i);

			int32 x = i / size;
			int32 y = i % size;

			TempVertex->VertexPosition.m_x = 0.0f;
			TempVertex->VertexPosition.m_y = CastStatic<float>(((distVertex*(size - x)) - halfSize) - distVertex);
			TempVertex->VertexPosition.m_z = CastStatic<float>(((distVertex*(size - y)) - halfSize) - distVertex);

			TempVertex->VertexPosition += startPos;

			TempVertex->VertexPosition = CalculateVertexPos(TempVertex->VertexPosition, TempVertex->Displacement);

			TempVertex->TextureCoordinates.m_x = float(y)  * distVertex * 10;
			TempVertex->TextureCoordinates.m_y = float(x)  * distVertex * 10;
		}

	}

	void PlanetQuadTreeNode::GenerateMeshZPos(const Vector3& startPos)
	{
		TerrainVertex* TempVertex;
		auto size = Size;
		auto vertexSize = m_CurrentVertices->size();

		double halfSize = HalfSize;

#pragma omp parallel for
		for (uint32 i = 0; i < vertexSize; ++i)
		{
			TempVertex = m_CurrentVertices->at(i);

			int32 x = i / size;
			int32 y = i % size;

			TempVertex->VertexPosition.m_x = CastStatic<float>((distVertex*y) - halfSize);
			TempVertex->VertexPosition.m_y = CastStatic<float>((distVertex*x) - halfSize);
			TempVertex->VertexPosition.m_z = 0.0f;

			TempVertex->VertexPosition += startPos;

			TempVertex->VertexPosition = CalculateVertexPos(TempVertex->VertexPosition, TempVertex->Displacement);

			TempVertex->TextureCoordinates.m_x = float(y)  * distVertex * 10;
			TempVertex->TextureCoordinates.m_y = float(x)  * distVertex * 10;
		}

	}

	void PlanetQuadTreeNode::GenerateMeshZNeg(const Vector3& startPos)
	{
		TerrainVertex* TempVertex;
		auto size = Size;
		auto vertexSize = m_CurrentVertices->size();

		double halfSize = HalfSize;

#pragma omp parallel for
		for (uint32 i = 0; i < vertexSize; ++i)
		{
			TempVertex = m_CurrentVertices->at(i);

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
