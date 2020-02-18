#include <RZRendererPCH.h>

namespace rczEngine
{
	void TerrainGeometryPass::CreatePipeline(eRenderingPipelines mode)
	{
		///Geometry Vertex Shader
		m_gfx->CompileAndCreateVertexShader(m_Vshader, L"Shaders/ProcGen/Terrain/TerrainGenVS.hlsl");
		m_Vshader.ReflectLayout(0, m_gfx);

		m_gfx->CompileAndCreateHullShader(m_Hshader, L"Shaders/Tess/TessHS.hlsl");

		m_gfx->CompileAndCreateDomainShader(m_Dshader, L"Shaders/ProcGen/Terrain/TerrainDomainShaderTexSampling.hlsl");

		///Geometry Pixel Shader
		m_PShaderPath = L"Shaders/ProcGen/Terrain/TerrainGeometryPass.hlsl";
		m_gfx->CompileAndCreatePixelShader(m_PShader, m_PShaderPath.c_str());

		wire.Init(Gfx::FILL_WIREFRAME);
		wire.CreateRasterizerState(m_gfx);
	}

	void TerrainGeometryPass::PreRenderPass()
	{

		///Set the shaders.
		m_PShader.SetThisPixelShader(m_gfx);
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
		//RacrozRenderer::PrepareRender(SceneManager::Pointer()->GetActiveScene().get(), CMP_TERRAIN_RENDERER, PBR_MetRough, true);
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