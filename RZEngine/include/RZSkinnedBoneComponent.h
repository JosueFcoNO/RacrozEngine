#pragma once

namespace rczEngine
{
	class RZ_EXP BoneComponent : public Component
	{
	public:
		~BoneComponent() { Destroy(); };

		void Init() { };
		void Update(float deltaTime);
		void Render(Gfx::GfxCore* gfx, ResVault* res, Scene* scene, MATERIAL_TYPE matType = MAT_ANY);
		void Destroy() {};

		virtual void Serialize()
		{
			Serializer::Pointer()->SetNextObjectSerial(s_ComponentType + SERIAL_COMPONENT_OFFSET);
		};
		virtual void DeSerialize() {};

		void SetBone(Gfx::MeshAPI* mesh)
		{
			m_MeshToDraw = mesh;
		}

#ifndef RZ_EDITOR
		virtual void RenderComponent()
		{
			ImGui::Separator();
			ImGui::Text("Bone");
		}
#endif

		static const ComponentType s_ComponentType = CMP_BONE;
		virtual ComponentType GetComponentType() { return BoneComponent::s_ComponentType; };
		virtual ComponentId GetComponentID() { return m_ID; };

	private:
		Gfx::MeshAPI* m_MeshToDraw = nullptr;
	};
}