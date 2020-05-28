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
		TP_SHADOW_FINAL,
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
	class RZ_EXP RacrozRenderer
	{
	public:

		///Inits the Renderer
		void InitRenderer();

		///Renders the scene and Canvas
		void Render(const String & name, StrPtr<Scene> sceneGraph);

		///Destroys the renderer
		void Destroy();

		////////////////////////////
		/// Pipeline
		////////////////////////////

		///Inits the variables, render targets and shaders for a specific rendering mode. Can be changed mid game.
		void CreatePipeline(const String& name, eRenderingPipelines renderingMode);

		///Returns a pipeline by ID.
		WeakPtr<RenderPipeline> GetPipeline(const String & name);

		Map<String, StrPtr<RenderPipeline>> m_Pipelines;

		////////////////////////////
		/// Cubemap 
		////////////////////////////

		///Changes the ambient cube map
		void ChangeSkyBox(StrPtr<SkyBox> cube)
		{
			m_ActiveSkyBox = cube;
		}

		WeakPtr<CubeMap> GetSkyBox()
		{
			return m_ActiveSkyBox->GetCubeMap();
		}

		///The SkyBox currently set.
		StrPtr<SkyBox> m_ActiveSkyBox;

		ResourceHandle CreateCubeMap(const String & name, Scene * sceneGraph, RenderPipeline * renderPipeline, int width, int height);

		////////////////////////////
		/// Object Drawing
		////////////////////////////

		///Draws the sceneGraph
		void PrepareDrawableObjects(StrPtr<Scene> sceneGraph);

		///Renders the objects of the active scene by properties.
		void DrawObjectsByProperties(bool forward, eComponentID componentID, eMaterialType matType, eShadingType shading, eBlendType blendType, bool Tesselated, bool TwoSided, bool blendedMaterial, bool wireframe);

		///Draws a screen aligned quad
		void DrawScreenQuad();

		///The objects to draw mapped by property and depth.
		UoMap<int, MMap<float, WeakGameObjPtr>> m_ObjectsToDraw;

		///A screen quad used for deferred rendering and post-processing.
		ScreenQuad m_ScreenQuad;

		////////////////////////////
		/// Render Textures
		////////////////////////////

		///Returns a created Render Target and its corresponding texture, you can specify number of mip maps, width and height and format.
		StrPtr<Gfx::RenderTarget> CreateRenderTexture(const String& name, StrPtr<Texture2D>& out_Texture, int mipMaps, int32 width, int32 height, Gfx::eFORMAT format = Gfx::eFORMAT::FORMAT_R8G8B8A8_UNORM);

		///Does a blur render pass, with the texture passes
		void DoBlurPass(StrPtr<Gfx::RenderTarget> outRenderTarget, StrPtr<Texture2D> inTexture, int width, int height);

		////////////////////////////
		/// Voxel Global Illumination
		////////////////////////////

		///Inits the GI.
		void PrepareGI(bool prepareScene = false);

		///The implementation of VXGI
		VXGIImplement m_VXGI;

		///If it's ready to be used.
		bool m_GIPrepared = false;

		///Normals and albedo copies to feed into VXGI
		StrPtr<Texture2D> m_Normals;
		StrPtr<Texture2D> m_Albedo;

		////////////////////////////
		/// Default Pipeline Resources
		////////////////////////////

		///Inits all default rasterizers
		void InitRasterizers();

		///Inits all default sampler states.
		void InitSamplerStates();

		///Set sampler states
		void SetSamplerStates();

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

		GaussPass m_BlurPass;

		////////////////////////////
		/// Camera | Viewport
		////////////////////////////
	public:
		///Reset viewport.
		void ResetViewport();

		///Sets the current viewport.
		void SetViewport(int width, int height);

		///Updates and sets the camera buffer with the data of the current active camera.
		void UpdateCameraBuffersActiveCamera();

		///Updates and sets the camera buffers from the data of an existing camera.
		void UpdateCameraBuffers(const CameraData& cameraData);

		///Updates and sets a perspective camera buffers from the parameters passed. Used for rendering without a logical camera.
		void UpdateCameraBuffersPerspective(const Vector3& position, const Vector3& target, float fov = 70.0f, float nearPlane = 0.1f, float farPlane = 1000.0f);
		
		///Updates and sets an orthographic camera buffers from the parameters passed. Used for rendering without a logical camera.
		void UpdateCameraBuffersOrtographic(const Vector3& position, const Vector3& target, float width = 1024, float height = 1024, float nearPlane = 0.1f, float farPlane = 1000.0f);

		///Updates and sets the camera buffers from view and projection matrices passed.
		void UpdateCameraBuffersMatrices(const Matrix4& viewMatrix, const Matrix4& projMatrix);
		
		///Set the camera buffers in the pipeline
		void SetCameraBuffersInPipeline(int pixelShaderSlot = 3, int vertexShaderSlot = 3);

	private:
		///A constant Buffer to hold Camera Data
		Gfx::ConstantBuffer m_CameraBuffer;

		///Temporal CameraData struct;
		CameraData m_TempCameraData;

		/////////////////////////////////////
		///// Geometry Shaders
		/////////////////////////////////////

	private:
		///Inits all geometry shaders for the models. These get auto bound on RenderObjects.
		void InitGeometryShaders();

	public:
		///Passthrough shader for screen quad.
		Gfx::VertexShader m_ScreenQuadVS;

		///The default WVP output shader.
		Gfx::VertexShader m_DefaultVertexShader;

		///The default WVP output shader.
		Gfx::VertexShader m_SimpleTransformVertexShader;

		///The skinned vertex shader for animated models.
		Gfx::VertexShader m_SkinnedVertexShader;

		///The geometry shaders for tesselated models.
		Gfx::HullShader m_TessHullShader;
		Gfx::DomainShader m_TessDomainShader;
		Gfx::VertexShader m_TessVertexShader;

		////////////////////////////
		/// Variables
		////////////////////////////
	private:
		///Screen Width
		int m_Width;
		///Screen Height
		int m_Height;


		LightDepthPass lightDepth;

		///Pointer to the gfx gfx instance
		Gfx::GfxCore* m_gfx = NULL;

		///Pointer to the resource manager instance
		ResVault* m_res = NULL;

		////////////////////////////
		/// Boilerplate
		////////////////////////////

	private:
		static RacrozRenderer*& _Instance();

	public:
		static void Start();
		static RacrozRenderer* Pointer();
		static void ShutDown();

	private:
		///Private un-implemented Copy Constructor to prevent copying by accident.
		RacrozRenderer(const RacrozRenderer&);
		///Private un-implemented Copy Operator.
		RacrozRenderer& operator= (const RacrozRenderer& other) { return RacrozRenderer(); };
		RacrozRenderer() {};
		///Destructor, calls Destroy().
		~RacrozRenderer() noexcept { Destroy(); };

	};
}