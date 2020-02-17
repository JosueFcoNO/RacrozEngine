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
		auto passDebugger = m_Renderer->CreatePass(name + "Debugger", PASSES::GDEBUGGER, eRenderingPipelines::Deferred);

		passDebugger->AddRenderTarget(m_RTs["Debug"], 0);
		passDebugger->AddDepthStencyl(&depth);

		m_PassesOrder.push_back(name + "Debugger");
	}

	void PipelineDebug::DoRender()
	{
		int32 i = 0;

		for (i = 0; i < m_PassesOrder.size(); ++i)
		{
			if (m_PassesOrder[i] == "PostProcess")
			{
				m_Renderer->StartPostProcessing();
				continue;
			}

			auto pass = m_Renderer->m_Passes[m_PassesOrder[i]];

			pass->PreRenderPass();
			pass->RenderPass();
			pass->PostRenderPass();
		}
	}
}