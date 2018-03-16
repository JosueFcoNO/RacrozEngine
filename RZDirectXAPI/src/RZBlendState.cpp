#include <RZDirectXPCH.h>
#include "RZBlendState.h"

namespace rczEngine
{
	namespace Gfx
	{
		void BlendState::InitBlendState(bool blendEnable, BLEND_OP blendOp, BLEND_OP blendOpAlpha, BLEND destBlend, BLEND destBlendAlpha, BLEND srcBlend, BLEND srcBlendAlpha)
		{
			m_BlendEnable = blendEnable;
			m_BlendOp = blendOp;
			m_BlendOpAlpha = blendOpAlpha;
			m_DestBlend = destBlend;
			m_DestBlendAlpha = destBlendAlpha;
			m_SrcBlendAlpha = srcBlendAlpha;
			m_SrcBlend = srcBlend;
		}

		bool BlendState::CreateBlendState(GfxCore * gfx)
		{
			return gfx->CreateBlendState(*this);
		}

		bool BlendState::SetThisBlendState(GfxCore * gfx)
		{
			return gfx->SetBlendState(*this);
		}
	}
};