#include <RZEnginePCH.h>

namespace rczEngine
{
	GraphicDebugger*& GraphicDebugger::_Instance()
	{
		static GraphicDebugger* instance = nullptr;
		return instance;
	}

	void GraphicDebugger::Start()
	{
		(_Instance()) = new GraphicDebugger;
	}

	GraphicDebugger* GraphicDebugger::Pointer()
	{
		return _Instance();
	}

	void GraphicDebugger::ShutDown()
	{
		delete _Instance();
	}

	void GraphicDebugger::Init()
	{
		auto gfx = Gfx::GfxCore::Pointer();
		m_Shader.CreatePixelShader(L"Shaders/DebuggerShader.hlsl", gfx);
		m_ColorCB.CreateConstantBuffer(sizeof(Vector4), Gfx::USAGE_DEFAULT, gfx);
		m_LinesRS.Init(Gfx::eFILL_MODE::FILL_WIREFRAME);
		m_SolidRS.Init(Gfx::eFILL_MODE::FILL_SOLID);
		m_WorldMatrixCB.CreateConstantBuffer(sizeof(Matrix4::m_elements), Gfx::USAGE_DEFAULT, gfx);
		m_SolidRS.CreateRasterizerState(gfx);
		m_LinesRS.CreateRasterizerState(gfx);

		StrPtr<Model> Sphere = std::make_shared<Model>();
		Sphere->Load("RacrozEngineAssets/EsferaLowPoly.fbx", "DebuggerSphere");

		m_SphereModel = ResVault::Pointer()->InsertResource(Sphere);
	}

	void GraphicDebugger::Destroy()
	{
		m_StaticVertexBuffer.Destroy();
		m_StaticIndexBuffer.Destroy();
		m_FrameIndexBuffer.Destroy();
		m_FrameVertexBuffer.Destroy();
		m_ColorCB.Destroy();
	}

