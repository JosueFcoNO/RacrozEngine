#include <RZEnginePCH.h>

namespace rczEngine
{
	void ModelRenderer::Serialize()
	{
		auto ser = Serializer::Pointer();
		ser->SetNextObjectSerial(s_ComponentType + SERIAL_COMPONENT_OFFSET);

		ser->WriteData(&m_ID, sizeof(m_ID));

		//ModelHandle 
		ser->WriteData(&m_Model, sizeof(size_t));

		//TODO: Material Override

	}

	void ModelRenderer::DeSerialize()
	{
		auto ser = Serializer::Pointer();
		ser->ReadData(&m_ID, sizeof(m_ID));

		//ModelHandle 
		ResourceHandle model;
		ser->ReadData(&model, sizeof(size_t));

		SetModel(model, ResVault::Pointer());
	}
}