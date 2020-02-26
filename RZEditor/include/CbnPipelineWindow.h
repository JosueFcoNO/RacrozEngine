#pragma once

namespace rczEngine
{
	class RZ_EXP PipelineWindow : public BaseWindow
	{
	public:

		void SetRenderPipeline(StrPtr<RenderPipeline> pipeline) { m_LinkedRenderPipeline = pipeline; };

	protected:
		virtual void Render() override;

		WeakPtr<RenderPipeline> m_LinkedRenderPipeline;

		int m_CurrentPass = 0;
		int m_CurrentRT = 0;
	};
}