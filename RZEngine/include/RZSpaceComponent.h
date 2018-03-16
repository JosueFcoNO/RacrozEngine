#pragma once

namespace rczEngine
{
	class RZ_UTILITY_EXPORT SpaceComponent : public Component
	{
	public:
		SpaceComponent() {};

		virtual void Init() { };
		virtual void Update(float deltaTime) { m_Space.Update(deltaTime); };
		virtual void Render(Gfx::GfxCore* gfx, ResVault* res, Scene* scene)
		{
			m_Space.Render();
		};

		static const ComponentType s_ComponentType = CMP_SPACE;
		virtual ComponentType GetComponentType() { return SpaceComponent::s_ComponentType; };
		virtual ComponentId GetComponentID() { return m_ID; };

		SpaceManager m_Space;
	};
}
