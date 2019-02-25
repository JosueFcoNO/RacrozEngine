#pragma once

namespace rczEngine
{
	enum PASSES
	{
		SKYBOX,
		GEOMETRY_PASS,
		TERRAIN_GEOMETRY_PASS,
		PBR,
		PBR_FORWARD,
		PBR_TRANSPARENT,
		COLOR_CORRECTION,
		LUMINANCE,
		BRIGHT,
		BLOOM,
		AVG_LUMINANCE,
		HDR_BLOOM,
		MOTION_BLUR,
		PLANET_PASS,
		ATMOS_SCATTER_PASS,
		PERLIN3D,
		SSAO
	};

	enum TEXTURES_PASS
	{
		TP_POSITION_DEPTH,
		TP_NORMALS_MET,
		TP_COLOR_AO,
		TP_EMMISIVE,
		//TP_SSAO,
		TP_BLURH,
		TP_BLURV,
		//TP_SHADOWMAP1,
		//TP_SHADOW_FINAL,
		TP_PBR,
		TP_LUMINANCE,
		TP_BRIGHT,
		TP_BRIGHTADD,
		TP_AVGLUM,
		TP_PBRBLOOM,
		TP_FXAA,
		TP_FOG,
		TP_DEPTH_OF_FIELD,
		TP_COLOR_CORRECTION,
		TP_PERLIN
	};

	///The Renderer class, handles the render passes.
	class RZ_EXP RacrozRenderer : GUIBaseWindow
	{
	private:
		static RacrozRenderer*& _Instance();

	public:
		static void Start();
		static RacrozRenderer* Pointer();
		static void ShutDown();

		int CurrentPass = 0;

		void InitWindow()
		{
			GUIEditor::Pointer()->AddWindow("Renderer", this);
		};

		void RenderWindow()
		{
			ImGui::Begin("Renderer");
			{

				if (ImGui::Button("Previous Pass"))
				{
					CurrentPass = Math::Max(CurrentPass - 1, 0);
				}
				ImGui::SameLine();
				if (ImGui::Button("Next Pass"))
				{
					CurrentPass = Math::Min(CurrentPass + 1, (int)m_PassesOrder.size() - 1);
				}

				if (m_Passes[m_PassesOrder[CurrentPass]]) m_Passes[m_PassesOrder[CurrentPass]]->RenderPassGUI();
			}
		};

	public:
		RacrozRenderer() {};
		///Destructor, calls Destroy().
		~RacrozRenderer() noexcept { Destroy(); };

		///Inits the Renderer
		void InitRenderer();
		///Inits the variables, render targets and shaders for a specific rendering mode. Can be changed mid game.
		void SetRenderingMode(RENDERING_MODE renderingMode);
		///Renders the scene and Canvas
		void Render(Scene* sceneGraph, GUIEditor * editor);
		///Destroys the renderer
		void Destroy();

		///Changes the ambient cube map
		void ChangeSkyBox(StrPtr<SkyBox> cube)
		{
			m_ActiveSkyBox = cube;
		}

		WeakPtr<CubeMap> GetSkyBox()
		{
			return m_ActiveSkyBox->GetCubeMap();
		}

		ResourceHandle CreateCubeMap(const String& name, Scene* sceneGraph, Vector<String>& RenderPasses, int width, int height);

		///Draws the sceneGraph
		static void RenderScene(Scene * sceneGraph, eCOMPONENT_ID componentID, MATERIAL_TYPE matType, bool Forward);
		///Draws a screen aligned quad
		static void RenderScreenAlignedQuad();

		///Returns a created Render Target and its corresponding texture, you can specify number of mip maps, width and height and format.
		StrPtr<Gfx::RenderTarget> CreateRenderTargetAndTexture_XYScales(const String& name, StrPtr<Texture2D>& out_Texture, int mipMaps, float screenWidthScale = 1, float screenHeightScale = 1, Gfx::eFORMAT format = Gfx::eFORMAT::FORMAT_R8G8B8A8_UNORM);

		///Returns a created Render Target and its corresponding texture, you can specify number of mip maps, width and height and format.
		StrPtr<Gfx::RenderTarget> CreateRenderTargetAndTexture_WidthHeight(const String& name, StrPtr<Texture2D>& out_Texture, int mipMaps, int32 width, int32 height, Gfx::eFORMAT format = Gfx::eFORMAT::FORMAT_R8G8B8A8_UNORM);

		///Does a blur render pass, with the texture passes
		void DoBlurPass(StrPtr<Gfx::RenderTarget> outRenderTarget, StrPtr<Texture2D> inTexture, int width, int height);

	private:
		///Private un-implemented Copy Constructor to prevent copying by accident.
		RacrozRenderer(const RacrozRenderer&);
		///Private un-implemented Copy Operator.
		RacrozRenderer& operator= (const RacrozRenderer& other) { return RacrozRenderer(); };

		RENDERING_MODE m_CurrentRenderingMode;

		///The SkyBox currently set.
		StrPtr<SkyBox> m_ActiveSkyBox;

		void SetDeferred();
		void SetForward();
		void SetForwardPlus();

		void StartPostProcessing();
		Gfx::VertexShader m_ScreenQuadVS;

		StrPtr<Pass> CreatePass(const String& name, PASSES pass, RENDERING_MODE renderMode = DEFERRED);

		void InitRasterizers();
		void InitSamplerStates();
		
		Map<String, StrPtr<Pass>> m_Passes;

		GaussPass m_BlurPass;

		///Screen Width
		int m_Width;
		///Screen Height
		int m_Height;

		///Pointer to the gfx gfx instance
		Gfx::GfxCore* m_gfx = NULL;

		///Pointer to the resource manager instance
		ResVault* m_res = NULL;

		Vector<String> m_PassesOrder;

		///A map of textures from the Render passes
		Map<String, StrPtr<Texture2D>> m_Textures;

		///A Map from the render targets.
		Map<String, StrPtr<Gfx::RenderTarget>> m_RTs;

		///A screen quad used for deferred rendering and post-processing.
		ScreenQuad m_ScreenQuad;

		///Samplers
		Gfx::SamplerState m_LinealWrapSampler;
		Gfx::SamplerState m_AnisotropicWrapSampler;
		Gfx::SamplerState m_LinealClampSampler;
		Gfx::SamplerState m_AnisotropicClampSampler;
		Gfx::SamplerState m_PointWrapSampler;

		///Rasterizers
		Gfx::RasterizerState m_RSWireframe;
		Gfx::RasterizerState m_RSSolid;
		Gfx::RasterizerState m_RSSolidCullNone;
	};
}