#include <RZRendererPCH.h>

namespace rczEngine
{
	void GraphicDebuggerPass::SetRenderingMode(RENDERING_MODE mode)
	{
		m_PShaderPath = L"Shaders/DebuggerShader.hlsl";
		m_gfx->CompileAndCreatePixelShader(m_PShader, m_PShaderPath.c_str());

		m_gfx->CompileAndCreateVertexShader(m_VShader, m_PShaderPath.c_str());
		m_VShader.ReflectLayout(0, m_gfx);

		UseDepth = true;
	}

	void GraphicDebuggerPass::PreRenderPass()
	{
		m_PShader.SetThisPixelShader(m_gfx);
		m_VShader.SetThisVertexShaderAndInputLayout(m_gfx);
		
		SetRenderTargetsInPipeline();
	}

	void GraphicDebuggerPass::RenderPass()
	{
		GraphicDebugger::Pointer()->Render(m_gfx);
	}

	void GraphicDebuggerPass::PostRenderPass()
	{
		m_gfx->UnbindRenderTargets();
	}
};