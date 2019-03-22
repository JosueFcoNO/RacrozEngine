#include <RZRendererPCH.h>

namespace rczEngine
{
	void AtmosScatterPass::SetRenderingMode(RENDERING_MODE mode)
	{
		m_ActiveScene = SceneManager::Pointer()->GetActiveScene();

		//m_gfx->CompileAndCreateVertexShader(m_GroundFromSpaceVS, L"Shaders/ProcGen/Planet/GroundFromSpaceAtmos.hlsl");
		//m_gfx->CompileAndCreatePixelShader(m_GroundFromSpacePS, L"Shaders/ProcGen/Planet/GroundFromSpaceAtmos.hlsl");
		//m_GroundFromSpaceVS.ReflectLayout(0, m_gfx);

		m_gfx->CompileAndCreateVertexShader(m_SkyFromSpaceVS, L"Shaders/ProcGen/Planet/SkyFromSpaceAtmos.hlsl");
		m_gfx->CompileAndCreatePixelShader(m_SkyFromSpacePS, L"Shaders/ProcGen/Planet/SkyFromSpaceAtmos.hlsl");
		m_SkyFromSpaceVS.ReflectLayout(0, m_gfx);

		m_AtmosValues.CreateConstantBuffer(sizeof(AtmosData), Gfx::USAGE_DEFAULT, m_gfx);
		atmosData.InvWaveLength.Set(5.602, 9.478, 19.646);
		m_AtmosValues.UpdateConstantBuffer(&atmosData, m_gfx);
		
		Transparent.InitBlendState();
		Transparent.CreateBlendState(m_gfx);

		m_CullBack.Init(Gfx::FILL_SOLID, Gfx::CULL_FRONT);
		m_CullBack.CreateRasterizerState(m_gfx);
	}

	void AtmosScatterPass::PreRenderPass()
	{
		SetTexturesInPipeline();
		SetRasterizerState();

		UseDepth = true;

		///Set the rasterizer state.
		m_gfx->SetRSStateDefault();

		Transparent.SetThisBlendState(m_gfx);
	}

	void AtmosScatterPass::RenderPass()
	{
		if (Input::Pointer()->CheckKeyboardState(KEY_NUMPAD4))
		{
			m_GroundFromSpacePS.Destroy();
			m_gfx->CompileAndCreatePixelShader(m_GroundFromSpacePS, L"Shaders/ProcGen/Planet/GroundFromSpaceAtmos.hlsl");
		}
		
		if (Input::Pointer()->CheckKeyboardState(KEY_NUMPAD5))
		{
			m_GroundFromSpaceVS.Destroy();
			m_gfx->CompileAndCreateVertexShader(m_GroundFromSpaceVS, L"Shaders/ProcGen/Planet/GroundFromSpaceAtmos.hlsl");
		}
		//
		//if (Input::Pointer()->CheckKeyboardState(KEY_NUMPAD6))
		//{
		//	m_SkyFromSpacePS.Destroy();
		//	m_gfx->CompileAndCreatePixelShader(m_SkyFromSpacePS, L"Shaders/ProcGen/Planet/SkyFromSpaceAtmos.hlsl");
		//}
		//
		//if (Input::Pointer()->CheckKeyboardState(KEY_NUMPAD7))
		//{
		//	m_SkyFromSpaceVS.Destroy();
		//	m_gfx->CompileAndCreateVertexShader(m_SkyFromSpaceVS, L"Shaders/ProcGen/Planet/SkyFromSpaceAtmos.hlsl");
		//}
		//
		//
		auto space = SpaceManager::Pointer();
		
		if (space)
		{
			m_AtmosValues.SetBufferInVS(10, m_gfx);
			m_AtmosValues.SetBufferInPS(10, m_gfx);
			
			UseDepth = true;
			SetRenderTargetsInPipeline();
		
			//m_GroundFromSpaceVS.SetThisVertexShaderAndInputLayout(m_gfx);
			//m_GroundFromSpacePS.SetThisPixelShader(m_gfx);
			//
			//space->RenderAtmos(0);
		
			m_SkyFromSpaceVS.SetThisVertexShaderAndInputLayout(m_gfx);
			m_SkyFromSpacePS.SetThisPixelShader(m_gfx);
			m_CullBack.SetThisRasterizerState(m_gfx);
		
			space->RenderAtmos(1.1f);
		
			m_gfx->UnbindRenderTargets();
		
		}

	}

	void AtmosScatterPass::PostRenderPass()
	{
		for (int32 i = 0; i < 10; ++i)
			m_gfx->UnbindPSShaderResource(i);

			m_gfx->SetRSStateDefault();

			m_gfx->SetBlendStateDefault();
	}
};