#include <RZDirectXPCH.h>
#include "RZRasterizerState.h"

namespace rczEngine
{
	namespace Gfx
	{
		void RasterizerState::Init(eFILL_MODE fillMode, eCULL_MODE cullMode, bool frontCounterClockwise, uint32 depthBias, float depthBiasClamp, float slopeScaledDepthBias, bool depthClipEnable, bool scissorEnable, bool multisampleEnable, bool antialiasedLineEnable)
		{
			m_FillMode = fillMode;
			m_CullMode = cullMode;
			m_FrontCounterClockwise = frontCounterClockwise;
			m_DepthBias = depthBias;
			m_DepthBiasClamp = depthBiasClamp;
			m_DepthClipEnable = depthClipEnable;
			m_AntialiasedLineEnable = antialiasedLineEnable;
			m_MultisampleEnable = multisampleEnable;
			m_ScissorEnable = scissorEnable;
			m_SlopeScaledDepthBias = slopeScaledDepthBias;
		}

		bool RasterizerState::CreateRasterizerState(GfxCore* gfx)
		{
			return gfx->CreateRasterizerState(*this);
		}

		bool RasterizerState::SetThisRasterizerState(GfxCore* gfx)
		{
			return gfx->SetRSState(*this);
		}
	}
};