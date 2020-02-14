#include <RZRendererPCH.h>

namespace rczEngine
{
	void GeometryPass::SetRenderingMode(RENDERING_MODE mode)
	{
		m_ActiveScene = SceneManager::Pointer()->GetActiveScene();

		///Geometry Vertex Shader
		m_gfx->CompileAndCreateVertexShader(m_Vshader, L"Shaders/GPassMetRough.hlsl");
		m_Vshader.ReflectLayout(0, m_gfx);

		///Geometry Skinned Vertex Shader
		m_gfx->CompileAndCreateVertexShader(m_SkinnedVshader, L"Shaders/SkinnedVertexShader.hlsl");
		m_SkinnedVshader.ReflectLayout(0, m_gfx);

		///Geometry Pixel Shader
		m_PShaderPath = L"Shaders/GPassMetRough.hlsl";
		m_gfx->CompileAndCreatePixelShader(m_PShader, m_PShaderPath.c_str());

		m_gfx->CompileAndCreatePixelShader(m_SmoothPS, L"Shaders/GPassSpecSmooth.hlsl");
		m_gfx->CompileAndCreatePixelShader(m_SmoothSpecAlphaPS, L"Shaders/GPassSpecSmoothAlpha.hlsl");

		m_gfx->CompileAndCreateHullShader(m_Hshader, L"Shaders/Tess/TessHS.hlsl");
		m_gfx->CompileAndCreateVertexShader(m_VTessShader, L"Shaders/Tess/TessVS.hlsl");
		m_VTessShader.ReflectLayout(0, m_gfx);
		m_gfx->CompileAndCreateDomainShader(m_Dshader, L"Shaders/Tess/TessDS.hlsl");

	}

	void GeometryPass::PreRenderPass()
	{
		///Set the shaders.
		m_Vshader.SetThisVertexShaderAndInputLayout(m_gfx);

		SetRenderTargetsInPipeline();
		SetTexturesInPipeline();
		SetRasterizerState();

		m_gfx->ClearDepthTargetView();
		m_gfx->ClearRenderTargetView(1, 0.0f, 0.0f, 0.0f, 0.0f);
		m_gfx->ClearRenderTargetView(2, 0.0f, 0.0f, 0.0f, 0.0f);
		m_gfx->ClearRenderTargetView(3, 0.0f, 0.0f, 0.0f, 0.0f);
		m_gfx->ClearRenderTargetView(4, 0.0f, 0.0f, 0.0f, 0.0f);

		///Set the rasterizer state.
		m_gfx->SetRSStateDefault();
	}

	void GeometryPass::RenderPass()
	{
		auto activeScene = m_ActiveScene.get();
		auto renderer = RacrozRenderer::Pointer();

		m_PShader.SetThisPixelShader(m_gfx);
		renderer->RenderObjs(true, CMP_MODEL_RENDERER, eMaterialType::PBR_MetRough, eShadingType::PBR, eBlendType::Opaque, false, false, false, false);

		m_SmoothPS.SetThisPixelShader(m_gfx);
		renderer->RenderObjs(true, CMP_MODEL_RENDERER, eMaterialType::PBR_SpecSmooth, eShadingType::PBR, eBlendType::Opaque, false, false, false, false);

		m_VTessShader.SetThisVertexShaderAndInputLayout(m_gfx);
		m_Hshader.SetThisHullShader(m_gfx);
		m_Dshader.SetThisDomainShader(m_gfx);

		m_PShader.SetThisPixelShader(m_gfx);
		renderer->RenderObjs(true, CMP_MODEL_RENDERER, eMaterialType::PBR_MetRough, eShadingType::PBR, eBlendType::Opaque, true, false, false, false);
		
		m_SmoothPS.SetThisPixelShader(m_gfx);
		renderer->RenderObjs(true, CMP_MODEL_RENDERER, eMaterialType::PBR_SpecSmooth, eShadingType::PBR, eBlendType::Opaque, true, false, false, false);
		
		m_gfx->RemoveDomainShader();
		m_gfx->RemoveHullShader();
		
		//m_SkinnedVshader.SetThisVertexShaderAndInputLayout(m_gfx);
		//RacrozRenderer::PrepareRender(SceneManager::Pointer()->GetActiveScene().get(), CMP_SKINNED_MODEL_RENDERER, PBR_MetRough, true);
	}

	void GeometryPass::PostRenderPass()
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