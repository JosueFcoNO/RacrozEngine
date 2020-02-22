#include <RZRendererPCH.h>

namespace rczEngine
{
	void PBR_Transparent_Pass::CreatePipeline(eRenderingPipelines mode)
	{
		///Load the LUT.
		m_LUT = m_res->GetResource<Texture2D>(m_res->LoadResource("RacrozEngineAssets/BRDF LUT.png", "LUT")).lock();
		
		m_PShaderPath = L"Shaders/PBR_Forward.hlsl";
		m_gfx->CompileAndCreatePixelShader(m_PShader, m_PShaderPath.c_str());

		AddTexture2D(m_LUT, 7);

		config.AmbientLightIntensity = 1.0f;
		config.LightIntensity = 1.0f;
		config.ReflectionIntensity = 1.0f;
		config.Wireframe = false;
		config.R = config.G = config.B = 1;

		//UserDisney.CreateConstantBuffer(sizeof(RendererConfig), Gfx::USAGE_DEFAULT, m_gfx);
		//UserDisney.UpdateConstantBuffer(&config, m_gfx);
		//UserDisney.SetBufferInPS(6, m_gfx);

		Transparent.InitBlendState();
		Transparent.CreateBlendState(m_gfx);

		CullBack.Init();
		CullBack.m_CullMode = Gfx::CULL_FRONT;
		CullBack.m_FrontCounterClockwise = true;
		CullBack.CreateRasterizerState(m_gfx);


		CullFront.Init();
		CullFront.m_CullMode = Gfx::CULL_FRONT;
		CullBack.m_FrontCounterClockwise = false;
		CullFront.CreateRasterizerState(m_gfx);

		UseDepth = true;
	}

	void PBR_Transparent_Pass::PreRenderPass()
	{
		LightManager::Pointer()->SetLightsBuffers();

		m_PShader.SetThisPixelShader(m_gfx);

		//UserDisney.UpdateConstantBuffer(&config, m_gfx);
		//UserDisney.SetBufferInPS(6, m_gfx);

		//m_gfx->ClearDepthTargetView();

		SetTexturesInPipeline();
		SetRasterizerState();


		Transparent.SetThisBlendState(m_gfx);
	}

	void PBR_Transparent_Pass::RenderPass()
	{
		UseDepth = true;

		//SetRenderTargetsInPipeline();
		//CullFront.SetThisRasterizerState(m_gfx);
		//RacrozRenderer::PrepareRender(SceneManager::Pointer()->GetActiveScene().get(), CMP_MODEL_RENDERER, MAT_PBR_MetRough_Trans, false);
		////m_gfx->UnbindRenderTargets();
		//
		////SetRenderTargetsInPipeline();
		//CullBack.SetThisRasterizerState(m_gfx);
		//RacrozRenderer::PrepareRender(SceneManager::Pointer()->GetActiveScene().get(), CMP_MODEL_RENDERER, MAT_PBR_MetRough_Trans, false);

		m_gfx->SetRSStateDefault();
		
		m_gfx->SetBlendStateDefault();
	}

	void PBR_Transparent_Pass::PostRenderPass()
	{
		m_gfx->UnbindRenderTargets();
		for (int32 i = 0; i < 10; ++i)
			m_gfx->UnbindPSShaderResource(i);

	}
};