#pragma once

namespace rczEngine
{
	///The Rendering Modes the engine can use.
	enum RENDERING_MODE
	{
		FORWARD,
		DEFERRED,
		FORWARD_PLUS
	};

	///A Base Class to derive into concrete Passes.
	class RZ_UTILITY_EXPORT Pass
	{
	public:
		virtual ~Pass() {};

		///Sets the rendering mode for the pass.
		virtual void SetRenderingMode(RENDERING_MODE mode) = 0;

		///Sets the buffers, shaders, textures and render targets before rendering geomtry.
		virtual void PreRenderPass() = 0;

		///Renders the geometry from the pass.
		virtual void RenderPass() = 0;

		///Unbinds the render targets.
		virtual void PostRenderPass() = 0;

		///Adds a texture to set on before rendering this pass on the specified slot.
		void AddTexture2D(Texture2D* texture, int32 slot);

		///Binds a Render Target to a slot for this pass to render to.
		void AddRenderTarget(Gfx::RenderTarget* renderTarget, int32 slot);

		///Adds a RasterizerState for this pass to use. If there's not one set, it will use the default rasterizer.
		void AddRasterizerState(Gfx::RasterizerState* rasterizer);

		///Adds a BlendState for this pass to use. If there's not one set, it will use the default blend state.
		void AddBlendState(Gfx::BlendState* blendState, int32 slot);

	protected:
		///Sets the rasterizer state.
		void SetRasterizerState();

		///Sets the textures on the pipeline.
		void SetTexturesInPipeline();

		///Binds the render targets on the pipeline.
		void SetRenderTargetsInPipeline();
		
		static const char MAX_TEXTURES_PASS = 10;
		static const char MAX_RENDER_TARGETS = 8;

		Gfx::GfxCore* m_gfx = Gfx::GfxCore::Pointer();
		ResVault* m_res = ResVault::Pointer();

		String m_Name;
	
		Texture2D* m_Texture2D[MAX_TEXTURES_PASS] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
		
		Gfx::RenderTarget* m_RenderTargets[MAX_RENDER_TARGETS] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
		Gfx::RasterizerState* m_RasterizerState = NULL;
		Gfx::BlendState* m_BlendState = NULL;
	};
}