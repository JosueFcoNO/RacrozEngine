#include <RZDirectXPCH.h>
#include "RZSamplerState.h"

namespace rczEngine
{
	namespace Gfx
	{
		void SamplerState::Init(eTEXTURE_ADDRESS u, eTEXTURE_ADDRESS v, eTEXTURE_ADDRESS w, eCOMPARISON_FUNC cFunc, eFILTER filter, Vector4 borderColor, uint32 maxAniso, float maxLOD, float minLOD, float mipLODBias)
		{
			m_TexAddressU = u;
			m_TexAddressV = v;
			m_TexAddressW = w;
			m_ComparisonFunc = cFunc;
			m_Filter = filter;
			m_BorderColor = borderColor;
			m_MaxAnisotropy = maxAniso;
			m_MaxLOD = maxLOD;
			m_MinLOD = minLOD;
			m_MipLODBias = mipLODBias;
		}

		bool SamplerState::CreateSamplerState(GfxCore* gfx)
		{
			return gfx->CreateSamplerState(*this);
		}

		void SamplerState::PSSetThisSamplerState(uint32 startSlot, uint32 NumView, GfxCore* gfx)
		{
			gfx->SetPSSamplers(*this, startSlot, NumView);
		}

		void SamplerState::HSSetThisSamplerState(uint32 startSlot, uint32 NumView, GfxCore * gfx)
		{
			gfx->SetHSSamplers(*this, startSlot, NumView);
		}

		void SamplerState::DSSetThisSamplerState(uint32 startSlot, uint32 NumView, GfxCore * gfx)
		{
			gfx->SetDSSamplers(*this, startSlot, NumView);
		}
	}
}