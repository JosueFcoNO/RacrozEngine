#pragma once

namespace rczEngine
{
	class RacrozRenderer;

	///TODO: Move Passes here

	class RenderPipeline
	{
	public:
		RZ_EXP virtual void InitRenderPipeline(const String& name, int32 width, int32 height, RacrozRenderer * renderer) = 0;
		RZ_EXP virtual void DoRender();

		RZ_EXP void CreateRenderTarget(const String& name, int32 width, int32 height, Gfx::eFORMAT format, int32 mipmaps) ;

		RZ_EXP WeakPtr<Gfx::RenderTarget> GetFinalRenderTarget() 
		{ 
			const auto index = m_PassesOrder.size() - 1;
			const auto rt = m_Passes[m_PassesOrder[index]];
			
			return rt->GetRenderTarget();
		};
		RZ_EXP WeakPtr<Gfx::RenderTarget> GetRenderTarget(const String& renderTarget) { return m_RTs[renderTarget]; };

		RZ_EXP const Vector<String>& GetPassesOrder() { return m_PassesOrder; };

		RZ_EXP int GetPassesNum() { return m_PassesOrder.size(); };

		RZ_EXP const StrPtr<Pass> GetPass(int index) 
		{ 
			if (m_PassesOrder[index] == "PostProcess")
			{
				return nullptr;
			}
			{
				return m_Passes[m_PassesOrder[index]];
			}
		};

		RZ_EXP StrPtr<Pass> CreatePass(const String& name, ePasses pass, eRenderingPipelines renderMode);

	protected:
		RacrozRenderer * m_Renderer;

		Map<String, StrPtr<Pass>> m_Passes;

		///A map of textures from the Render passes
		Map<String, StrPtr<Texture2D>> m_Textures;

		///A Map from the render targets.
		Map<String, StrPtr<Gfx::RenderTarget>> m_RTs;

		///The passes order.
		Vector<String> m_PassesOrder;

		///Base Width
		int m_Width;

		///Base Height
		int m_Height;
	};
}