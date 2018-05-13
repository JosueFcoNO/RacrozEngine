#pragma once

namespace rczEngine
{
#if RZ_PLATFORM == RZ_PLATFORM_WINDOWS
	struct PlatformData
	{
		HWND WindowHandle;
		HINSTANCE HandleInstance;
	};
#endif

	namespace Gfx
	{
		///The DirectX Graphics Core
		class RZ_UTILITY_EXPORT GfxCore
		{
		private:
			static GfxCore*& _Instance();

		public:
			static void Start();
			static GfxCore* Pointer();
			static void ShutDown();

			~GfxCore() { Destroy(); };

#pragma region =	| Init Functions |

			///Starts and sets everything up.
			void InitAndStart(PlatformData* platform, int width, int height, bool isWindowed);

			///Releases the allocated memory
			void Destroy();

			///Creates the Device
			bool CreateDevice(int32 width, int32 height, int BufferCount, int RefreshRate, int SampleCount, int Quality, bool IsWindowed);

			///Creates a context
			bool CreateDeviceContext();

			///Creates the swapchain
			bool CreateSwapChain();

			///Gets the back buffer into a Texture2D Interface
			void GetBackBufferInterface();

			///Sets the Render Target View and Depth View into the Output Merger
			bool SetRenderTargetViewAndDepthStencil();

			///Sets The Viewport using m_ScreenWidth and m_ScreenHeight
			bool SetViewPort();

			///Resize the window
			void Resize(uint32 Width, uint32 Height);

			///Get the size of the backbuffer
			void GetScreenSize(int32& out_width, int32& out_height);

			///Get the width
			int32 GetWidth() { return m_ScreenWidth; };

			///Get the height
			int32 GetHeight() { return m_ScreenHeight; };

			ID3D11Device* GetDevice() { return m_Device; };

#pragma endregion

#pragma region =	| Pipeline Functions |

			///Creates a DepthStencyl and Depth Stencyl View
			bool CreateDepthStencyl(DepthStencyl & out_depthStencyl, int32 width = -1, int32 height = -1, eBUFFER_USAGE usage = USAGE_DEFAULT, eCPU_ACCESS_FLAGS cpu_access = CPU_DEFAULT, int32 sample_count = 4, int32 sample_quality = 0);

			///Sets the primitive topology to the device context.
			void SetPrimitiveTopology(eTOPOLOGY usedTopology);

			///Sets InputLayout.
			void SetInputLayout(const InputLayout& iLayout);

			///Creates a Render Target And Render Target View
			bool CreateRenderTarget(RenderTarget & out_renderTarget, const char* name, bool drawToBackBuffer = false, int mipMaps = 1, int32 width = -1, int32 height = -1, eFORMAT format = eFORMAT::FORMAT_R8G8B8A8_UNORM, eBUFFER_USAGE usage = USAGE_DEFAULT, eCPU_ACCESS_FLAGS cpu_access = CPU_DEFAULT, int32 sample_count = 1, int32 sample_quality = 0);

			///Adds the Render Target View to a internal array of render targets, to be set later.
			bool AddRenderTarget(RenderTarget& renderTarget, int32 renderTargetSlot);

			///Sets the Render Targets added before to the pipeline.
			bool SetRenderTargets(bool UseDepthStencyl = true);

			///Removes the Render Targets from the pipeline
			void UnbindRenderTargets();

			///Sets the number of Render Targets
			void SetNumberOfRenderTargets(int32 numberRenderTargets);

			///Sets the render target to the swap chain
			void SetTheRenderTargetToTheSwapChain(RenderTarget& renderTarget);

			///Sets the Vertex Buffer.
			template <class t>
			void SetVertexBuffer(VertexBuffer<t> & vertexBuffer, uint32 nBuffers, uint32 startSlot, uint32 sizeOfVertex, uint32 bufferOffset)
			{
				m_DeviceContext->IASetVertexBuffers(startSlot, nBuffers, &vertexBuffer.m_Buffer, &sizeOfVertex, &bufferOffset);
			};

			///Sets the Index Buffer.
			void SetIndexBuffer(IndexBuffer & IndexBuffer, uint32 bufferOffset);

