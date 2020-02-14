#pragma once

namespace rczEngine
{
	class RZ_EXP Component : public Object, public Serializable
	{
	public:
		virtual ~Component() {};

		virtual void Init() {};
		virtual void Update(float deltaTime) {};
		virtual void Render(Gfx::GfxCore* gfx, ResVault* res) {};

		ComponentId m_ID;

		virtual eComponentID GetComponentEnumType() { return (eComponentID)GetComponentType(); };
		virtual ComponentType GetComponentType() { return CMP_NONE; };
		virtual ComponentId GetComponentId() { return m_ID; };
		bool IsRendereable() const { return m_Renderable; };

		void SetOwner(WeakGameObjPtr owner) { m_Owner = owner; };
		WeakGameObjPtr GetOwner() { return m_Owner; };

#ifndef RZ_EDITOR
		virtual void RenderComponent()
		{
			ImGui::Separator();
			ImGui::Text("Component %d", (int)m_ID);
		}
#endif

	protected:
		bool m_Renderable = false;
		WeakGameObjPtr m_Owner;
	};
}