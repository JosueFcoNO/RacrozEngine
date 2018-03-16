#include <RZVulkanPCH.h>
#include <RZUtilityPCH.h>

namespace rczEngine
{
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

	bool InputLayout::SetThisInputLayout(GfxCore* gfx)
	{
		return gfx->SetInputLayout(*this);
	}
};