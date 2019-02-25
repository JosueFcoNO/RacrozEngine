#include <RZEnginePCH.h>

namespace rczEngine
{
	void PlanetQuadTreeNode::InitQuadTree(Planet* planetRef, PerlinNoise3D* noise, Vector3 StartPos, int32 ChildNumber, int32 depth, eMeshPlaneOrientation side, ParentSidesData sideData)
	{
		for (int i = 0; i < 4; ++i)
			SetChildrenReady(i, false);

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

		for (int i = 0; i < 4; ++i)
		{
			if (sideData.Hashes[i] == "") continue;

			auto patch = PlanetOwner->GetPatchData(sideData.Hashes[i]);

			if (patch->Vertices.size() > 0)
				for (auto k = 0; k < MESH_RES; ++k)
				{
					switch ((eSide)(k))
					{
					case eSide::Up:
						GetVertex(k, 0) = patch->Vertices[k];
						break;
					case eSide::Left:
						GetVertex(0, k) = patch->Vertices[k];
						break;
					case eSide::Right:
						GetVertex(MESH_ROW_SIZE, k) = patch->Vertices[k];
						break;
					case eSide::Down:
						GetVertex(k, MESH_ROW_SIZE) = patch->Vertices[k];
						break;
					}
				}
		}

		m_VertexBuffer.CreateVertexBuffer(Gfx::USAGE_DEFAULT, false, m_gfx);

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
		PlanetOwner->m_NodeAdyacency.insert(Pair<float, PlanetQuadTreeNode*>(m_CornersID[0], this));

		m_CornersID[1] = HashCorner(topright.GetNormalized());
		PlanetOwner->m_NodeAdyacency.insert(Pair<float, PlanetQuadTreeNode*>(m_CornersID[1], this));

		m_CornersID[2] = HashCorner(bottomright.GetNormalized());
		PlanetOwner->m_NodeAdyacency.insert(Pair<float, PlanetQuadTreeNode*>(m_CornersID[2], this));

		m_CornersID[3] = HashCorner(bottomleft.GetNormalized());
		PlanetOwner->m_NodeAdyacency.insert(Pair<float, PlanetQuadTreeNode*>(m_CornersID[3], this));

		auto cLeftUp = std::to_string(m_CornersID[0]);
		auto cRightUp = std::to_string(m_CornersID[1]);
		auto cRightDown = std::to_string(m_CornersID[2]);
		auto cLeftDown = std::to_string(m_CornersID[3]);

		String hash;

		if (m_CornersID[0] < m_CornersID[3])
		{
			hash = cLeftUp + "|" + cLeftDown;
			PlanetOwner->ProcessBorderData(hash, this, eSide::Left, cLeftUp, cLeftDown);
		}
		else
		{
			hash = cLeftDown + "|" + cLeftUp;
			PlanetOwner->ProcessBorderData(hash, this, eSide::Left, cLeftDown, cLeftUp);
		}

		sideHashData.Hashes[(int)eSide::Left] = hash;

		if (m_CornersID[1] < m_CornersID[2])
		{
			hash = cRightUp + "|" + cRightDown;
			PlanetOwner->ProcessBorderData(hash, this, eSide::Right, cRightUp, cRightDown);
		}
		else
		{
			hash = cRightDown + "|" + cRightUp;
			PlanetOwner->ProcessBorderData(hash, this, eSide::Right, cRightDown, cRightUp);
		}

		sideHashData.Hashes[(int)eSide::Right] = hash;

		if (m_CornersID[3] < m_CornersID[2])
		{
			hash = cLeftDown + "|" + cRightDown;
			PlanetOwner->ProcessBorderData(hash, this, eSide::Down, cLeftDown, cRightDown);
		}
		else
		{
			hash = cRightDown + "|" + cLeftDown;
			PlanetOwner->ProcessBorderData(hash, this, eSide::Down, cRightDown, cLeftDown);
		}

		sideHashData.Hashes[(int)eSide::Down] = hash;

		if (m_CornersID[0] < m_CornersID[1])
		{
			hash = cLeftUp + "|" + cRightUp;
			PlanetOwner->ProcessBorderData(hash, this, eSide::Up, cLeftUp, cRightUp);
		}
		else
		{
			hash = cRightUp + "|" + cLeftUp;
			PlanetOwner->ProcessBorderData(hash, this, eSide::Up, cRightUp, cLeftUp);
		}

		sideHashData.Hashes[(int)eSide::Up] = hash;
		
		for (int i = 0; i < 4; ++i)
		{
			if (sideHashData.Hashes[i] == "") continue;
		
			auto patch = PlanetOwner->GetPatchData(sideHashData.Hashes[i]);

			if (patch->First) continue;
		
			if (patch->Vertices.size() > 0)
			{
				eSide side = eSide::Down;
				bool Reverse = false;
		
				if (patch->VertexStartHash == cLeftUp && patch->VertexEndHash == cLeftDown)
				{
					side = eSide::Left;
				}
				else
				if (patch->VertexEndHash == cLeftUp && patch->VertexStartHash == cLeftDown)
				{
					side = eSide::Left;
					Reverse = true;
				}
		
				if (patch->VertexStartHash == cLeftUp && patch->VertexEndHash == cRightUp)
				{
					side = eSide::Up;
				} else if (patch->VertexEndHash == cLeftUp && patch->VertexStartHash == cRightUp)
				{
					side = eSide::Up;
					Reverse = true;
				}
		
				if (patch->VertexStartHash == cRightUp && patch->VertexEndHash == cRightDown)
				{
					side = eSide::Right;
				}
				else if (patch->VertexEndHash == cRightUp && patch->VertexStartHash == cRightDown)
				{
					side = eSide::Right;
					Reverse = true;
				}
		
				if (patch->VertexStartHash == cLeftDown && patch->VertexEndHash == cRightDown)
				{
					side = eSide::Down;
				}
				else if (patch->VertexEndHash == cLeftDown && patch->VertexStartHash == cRightDown)
				{
					side = eSide::Down;
					Reverse = true;
				}
		
				for (auto k = 0; k < MESH_RES; ++k)
				{
					switch (side)
					{
					case eSide::Up:
						if (!Reverse || true)
						{
							auto normal = patch->Vertices[k].VertexNormals;
							GetVertex(k, 0) = patch->Vertices[k];
							GetVertex(k, 0).VertexNormals = ((normal + GetVertex(k, 0).VertexNormals) / 2.0f).GetNormalized();
						}
						else
						{
							GetVertex(k, 0) = patch->Vertices[PlanetQuadTreeNode::MESH_ROW_SIZE - k];
						}
						break;
					case eSide::Left:
						if (!Reverse || true)
						{
							auto normal = patch->Vertices[k].VertexNormals;
							GetVertex(0, k) = patch->Vertices[k];
							GetVertex(0, k).VertexNormals = ((normal + GetVertex(0, k).VertexNormals) / 2.0f).GetNormalized();
						}
						else
						{
							GetVertex(0, k) = patch->Vertices[PlanetQuadTreeNode::MESH_ROW_SIZE - k];
						}
						break;
					case eSide::Right:
						if (!Reverse || true)
						{
							auto normal = patch->Vertices[k].VertexNormals;
							GetVertex(MESH_ROW_SIZE, k) = patch->Vertices[k];
							GetVertex(MESH_ROW_SIZE, k).VertexNormals = ((normal + GetVertex(MESH_ROW_SIZE, k).VertexNormals) / 2.0f).GetNormalized();
						}
						else
						{
							GetVertex(MESH_ROW_SIZE, k) = patch->Vertices[PlanetQuadTreeNode::MESH_ROW_SIZE - k];
						}
						break;
					case eSide::Down:
						if (!Reverse || true)
						{
							auto normal = patch->Vertices[k].VertexNormals;
							GetVertex(k, MESH_ROW_SIZE) = patch->Vertices[k];
							GetVertex(k, MESH_ROW_SIZE).VertexNormals = ((normal + GetVertex(k, MESH_ROW_SIZE).VertexNormals) / 2.0f).GetNormalized();
						}
						else
						{
							GetVertex(k, MESH_ROW_SIZE) = patch->Vertices[PlanetQuadTreeNode::MESH_ROW_SIZE - k];
						}
						break;
					}
				}
			}
		}

		m_VertexBuffer.UpdateVertexBuffer(m_gfx);

		m_Material = m_res->FindResourceByName<Material>("lambert1").lock()->GetHandle();
	}

