#include <RZEnginePCH.h>

namespace rczEngine
{
	void PlanetQuadTreeNode::InitQuadTree(Planet* planetRef, PerlinNoise3D* noise, Vector3 StartPos, int32 ChildNumber, int32 depth, eMeshPlaneOrientation side)
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

	void PlanetQuadTreeNode::Render()
	{
		const auto resPtr = ResVault::Pointer();
		const auto gfxPtr = Gfx::GfxCore::Pointer();

		auto ptr = resPtr->GetResource<Material>(m_Material).lock();

		if (ActiveTouch)
			ptr->m_core.g_Albedo.Set(1.0f, 0.0f, 0.0f);
		else
			ptr->m_core.g_Albedo.Set(1.0f, 1.0f, 1.0f);

		ptr->SetThisMaterial(gfxPtr, resPtr);

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
		Children[index]->InitQuadTree(PlanetOwner, Noise, m_StartPos, index, depth, m_Side);
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