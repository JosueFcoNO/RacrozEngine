#pragma once

namespace rczEngine
{
	class RZ_EXP ModelRenderer : public Component 
	{
	public:
		ModelRenderer() {};

		virtual void Init();

		virtual void Update(float deltaTime) {};

		virtual void Render(Gfx::GfxCore* gfx, ResVault* res, Scene* scene, MATERIAL_TYPE matType = MAT_ANY);

		void SetModel(ResourceHandle model, ResVault* resManager);

		void OverrideMaterial(ResourceHandle material, char* materialName);

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
				ResVault::Pointer()->LoadModel(m_Owner.lock(), GetFilePath("Choose Model").c_str(), false);
			}

			ImGui::SameLine();

			if (ImGui::Button("Change Model To Hierarchy"))
			{
				ResVault::Pointer()->LoadModel(m_Owner.lock(), GetFilePath("Choose Model").c_str(), true);
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