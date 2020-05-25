#include <RZRendererPCH.h>

namespace rczEngine
{
	void SkyBoxPass::CreatePipeline(eRenderingPipelines mode)
	{
		///Load the skybox Cube
		auto resMng = ResVault::Pointer();
		m_Cube = resMng->GetResource<Model>(resMng->m_ModelCube);

		m_gfx = Gfx::GfxCore::Pointer();
		m_res = resMng;

		m_gfx->CompileAndCreateVertexShader(m_VShader, L"Shaders/SkyBoxShader.hlsl");
		m_VShader.ReflectLayout(0, m_gfx);

		m_PShaderPath = L"Shaders/SkyBoxShader.hlsl";
		m_gfx->CompileAndCreatePixelShader(m_PShader, m_PShaderPath.c_str());

		m_SkyBoxBuffer.CreateConstantBuffer(sizeof(Matrix4::m_elements), Gfx::USAGE_DEFAULT, m_gfx);

		m_SkyMatrix.Identity();

		m_CubeMap = ResVault::Pointer()->GetResource<CubeMap>(ResVault::Pointer()->m_CubeMapDefault);
	}

	void SkyBoxPass::PreRenderPass()
	{
		RacrozRenderer::Pointer()->UpdateCameraBuffersActiveCamera();
		RacrozRenderer::Pointer()->SetCameraBuffersInPipeline(5, 5);

		m_VShader.SetThisVertexShaderAndInputLayout(m_gfx);
		m_PShader.SetThisPixelShader(m_gfx);

		SetRenderTargetsInPipeline();
		m_CubeMap.lock()->SetThisTextureInPS(12, 1, m_gfx);
		SetRasterizerState();

		m_gfx->ClearRenderTargetView(0, 0, 0, 0, 0);
		m_gfx->ClearDepthTargetView();

		m_SkyMatrix = CameraManager::Pointer()->GetActiveCamera().lock()->GetViewMatrix();
		m_SkyMatrix.m_rows[3].Set(0, 0, 0, 1);
		m_SkyMatrix.Transpose();

		m_SkyBoxBuffer.UpdateConstantBuffer(&m_SkyMatrix, m_gfx);
		m_SkyBoxBuffer.SetBufferInPS(0, m_gfx);
		m_SkyBoxBuffer.SetBufferInVS(0, m_gfx);
	}

	void SkyBoxPass::RenderPass()
	{
		m_res->GetResource<Model>(m_res->m_ModelCube).lock()->DrawModel(NULL);
	}

	void SkyBoxPass::PostRenderPass()
	{
		m_gfx->UnbindRenderTargets();
	}

}