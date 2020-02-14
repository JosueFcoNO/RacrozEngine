#pragma once

namespace rczEngine
{
	class RZ_EXP SkinnedModelRenderer : public Component
	{
	public:
		~SkinnedModelRenderer() { Destroy(); };

		virtual void Init() { m_Model = INVALID_RESOURCE; m_Renderable = true;};
		virtual void Update(float deltaTime);
		virtual void Render(Gfx::GfxCore* gfx, ResVault* res, Scene* scene, eMaterialType matType = eMaterialType::AnyMaterial) {};
		void Destroy() {};

		virtual void Serialize()
		{
			Serializer::Pointer()->SetNextObjectSerial(s_ComponentType + SERIAL_COMPONENT_OFFSET);
		};
		virtual void DeSerialize() {};

		void RenderSkinned(Gfx::GfxCore* gfx, ResVault* res, eMaterialType matType = eMaterialType::AnyMaterial)
		{
			if (m_Model != INVALID_RESOURCE)
			{
				auto Skinned = res->GetResource<SkinnedModel>(m_Model).lock();
				Skinned->DrawModel(gfx, res, &m_Materials, matType);
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

#ifndef RZ_EDITOR
		virtual void RenderComponent()
		{
			ImGui::Separator();
			ImGui::Text("Skinned Model Renderer");

			if (m_Model != -1)
			{
				auto ModelObj = ResVault::Pointer()->GetResource<Model>(m_Model).lock();
				ImGui::Text("Current Model: %s", ModelObj->GetName());
			}

			if (ImGui::Button("Change Model"))
			{
				ResVault::Pointer()->LoadModel(m_Owner.lock(), GetFilePath("Choose Model").c_str(), true);
			}
		}
#endif

		static const ComponentType s_ComponentType = CMP_SKINNED_MODEL_RENDERER;
		virtual ComponentType GetComponentType() { return SkinnedModelRenderer::s_ComponentType; };
		virtual ComponentId GetComponentID() { return m_ID; };

		ResourceHandle m_Model = 0;

		Map<String, ResourceHandle> m_Materials;

	private:
		float m_CurrentTime = 0.0f;
	};
}