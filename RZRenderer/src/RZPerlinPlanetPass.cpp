#include <RZRendererPCH.h>

namespace rczEngine
{
	void PerlinPlanetPass::SetRenderingMode(eRenderingPipelines mode)
	{
		///Load the skybox Cube
		auto resMng = ResVault::Pointer();
		m_Cube = resMng->GetResource<Model>(resMng->m_ModelCube);

		m_gfx = Gfx::GfxCore::Pointer();
		m_res = resMng;

		m_gfx->CompileAndCreateVertexShader(m_VShader, L"Shaders/ProcGen/Planet/PlanetHeightShader.hlsl");
		m_VShader.ReflectLayout(0, m_gfx);

		m_gfx->CompileAndCreatePixelShader(m_PShader, L"Shaders/ProcGen/Planet/PlanetHeightShader.hlsl");

		rzState.Init(Gfx::FILL_SOLID, Gfx::CULL_NONE);
		rzState.CreateRasterizerState(m_gfx);
		//m_Planet = resMng->LoadResource("Models/Planet/PlanetSurfaceH.dae");

	}

	void PerlinPlanetPass::PreRenderPass()
	{
		CameraManager::Pointer()->UpdateAndSetCameraBuffer(m_gfx, 5, 5);

		m_VShader.SetThisVertexShaderAndInputLayout(m_gfx);
		m_PShader.SetThisPixelShader(m_gfx);

		rzState.SetThisRasterizerState(m_gfx);

		//m_gfx->ClearRenderTargetView(0, 0, 0, 1, 1);
		m_gfx->ClearDepthTargetView();

	}

	void PerlinPlanetPass::RenderPass()
	{
		ResVault::Pointer()->GetResource<Model>(m_Planet).lock()->DrawModel(NULL);
	}

	void PerlinPlanetPass::PostRenderPass()
	{
		m_gfx->UnbindRenderTargets();
	}

}