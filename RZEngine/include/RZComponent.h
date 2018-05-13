#pragma once

namespace rczEngine
{
	typedef uint16 ComponentId;

	class RZ_UTILITY_EXPORT Component : public Object, public Serializable
	{
	public:
		virtual ~Component() {};

		virtual void Init() {};
		virtual void Update(float deltaTime) {};
		virtual void Render(Gfx::GfxCore* gfx, ResVault* res, Scene* scene) {};

		ComponentId m_ID;

		virtual ComponentType GetComponentType() { return CMP_NONE; };
		virtual ComponentId GetComponentId() { return m_ID; };

		void SetOwner(WeakGameObjectPtr owner) { m_Owner = owner; };
		WeakGameObjectPtr GetOwner() { return m_Owner; };

#ifndef RZ_EDITOR
		virtual void RenderComponent()
		{
			ImGui::Separator();
			ImGui::Text("Component %d", (int)m_ID);
		}
#endif

	protected:
		WeakGameObjectPtr m_Owner;
	};
}