	void PlanetQuadTreeNode::Update(Vector3 playerPos)
	{
		if (m_ChildGenerated)
		{
			if (CheckChildrenReady())
			{
				for (int i = 0; i < 4; ++i)
				{
					if (Child[i].joinable())
						Child[i].join();	
				}

			}
		} else
		{
			//CalculateLOD(playerPos);
		}

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

		auto Right = m_VertexBuffer.GetVertex(MESH_RES * MESH_RES/2).VertexPosition;
		Right += (Right - centerVertex).GetNormalized()*diff;
		Right *= 1.5f;

		GraphicDebugger::Pointer()->AddFrameDebuggerPoint(Right, 0.0025f, 1, 0, 0);
		if (Node->TestIfInside(Right))
		{
			return true;
		}

		auto Up = m_VertexBuffer.GetVertex(MESH_RES * MESH_RES / 2 + (MESH_RES-1)).VertexPosition;
		Up += (Up - centerVertex).GetNormalized()*diff;
		Up *= 1.5f;

		GraphicDebugger::Pointer()->AddFrameDebuggerPoint(Up, 0.0025f, 1, 0, 0);
		if (Node->TestIfInside(Up))
		{
			return true;
		}

		auto Down = m_VertexBuffer.GetVertex(MESH_RES * (MESH_RES-1) + (MESH_RES / 2)).VertexPosition;
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

		auto D = m_VertexBuffer.GetVertex(MESH_RES*MESH_RES-1).VertexPosition;
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
		if (TestIfInside(pos))
		{
			ActiveTouch = true;

			auto mag = pos.Magnitude() - 1;

			if (1.0f / (m_QuadTreeDepth+1) > mag)
			{
				//m_Dirty = true;
			}
			else
			{
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
						//m_Dirty = true;
						i = 5; //Exit both fors
						break;
					}
				};
			}
		}

		if (m_Dirty)
		{
			DeathTimer.StartTimer();

			if (!m_ChildGenerated && m_QuadTreeDepth < 64)
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

	void PlanetQuadTreeNode::GetSide(eSide side, Vector<Gfx::Vertex>& out_VertexList)
	{
		switch (side)
		{
		case eSide::Up:
			for (int32 i = 0; i < MESH_RES; ++i) out_VertexList.push_back(GetVertex(i, 0));
			break;
		case eSide::Down:
			for (int32 i = 0; i < MESH_RES; ++i) out_VertexList.push_back(GetVertex(i, MESH_ROW_SIZE));
			break;
		case eSide::Left:
			for (int32 i = 0; i < MESH_RES; ++i) out_VertexList.push_back(GetVertex(0, i));
			break;
		case eSide::Right:
			for (int32 i = 0; i < MESH_RES; ++i) out_VertexList.push_back(GetVertex(MESH_ROW_SIZE, i));
			break;
		default:
			break;
		}
	}

	void PlanetQuadTreeNode::Render()
	{
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
		Vector3 PosNormal = Math::CubeToSphere(pos);
		Vector3 NoisePos = PosNormal + Vector3(1.0f, 1.0f, 1.0f);
		NoisePos *= 0.5f;

		float noise;

		noise = pow(Noise->RidgedOctaveNoise(NoisePos*128.0f, 6, 0.4f), 2) *
			Noise->OctaveNoise(NoisePos*24.0f, 6, 0.5f) *
			Noise->OctaveNoise(NoisePos, 2, 0.5f) +
			pow(Noise->RidgedOctaveNoise(NoisePos*12.0f, 2, 0.2f), 2) *
			pow(Noise->OctaveNoise(NoisePos, 2, 1), 2);

		return PosNormal + (PosNormal.GetNormalized())*noise*.01f;
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

	float PlanetQuadTreeNode::HashCorner(Vector3 v)
	{
		auto temp = (v + Vector3(1.0f, 1.0f, 1.0f)) * 1000000;
		return Vector3::Hash(temp);
	}
}