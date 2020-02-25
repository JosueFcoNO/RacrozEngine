#pragma once

namespace rczEngine
{
	class RZ_EXP PipelineDeferred : public RenderPipeline
	{
	public:
		void InitRenderPipeline(const String& name, int32 width, int32 height, RacrozRenderer * renderer);
		void DoRender();


		Gfx::DepthStencyl* depth;

	private:
	};
}