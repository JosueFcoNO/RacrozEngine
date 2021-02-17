#pragma once

namespace rczEngine
{
	class RZ_EXP PipelineWindow : public BaseWindow
	{
	public:

		void SetRenderPipeline(StrPtr<RenderPipeline> pipeline) 
		{ 
			m_LinkedRenderPipeline = pipeline; 
		
			const auto pipeline2 = m_LinkedRenderPipeline.lock();
			const auto passNum = pipeline2->GetPassesNum();

			m_CurrentPass = passNum - 1;
			m_CurrentRT = 0;
		};

	protected:
		virtual void Render() override;

		WeakPtr<RenderPipeline> m_LinkedRenderPipeline;

		int m_CurrentPass = 0;
		int m_CurrentRT = 0;
	};
}