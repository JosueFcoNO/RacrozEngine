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
	class RZ_EXP Pass
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
		void AddTexture2D(StrPtr<Texture2D>&  texture, int32 slot);

		///Binds a Render Target to a slot for this pass to render to.
		void AddRenderTarget(StrPtr<Gfx::RenderTarget>& renderTarget, int32 slot);

		///Adds a RasterizerState for this pass to use. If there's not one set, it will use the default rasterizer.
		void AddRasterizerState(Gfx::RasterizerState* rasterizer);

		///Adds a BlendState for this pass to use. If there's not one set, it will use the default blend state.
		void AddBlendState(Gfx::BlendState* blendState, int32 slot);

		bool UseDefaultRenderTarget = true;

#ifndef EDITOR
		virtual void RenderPassGUI()
		{
			ImGui::Text("%s", m_Name.c_str());

			

			if (ImGui::Button("Recompile Pixel Shader"))
			{
				m_PShader.Destroy();
				m_gfx->CompileAndCreatePixelShader(m_PShader, m_PShaderPath.c_str());
			}

			for (int i = 0; i < 8; ++i)
			{
				if (m_RenderTargets[i])
				{
					ImVec2 size = ImGui::GetWindowSize();
					size.y *= m_RenderTargets[i]->GetTextureCore()->m_Width / m_RenderTargets[i]->GetTextureCore()->m_Height;
					ImGui::Text("Renders Target %d: %s", i, m_RenderTargets[i]->m_Name.c_str());
					ImGui::Image(m_RenderTargets[i]->GetTextureCore()->m_ShaderResource, size);
				}
			}
		}
#endif

		String m_Name;

	protected:
		///Sets the rasterizer state.
		void SetRasterizerState();

		///Sets the textures on the pipeline.
		void SetTexturesInPipeline();

		///Binds the render targets on the pipeline.
		void SetRenderTargetsInPipeline();
		
		static const char MAX_TEXTURES_PASS = 10;
		static const char MAX_RENDER_TARGETS = 8;

		bool UseDepth = true;

		Gfx::GfxCore* m_gfx = Gfx::GfxCore::Pointer();
		ResVault* m_res = ResVault::Pointer();

		Gfx::PixelShader m_PShader;
		StringW m_PShaderPath;

		StrPtr<Texture2D> m_Texture2D[MAX_TEXTURES_PASS] = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
		
		StrPtr<Gfx::RenderTarget> m_RenderTargets[MAX_RENDER_TARGETS] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
		Gfx::RasterizerState* m_RasterizerState = nullptr;
		Gfx::BlendState* m_BlendState = nullptr;
	};
}