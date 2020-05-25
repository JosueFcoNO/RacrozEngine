#pragma once

namespace rczEngine
{
	struct LightBufferStruct
	{
		int m_LightNumber[4];
		LightCore m_Lights[8];
	};

	class RZ_EXP LightManager
	{
	private:
		static LightManager*& _Instance();

	public:
		static void Start();
		static LightManager* Pointer();
		static void ShutDown();

	public:
		void InitLightManager();

		Light* AddLight();
		void SetLightsBuffers();
		void SetSingleLightBuffer(uint32 lightSlot);
		Light* GetLight(uint32 slot) { return &m_Lights[slot]; };
		uint32 GetLightNumber() { return m_LightStruct.m_LightNumber[0]; };

	private:
		LightBufferStruct m_LightStruct;

		Vector<Light> m_Lights;

		Gfx::ConstantBuffer m_LightBuffer;
		Gfx::ConstantBuffer m_SingleLightBuffer;

		Gfx::GfxCore* m_gfx;
	};
};
