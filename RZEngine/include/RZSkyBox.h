#pragma once

namespace rczEngine
{
	class RZ_EXP SkyBox
	{
	public:
		void InitSkyBox(char* cubeMapPath, char* cubeMapName, Gfx::GfxCore* gfx, ResVault* res);
		void InitSkyBox(ResourceHandle cubeMapHandle, Gfx::GfxCore* gfx, ResVault* res);

		void RenderSkyBox(WeakPtr<Model> cube, Matrix4& viewMatrix, Gfx::GfxCore* gfx, ResVault* res);

		
		FORCEINLINE WeakPtr<CubeMap> GetCubeMap() { return m_CubeMap; };

	private:
		Gfx::ConstantBuffer m_SkyBoxBuffer;
		Matrix4 m_ViewMatrix;
		WeakPtr<CubeMap> m_CubeMap;
	};
}