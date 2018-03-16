#include <RZDirectXPCH.h>
#include <RZUtilityPCH.h>

namespace rczEngine
{
	namespace Gfx
	{
		void InputLayout::Destroy()
		{
			if (m_InputLayout)
			{
				m_InputLayout->Release();
			}

			m_VectorElements.clear();
		}

		void InputLayout::AddElement(const char* SemanticName, uint32 SemanticIndex, eFORMAT Format, uint32 InputSlot, uint32 AlignedByteOffset, eINPUT_CLASSIFICATION InputSlotClass, uint32 InstanceDataStepRate)
		{
			InputLayoutElement Temp;
			Temp.SemanticName = SemanticName;
			Temp.SemanticIndex = SemanticIndex;
			Temp.Format = Format;
			Temp.InputSlot = InputSlot;
			Temp.AlignedByteOffset = AlignedByteOffset;
			Temp.InputSlotClass = InputSlotClass;
			Temp.InstanceDataStepRate = InstanceDataStepRate;

			m_VectorElements.push_back(Temp);
		}

		void InputLayout::ClearElementList()
		{
			m_VectorElements.clear();
		}

		bool InputLayout::CreateInputLayout(VertexShader & VShader, GfxCore* gfx)
		{
			return gfx->CreateInputLayout(*this, VShader);
		}

		uint32 InputLayout::GetElementSize()
		{
			return uint32(m_VectorElements.size());
		}

		void InputLayout::SetThisInputLayout(GfxCore* gfx)
		{
			gfx->SetInputLayout(*this);
		}
	}
};