#include <RZRendererPCH.h>

namespace rczEngine
{
	void PlanetPass::SetRenderingMode(RENDERING_MODE mode)
	{
		m_ActiveScene = SceneManager::Pointer()->GetActiveScene();
		
		m_gfx->CompileAndCreateVertexShader(m_PlanetVshader, L"Shaders/ProcGen/Planet/PlanetGeometryPass.hlsl");
		m_PlanetVshader.ReflectLayout(0, m_gfx);

		m_PShaderPath = L"Shaders/ProcGen/Planet/PlanetPixelShader.hlsl";
		m_gfx->CompileAndCreatePixelShader(m_PShader, m_PShaderPath.c_str());
	}

	void PlanetPass::PreRenderPass()
	{
		SetRenderTargetsInPipeline();
		SetTexturesInPipeline();
		SetRasterizerState();

		RacrozRenderer::Pointer()->GetSkyBox().lock()->SetThisTextureInPS(12, 1, m_gfx);

		UseDepth = true;

		///Set the rasterizer state.
		m_gfx->SetRSStateDefault();
	}

	void PlanetPass::RenderPass()
	{
		auto space = SpaceManager::Pointer();

		if (space)
		{
			//m_PlanetHshader.SetThisHullShader(m_gfx);
			//m_PlanetDshader.SetThisDomainShader(m_gfx);
			m_PlanetVshader.SetThisVertexShaderAndInputLayout(m_gfx);
			m_PShader.SetThisPixelShader(m_gfx);

			space->Render();

			//m_gfx->RemoveDomainShader();
			//m_gfx->RemoveHullShader();
		}

	}

	void PlanetPass::PostRenderPass()
	{
		m_gfx->UnbindRenderTargets();
		for (int32 i = 0; i < 10; ++i)
			m_gfx->UnbindPSShaderResource(i);

		if (m_RasterizerState)
			m_gfx->SetRSStateDefault();

		if (m_BlendState)
			m_gfx->SetBlendStateDefault();
	}
};