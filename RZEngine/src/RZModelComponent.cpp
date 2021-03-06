#include <RZEnginePCH.h>

namespace rczEngine
{
	void ModelRenderer::Init()
	{
		m_Model = INVALID_RESOURCE;
		m_Materials.clear();

		auto resVault = ResVault::Pointer();

		m_Renderable = true;

		//SetModel(resVault->m_ModelCube, resVault);
	}

	void ModelRenderer::Render(Gfx::GfxCore * gfx, ResVault * res, int renderHash)
	{
		if (m_Materials.size())
		{
			res->GetResource<Model>(m_Model).lock()->DrawModel(&m_Materials, renderHash);
		}
		else
		{
			res->GetResource<Model>(m_Model).lock()->DrawModel(nullptr, renderHash);
		}
	}

	void ModelRenderer::SetModel(ResourceHandle model, ResVault * resManager)
	{
		m_Model = model;

		auto ModelObj = resManager->GetResource<Model>(model).lock();

		for (auto it = ModelObj->m_MaterialMap.begin(); it != ModelObj->m_MaterialMap.end(); ++it)
		{
			m_Materials[it->first] = it->second;
		}
	}

	void ModelRenderer::OverrideMaterial(ResourceHandle material, char * materialName)
	{
		m_Materials[materialName] = material;
	}

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