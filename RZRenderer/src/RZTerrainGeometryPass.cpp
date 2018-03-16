#include <RZRendererPCH.h>

namespace rczEngine
{
	void TerrainGeometryPass::SetRenderingMode(RENDERING_MODE mode)
	{
		///Geometry Vertex Shader
		m_gfx->CompileAndCreateVertexShader(m_Vshader, L"Shaders/pGenShaders/TerrainGenVS.hlsl");
		m_Vshader.ReflectLayout(m_gfx);

		m_gfx->CompileAndCreateHullShader(m_Hshader, L"Shaders/Tess/TessHS.hlsl");

		m_gfx->CompileAndCreateDomainShader(m_Dshader, L"Shaders/pGenShaders/TerrainDomainShaderTexSampling.hlsl");

		///Geometry Pixel Shader
		m_gfx->CompileAndCreatePixelShader(m_Pshader, L"Shaders/pGenShaders/TerrainGeometryPass.hlsl");

		wire.Init(Gfx::FILL_WIREFRAME);
		wire.CreateRasterizerState(m_gfx);
	}

	void TerrainGeometryPass::PreRenderPass()
	{

		///Set the shaders.
		m_Pshader.SetThisPixelShader(m_gfx);
		m_Vshader.SetThisVertexShaderAndInputLayout(m_gfx);
		m_Hshader.SetThisHullShader(m_gfx);
		m_Dshader.SetThisDomainShader(m_gfx);

		m_gfx->ClearDepthTargetView();

		SetRenderTargetsInPipeline();
		SetTexturesInPipeline();
		SetRasterizerState();
	}

	void TerrainGeometryPass::RenderPass()
	{
		///NORMAL MATERIALS
		RacrozRenderer::RenderScene(SceneManager::Pointer()->GetActiveScene().get(), CMP_TERRAIN_RENDERER, MAT_PBR_MetRough, true);
	}

	void TerrainGeometryPass::PostRenderPass()
	{
		///Set the rasterizer state.
		m_gfx->SetRSStateDefault();

		m_gfx->RemoveDomainShader();
		m_gfx->RemoveHullShader();

		m_gfx->UnbindRenderTargets();
		for (int32 i = 0; i < 10; ++i)
			m_gfx->UnbindPSShaderResource(i);

		if (m_RasterizerState)
			m_gfx->SetRSStateDefault();

		if (m_BlendState)
			m_gfx->SetBlendStateDefault();
	}
};