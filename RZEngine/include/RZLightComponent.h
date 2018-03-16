#pragma once


namespace rczEngine
{
	class RZ_UTILITY_EXPORT LightComponent : public Component
	{
	public:
		LightComponent() {};

		virtual void Init() {};
		virtual void Update(float deltaTime) { m_Light->m_LightPosition = m_Owner.lock()->GetAccumulatedPosition(); };

		void InitLightInManager(LightManager* lManager, Gfx::GfxCore* gfx, eLIGHT_TYPE lightType, Vector3 pos, Vector3 dir, Vector4 color, bool castsShadows = false, float pointRadius = 1.0f, float spotFactor = .25f);
		void UpdateLight(Vector3 pos, Vector3 dir);
		void SetLightViewMatrix(Gfx::GfxCore* gfx);

		static const ComponentType s_ComponentType = CMP_LIGHT;
		virtual ComponentType GetComponentType() { return LightComponent::s_ComponentType; };
		virtual ComponentId GetComponentID() { return m_ID; };

		Light* m_Light;

	private:
		Gfx::ConstantBuffer m_LightViewBuffer;
	};
};