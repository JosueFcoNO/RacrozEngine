#include <RZRendererPCH.h>

namespace rczEngine
{
	void GraphicDebuggerPass::CreatePipeline(eRenderingPipelines mode)
	{
		m_PShaderPath = L"Shaders/DebuggerShader.hlsl";
		m_gfx->CompileAndCreatePixelShader(m_PShader, m_PShaderPath.c_str());

		m_gfx->CompileAndCreateVertexShader(m_VShader, m_PShaderPath.c_str());
		m_VShader.ReflectLayout(0, m_gfx);

		UseDepth = true;
	}

	void GraphicDebuggerPass::PreRenderPass()
	{
		RacrozRenderer::Pointer()->UpdateCameraBuffersActiveCamera();
		RacrozRenderer::Pointer()->SetCameraBuffersInPipeline(5, 5);
	
		m_PShader.SetThisPixelShader(m_gfx);
		m_VShader.SetThisVertexShaderAndInputLayout(m_gfx);
		
		SetRenderTargetsInPipeline();

		if (ClearBeforeRender)
		{
			m_gfx->ClearRenderTargetView(0, 0, 0, 0, 1.0f);
			m_gfx->ClearDepthTargetView();
		}
	}

	void GraphicDebuggerPass::RenderPass()
	{
		GraphicDebugger::Pointer()->RenderGDebugger(m_gfx);
	}

	void GraphicDebuggerPass::PostRenderPass()
	{
		m_gfx->UnbindRenderTargets();
	}
};