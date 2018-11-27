#pragma once

namespace rczEngine
{
	struct AtmosData
	{
		int nSamples = 3;
		float fSamples = 3.0f;

		float SkyScale = 5.0f;
		float OuterRadius = 205;

		Vector3 InvWaveLength;
		float OuterRadiusSquared = 42025.0;

		float InnerRadius = 200.0;
		float InnerRadiusSquared = 40000.0;
		float KrESun = .0375;
		float KmESun = .0225;

		float Kr4PI = .0314;
		float Km4PI = .0188;
		float Scale = .2;
		float ScaleOverScaleDepth = .8;

		float InvScaleDepth = 4;
		float ScaleDepth = .25;
		float G = -.95;
		float GSquared = .9025;
	};

	class RZ_EXP AtmosScatterPass : public Pass
	{
	public:
		///Inits the pass with a name and a rendering mode.
		virtual void SetRenderingMode(RENDERING_MODE mode);

		///Sets the buffers, shaders, textures and render targets before rendering geomtry.
		virtual void PreRenderPass();

		///Renders the geometry from the pass.
		virtual void RenderPass();

		///Unbinds the render targets.
		virtual void PostRenderPass();

	private:
		Gfx::VertexShader m_GroundFromSpaceVS;
		Gfx::PixelShader  m_GroundFromSpacePS;

		Gfx::VertexShader m_GroundFromAtmosphereVS;
		Gfx::PixelShader  m_GroundFromAtmospherePS;

		Gfx::VertexShader m_SkyFromSpaceVS;
		Gfx::PixelShader  m_SkyFromSpacePS;

		Gfx::VertexShader m_SkyFromAtmosphereVS;
		Gfx::PixelShader  m_SkyFromAtmospherePS;

		Gfx::ConstantBuffer m_AtmosValues;
		AtmosData data;

		StrPtr<Scene> m_ActiveScene;
		Gfx::BlendState Transparent;

		Gfx::RasterizerState m_CullBack;

	};

}