	void GraphicDebugger::Render(Gfx::GfxCore * graphicsapi_instance)
	{
		Vector<DebuggerPoint*> staticpoints;

		///Set the Graphic debugger's  pixel shader
		m_Shader.SetThisPixelShader(graphicsapi_instance);

		///Set the ColorBuffer in the pipeline
		m_ColorCB.SetBufferInPS(0, graphicsapi_instance);

		///Set the Rasterizer State
		m_LinesRS.SetThisRasterizerState(graphicsapi_instance);

		///Change the Topology to linelist
		graphicsapi_instance->SetPrimitiveTopology(Gfx::eTOPOLOGY::TOPO_LINELIST);

		Matrix4 M(INIT_UNIT);
		m_WorldMatrixCB.SetBufferInVS(2, graphicsapi_instance);
		m_WorldMatrixCB.UpdateConstantBuffer(&M, graphicsapi_instance);
	
		if (m_FrameObjs.size())
		{
			///Destroy the Static Index And Gfx::Vertex Buffers
			//m_FrameIndexBuffer.Destroy();
			//m_FrameVertexBuffer.Destroy();

			///Create the Frame Index Buffer and the Gfx::Vertex Buffer
			//m_FrameIndexBuffer.CreateIndexBuffer(Gfx::USAGE_DEFAULT, graphicsapi_instance);
			//m_FrameVertexBuffer.CreateVertexBuffer(Gfx::USAGE_DEFAULT, false, graphicsapi_instance);

			///Set the Framem Index and Gfx::Vertex Buffer ont the pipeline
			//m_FrameIndexBuffer.SetThisIndexBuffer(graphicsapi_instance);
			//m_FrameVertexBuffer.SetThisVertexBuffer(graphicsapi_instance, 0);

			///Iterate through the frame objects, render and delete them
			for (int i = 0; i < m_FrameObjs.size(); i++)
			{
				if (m_FrameObjs[i]->GetElementType() == DEBUG_POINT)
				{
					m_ColorCB.UpdateConstantBuffer(&m_FrameObjs[i]->m_Color, graphicsapi_instance);
				}
				else
				{
					///Update the color buffer with the obj color.
					m_ColorCB.UpdateConstantBuffer(&m_FrameObjs[i]->m_Color, graphicsapi_instance);
					///Render the frame object
					m_FrameObjs[i]->Render(graphicsapi_instance);
					///Release the frame object memory
					delete (m_FrameObjs[i]);
				}
			}

		}

		if (m_StaticObjs.size())
		{
			///Destroy the Static Index And Gfx::Vertex Buffers
			m_StaticIndexBuffer.Destroy();
			m_StaticVertexBuffer.Destroy();

			///Recreate the Static Index And Gfx::Vertex Buffers
			m_StaticIndexBuffer.CreateIndexBuffer(Gfx::USAGE_DEFAULT, graphicsapi_instance);
			m_StaticVertexBuffer.CreateVertexBuffer(Gfx::USAGE_DEFAULT, false, graphicsapi_instance);

			///Set them int the Gfx::Vertex and Index Buffer in pipeline
			m_StaticIndexBuffer.SetThisIndexBuffer(graphicsapi_instance);
			m_StaticVertexBuffer.SetThisVertexBuffer(graphicsapi_instance, 0);

			///Iterate through the Static Objs and Draw Them
			for (int i = 0; i < m_StaticObjs.size(); i++)
			{
				if (m_StaticObjs[i]->GetElementType() == DEBUG_POINT)
				{
					m_ColorCB.UpdateConstantBuffer(&m_StaticObjs[i]->m_Color, graphicsapi_instance);

					staticpoints.push_back((DebuggerPoint*)m_StaticObjs[i]);
				}
				else
				{
					///Update the color buffer
					m_ColorCB.UpdateConstantBuffer(&m_StaticObjs[i]->m_Color, graphicsapi_instance);
					///Render the static object
					m_StaticObjs[i]->Render(graphicsapi_instance);
				}
			}
		}

		m_SolidRS.SetThisRasterizerState(graphicsapi_instance);
		graphicsapi_instance->SetPrimitiveTopology(Gfx::eTOPOLOGY::TOPO_TRIANGLELIST);

		for (int i = 0; i < staticpoints.size(); ++i)
		{
			M = Matrix4::Scale3D(staticpoints[i]->m_Radius, staticpoints[i]->m_Radius, staticpoints[i]->m_Radius)*Matrix4::Translate3D(staticpoints[i]->m_Position.m_x, staticpoints[i]->m_Position.m_y, staticpoints[i]->m_Position.m_z);
			M.Transpose();
			m_WorldMatrixCB.UpdateConstantBuffer(&M, graphicsapi_instance);
			m_WorldMatrixCB.SetBufferInVS(2, graphicsapi_instance);

			m_ColorCB.UpdateConstantBuffer(&staticpoints[i]->m_Color, graphicsapi_instance);

			staticpoints[i]->Render(graphicsapi_instance);
		}

		for (int i = 0; i < m_FrameObjs.size(); ++i)
		{
			auto ptr = (DebuggerPoint*)m_FrameObjs[i];
			M = Matrix4::Scale3D(ptr->m_Radius, ptr->m_Radius, ptr->m_Radius)*Matrix4::Translate3D(ptr->m_Position.m_x, ptr->m_Position.m_y, ptr->m_Position.m_z);
			M.Transpose();
			m_WorldMatrixCB.UpdateConstantBuffer(&M, graphicsapi_instance);
			m_WorldMatrixCB.SetBufferInVS(2, graphicsapi_instance);

			m_ColorCB.UpdateConstantBuffer(&ptr->m_Color, graphicsapi_instance);

			ptr->Render(graphicsapi_instance);

			delete m_FrameObjs[i];
		}

		staticpoints.clear();

		///Clear the Frame Object Vector
		m_FrameObjs.clear();
	}

	void GraphicDebugger::AddStaticDebuggerLine(float x1, float y1, float z1, float x2, float y2, float z2, float r, float g, float b)
	{
		Gfx::Vertex V1, V2;
		Vector3 P1(x1,y1,z1), P2(x2,y2,z2);
		V1.VertexPosition = P1;
		V2.VertexPosition = P2;

		DebuggerLine* Temp = new DebuggerLine;

		Temp->m_IndexOffset = m_StaticIndicesNum;
		Temp->m_Color.m_x = r;
		Temp->m_Color.m_y = g;
		Temp->m_Color.m_z = b;

		m_StaticVertexBuffer.AddVertex(V1);
		m_StaticVerticesNum++;
		m_StaticVertexBuffer.AddVertex(V2);
		m_StaticVerticesNum++;

		m_StaticIndexBuffer.AddIndex(m_StaticIndicesNum++);
		m_StaticIndexBuffer.AddIndex(m_StaticIndicesNum++);

		m_StaticObjs.push_back(Temp);
	}

