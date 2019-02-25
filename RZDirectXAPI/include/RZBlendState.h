#pragma once
#include <RZDirectXPCH.h>

namespace rczEngine
{
	namespace Gfx
	{
		class RZ_EXP BlendState
		{
		public:
			BlendState() { m_BlendState = nullptr; };
			~BlendState() { if (m_BlendState) m_BlendState->Release(); };

			void InitBlendState(
				bool m_BlendEnable = true,
				BLEND_OP m_BlendOp = BLEND_OP::BLEND_OP_ADD,
				BLEND_OP m_BlendOpAlpha = BLEND_OP::BLEND_OP_ADD,
				BLEND m_DestBlend = BLEND::BLEND_INV_SRC_ALPHA,
				BLEND m_DestBlendAlpha = BLEND::BLEND_INV_SRC_ALPHA,
				BLEND m_SrcBlend = BLEND::BLEND_SRC_ALPHA,
				BLEND m_SrcBlendAlpha = BLEND::BLEND_INV_SRC_ALPHA);


			bool CreateBlendState(GfxCore* gfx);
			bool SetThisBlendState(GfxCore* gfx);

			ID3D11BlendState* m_BlendState;

			bool m_BlendEnable = true;
			BLEND_OP m_BlendOp = BLEND_OP::BLEND_OP_ADD;
			BLEND_OP m_BlendOpAlpha = BLEND_OP::BLEND_OP_ADD;
			BLEND m_DestBlend = BLEND::BLEND_INV_SRC_ALPHA;
			BLEND m_DestBlendAlpha = BLEND::BLEND_INV_SRC1_ALPHA;
			BLEND m_SrcBlend = BLEND::BLEND_INV_SRC_COLOR;
			BLEND m_SrcBlendAlpha = BLEND::BLEND_INV_SRC_ALPHA;

		};
	}
}