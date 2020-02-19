#pragma once

namespace rczEngine
{
	class RZ_EXP PipelineForward : public RenderPipeline
	{
	public:
		void InitRenderPipeline(const String& name, int32 width, int32 height, RacrozRenderer * renderer);

		Gfx::DepthStencyl depth;

	private:
	};
}