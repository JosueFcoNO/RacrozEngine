#include <RZEnginePCH.h>

namespace rczEngine
{
	LightManager*& LightManager::_Instance()
	{
		static LightManager* instance = nullptr;
		return instance;
	}

	void LightManager::Start()
	{
		(_Instance()) = new LightManager;
	}

	LightManager* LightManager::Pointer()
	{
		return _Instance();
	}

	void LightManager::ShutDown()
	{
		delete _Instance();
	}

	void LightManager::InitLightManager()
	{
		m_gfx = Gfx::GfxCore::Pointer();

		m_LightBuffer.CreateConstantBuffer(sizeof(LightBufferStruct), Gfx::USAGE_DEFAULT, m_gfx);
		m_LightBuffer.UpdateConstantBuffer(&m_LightStruct, m_gfx);

		m_LightStruct.m_LightNumber[0] = 0;

		m_SingleLightBuffer.CreateConstantBuffer(sizeof(Light), Gfx::USAGE_DEFAULT, m_gfx);
	}

	Light * LightManager::AddLight()
	{
		Light* ptr = &m_LightStruct.m_Lights[m_LightStruct.m_LightNumber[0]++];
		return ptr;
	}

	void LightManager::SetLightsBuffers()
	{
		m_LightBuffer.UpdateConstantBuffer(&m_LightStruct, m_gfx);
		m_LightBuffer.SetBufferInPS(3, m_gfx);
		m_LightBuffer.SetBufferInVS(3, m_gfx);

	}

	void LightManager::SetSingleLightBuffer(uint32 lightSlot)
	{
		m_SingleLightBuffer.UpdateConstantBuffer(&m_LightStruct.m_Lights[lightSlot], m_gfx);
		m_SingleLightBuffer.SetBufferInVS(3, m_gfx);
		m_SingleLightBuffer.SetBufferInPS(3, m_gfx);
	}
};