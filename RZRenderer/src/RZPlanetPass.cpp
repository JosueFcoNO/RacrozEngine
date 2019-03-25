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
		if (Input::Pointer()->CheckKeyboardState(KEY_NUMPAD1))
		{
			m_PlanetDshader.Destroy();
			m_gfx->CompileAndCreateDomainShader(m_PlanetDshader, L"Shaders/ProcGen/Planet/PlanetDomainShader.hlsl");
		}

		if (Input::Pointer()->CheckKeyboardState(KEY_NUMPAD3))
		{
			m_PlanetHshader.Destroy();
			m_gfx->CompileAndCreateHullShader(m_PlanetHshader, L"Shaders/ProcGen/Planet/PlanetHullShader.hlsl");
		}

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