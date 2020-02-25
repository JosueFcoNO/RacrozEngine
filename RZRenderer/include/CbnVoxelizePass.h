#pragma once

namespace rczEngine
{
	class RZ_EXP VoxelizePass : public Pass
	{
	public:
		///Inits the pass with a name and a rendering mode.
		virtual void CreatePipeline(eRenderingPipelines mode);

		///Sets the buffers, shaders, textures and render targets before rendering geomtry.
		virtual void PreRenderPass();

		///Renders the geometry from the pass.
		virtual void RenderPass();

		///Unbinds the render targets.
		virtual void PostRenderPass();

		Matrix4 VoxelizeViewMatrix;
		Matrix4 VoxelizeProjMatrix;

	private:
		

		WeakPtr<Scene> m_ActiveScene;
		
		RendererConfig config;

		StrPtr<Texture2D> m_LUT;

		Gfx::VertexShader m_GeometryPBRShader;
		Gfx::VertexShader m_SkinnedGeometryPBRShader;

		Gfx::ConstantBuffer UserDisney;
	};

}
