#pragma once
#include <GFSDK_NVRHI_D3D11.h>
#include <nvapi.h>
#include <GFSDK_VXGI.h>
#include <GFSDK_NVRHI.h>

#define API_STRING "D3D11"
	
#define FLUSH_COMMAND_LIST 

namespace rczEngine
{
	enum class RenderingMode
	{
		NORMAL,
		DIFFUSE_CHANNEL,
		SPECULAR_CHANNEL,
		OPACITY_VOXELS,
		EMITTANCE_VOXELS,
		IRRADIANCE_VOXELS
	};

	class RendererErrorCallback : public NVRHI::IErrorCallback
	{
		void signalError(const char* file, int line, const char* errorDesc)
		{
			char buffer[4096];
			int length = (int)strlen(errorDesc);
			length = std::min(length, 4000); // avoid a "buffer too small" exception for really long error messages
			sprintf_s(buffer, "%s:%i\n%.*s", file, line, length, errorDesc);

			OutputDebugStringA(buffer);
			OutputDebugStringA("\n");
			MessageBoxA(NULL, buffer, "ERROR", MB_ICONERROR | MB_OK);
		}
	};

	struct VXGIMaterialAdapter : public VXGI::MaterialInfo
	{
		MaterialCore m_core;

		ResourceHandle m_TextureAlbedo = INVALID_RESOURCE;
		ResourceHandle m_TextureNormal = INVALID_RESOURCE;
		ResourceHandle m_TextureMetSpec = INVALID_RESOURCE;
		ResourceHandle m_TextureRoughSmooth = INVALID_RESOURCE;
		ResourceHandle m_TextureAO = INVALID_RESOURCE;
		ResourceHandle m_TextureEm = INVALID_RESOURCE;
		ResourceHandle m_TextureH = INVALID_RESOURCE;
		ResourceHandle m_TextureO = INVALID_RESOURCE;

		VXGIMaterialAdapter()
		{}
	};

	typedef std::function<void(const VXGIMaterialAdapter&)> MaterialCallback;

	class RZ_EXP VXGIImplement
	{
	public:
		NVRHI::RendererInterfaceD3D11* g_pRendererInterface = NULL;

		RendererErrorCallback g_ErrorCallback;

		virtual HRESULT InitVXGI();

		virtual void DestroyVXGI();

		VoxelizePass pass;

		void VoxelizeScene();

		virtual void RenderChannels();
		RenderingMode g_RenderingMode = RenderingMode::NORMAL;

		StrPtr<Texture2D> SpecularResultTex;
		StrPtr<Texture2D> DiffuseResultTex;
		StrPtr<Texture2D> SSAOResultTex;

		StrPtr<Gfx::RenderTarget> DiffuseResult;
		StrPtr<Gfx::RenderTarget> SpecularResult;
		StrPtr<Gfx::RenderTarget> SSAOResult;

		StrPtr<Gfx::RenderTarget> OldNormal;
		StrPtr<Texture2D> OldNormalTex;

		StrPtr<Gfx::DepthStencyl> OldDepth;
		StrPtr<Texture2D> OldDepthTex;

		StrPtr<Gfx::RenderTarget> DebugResult;
		StrPtr<Texture2D> DebugResultTex;

		float g_fCameraClipNear = 1.0f;
		float g_fCameraClipFar = 10000.0f;
		float g_fVoxelSize = 8.0f;
		float g_fDiffuseScale = 1.0f;
		float g_fSpecularScale = 1.0f;
		bool g_bEnableMultiBounce = true;
		float g_fMultiBounceScale = 1.0f;
		float g_fLightSize = 250.0f;
		bool g_bEnableGI = true;
		bool g_bInitialized = false;
		float g_fSamplingRate = 1.0f;
		float g_fQuality = 0.1f;
		int g_nMapSize = 128;
		bool g_bDrawTransparent = false;
		float g_TransparentRoughness = 0.1f;
		float g_TransparentReflectance = 0.1f;
		bool g_bTemporalFiltering = true;

		float g_fVxaoRange = 512.0f;
		float g_fVxaoScale = 2.0f;

		float g_ssaoRadius = 50.f;

	private:

		Gfx::GfxCore* m_gfx;

		void DrawSceneForVoxelization();

		Vector3 CalculateVoxelCenterPoint(WeakPtr<CameraCmp> activeCamera);

		void ComputeIndirectChannels();

		void RenderDebugChannels();

		HRESULT CreateVXGIObject();

		HRESULT CreateVoxelizationGS(VXGI::IShaderCompiler* pCompiler, VXGI::IGlobalIllumination* pGI);

		HRESULT CreateVoxelizationPS(VXGI::IShaderCompiler* pCompiler, VXGI::IGlobalIllumination* pGI);

		HRESULT CreateTransparentGeometryPS(VXGI::IShaderCompiler* pCompiler, VXGI::IGlobalIllumination* pGI);

		void AllocateViewDependentResources(UINT width, UINT height, UINT sampleCount);

		void ReleaseResources(VXGI::IGlobalIllumination* pGI);

		VXGI::Status::Enum SetVoxelizationParameters();


		VXGI::IUserDefinedShaderSet* m_pVoxelizationGS;
		VXGI::IUserDefinedShaderSet* m_pVoxelizationPS;
		VXGI::IUserDefinedShaderSet* m_pTransparentGeometryPS;

		///Global objects
		VXGI::IGlobalIllumination* g_pGI = NULL;
		VXGI::IShaderCompiler* g_pGICompiler = NULL;
		VXGI::IBasicViewTracer* g_pGITracer = NULL;

		///Input buffers
		VXGI::IBasicViewTracer::InputBuffers g_InputBuffersPrev;
		bool g_InputBuffersPrevValid = false;

		StrPtr<Gfx::RenderTarget> rTarget;
		StrPtr<Texture2D> texture;

		///////// Scene Renderer

		UINT                    m_Width;
		UINT                    m_Height;
		UINT                    m_SampleCount;
		NVRHI::TextureRef       m_Albedo;
		NVRHI::TextureHandle	alb;
		NVRHI::TextureRef       m_TargetNormal;
		NVRHI::TextureRef       m_TargetDepth;
	};
};