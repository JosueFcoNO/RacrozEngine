#pragma once

namespace rczEngine
{
	class RZ_UTILITY_EXPORT ModelRenderer : public Component 
	{
	public:
		ModelRenderer() 
		{
		};

		virtual void Init() 
		{
			m_Model = INVALID_RESOURCE;
			m_Materials.clear();

			auto resVault = ResVault::Pointer();

			SetModel(resVault->m_ModelCube, resVault);
		};

		void Update(float deltaTime) {};

		void Render(Gfx::GfxCore* gfx, ResVault* res, Scene* scene, MATERIAL_TYPE matType = MAT_ANY) 
		{ 
			if (m_Materials.size())
			{
				res->GetResource<Model>(m_Model).lock()->DrawModel(gfx, res, &m_Materials, matType);
			}
		};

		void SetModel(ResourceHandle model, ResVault* resManager)
		{
			m_Model = model;

			auto ModelObj = resManager->GetResource<Model>(model).lock();
			
			for (auto it = ModelObj->m_MaterialMap.begin(); it != ModelObj->m_MaterialMap.end(); ++it)
			{
				m_Materials[it->first] = it->second;
			}
		}

		void OverrideMaterial(ResourceHandle material, char* materialName)
		{
			m_Materials[materialName] = material;
		}

		virtual void Serialize();
		virtual void DeSerialize();

#ifndef RZ_EDITOR
		virtual void RenderComponent()
		{
			ImGui::Separator();
			ImGui::Text("Model Renderer");

			auto ModelObj = ResVault::Pointer()->GetResource<Model>(m_Model).lock();
			ImGui::Text("Current Model: %s", ModelObj->GetName());

			if (ImGui::Button("Change Model"))
			{
				SetModel(LoadFile("New Model", "Modelo", ResVault::Pointer()), ResVault::Pointer());
			}
		}
#endif

		static const ComponentType s_ComponentType = CMP_MODEL_RENDERER;
		virtual ComponentType GetComponentType() { return ModelRenderer::s_ComponentType; };
		virtual ComponentId GetComponentID() { return m_ID; };

		ResourceHandle m_Model = NULL;

		Map<String, ResourceHandle> m_Materials;
	};
}