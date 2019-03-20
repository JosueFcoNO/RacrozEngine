#include <RZRendererPCH.h>

namespace rczEngine
{
	void RenderPipeline::CreateRenderTarget(const String & name, int32 width, int32 height, Gfx::eFORMAT format, int32 mipmaps)
	{
		m_RTs[name.c_str()] = m_Renderer->CreateRenderTargetAndTexture_WidthHeight(name, m_Textures[name], mipmaps, width, height, format);
	}
}