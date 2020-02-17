#include <RZRendererPCH.h>

namespace rczEngine
{
	void GraphicDebuggerPass::SetRenderingMode(eRenderingPipelines mode)
	{
		m_PShaderPath = L"Shaders/DebuggerShader.hlsl";
		m_gfx->CompileAndCreatePixelShader(m_PShader, m_PShaderPath.c_str());

		m_gfx->CompileAndCreateVertexShader(m_VShader, m_PShaderPath.c_str());
		m_VShader.ReflectLayout(0, m_gfx);

		UseDepth = true;
	}

	void GraphicDebuggerPass::PreRenderPass()
	{
		CameraManager::Pointer()->UpdateAndSetCameraBuffer(m_gfx, 5, 5);

		m_PShader.SetThisPixelShader(m_gfx);
		m_VShader.SetThisVertexShaderAndInputLayout(m_gfx);
		
		SetRenderTargetsInPipeline();
		m_gfx->ClearRenderTargetView(0, 0.0f, 0,0,1);
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