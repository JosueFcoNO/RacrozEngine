#include <RZVulkanPCH.h>
#include "RZSamplerState.h"

namespace rczEngine
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

	bool SamplerState::PSSetThisSamplerState(uint32 startSlot, uint32 NumView, GfxCore* gfx)
	{
		return gfx->SetPSSamplers(*this, startSlot, NumView);
	}
}