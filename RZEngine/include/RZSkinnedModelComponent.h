#pragma once

namespace rczEngine
{
	class RZ_UTILITY_EXPORT SkinnedModelRenderer : public Component
	{
	public:
		SkinnedModelRenderer() {};

		void Init() {};
		void Update(float deltaTime);
		void Render(Gfx::GfxCore* gfx, ResVault* res, Scene* scene, MATERIAL_TYPE matType = MAT_ANY) {};

		void RenderSkinned(Gfx::GfxCore* gfx, ResVault* res, Scene* scene, MATERIAL_TYPE matType = MAT_ANY)
		{
			res->GetResource<SkinnedModel>(m_Model).lock()->DrawModel(gfx, res, &m_Materials, matType);
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

		static const ComponentType s_ComponentType = CMP_SKINNED_MODEL_RENDERER;
		virtual ComponentType GetComponentType() { return SkinnedModelRenderer::s_ComponentType; };
		virtual ComponentId GetComponentID() { return m_ID; };

		ResourceHandle m_Model = -1;

		Map<String, ResourceHandle> m_Materials;

	private:
		float m_CurrentTime = 0.0f;
	};
}