	void GraphicDebugger::AddStaticDebuggerLine(Vector3 point1, Vector3 point2, float r, float g, float b)
	{
		AddStaticDebuggerLine(point1.m_x, point1.m_y, point1.m_z, point2.m_x, point2.m_y, point2.m_z, r,g,b);
	}

	void GraphicDebugger::AddStaticDebuggerPoint(float x, float y, float z, float radius, float r, float g, float b)
	{
		DebuggerPoint* Temp = new DebuggerPoint;
		Temp->m_Position.m_x = x;
		Temp->m_Position.m_y = y;
		Temp->m_Position.m_z = z;
		Temp->m_Radius = radius;
		Temp->handle = m_SphereModel;
		Temp->m_Color.m_x = r;
		Temp->m_Color.m_z = b;
		Temp->m_Color.m_y = g;

		m_StaticObjs.push_back(Temp);
	}

	void GraphicDebugger::AddStaticDebuggerPoint(Vector3 point, float radius, float r, float g, float b)
	{
		AddStaticDebuggerPoint(point.m_x, point.m_y, point.m_z, radius, r,g,b);
	}

	void GraphicDebugger::AddStaticDebuggerCube(float x, float y, float z, float xScale, float yScale, float zScale, float r, float g, float b)
	{
		Gfx::Vertex VTopB1, VTopB2, VTopF1, VTopF2, VBottomB1, VBottomB2, VBottomF1, VBottomF2;

		float xScale2 = xScale/2, yScale2 = yScale/2, zScale2 = zScale/2;
		Vector3
			TopB1(x - xScale2, y + yScale2, z + zScale2),
			TopB2(x + xScale2, y + yScale2, z + zScale2),
			TopF1(x - xScale2, y + yScale2, z - zScale2),
			TopF2(x + xScale2, y + yScale2, z - zScale2),

			BottomB1(x - xScale2, y - yScale2, z + zScale2),
			BottomB2(x + xScale2, y - yScale2, z + zScale2),
			BottomF1(x - xScale2, y - yScale2, z - zScale2),
			BottomF2(x + xScale2, y - yScale2, z - zScale2)
			;

		DebuggerCube* Temp = new DebuggerCube;

		Temp->m_IndexOffset = m_StaticIndicesNum;
		Temp->m_Color.m_x = r;
		Temp->m_Color.m_y = g;
		Temp->m_Color.m_z = b;

		VBottomB1.VertexPosition = BottomB1;
		VBottomB2.VertexPosition = BottomB2;
		VBottomF1.VertexPosition = BottomF1;
		VBottomF2.VertexPosition = BottomF2;

		VTopB1.VertexPosition = TopB1;
		VTopB2.VertexPosition = TopB2;
		VTopF1.VertexPosition = TopF1;
		VTopF2.VertexPosition = TopF2;

		m_StaticVertexBuffer.AddVertex(VBottomB1);
		m_StaticVertexBuffer.AddVertex(VBottomB2);
		m_StaticVertexBuffer.AddVertex(VBottomF1);
		m_StaticVertexBuffer.AddVertex(VBottomF2);
		m_StaticVertexBuffer.AddVertex(VTopB1);
		m_StaticVertexBuffer.AddVertex(VTopB2);
		m_StaticVertexBuffer.AddVertex(VTopF1);
		m_StaticVertexBuffer.AddVertex(VTopF2);
		m_StaticIndexBuffer.AddIndex(m_StaticVerticesNum);
		m_StaticIndexBuffer.AddIndex(m_StaticVerticesNum + 1);
		m_StaticIndexBuffer.AddIndex(m_StaticVerticesNum);
		m_StaticIndexBuffer.AddIndex(m_StaticVerticesNum + 2);
		m_StaticIndexBuffer.AddIndex(m_StaticVerticesNum);
		m_StaticIndexBuffer.AddIndex(m_StaticVerticesNum + 4);
		m_StaticIndexBuffer.AddIndex(m_StaticVerticesNum + 2);
		m_StaticIndexBuffer.AddIndex(m_StaticVerticesNum + 3);
		m_StaticIndexBuffer.AddIndex(m_StaticVerticesNum + 1);
		m_StaticIndexBuffer.AddIndex(m_StaticVerticesNum + 3);
		m_StaticIndexBuffer.AddIndex(m_StaticVerticesNum + 2);
		m_StaticIndexBuffer.AddIndex(m_StaticVerticesNum + 6);
		m_StaticIndexBuffer.AddIndex(m_StaticVerticesNum + 7);
		m_StaticIndexBuffer.AddIndex(m_StaticVerticesNum + 6);
		m_StaticIndexBuffer.AddIndex(m_StaticVerticesNum + 4);
		m_StaticIndexBuffer.AddIndex(m_StaticVerticesNum + 6);
		m_StaticIndexBuffer.AddIndex(m_StaticVerticesNum + 4);
		m_StaticIndexBuffer.AddIndex(m_StaticVerticesNum + 5);
		m_StaticIndexBuffer.AddIndex(m_StaticVerticesNum + 5);
		m_StaticIndexBuffer.AddIndex(m_StaticVerticesNum + 7);
		m_StaticIndexBuffer.AddIndex(m_StaticVerticesNum + 5);
		m_StaticIndexBuffer.AddIndex(m_StaticVerticesNum + 1);
		m_StaticIndexBuffer.AddIndex(m_StaticVerticesNum + 3);
		m_StaticIndexBuffer.AddIndex(m_StaticVerticesNum + 7);
		m_StaticVerticesNum += 8;
		m_StaticIndicesNum += 24;
		m_StaticObjs.push_back(Temp);
	}