			///Updates a resource with new data.
			void UpdateSubResource(BasicBuffer* buffer, void* ptrToData, int32 pitch, int32 slice, int32 size = 0);

			///Updates a resource with new data.
			void UpdateSubResource(TextureCore2D* buffer, void* ptrToData, int32 pitch, int32 slice, int32 size = 0);

			///Updates a Constant Buffer with the data on srcData
			void UpdateConstantBuffer(ConstantBuffer& cBuffer, void* srcData);

#pragma endregion

#pragma region =	| Draw Functions |

			///Clears the view with a color in 4 float array.
			void ClearRenderTargetView(int32 renderTargetSlot, const float Red, const float Blue, const float Green, const float Alpha);

			///Clears the depth stencyl view
			void ClearDepthTargetView();

			///Moves the backbuffer to the frontbuffer.
			void Present();

			///Draws
			bool Draw(uint32 vertexCount, uint32 vertex_start);

			///Draw Indexed
			bool DrawIndexed(uint32 indexCount, uint32 startIndex, uint32 startVertex);

#pragma endregion

#pragma region =	| Vertex Shader |

			///Compiles and creates a vertex shader, it also reflects the input layout and saves it.
			bool CompileAndCreateVertexShader(VertexShader& out_VertexShader, const UNICHAR* pszShaderFile);

			///Creates a vertex shader.
			bool CreateVertexShader(VertexShader& out_ShaderObj);

			///Sets the Vertex Shader on the pipeline. return false on failure.
			void SetVertexShader(VertexShader& vertexShader);

			///Sets a Constant Buffer to the Vertex Shader
			void VSSetConstantBuffer(ConstantBuffer& cBuffer, uint32 startSlot, uint32 numBuffers);

			///Sets the Vertex Shader's Resources
			void SetVSShaderResource(uint32 startSlot, uint32 NumView, ShaderResource* shaderResource);

#pragma endregion

#pragma region =	| Pixel Shader |

			///Compiles and creates a Pixel Shader.
			bool CompileAndCreatePixelShader(PixelShader& out_PixelShader, const UNICHAR* pszShaderFile);

			///Creates a pixel shader
			bool CreatePixelShader(PixelShader& out_ShaderObj);

			///Sets the Pixel Shader on the pipeline. returns false on failure.
			void SetPixelShader(PixelShader& pixelShader);

			///Sets a Constant Buffer to the Pixel Shader
			void PSSetConstantBuffer(ConstantBuffer& cBuffer, uint32 startSlot, uint32 numBuffers);

			///Sets the Pixel Shader's Resources
			void SetPSShaderResource(uint32 startSlot, uint32 NumView, ShaderResource* shaderResource);

			///Removes the shader resources from the pipeline.
			void UnbindPSShaderResource(int32 slot);

			///Sets the Pixel Shader Sampler
			void SetPSSamplers(const SamplerState& samplerState, uint32 startSlot, uint32 NumView);

#pragma endregion

#pragma region =	| Hull Shader |

			///Compile and create a Hull Shader
			bool CompileAndCreateHullShader(HullShader& out_HullShader, const UNICHAR* pszShaderFile);

			///Creates a Hull Shader
			bool CreateHullShader(HullShader& out_ShaderObj);

			///Sets the Hull Shader on the pipeline. returns false on failure.
			bool SetHullShader(HullShader& hullShader);

			///Sets a Constant Buffer to the Hull Shader
			bool HSSetConstantBuffer(ConstantBuffer& cBuffer, uint32 startSlot, uint32 numBuffers);

			///Disables the hull shader stage.
			void RemoveHullShader();

			///Sets a shader resource to the HS.
			void SetHSShaderResource(uint32 startSlot, uint32 NumView, ShaderResource* shaderResource);

			///Removes the shader resourcese from the pipeline.
			void UnbindHSShaderResource(int32 slot);

			///Sets the Pixel Shader Sampler
			void SetHSSamplers(const SamplerState& samplerState, uint32 startSlot, uint32 NumView);

#pragma endregion

#pragma region =	| Domain Shader |

