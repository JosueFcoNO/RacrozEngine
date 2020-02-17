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

	void GraphicDebugger::InitGDebugger()
	{
		auto gfx = Gfx::GfxCore::Pointer();
		m_Shader.CreatePixelShader(L"Shaders/DebuggerShader.hlsl", gfx);
		m_ColorCB.CreateConstantBuffer(sizeof(Vector4), Gfx::USAGE_DEFAULT, gfx);

		m_LinesRS.Init(Gfx::eFILL_MODE::FILL_WIREFRAME);
		m_LinesRS.CreateRasterizerState(gfx);
	}

	void GraphicDebugger::DestroyGDebugger()
	{
		m_ColorCB.Destroy();
	}

	void GraphicDebugger::RenderGDebugger(Gfx::GfxCore * graphicsapi_instance)
	{
		///Set the Graphic debugger's  pixel shader
		//m_Shader.SetThisPixelShader(graphicsapi_instance);

		///Set the ColorBuffer in the pipeline
		m_ColorCB.SetBufferInPS(0, graphicsapi_instance);

		///Set the Rasterizer State
		//m_LinesRS.SetThisRasterizerState(graphicsapi_instance);

		///Change the Topology to linelist
		//graphicsapi_instance->SetPrimitiveTopology(Gfx::TOPO_LINELIST);

		///Iterate through the Static Objs and Draw Them
		for (auto obj : m_Objects)
		{
			if (!obj.second) continue;

			auto color = obj.second->GetColor();

			///Update the color buffer
			m_ColorCB.UpdateConstantBuffer(&color, graphicsapi_instance);
			///Render the static object
			obj.second->Render(graphicsapi_instance);
		}

		graphicsapi_instance->SetPrimitiveTopology(Gfx::eTOPOLOGY::TOPO_TRIANGLELIST);
	}

	void GraphicDebugger::UpdateGDebugger(float deltaTime)
	{
		///Iterate through the Static Objs and Draw Them
		for (auto obj : m_Objects)
		{
			obj.second->Update(deltaTime);

			if (obj.second->CanRemove())
			{
				//m_Objects.erase(obj.first);
			}
		}
	}

	WeakPtr<DebuggerLineList> GraphicDebugger::AddLineList(const String& ID, const Vector<Vector3>& pointList, const Color& color, float time)
	{
		auto newLineList = std::make_shared<DebuggerLineList>();

		newLineList->SetLineList(pointList);
		newLineList->SetColor(color);

		m_Objects[ID] = (newLineList);

		return newLineList;
	}

	WeakPtr<DebuggerLineList> GraphicDebugger::AddLineListIndex(const String & ID, const Vector<Vector3>& pointList, const Vector<uint32>& indices, const Color & color, float time)
	{
		auto newLineList = std::make_shared<DebuggerLineList>();

		newLineList->SetLineListIndex(pointList, indices);
		newLineList->SetColor(color);

		m_Objects[ID] = (newLineList);

		return newLineList;
	}

	WeakPtr<DebuggerPoint> GraphicDebugger::AddPoint(const String& ID, const Vector3& point, float radius, const Color& color, float time)
	{
		auto newPoint = std::make_shared<DebuggerPoint>();

		newPoint->SetPoint(point, radius);
		newPoint->SetColor(color);

		if (m_Objects.find(ID) != m_Objects.end())
		{
			m_Objects[ID+std::to_string(rand()%10000)] = newPoint;
			return newPoint;
		}

		m_Objects[ID] = (newPoint);

		return newPoint;
	}

	WeakPtr<DebuggerCube> GraphicDebugger::AddCube(const String& ID, const Vector3& center, const Vector3& scale, const Color& color, float time)
	{
		auto newCube = std::make_shared<DebuggerCube>();

		newCube->SetCube(center, scale);
		newCube->SetColor(color);

		m_Objects[ID] = (newCube);

		return newCube;

	}
}