	void GraphicDebugger::AddStaticDebuggerCube(Vector3 center, float xScale, float yScale, float zScale, float r, float g, float b)
	{
		AddStaticDebuggerCube(center.m_x, center.m_y, center.m_z, xScale, yScale, zScale, r,g,b);
	}

	void GraphicDebugger::AddFrameDebuggerLine(float x1, float y1, float z1, float x2, float y2, float z2, float r, float g, float b)
	{
		Gfx::Vertex V1, V2;
		Vector3 P1(x1, y1, z1), P2(x2, y2, z2);
		V1.VertexPosition = P1;
		V2.VertexPosition = P2;

		DebuggerLine* Temp = new DebuggerLine;

		Temp->m_IndexOffset = m_FrameIndicesNum;
		Temp->m_Color.m_x = r;
		Temp->m_Color.m_y = g;
		Temp->m_Color.m_z = b;

		m_FrameVertexBuffer.AddVertex(V1);
		m_FrameVerticesNum++;
		m_FrameVertexBuffer.AddVertex(V2);
		m_FrameVerticesNum++;

		m_FrameIndexBuffer.AddIndex(m_FrameIndicesNum++);
		m_FrameIndexBuffer.AddIndex(m_FrameIndicesNum++);

		m_FrameObjs.push_back(Temp);
	}

	void GraphicDebugger::AddFrameDebuggerLine(Vector3 point1, Vector3 point2, float r, float g, float b)
	{
		AddFrameDebuggerLine(point1.m_x, point1.m_y, point1.m_z, point2.m_x, point2.m_y, point2.m_z, r,g,b);
	}

	void GraphicDebugger::AddFrameDebuggerPoint(float x, float y, float z, float radius, float r, float g, float b)
	{
		DebuggerPoint* Temp = new DebuggerPoint;
		Temp->m_Position.m_x = x;
		Temp->m_Position.m_y = y;
		Temp->m_Position.m_z = z;
		Temp->m_Radius = radius*10;

		Temp->m_Color.m_x = r;
		Temp->m_Color.m_z = b;
		Temp->m_Color.m_y = g;

		m_FrameObjs.push_back(Temp);
	}

	void GraphicDebugger::AddFrameDebuggerPoint(Vector3 point, float radius, float r, float g, float b)
	{
		AddFrameDebuggerPoint(point.m_x, point.m_y, point.m_z, radius, r,g,b);
	}

