#pragma once

namespace rczEngine
{
	namespace Gfx
	{
		class RZ_EXP SamplerState
		{
		public:
			SamplerState() : m_BorderColor(INIT_ZERO)
			{
				m_SamplerState = NULL;
			};

			~SamplerState()
			{
				if (m_SamplerState)
				{
					if (m_SamplerState->Release())
					{
						Logger::Pointer()->LogMessageToFileLog("Gfx","Sampler State no se libero bien");
					};
				}
			};

			void Init(
				eTEXTURE_ADDRESS u = TEXTURE_ADDRESS_CLAMP,
				eTEXTURE_ADDRESS v = TEXTURE_ADDRESS_CLAMP,
				eTEXTURE_ADDRESS w = TEXTURE_ADDRESS_CLAMP,
				eCOMPARISON_FUNC cFunc = COMPARISON_NEVER,
				eFILTER filter = FILTER_MIN_MAG_MIP_LINEAR,
				Vector4 borderColor = { 1,1,1,1 },
				uint32 maxAniso = 1,
				float maxLOD = FLT_MAX,
				float minLOD = -FLT_MAX,
				float mipLODBias = 0);

			bool CreateSamplerState(GfxCore* gfx = NULL);
			void PSSetThisSamplerState(uint32 startSlot, uint32 NumView, GfxCore* gfx = NULL);
			void HSSetThisSamplerState(uint32 startSlot, uint32 NumView, GfxCore* gfx = NULL);
			void DSSetThisSamplerState(uint32 startSlot, uint32 NumView, GfxCore* gfx = NULL);

			ID3D11SamplerState* m_SamplerState;

			eTEXTURE_ADDRESS m_TexAddressU;
			eTEXTURE_ADDRESS m_TexAddressV;
			eTEXTURE_ADDRESS m_TexAddressW;
			eCOMPARISON_FUNC m_ComparisonFunc;
			eFILTER			 m_Filter;
			Vector4			 m_BorderColor;
			uint32			 m_MaxAnisotropy;
			float			 m_MaxLOD;
			float			 m_MinLOD;
			float			 m_MipLODBias;

		};
	}
}