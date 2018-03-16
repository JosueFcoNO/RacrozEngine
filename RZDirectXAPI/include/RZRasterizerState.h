#pragma once

namespace rczEngine
{
	namespace Gfx
	{
		class RZ_UTILITY_EXPORT RasterizerState
		{
		public:
			RasterizerState() { m_RasterizerState = NULL; };
			~RasterizerState() { if (m_RasterizerState) m_RasterizerState->Release(); };

			void Init(
				eFILL_MODE fillMode = FILL_SOLID,
				eCULL_MODE cullMode = CULL_BACK,
				bool frontCounterClockwise = false,
				uint32 depthBias = 0,
				float depthBiasClamp = 0.0f,
				float slopeScaledDepthBias = 0.0f,
				bool depthClipEnable = true,
				bool scissorEnable = false,
				bool multisampleEnable = false,
				bool antialiasedLineEnable = false);

			bool CreateRasterizerState(GfxCore* gfx = NULL);
			bool SetThisRasterizerState(GfxCore* gfx = NULL);

			ID3D11RasterizerState* m_RasterizerState;

			eFILL_MODE m_FillMode;
			eCULL_MODE m_CullMode;
			bool	   m_FrontCounterClockwise;
			uint32     m_DepthBias;
			float      m_DepthBiasClamp;
			float      m_SlopeScaledDepthBias;
			bool       m_DepthClipEnable;
			bool       m_ScissorEnable;
			bool       m_MultisampleEnable;
			bool       m_AntialiasedLineEnable;

		};
	}
}