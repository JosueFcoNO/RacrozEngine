#pragma once

namespace rczEngine
{
	class RacrozRenderer;

	class RZ_EXP RenderPipeline
	{
	public:
		virtual void InitRenderPipeline(const String& name, int32 width, int32 height, RacrozRenderer * renderer) = 0;
		virtual void DoRender() = 0;

		void CreateRenderTarget(const String& name, int32 width, int32 height, Gfx::eFORMAT format, int32 mipmaps) ; 

		Vector<String> m_PassesOrder;

	protected:
		RacrozRenderer * m_Renderer;

		///A map of textures from the Render passes
		Map<String, StrPtr<Texture2D>> m_Textures;

		///A Map from the render targets.
		Map<String, StrPtr<Gfx::RenderTarget>> m_RTs;


		///Base Width
		int m_Width;

		///Base Height
		int m_Height;
	};
}