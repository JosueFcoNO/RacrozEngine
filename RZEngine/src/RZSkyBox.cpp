#include <RZEnginePCH.h>

namespace rczEngine
{
	void SkyBox::InitSkyBox(char * cubeMapPath, char* cubeMapName, Gfx::GfxCore * gfx, ResVault * res)
	{
		m_CubeMap = res->GetResource<CubeMap>(res->LoadResource(cubeMapPath, cubeMapName));

		m_SkyBoxBuffer.CreateConstantBuffer(sizeof(Matrix4), Gfx::USAGE_DEFAULT, gfx);
	}

	void SkyBox::InitSkyBox(ResourceHandle cubeMapHandle, Gfx::GfxCore * gfx, ResVault * res)
	{
		m_CubeMap = res->GetResource<CubeMap>(cubeMapHandle);

		m_SkyBoxBuffer.CreateConstantBuffer(sizeof(Matrix4), Gfx::USAGE_DEFAULT, gfx);
	}

	void SkyBox::RenderSkyBox(WeakPtr<Model> cube, Matrix4& viewMatrix, Gfx::GfxCore* gfx, ResVault* res)
	{
		m_ViewMatrix = viewMatrix.GetTransposed();
		m_ViewMatrix.m_rows[0].m_z = 0.0f;
		m_ViewMatrix.m_rows[1].m_z = 0.0f;
		m_ViewMatrix.m_rows[2].m_z = 0.0f;
		m_ViewMatrix.m_rows[3].m_z = 1.0f;

		m_SkyBoxBuffer.UpdateConstantBuffer(&m_ViewMatrix, gfx);
		m_SkyBoxBuffer.SetBufferInVS(0, gfx);

		m_CubeMap.lock()->SetThisTextureInPS(5, 0, gfx);

		cube.lock()->DrawModel(gfx, res, NULL);
	}
}