	void GraphicDebugger::AddFrameDebuggerCube(float x, float y, float z, float xScale, float yScale, float zScale, float r, float g, float b)
	{
		Gfx::Vertex VTopB1, VTopB2, VTopF1, VTopF2, VBottomB1, VBottomB2, VBottomF1, VBottomF2;

		float xScale2 = xScale / 2, yScale2 = yScale / 2, zScale2 = zScale / 2;
		Vector3
			TopB1(x - xScale2, y + yScale2, z + zScale2),
			TopB2(x + xScale2, y + yScale2, z + zScale2),
			TopF1(x - xScale2, y + yScale2, z - zScale2),
			TopF2(x + xScale2, y + yScale2, z - zScale2),

			BottomB1(x - xScale2, y - yScale2, z + zScale2),
			BottomB2(x + xScale2, y - yScale2, z + zScale2),
			BottomF1(x - xScale2, y - yScale2, z - zScale2),
			BottomF2(x + xScale2, y - yScale2, z - zScale2)
			;

		DebuggerCube* Temp = new DebuggerCube;

		Temp->m_IndexOffset = m_FrameIndicesNum;
		Temp->m_Color.m_x = r;
		Temp->m_Color.m_y = g;
		Temp->m_Color.m_z = b;

		VBottomB1.VertexPosition = BottomB1;
		VBottomB2.VertexPosition = BottomB2;
		VBottomF1.VertexPosition = BottomF1;
		VBottomF2.VertexPosition = BottomF2;

		VTopB1.VertexPosition = TopB1;
		VTopB2.VertexPosition = TopB2;
		VTopF1.VertexPosition = TopF1;
		VTopF2.VertexPosition = TopF2;

		m_FrameVertexBuffer.AddVertex(VBottomB1);
		m_FrameVertexBuffer.AddVertex(VBottomB2);
		m_FrameVertexBuffer.AddVertex(VBottomF1);
		m_FrameVertexBuffer.AddVertex(VBottomF2);
		m_FrameVertexBuffer.AddVertex(VTopB1);
		m_FrameVertexBuffer.AddVertex(VTopB2);
		m_FrameVertexBuffer.AddVertex(VTopF1);
		m_FrameVertexBuffer.AddVertex(VTopF2);


		m_FrameIndexBuffer.AddIndex(m_FrameVerticesNum);
		m_FrameIndexBuffer.AddIndex(m_FrameVerticesNum + 1);
		m_FrameIndexBuffer.AddIndex(m_FrameVerticesNum);
		m_FrameIndexBuffer.AddIndex(m_FrameVerticesNum + 2);
		m_FrameIndexBuffer.AddIndex(m_FrameVerticesNum);
		m_FrameIndexBuffer.AddIndex(m_FrameVerticesNum + 4);
		m_FrameIndexBuffer.AddIndex(m_FrameVerticesNum + 2);
		m_FrameIndexBuffer.AddIndex(m_FrameVerticesNum + 3);
		m_FrameIndexBuffer.AddIndex(m_FrameVerticesNum + 1);
		m_FrameIndexBuffer.AddIndex(m_FrameVerticesNum + 3);
		m_FrameIndexBuffer.AddIndex(m_FrameVerticesNum + 2);
		m_FrameIndexBuffer.AddIndex(m_FrameVerticesNum + 6);
		m_FrameIndexBuffer.AddIndex(m_FrameVerticesNum + 7);
		m_FrameIndexBuffer.AddIndex(m_FrameVerticesNum + 6);
		m_FrameIndexBuffer.AddIndex(m_FrameVerticesNum + 4);
		m_FrameIndexBuffer.AddIndex(m_FrameVerticesNum + 6);
		m_FrameIndexBuffer.AddIndex(m_FrameVerticesNum + 4);
		m_FrameIndexBuffer.AddIndex(m_FrameVerticesNum + 5);
		m_FrameIndexBuffer.AddIndex(m_FrameVerticesNum + 5);
		m_FrameIndexBuffer.AddIndex(m_FrameVerticesNum + 7);
		m_FrameIndexBuffer.AddIndex(m_FrameVerticesNum + 5);
		m_FrameIndexBuffer.AddIndex(m_FrameVerticesNum + 1);
		m_FrameIndexBuffer.AddIndex(m_FrameVerticesNum + 3);
		m_FrameIndexBuffer.AddIndex(m_FrameVerticesNum + 7);

		m_FrameIndicesNum += 24;
		m_FrameVerticesNum += 8;

		m_FrameObjs.push_back(Temp);
	}

	void GraphicDebugger::AddFrameDebuggerCube(Vector3 center, float xScale, float yScale, float zScale, float r, float g, float b)
	{
		AddFrameDebuggerCube(center.m_x, center.m_y, center.m_z, xScale, yScale, zScale, r,g,b);
	}
}