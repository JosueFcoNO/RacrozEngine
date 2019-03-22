#pragma once

namespace rczEngine
{
	struct AtmosData
	{
		int nSamples = 3;
		float fSamples = 3.0f;

		float SkyScale = 1;
		float OuterRadius = 3;

		Vector3 InvWaveLength;
		float OuterRadiusSquared = OuterRadius * OuterRadius;

		float InnerRadius = 2;
		float InnerRadiusSquared = InnerRadius*InnerRadius;
		float KrESun = .0375;
		float KmESun = .0225;

		float Kr4PI = .0314;
		float Km4PI = .0188;
		float Scale = 1.0f / (OuterRadius - InnerRadius);
		float ScaleDepth = .25;

		float InvScaleDepth = 1.0f / ScaleDepth;
		float ScaleOverScaleDepth = Scale / ScaleDepth;
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
		AtmosData atmosData;

		StrPtr<Scene> m_ActiveScene;
		Gfx::BlendState Transparent;

		Gfx::RasterizerState m_CullBack;

	};

}