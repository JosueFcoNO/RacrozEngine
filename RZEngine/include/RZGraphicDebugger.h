#pragma once

namespace rczEngine {
	class RZ_EXP GraphicDebugger
	{
	private:
		static GraphicDebugger*& _Instance();

	public:
		static void Start();
		static GraphicDebugger* Pointer();
		static void ShutDown();

	public:
		~GraphicDebugger() { DestroyGDebugger(); };

		///Init the Graphic Debugger
		void InitGDebugger();
		///Frees the buffers, shaders and vectors
		void DestroyGDebugger();
		///Renders the objects
		void RenderGDebugger(Gfx::GfxCore* gfx);
		///Updates the objects
		void UpdateGDebugger(float deltaTime);

		WeakPtr<DebuggerLineList> AddLineList(const String& ID, const Vector<Vector3>& pointList, const Color& color, float time = -1.0f);
		WeakPtr<DebuggerLineList> AddLineListIndex(const String & ID, const Vector<Vector3>& pointList, const Vector<uint32>& indices, const Color & color, float time);
		
		WeakPtr<DebuggerPoint> AddPoint(const String& ID, const Vector3& point, float radius, const Color& color, float time = -1.0f);
		WeakPtr<DebuggerCube> AddCube(const String& ID, const Vector3& center, const Vector3& scale, const Color& color, float time = -1.0f);

	private:
		Map<String, StrPtr<GraphicDebuggerElement>> m_Objects;

		Gfx::PixelShader m_Shader;
		Gfx::ConstantBuffer m_ColorCB;
		Gfx::RasterizerState m_LinesRS;
	};
}