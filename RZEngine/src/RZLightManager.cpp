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

		m_SingleLightBuffer.CreateConstantBuffer(sizeof(LightCore), Gfx::USAGE_DEFAULT, m_gfx);
	}

	Light * LightManager::AddLight()
	{
		m_Lights.push_back(Light());

		m_LightStruct.m_LightNumber[0]++;

		return &m_Lights[m_Lights.size()-1];
	}

	void LightManager::SetLightsBuffers()
	{
		auto i = 0;
		for (auto i= 0; i < m_LightStruct.m_LightNumber[0]; ++i)
		{
			m_LightStruct.m_Lights[i] = m_Lights[i].m_Core;
		}

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