			///Compile and create a Domain Shader
			bool CompileAndCreateDomainShader(DomainShader& out_DomainShader, const UNICHAR* pszShaderFile);

			///Creates a Domain Shader
			bool CreateDomainShader(DomainShader& out_ShaderObj);

			///Sets the Domain Shader on the pipeline. returns false on failure.
			bool SetDomainShader(DomainShader& domainShader);

			///Sets a Constant Buffer to the Domain Shader
			bool DSSetConstantBuffer(ConstantBuffer& cBuffer, uint32 startSlot, uint32 numBuffers);

			//Disables the Domain Shader stage
			void RemoveDomainShader();

			///Sets a shader resource to the DS.
			void SetDSShaderResource(uint32 startSlot, uint32 NumView, ShaderResource* shaderResource);

			///Removes the shader resourcese from the pipeline.
			void UnbindDSShaderResource(int32 slot);

			///Sets the Pixel Shader Sampler
			void SetDSSamplers(const SamplerState& samplerState, uint32 startSlot, uint32 NumView);

#pragma endregion

#pragma region =	| Geometry Shader |

			///Compile and create a Domain Shader
			bool CompileAndCreateGeometryShader(GeometryShader& out_GeometryShader, const UNICHAR* pszShaderFile);

			///Creates a Domain Shader
			bool CreateGeometryShader(GeometryShader& out_ShaderObj);

			///Sets the Geometry Shader on the pipeline. returns false on failure.
			bool SetGeometryShader(GeometryShader& geometryShader);

			///Sets a Constant Buffer to the Geometry Shader
			bool GSSetConstantBuffer(ConstantBuffer& cBuffer, uint32 startSlot, uint32 numBuffers);

			///Clears the Geometry Shader
			void ClearGeometryShader();

#pragma endregion

#pragma region =	| Rasterizer State |

			///Creates a rasterizer State
			bool CreateRasterizerState(RasterizerState& out_rasterizerState);

			///Sets the Rasterizer State
			bool SetRSState(const RasterizerState& rasterizerState);

			///Set the default Rasterizer State
			void SetRSStateDefault();

#pragma endregion

#pragma region =	| Blend State |

			///Create a Blend State
			bool CreateBlendState(BlendState& blendState);

			///Sets the Blend State
			bool SetBlendState(const BlendState& blendState);

			///Sets a default blend state
			void SetBlendStateDefault();

#pragma endregion

#pragma region =	| Object Creation |

			///Creates the sampler state
			bool CreateSamplerState(SamplerState& out_samplerState);

			///Compiles a Shader.
			bool CompileShader(Shader& out_ShaderObj, const UNICHAR* pszShaderFile, const char* version, const char* entryPoint);

			///Reflects a shader
			bool ReflectShader(InputLayout& out_InputLayout, Shader& shaderToReflect);

			///Creates a buffer and returns it in out_Buffer.
			bool CreateBuffer(BasicBuffer& out_Buffer, uint32 sizeOfElement, uint32 numOfElements, void* arrayOfElements,
				uint32 elementsPitch, uint32 elementsSlice, eBUFFER_USAGE usage, eBIND_FLAGS bindflags);

			///Creates an input layout. It needs the Shader object to get the vertex shader blob.
			bool CreateInputLayout(InputLayout& out_ILayout, const VertexShader& vertexShader);

			void* GetBufferData(BasicBuffer& buffer);

			void* GetTexture2DData(TextureCore2D& texture);
#pragma endregion

#pragma region =	| Texture Functions |

			///Create a Texture from a file
			bool CreateTextureFromFile(const char* pszFilePath, TextureCore2D& out_Texture, eBUFFER_USAGE usage, eBIND_FLAGS bind_flags,
				eCPU_ACCESS_FLAGS cpu_access_flags);

			///Create a Texture from memory
			bool CreateTextureFromMemory(bool bCompressed, int width, int height, void* memory, TextureCore2D& out_Texture, eBUFFER_USAGE usage, eBIND_FLAGS bind_flags,
				eCPU_ACCESS_FLAGS cpu_access_flags);

