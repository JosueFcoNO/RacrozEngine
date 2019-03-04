#include <RZEnginePCH.h>

namespace rczEngine
{
	void PlanetQuadTreeNode::InitQuadTree(Planet* planetRef, PerlinNoise3D* noise, Vector3 StartPos, int32 ChildNumber, int32 depth, eMeshPlaneOrientation side, ParentSidesData sideData)
	{
		ProfilerObj obj("InitQuadTree", PROFILE_EVENTS::PROF_GAME);

		for (int i = 0; i < 4; ++i)
			SetChildrenReady(i, false);

		planetRef->QuadTreesNodes++;

		PlanetOwner = planetRef;

		m_QuadTreeDepth = depth;

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

		auto topleft = GetVertex(0, 0).VertexPosition;
		auto topright = GetVertex(MESH_ROW_SIZE, 0).VertexPosition;
		auto bottomright = GetVertex(MESH_ROW_SIZE, MESH_ROW_SIZE).VertexPosition;
		auto bottomleft = GetVertex(0, MESH_ROW_SIZE).VertexPosition;

		Planes[0].ConstructFromPoints(topleft, topright, topleft.GetNormalized() * 2);
		Planes[1].ConstructFromPoints(topright, bottomright, topright.GetNormalized() * 2);
		Planes[2].ConstructFromPoints(bottomright, bottomleft, bottomright.GetNormalized() * 2);
		Planes[3].ConstructFromPoints(bottomleft, topleft, bottomleft.GetNormalized() * 2);

		Planes[4].ConstructFromPoints(topleft, topright, bottomright);

		m_CornersID[0] = HashCorner(topleft.GetNormalized());
		PlanetOwner->m_NodeAdyacency.insert(Pair<uint32, PlanetQuadTreeNode*>(m_CornersID[0], this));

		m_CornersID[1] = HashCorner(topright.GetNormalized());
		PlanetOwner->m_NodeAdyacency.insert(Pair<uint32, PlanetQuadTreeNode*>(m_CornersID[1], this));

		m_CornersID[2] = HashCorner(bottomright.GetNormalized());
		PlanetOwner->m_NodeAdyacency.insert(Pair<uint32, PlanetQuadTreeNode*>(m_CornersID[2], this));

		m_CornersID[3] = HashCorner(bottomleft.GetNormalized());
		PlanetOwner->m_NodeAdyacency.insert(Pair<uint32, PlanetQuadTreeNode*>(m_CornersID[3], this));

		Map<PlanetQuadTreeNode*, int> AdyMap;

		for (int i = 0; i < 4; ++i)
		{
			auto patch = PlanetOwner->m_NodeAdyacency.equal_range(m_CornersID[i]);

			for (auto node = patch.first; node != patch.second; ++node)
			{
				if (node->second == this) continue;

				if (AdyMap.find(node->second) == AdyMap.end()) AdyMap[node->second] = 0;

				++AdyMap[node->second];
			}
		}

		if (AdyMap.size() > 0)
		{
			for (auto Connectee : AdyMap)
			{
				if (Connectee.second > 0) //Same height.
					Connect(this, Connectee.first);
			}
		}

		std::mutex buenMutex;
		buenMutex.lock();
		m_VertexBuffer.CreateVertexBuffer(Gfx::USAGE_DYNAMIC, false, m_gfx);
		buenMutex.unlock();

		m_Material = m_res->FindResourceByName<Material>("lambert1").lock()->GetHandle();
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

	bool PlanetQuadTreeNode::TestIfAdyacent(PlanetQuadTreeNode * Node)
	{
		auto centerVertex = m_VertexBuffer.GetVertex(MESH_RES * MESH_RES / 2 + MESH_ROW_SIZE / 2 + 1).VertexPosition;

		auto diff = (centerVertex - m_VertexBuffer.GetVertex(MESH_RES * MESH_RES / 2 + MESH_ROW_SIZE / 2 + 2).VertexPosition).Magnitude();

		GraphicDebugger::Pointer()->AddFrameDebuggerPoint(centerVertex, 0.0025f, 1, 0, 0);

		auto Left = m_VertexBuffer.GetVertex(MESH_ROW_SIZE / 2).VertexPosition;
		Left += (Left - centerVertex).GetNormalized()*diff;
		Left *= 1.5f;

		GraphicDebugger::Pointer()->AddFrameDebuggerPoint(Left, 0.0025f, 1, 0, 0);
		if (Node->TestIfInside(Left))
		{
			return true;
		}

		auto Right = m_VertexBuffer.GetVertex(MESH_RES * MESH_RES / 2).VertexPosition;
		Right += (Right - centerVertex).GetNormalized()*diff;
		Right *= 1.5f;

		GraphicDebugger::Pointer()->AddFrameDebuggerPoint(Right, 0.0025f, 1, 0, 0);
		if (Node->TestIfInside(Right))
		{
			return true;
		}

		auto Up = m_VertexBuffer.GetVertex(MESH_RES * MESH_RES / 2 + (MESH_RES - 1)).VertexPosition;
		Up += (Up - centerVertex).GetNormalized()*diff;
		Up *= 1.5f;

		GraphicDebugger::Pointer()->AddFrameDebuggerPoint(Up, 0.0025f, 1, 0, 0);
		if (Node->TestIfInside(Up))
		{
			return true;
		}

		auto Down = m_VertexBuffer.GetVertex(MESH_RES * (MESH_RES - 1) + (MESH_RES / 2)).VertexPosition;
		Down += (Down - centerVertex).GetNormalized()*diff;
		Down *= 1.5f;

		GraphicDebugger::Pointer()->AddFrameDebuggerPoint(Down, 0.0025f, 1, 0, 0);
		if (Node->TestIfInside(Down))
		{
			return true;
		}

		auto A = m_VertexBuffer.GetVertex(0).VertexPosition;
		A += (A - centerVertex).GetNormalized()*diff;
		A *= 1.5f;

		GraphicDebugger::Pointer()->AddFrameDebuggerPoint(A, 0.0025f, 1, 0, 0);
		if (Node->TestIfInside(A))
		{
			return true;
		}

		auto B = m_VertexBuffer.GetVertex(MESH_ROW_SIZE).VertexPosition;
		B += (B - centerVertex).GetNormalized()*diff;
		B *= 1.5f;

		GraphicDebugger::Pointer()->AddFrameDebuggerPoint(B, 0.0025f, 1, 0, 0);
		if (Node->TestIfInside(B))
		{
			return true;
		}

		auto C = m_VertexBuffer.GetVertex(MESH_RES*(MESH_ROW_SIZE)).VertexPosition;
		C += (C - centerVertex).GetNormalized()*diff;
		C *= 1.5f;
		GraphicDebugger::Pointer()->AddFrameDebuggerPoint(C, 0.0025f, 1, 0, 0);
		if (Node->TestIfInside(C))
		{
			return true;
		}

		auto D = m_VertexBuffer.GetVertex(MESH_RES*MESH_RES - 1).VertexPosition;
		D += (D - centerVertex).GetNormalized()*diff;
		D *= 1.5f;
		GraphicDebugger::Pointer()->AddFrameDebuggerPoint(D, 0.0025f, 1, 0, 0);
		if (Node->TestIfInside(D))
		{
			return true;
		}

		return false;
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

		if (TestIfInside(pos))
		{
			ActiveTouch = true;

			auto mag = pos.Magnitude() - 1;

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
			ActiveTouch = false;

			int targetDepth = 0;

			for (int i = 0; i < 4; ++i)
			{
				auto ady = PlanetOwner->m_NodeAdyacency.equal_range(m_CornersID[i]);

				for (auto node = ady.first; node != ady.second && node != PlanetOwner->m_NodeAdyacency.end(); ++node)
				{
					if (node->second->m_QuadTreeDepth > m_QuadTreeDepth * 2)
					{
						m_Dirty = true;
						i = 5; //Exit both fors
						break;
					}
				};
			}

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

	void PlanetQuadTreeNode::GetSide(eSide side, Vector<Gfx::Vertex*>& out_VertexList)
	{
		switch (side)
		{
		case eSide::Up_Reverse:
		case eSide::Up:
			for (int32 i = 0; i < MESH_RES; ++i) out_VertexList.push_back(&GetVertex(i, 0));
			break;
		case eSide::Down_Reverse:
		case eSide::Down:
			for (int32 i = 0; i < MESH_RES; ++i) out_VertexList.push_back(&GetVertex(i, MESH_ROW_SIZE));
			break;
		case eSide::Left_Reverse:
		case eSide::Left:
			for (int32 i = 0; i < MESH_RES; ++i) out_VertexList.push_back(&GetVertex(0, i));
			break;
		case eSide::Right_Reverse:
		case eSide::Right:
			for (int32 i = 0; i < MESH_RES; ++i) out_VertexList.push_back(&GetVertex(MESH_ROW_SIZE, i));
			break;
		default:
			break;
		}
	}

	eSide PlanetQuadTreeNode::GetSideFromCorners(uint32 cornerOne, uint32 cornerTwo)
	{
		if (cornerOne == m_CornersID[gsl::narrow<int>(eCorner::TopLeft)] && cornerTwo == m_CornersID[gsl::narrow<int>(eCorner::TopRight)])
		{
			return eSide::Up;
		}
		else if (cornerOne == m_CornersID[gsl::narrow<int>(eCorner::TopRight)] && cornerTwo == m_CornersID[gsl::narrow<int>(eCorner::TopLeft)])
		{
			return eSide::Up_Reverse;
		}
		else if (cornerOne == m_CornersID[gsl::narrow<int>(eCorner::TopRight)] && cornerTwo == m_CornersID[gsl::narrow<int>(eCorner::DownRight)])
		{
			return eSide::Right;
		}
		else if (cornerOne == m_CornersID[gsl::narrow<int>(eCorner::DownRight)] && cornerTwo == m_CornersID[gsl::narrow<int>(eCorner::TopRight)])
		{
			return eSide::Right_Reverse;
		}
		else if (cornerOne == m_CornersID[gsl::narrow<int>(eCorner::DownLeft)] && cornerTwo == m_CornersID[gsl::narrow<int>(eCorner::DownRight)])
		{
			return eSide::Down;
		}
		else if (cornerOne == m_CornersID[gsl::narrow<int>(eCorner::DownRight)] && cornerTwo == m_CornersID[gsl::narrow<int>(eCorner::DownLeft)])
		{
			return eSide::Down_Reverse;
		}
		else  if (cornerOne == m_CornersID[gsl::narrow<int>(eCorner::TopLeft)] && cornerTwo == m_CornersID[gsl::narrow<int>(eCorner::DownLeft)])
		{
			return eSide::Left;
		}
		else  if (cornerOne == m_CornersID[gsl::narrow<int>(eCorner::DownLeft)] && cornerTwo == m_CornersID[gsl::narrow<int>(eCorner::TopLeft)])
		{
			return eSide::Left_Reverse;
		}
		else
		{
			return eSide::Null;
		}
	}

	bool PlanetQuadTreeNode::FindClosest(const Vector3 pos, eCorner& corner)
	{
		if ((pos - GetVertex(0, 0).VertexPosition).Magnitude() < m_MeshBuffer.HalfSize / 2.0)
		{
			corner = eCorner::TopLeft;
			return true;
		}

		if ((pos - GetVertex(MESH_ROW_SIZE, 0).VertexPosition).Magnitude() < m_MeshBuffer.HalfSize/2.0)
		{
			corner = eCorner::TopRight;
			return true;
		}

		if ((pos - GetVertex(0, MESH_ROW_SIZE).VertexPosition).Magnitude() < m_MeshBuffer.HalfSize / 2.0)
		{
			corner = eCorner::DownLeft;
			return true;
		}

		if ((pos - GetVertex(MESH_ROW_SIZE, MESH_ROW_SIZE).VertexPosition).Magnitude() < m_MeshBuffer.HalfSize / 2.0)
		{
			corner = eCorner::DownRight;
			return true;
		}

		return false;
	}

	void PlanetQuadTreeNode::Connect(PlanetQuadTreeNode * one, PlanetQuadTreeNode * two)
	{
		std::mutex buenMutex;
		buenMutex.lock();

		one->PlanetOwner->Connections++;

		///If they are both the same depth.
		if (one->m_QuadTreeDepth == two->m_QuadTreeDepth)
		{
			///Two corners should match, this determines the side.
			uint32 CornersMatching[2] = { 0, 0 };

			///Find which pair of corners match.
			for (int o = 0; o < 4; ++o)
				for (int t = 0; t < 4; ++t)
				{
					if (one->m_CornersID[o] == two->m_CornersID[t])
					{
						if (CornersMatching[0] == 0 && CornersMatching[0] != one->m_CornersID[o])
							CornersMatching[0] = one->m_CornersID[o];
						else
							CornersMatching[1] = two->m_CornersID[t];
					}
				}

			auto oneSide = one->GetSideFromCorners(CornersMatching[0], CornersMatching[1]);
			auto twoSide = two->GetSideFromCorners(CornersMatching[0], CornersMatching[1]);

			if (oneSide == eSide::Null && twoSide == eSide::Null) return;

			Vector<Gfx::Vertex*> oneVertices;
			one->GetSide(oneSide, oneVertices);

			Vector<Gfx::Vertex*> twoVertices;
			two->GetSide(twoSide, twoVertices);

			for (int i = 0; i < MESH_RES; ++i)
			{
				int indexOne;
				int indexTwo;

				if ((int)oneSide < 0)
					indexOne = MESH_ROW_SIZE - i;
				else
					indexOne = i;

				if ((int)twoSide < 0)
					indexTwo = MESH_ROW_SIZE - i;
				else
					indexTwo = i;

				*oneVertices[indexOne] = *twoVertices[indexTwo];
				//oneVertices[indexOne]->VertexNormals = { 0,0,0 };
				//twoVertices[indexTwo]->VertexNormals = { 0,0,0 };
			}
		}
		else if (one->m_QuadTreeDepth == two->m_QuadTreeDepth / 2)
		{

			//el two es el màs grande que no estoy modificando, entonces agarro el two. Saco el corner con el que correspondieron.
			///con ese corner ya sè cual de los dos lados es el que los conecta. De esos dos lados saco el punto medio y lo hasheo. 
			///Uno de esos dos puntos medios hasheados serà la otra esquina del one pequeño que conecta. Con eso ya puedo get los dos lados
			///de cada nodo y los conecto, brincandome uno en el one e iterando hasta la mitad del two que es màs grande.

			///Two corners should match, this determines the side.
			uint32 CornerMatch[2] = { 0, 0 };
			int CornerMatchIndex = -1;

			///Find which corners matches.
			for (int o = 0; o < 4; ++o)
				for (int t = 0; t < 4; ++t)
				{
					if (one->m_CornersID[o] == two->m_CornersID[t])
					{
						CornerMatch[0] = one->m_CornersID[o];
						CornerMatchIndex = t;
						o = 5;
						break;
					}
				}


			eSide CorrectSide = eSide::Null;
			Vector3 Pos;
			eCorner corner = eCorner::TopLeft;

			switch ((eCorner)(CornerMatchIndex))
			{
			default:
			case eCorner::TopLeft:
				Pos = two->GetVertex(MESH_ROW_HALF, 0).VertexPosition;

				if (one->FindClosest(Pos, corner))
				{
					CorrectSide = eSide::Up;
					break;
				}

				Pos = two->GetVertex(0, MESH_ROW_HALF).VertexPosition;
				if (one->FindClosest(Pos, corner))
				{
					CorrectSide = eSide::Left;
					break;
				}

				break;
			case eCorner::TopRight:
				Pos = two->GetVertex(MESH_ROW_HALF, 0).VertexPosition;

				if (one->FindClosest(Pos, corner))
				{
					CorrectSide = eSide::Up;
					break;
				}

				Pos = two->GetVertex(MESH_ROW_SIZE, MESH_ROW_HALF).VertexPosition;
				if (one->FindClosest(Pos, corner))
				{
					CorrectSide = eSide::Right;
					break;
				}

				break;
			case eCorner::DownRight:
				Pos = two->GetVertex(MESH_ROW_HALF, MESH_ROW_SIZE).VertexPosition;

				if (one->FindClosest(Pos, corner))
				{
					CorrectSide = eSide::Down;
					break;
				}

				Pos = two->GetVertex(MESH_ROW_SIZE, MESH_ROW_HALF).VertexPosition;
				if (one->FindClosest(Pos, corner))
				{
					CorrectSide = eSide::Right;
					break;
				}
				break;
			case eCorner::DownLeft:
				Pos = two->GetVertex(MESH_ROW_HALF, MESH_ROW_SIZE).VertexPosition;

				if (one->FindClosest(Pos, corner))
				{
					CorrectSide = eSide::Down;
					break;
				}

				Pos = two->GetVertex(0, MESH_ROW_HALF).VertexPosition;
				if (one->FindClosest(Pos, corner))
				{
					CorrectSide = eSide::Left;
					break;
				}
				break;
			}
		
			auto oneSide = one->GetSideFromCorners(CornerMatch[0], one->m_CornersID[(int)corner]); ///Now we can retrieve the side from one. The smaller one.

			if (oneSide == eSide::Null) return;

			Vector<Gfx::Vertex*> oneVertices;
			one->GetSide(oneSide, oneVertices);

			Vector<Gfx::Vertex*> twoVertices;
			two->GetSide(CorrectSide, twoVertices);

			for (int i = 0; i < MESH_RES; ++i)
			{
				int indexOne;
				int indexTwo = i / 2;

				if ((int)oneSide < 0)
					indexOne = MESH_ROW_SIZE - i;
				else
					indexOne = i;

				//If indexTwo is a mult of 2. It should be skipped.
				if ((indexOne + 1) % 2)
				{
					auto realIndex = indexTwo - 1;
					auto Pos = Math::Lerp(twoVertices[indexTwo]->VertexPosition, twoVertices[indexTwo + 1]->VertexPosition, 0.5f);
					auto Normals = Math::Lerp(twoVertices[indexTwo]->VertexNormals, twoVertices[indexTwo + 1]->VertexNormals, 0.5f);
					auto Tan = Math::Lerp(twoVertices[indexTwo]->Tangents, twoVertices[indexTwo + 1]->Tangents, 0.5f);
					auto Bin = Math::Lerp(twoVertices[indexTwo]->BiNormals, twoVertices[indexTwo + 1]->BiNormals, 0.5f);
					auto Tex = Math::Lerp(twoVertices[indexTwo]->TextureCoordinates, twoVertices[indexTwo + 1]->TextureCoordinates, 0.5f);

					//*oneVertices[indexOne] = Gfx::Vertex{ Pos, Tex, Normals, Tan, Bin };
					oneVertices[indexOne]->VertexNormals = { 1,0,0 };
				}
				else
				{
					*oneVertices[indexOne] = *twoVertices[indexTwo];
					oneVertices[indexOne]->VertexNormals = { 0,0,0 };
					//twoVertices[indexTwo]->VertexNormals = { 0,0,0 };
				}
			}
		}

		buenMutex.unlock();
	}

	void PlanetQuadTreeNode::Render()
	{
		//if (Parent)
		//{
		//	if (!Parent->TestIfInside(CameraManager::Pointer()->GetActiveCamera().lock()->GetPosition()))
		//	{
		//		return;
		//	}
		//}
		//else
		//{
		//	if (!TestIfInside(CameraManager::Pointer()->GetActiveCamera().lock()->GetPosition()))
		//	{
		//		return;
		//	}
		//}

		auto ptr = m_res->GetResource<Material>(m_Material).lock();

		if (ActiveTouch)
			ptr->m_core.g_Albedo.Set(1.0f, 0.0f, 0.0f);
		else
			ptr->m_core.g_Albedo.Set(1.0f, 1.0f, 1.0f);

		ptr->SetThisMaterial(m_gfx, m_res);

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

		noise = pow(Noise->RidgedOctaveNoise(Pos128, 6, 0.4f), 2)  *
			Noise->OctaveNoise(Pos24, 6, 0.5f) *
			Noise->OctaveNoise(NoisePos, 3, 0.5f) *
			pow(Noise->RidgedOctaveNoise(Pos12, 2, 0.2f), 2);

		return PosNormal + (PosNormal.GetNormalized())*noise*.02f;
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
			Child[i] = std::thread(&PlanetQuadTreeNode::GenerateChild, this, i, depth);
			Child[i].detach();
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
		Children[index]->InitQuadTree(PlanetOwner, Noise, m_StartPos, index, depth, m_Side, sideHashData);
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

	uint32 PlanetQuadTreeNode::HashCorner(Vector3 v)
	{
		auto temp = (v + Vector3(1.0f, 1.0f, 1.0f)) * 1000000;
		return Vector3::Hash(temp);
	}
}