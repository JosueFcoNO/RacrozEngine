#include <RZEnginePCH.h>

namespace rczEngine
{
	void PlanetQuadTreeNode::InitQuadTree(PerlinNoise3D* noise, Vector3 StartPos, int32 ChildNumber, int32 depth, eMeshPlaneOrientation side)
	{
		for (int i = 0; i < 4; ++i)
			SetChildrenReady(i, false);

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
		InitMeshPlane(32, planeSize, planeSize / 2.0, startPos, side);

		auto v1 = m_VertexBuffer.GetVertex(0);
		auto v2 = m_VertexBuffer.GetVertex(31);
		auto v3 = m_VertexBuffer.GetVertex(32 * 32 - 1);
		auto v4 = m_VertexBuffer.GetVertex(32 * 32 - 32);

		Planes[0].ConstructFromPoints(v1.VertexPosition, v2.VertexPosition, v1.VertexPosition.GetNormalized() * 2);
		Planes[1].ConstructFromPoints(v2.VertexPosition, v3.VertexPosition, v2.VertexPosition.GetNormalized() * 2);
		Planes[2].ConstructFromPoints(v3.VertexPosition, v4.VertexPosition, v3.VertexPosition.GetNormalized() * 2);
		Planes[3].ConstructFromPoints(v4.VertexPosition, v1.VertexPosition, v4.VertexPosition.GetNormalized() * 2);

		Planes[4].ConstructFromPoints(v1.VertexPosition, v2.VertexPosition, v3.VertexPosition);

		m_Material = m_res->FindResourceByName<Material>("lambert1").lock()->GetHandle();
	}

	void PlanetQuadTreeNode::SetAdyacents(PlanetQuadTreeNode * Left, PlanetQuadTreeNode * Right, PlanetQuadTreeNode * Up, PlanetQuadTreeNode * Down)
	{
		Adyacents[0] = Up;
		Adyacents[1] = Right;
		Adyacents[2] = Down;
		Adyacents[3] = Left;
	}

	void PlanetQuadTreeNode::Update(Vector3 playerPos)
	{
		Test(playerPos);

		if (AdyTimer.GetTime() > 0.1f)
		{
			IsAdy = false;
		}

		if (m_ChildGenerated)
		{
			if (CheckChildrenReady())
			{
				for (int i = 0; i < 4; ++i)
				{
					if (Child[i].joinable())
						Child[i].join();

					switch (i)
					{
					case 0:
						//Children[i]->SetAdyacents(Adyacents[3], Children[2], Adyacents[2], Children[1]);
						Children[i]->SetAdyacents(Adyacents[0], Children[2], Adyacents[0], Children[1]);

						break;
					case 1:
						Children[i]->SetAdyacents(Children[0], Adyacents[3], Adyacents[1], Children[3]);
						break;
					case 2:
						Children[i]->SetAdyacents(Children[3], Adyacents[0], Children[1], Adyacents[0]);
						break;
					case 3:
						Children[i]->SetAdyacents(Adyacents[0], Children[1], Children[2], Adyacents[0]);
						break;
					}
				}

				if (Adyacents[0]->m_QuadTreeDepth < m_QuadTreeDepth)
					Adyacents[0]->Override = true;

				if (Adyacents[1]->m_QuadTreeDepth < m_QuadTreeDepth)
					Adyacents[1]->Override = true;

				if (Adyacents[2]->m_QuadTreeDepth < m_QuadTreeDepth)
					Adyacents[2]->Override = true;

				if (Adyacents[3]->m_QuadTreeDepth < m_QuadTreeDepth)
					Adyacents[3]->Override = true;

			}
		}

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

	void PlanetQuadTreeNode::Test(Vector3 pos)
	{
		if (m_QuadTreeDepth > 16) return;

		//if (!Override)
		//	m_Dirty = m_MeshAABB.CheckPointCollision(pos);
		//else
		//	m_Dirty = true;

		if (Planes[0].SignedDistance(pos) < 0 &&
			Planes[1].SignedDistance(pos) < 0 && 
			Planes[2].SignedDistance(pos) < 0 && 
			Planes[3].SignedDistance(pos) < 0 && 
			Planes[4].SignedDistance(pos) > 0)
		{
			ActiveTouch = true;

			auto mag = pos.Magnitude() - 1;

			if (0.75f / (m_QuadTreeDepth+1) > mag)
			{
				m_Dirty = true;
			}

			if (!m_ChildGenerated)
			{
				for (int i = 0; i < 4; ++i)
				{
					Adyacents[i]->IsAdy = true;
					Adyacents[i]->AdyTimer.StartTimer();
				}
			}
		}
		else
		{
			ActiveTouch = false;
		}

		if (m_Dirty)
		{
			DeathTimer.StartTimer();

			if (!m_ChildGenerated)
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

	void PlanetQuadTreeNode::Render()
	{
		auto ptr = m_res->GetResource<Material>(m_Material).lock();

		//GraphicDebugger::Pointer()->AddFrameDebuggerPoint(m_MeshAABB.GetCenter(), 0.001f, 1.0, 0, 0);

		if (ActiveTouch)
			ptr->m_core.g_Albedo.Set(1.0f, 0.0f, 0.0f);
		else if (IsAdy)
			ptr->m_core.g_Albedo.Set(0.0f, 0.0f, 1.0f);
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
		Children[index]->InitQuadTree(Noise, m_StartPos, index, depth, m_Side);
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
}