			///Create a Texture void
			bool CreateTexture(void* memory, int32 pitch, int32 slice, TextureCore2D & out_Texture, int width, int height, eBIND_FLAGS bind_flags, eFORMAT format, int MipLevels = 1, eBUFFER_USAGE usage = USAGE_DEFAULT, eCPU_ACCESS_FLAGS cpu_access_flags = eCPU_ACCESS_FLAGS::CPU_DEFAULT);

			///Create a 3D void Texture
			bool CreateTexture3D(void* memory, int32 pitch, int32 slice, TextureCore3D & out_Texture, int width, int height, eBIND_FLAGS bind_flags, eFORMAT format, int MipLevels = 1, eBUFFER_USAGE usage = USAGE_DEFAULT, eCPU_ACCESS_FLAGS cpu_access_flags = eCPU_ACCESS_FLAGS::CPU_DEFAULT);

			///Create a 3D Texture from a file
			bool CreateTexture3DFromFile(const char* pszFilePath1, const char* pszFilePath2, const char* pszFilePath3, const char* pszFilePath4, TextureCore3D& out_Texture, eBUFFER_USAGE usage, eBIND_FLAGS bind_flags,
				eCPU_ACCESS_FLAGS cpu_access_flags);

			///Create a Cubemap from 6 image paths.
			bool CreateCubeMapFrom6Files(char * front, char * back, char * left, char * right, char * top, char * bottom, TextureCore2D & out_Cubemap, eFORMAT format, eBUFFER_USAGE usage = USAGE_DEFAULT, eBIND_FLAGS bind_flags = eBIND_FLAGS::BIND_SHADER_RESOURCE, eCPU_ACCESS_FLAGS cpu_access_flags = eCPU_ACCESS_FLAGS::CPU_DEFAULT);

			///Create a Cubemap from 6 memory pointers
			bool CreateCubeMapFrom6MemoryPtr(void * front, void * back, void * left, void * right, void * top, void * bottom, TextureCore2D & out_Cubemap, eFORMAT format, eBUFFER_USAGE usage = USAGE_DEFAULT, eBIND_FLAGS bind_flags = eBIND_FLAGS::BIND_SHADER_RESOURCE, eCPU_ACCESS_FLAGS cpu_access_flags = eCPU_ACCESS_FLAGS::CPU_DEFAULT);

			///Create a Cubemap from a DDS file path
			bool CreateCubeMapFromDDS(const char* pszFilePath, TextureCore2D& out_Cubemap, eFORMAT format = FORMAT_R8G8B8A8_UNORM, eBUFFER_USAGE usage = USAGE_DEFAULT, eBIND_FLAGS bind_flags = eBIND_FLAGS::BIND_SHADER_RESOURCE, eCPU_ACCESS_FLAGS cpu_access_flags = eCPU_ACCESS_FLAGS::CPU_DEFAULT);

			///Generates MipMaps
			void GenerateMipMaps(ShaderResource* texCore);

#pragma endregion

			ResourceHandle m_SetMaterial = INVALID_RESOURCE;

		private:
			friend GUIEditor;
		
			PlatformData* m_PlatformData;

			///The Device it is using
			ID3D11Device* m_Device;

			///The Device Context
			ID3D11DeviceContext* m_DeviceContext;

			///The SwapChain
			IDXGISwapChain* m_SwapChain;

			///The RenderTargetVews.
			ID3D11RenderTargetView* m_RenderTargetView[8];

			///The DepthStencilView
			ID3D11DepthStencilView* m_DepthStencilView;

			///The number of RenderTargets Bound to the pipeline
			byte m_RenderTargetNumber = 1;

			///The Back Buffer as a Texture2D 
			ID3D11Texture2D* m_BackBuffer;

			///The Depth Stencil as a Texture2D 
			ID3D11Texture2D* m_DepthStencil;

			///A Blend State
			ID3D11BlendState* m_BlendState;

			RasterizerState m_DefaultRS;

			///The screen's Width
			int m_ScreenWidth;
			///The screen's Height
			int m_ScreenHeight;


		};

	}//Namespace Gfx
} //Namespace rczEngine