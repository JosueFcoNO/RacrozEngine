#include <RZRendererPCH.h>

namespace rczEngine
{
	void PipelineDebug::InitRenderPipeline(const String & name, int32 width, int32 height, RacrozRenderer * renderer)
	{
		Gfx::GfxCore::Pointer()->CreateDepthStencyl(depth, width, height);

		m_Renderer = renderer;

		///Create Render targets and textures for Position,  Normals, Color, Metallic and Roughness, Tangents and Binormals.
		///This also inserts the render targets and the textures into the RenderTarget and Texture2D Maps.
		CreateRenderTarget("Debug", width, height, Gfx::FORMAT_R8G8B8A8_UNORM, 1);

		//////////////////////
		///GDebugger PASS///
		//////////////////////

		///Create the geometry pass
		auto passDebugger = std::static_pointer_cast<GraphicDebuggerPass, Pass>(CreatePass(name + "Debugger", ePasses::GraphicDebugger, eRenderingPipelines::Deferred));

		passDebugger->AddRenderTarget(m_RTs["Debug"], 0);
		passDebugger->AddDepthStencyl(&depth);
		passDebugger->ClearBeforeRender = true;

		m_PassesOrder.push_back(name + "Debugger");
	}
}