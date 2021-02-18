#pragma once

namespace rczEngine
{
	class RZ_EXP VXGIWindow : public BaseWindow
	{
	public:

	protected:
		virtual void Render() override;
		rczEngine::RenderingMode m_RenderingMode;

		float g_fVoxelSize = 8.0f;
		float g_fDiffuseScale = 1.0f;
		float g_fSpecularScale = 1.0f;
		bool g_bEnableMultiBounce = true;
		float g_fMultiBounceScale = 2.0f;
		bool g_bEnableGI = true;
		float g_fSamplingRate = 1.0f;
		float g_fQuality = 0.5f;
		int g_nMapSize = 128;
		bool g_bTemporalFiltering = true;

		float g_fVxaoRange = 512.0f;
		float g_fVxaoScale = 2.0f;
		
		float g_ssaoRadius = 80.f;
	};
}