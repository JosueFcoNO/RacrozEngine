#pragma once

namespace rczEngine {
	class RZ_UTILITY_EXPORT GraphicDebugger
	{
	private:
		static GraphicDebugger*& _Instance();

	public:
		static void Start();
		static GraphicDebugger* Pointer();
		static void ShutDown();

	public:
		GraphicDebugger() {};
		~GraphicDebugger() { Destroy(); };

		///Init the Graphic Debugger
		void Init();
		///Frees the buffers, shaders and vectors
		void Destroy();
		///Renders the objects
		void Render(Gfx::GfxCore* graphicsapi_instance);

		///Adds a Static Debugger Line from 2 coordinates
		void AddStaticDebuggerLine(float x1, float y1, float z1, float x2, float y2, float z2, float r, float g, float b);
		///Adds a Static Debugger Line from 2 vectors
		void AddStaticDebuggerLine(Vector3 point1, Vector3 point2, float r, float g, float b);

		void AddStaticDebuggerPoint(float x, float y, float z, float radius, float r, float g, float b);
		void AddStaticDebuggerPoint(Vector3 point, float radius, float r, float g, float b);
		
		///Adds a Static Debugger Cube from a coordinate and a scale
		void AddStaticDebuggerCube(float x, float y, float z, float xScale, float yScale, float zScale, float r, float g, float b);
		///Adds a Static Debugger Cube from a Vector and a scale
		void AddStaticDebuggerCube(Vector3 center, float xScale, float yScale, float zScale, float r, float g, float b);

		///Adds a Frame Debugger Line from 2 coordinates
		void AddFrameDebuggerLine(float x1, float y1, float z1, float x2, float y2, float z2, float r, float g, float b);
		///Adds a Frame Debugger Line from 2 vectors
		void AddFrameDebuggerLine(Vector3 point1, Vector3 point2, float r, float g, float b);
		
		///Adds a Frame Debugger Point from a coordinate and a radius
		void AddFrameDebuggerPoint(float x, float y, float z, float radius, float r, float g, float b);
		///Adds a Frame Debugger Point from a vector and a radius
		void AddFrameDebuggerPoint(Vector3 point, float radius,  float r, float g, float b);

		///Adds a Frame Debugger Cube from a coordinate and scales
		void AddFrameDebuggerCube(float x, float y, float z, float xScale, float yScale, float zScale, float r, float g, float b);
		///Adds a Frame Debugger Cube from a vector and scales
		void AddFrameDebuggerCube(Vector3 center, float xScale, float yScale, float zScale, float r, float g, float b);

	private:
		Gfx::VertexBuffer<Gfx::Vertex> m_StaticVertexBuffer;
		Gfx::IndexBuffer m_StaticIndexBuffer;

		Gfx::VertexBuffer<Gfx::Vertex> m_FrameVertexBuffer;
		Gfx::IndexBuffer m_FrameIndexBuffer;

		Vector<GraphicDebuggerElement*> m_StaticObjs;
		Vector<GraphicDebuggerElement*> m_FrameObjs;

		uint32 m_FrameIndicesNum = 0;
		uint32 m_FrameVerticesNum = 0;

		uint32 m_StaticIndicesNum = 0;
		uint32 m_StaticVerticesNum = 0;

		Model m_SphereModel;

		Gfx::PixelShader m_Shader;
		Gfx::ConstantBuffer m_ColorCB;
		Gfx::ConstantBuffer m_WorldMatrixCB;


		Gfx::RasterizerState m_LinesRS;
		Gfx::RasterizerState m_SolidRS;
	};
}