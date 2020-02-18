#pragma once

namespace rczEngine
{
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
		};

		void RenderWindow()
		{
		};

	public:
		RacrozRenderer() {};
		///Destructor, calls Destroy().
		~RacrozRenderer() noexcept { Destroy(); };

		///Inits the Renderer
		void InitRenderer();
		///Inits the variables, render targets and shaders for a specific rendering mode. Can be changed mid game.
		void CreatePipeline(const String& name, eRenderingPipelines renderingMode);
		
		///Renders the scene and Canvas
		void Render(const String & name, Scene * sceneGraph, ImGUIEditor * editor);
		
		WeakPtr<RenderPipeline> GetPipeline(const String & name);

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

		ResourceHandle CreateCubeMap(const String & name, Scene * sceneGraph, RenderPipeline * renderPipeline, int width, int height);

		///Draws the sceneGraph
		void PrepareRender(Scene * sceneGraph);

		UoMap<int, MMap<float, WeakGameObjPtr>> m_ObjectsToRender;

		void RenderObjs(bool forward, eComponentID componentID, eMaterialType matType, eShadingType shading, eBlendType blendType, bool Tesselated, bool TwoSided, bool blendedMaterial, bool wireframe);

		///Draws a screen aligned quad
		static void RenderScreenAlignedQuad();

		///Returns a created Render Target and its corresponding texture, you can specify number of mip maps, width and height and format.
		StrPtr<Gfx::RenderTarget> CreateRenderTargetAndTexture_XYScales(const String& name, StrPtr<Texture2D>& out_Texture, int mipMaps, float screenWidthScale = 1, float screenHeightScale = 1, Gfx::eFORMAT format = Gfx::eFORMAT::FORMAT_R8G8B8A8_UNORM);

		///Returns a created Render Target and its corresponding texture, you can specify number of mip maps, width and height and format.
		StrPtr<Gfx::RenderTarget> CreateRenderTargetAndTexture_WidthHeight(const String& name, StrPtr<Texture2D>& out_Texture, int mipMaps, int32 width, int32 height, Gfx::eFORMAT format = Gfx::eFORMAT::FORMAT_R8G8B8A8_UNORM);

		///Does a blur render pass, with the texture passes
		void DoBlurPass(StrPtr<Gfx::RenderTarget> outRenderTarget, StrPtr<Texture2D> inTexture, int width, int height);

		void StartPostProcessing();

		//StrPtr<Gfx::RenderTarget> GetRenderTarget(const String& name) { return m_RTs[name]; };

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

		Map<String, StrPtr<RenderPipeline>> m_Pipelines;

	private:
		///Private un-implemented Copy Constructor to prevent copying by accident.
		RacrozRenderer(const RacrozRenderer&);
		///Private un-implemented Copy Operator.
		RacrozRenderer& operator= (const RacrozRenderer& other) { return RacrozRenderer(); };

		///The SkyBox currently set.
		StrPtr<SkyBox> m_ActiveSkyBox;

		Gfx::VertexShader m_ScreenQuadVS;

		void InitRasterizers();
		void InitSamplerStates();

		GaussPass m_BlurPass;

		///Screen Width
		int m_Width;
		///Screen Height
		int m_Height;

		///Pointer to the gfx gfx instance
		Gfx::GfxCore* m_gfx = NULL;

		///Pointer to the resource manager instance
		ResVault* m_res = NULL;

		///A screen quad used for deferred rendering and post-processing.
		ScreenQuad m_ScreenQuad;
	};
}