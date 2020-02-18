#include <RZRendererPCH.h>

namespace rczEngine
{
	void PBR_Forward_Pass::CreatePipeline(eRenderingPipelines mode)
	{
		///Load the LUT.
		m_LUT = std::make_shared<Texture2D>();
		m_LUT->Load("RacrozEngineAssets/BRDF LUT.png", "LUT");

		m_PShaderPath = L"Shaders/PBR_Forward.hlsl";
		m_gfx->CompileAndCreatePixelShader(m_PShader, m_PShaderPath.c_str());

		m_gfx->CompileAndCreateVertexShader(m_GeometryPBRShader, L"Shaders/GeometryPass.hlsl");
		m_GeometryPBRShader.ReflectLayout(0, m_gfx);

		m_gfx->CompileAndCreateVertexShader(m_SkinnedGeometryPBRShader, L"Shaders/SkinnedVertexShader.hlsl");
		m_SkinnedGeometryPBRShader.ReflectLayout(0, m_gfx);

		AddTexture2D(m_LUT, 7);

		config.AmbientLightIntensity = 1.0f;
		config.LightIntensity = 1.0f;
		config.ReflectionIntensity = 1.0f;
		config.Wireframe = false;
		config.R = config.G = config.B = 1;

		UserDisney.CreateConstantBuffer(sizeof(RendererConfig), Gfx::USAGE_DEFAULT, m_gfx);
		UserDisney.UpdateConstantBuffer(&config, m_gfx);
		UserDisney.SetBufferInPS(6, m_gfx);
	}

	void PBR_Forward_Pass::PreRenderPass()
	{
		LightManager::Pointer()->SetLightsBuffers();

		m_PShader.SetThisPixelShader(m_gfx);

		UserDisney.UpdateConstantBuffer(&config, m_gfx);
		UserDisney.SetBufferInPS(6, m_gfx);

		m_gfx->ClearDepthTargetView();

		SetRenderTargetsInPipeline();
		SetTexturesInPipeline();
		SetRasterizerState();
	}

	void PBR_Forward_Pass::RenderPass()
	{
		//m_GeometryPBRShader.SetThisVertexShaderAndInputLayout(m_gfx);
		//RacrozRenderer::PrepareRender(SceneManager::Pointer()->GetActiveScene().get(), CMP_MODEL_RENDERER, PBR_MetRough, true);
		//m_SkinnedGeometryPBRShader.SetThisVertexShaderAndInputLayout(m_gfx);
		//RacrozRenderer::PrepareRender(SceneManager::Pointer()->GetActiveScene().get(), CMP_SKINNED_MODEL_RENDERER, PBR_MetRough, true);
	}

	void PBR_Forward_Pass::PostRenderPass()
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