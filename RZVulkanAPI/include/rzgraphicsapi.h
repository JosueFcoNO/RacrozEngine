#pragma once

namespace rczEngine
{
	class RZ_UTILITY_EXPORT FontRender;

	///The DirectX Graphical API
	class RZ_UTILITY_EXPORT GfxCore
	{
	public:

		/////////////////////////////////////////////////
		///////////////INIT FUNCTIONS////////////////////
		/////////////////////////////////////////////////

		///Releases the allocated memory
		void Destroy();
		///Creates the Device
		bool CreateDevice(HWND hWnd, int32 width, int32 height, int BufferCount, int RefreshRate, int SampleCount, int Quality, bool IsWindowed);
		///Creates a context
		bool CreateDeviceContext();
		///Creates the swapchain
		bool CreateSwapChain(HWND hWnd);
		///Gets the back buffer into a Texture2D Interface
		void GetBackBufferInterface();
		///Sets the Render Target View and Depth View into the Output Merger
		bool SetRenderTargetViewAndDepthStencil();
		///Creates a Render Target And Render Target View
		bool CreateRenderTarget(RenderTarget & out_renderTarget, char* name, bool drawToBackBuffer = false, int mipMaps = 1, int32 width = -1, int32 height = -1, eFORMAT format = eFORMAT::FORMAT_R8G8B8A8_UNORM, eBUFFER_USAGE usage = USAGE_DEFAULT, eCPU_ACCESS_FLAGS cpu_access = CPU_DEFAULT, int32 sample_count = 1, int32 sample_quality = 0);
		///Creates a DepthStencyl and Depth Stencyl View
		bool CreateDepthStencyl(DepthStencyl & out_depthStencyl, int32 width = -1, int32 height = -1, eBUFFER_USAGE usage = USAGE_DEFAULT, eCPU_ACCESS_FLAGS cpu_access = CPU_DEFAULT, int32 sample_count = 4, int32 sample_quality = 0);
		///Adds the Render Target View to a internal array of render targets, to be set later.
		bool AddRenderTarget(RenderTarget& renderTarget, int32 renderTargetSlot);
		///Sets the Render Targets added before to the pipeline.
		bool SetRenderTargets();
		///Removes the Render Targets from the pipeline
		void UnbindRenderTargets();
		///Removes the shader resourcese from the pipeline.
		void UnbindPSShaderResource(int32 slot);
		///Sets the number of Render Targets
		void SetNumberOfRenderTargets(int32 numberRenderTargets);
		///Sets the render target to the swap chain
		void SetTheRenderTargetToTheSwapChain(RenderTarget& renderTarget);
		///Sets The Viewport using m_ScreenWidth and m_ScreenHeight
		bool SetViewPort();
		///Generates MipMaps
		void GenerateMipMaps(TextureCore2D& texCore);
		void GenerateMipMaps(TextureCore3D& texCore);

		/////////////////////////////////////////////////
		//////////////PIPELINE FUNCTIONS/////////////////
		/////////////////////////////////////////////////

		///Sets the primitive topology to the device context.
		void SetPrimitiveTopology(eTOPOLOGY usedTopology);
		///Sets InputLayout.
		bool SetInputLayout(const InputLayout& iLayout);

		///Sets the Vertex Buffer.
		template <class t>
		void SetVertexBuffer(VertexBuffer<t> & vertexBuffer, uint32 nBuffers, uint32 startSlot, uint32 sizeOfVertex, uint32 bufferOffset)
		{
			m_DeviceContext->IASetVertexBuffers(startSlot, nBuffers, &vertexBuffer.m_Buffer, &sizeOfVertex, &bufferOffset);
		};
		///Updates a resource with new data.
		void UpdateSubResource(BasicBuffer* buffer, void* ptrToData, int32 pitch, int32 slice, int32 size = 0);

		///Sets the Index Buffer.
		void SetIndexBuffer(IndexBuffer & IndexBuffer, uint32 bufferOffset);
		///Updates a Constant Buffer with the data on srcData
		void UpdateConstantBuffer(ConstantBuffer& cBuffer, void* srcData);

		//////////////////
		//VERTEX SHADER///
		//////////////////
		///Sets the Vertex Shader on the pipeline. return false on failure.
		bool SetVertexShader(VertexShader& vertexShader);
		///Sets a Constant Buffer to the Vertex Shader
		bool VSSetConstantBuffer(ConstantBuffer& cBuffer, uint32 startSlot, uint32 numBuffers);
		///Sets the Vertex Shader's Resources
		bool SetVSShaderResource(uint32 startSlot, uint32 NumView, ID3D11ShaderResourceView* shaderResource);
		///Compiles and creates a vertex shader, it also reflects the input layout and saves it.
		bool CompileAndCreateVertexShader(VertexShader& out_VertexShader, const UNICHAR* pszShaderFile);
		///Creates a vertex shader.
		bool CreateVertexShader(VertexShader& out_ShaderObj);

		////////////////
		//PIXEL SHADER//
		////////////////
		///Sets the Pixel Shader on the pipeline. returns false on failure.
		bool SetPixelShader(PixelShader& pixelShader);
		///Sets a Constant Buffer to the Pixel Shader
		bool PSSetConstantBuffer(ConstantBuffer& cBuffer, uint32 startSlot, uint32 numBuffers);
		///Sets the Pixel Shader's Resources
		bool SetPSShaderResource(uint32 startSlot, uint32 NumView, ID3D11ShaderResourceView* shaderResource);
		///Sets the Pixel Shader Sampler
		bool SetPSSamplers(const SamplerState& samplerState, uint32 startSlot, uint32 NumView);
		///Compiles and creates a Pixel Shader.
		bool CompileAndCreatePixelShader(PixelShader& out_PixelShader, const UNICHAR* pszShaderFile);
		///Creates a pixel shader
		bool CreatePixelShader(PixelShader& out_ShaderObj);

		///////////////
		//HULL SHADER//
		///////////////
		///Sets the Hull Shader on the pipeline. returns false on failure.
		bool SetHullShader(HullShader& hullShader);
		///Sets a Constant Buffer to the Hull Shader
		bool HSSetConstantBuffer(ConstantBuffer& cBuffer, uint32 startSlot, uint32 numBuffers);
		///Compile and create a Hull Shader
		bool CompileAndCreateHullShader(HullShader& out_HullShader, const UNICHAR* pszShaderFile);
		///Creates a Hull Shader
		bool CreateHullShader(HullShader& out_ShaderObj);

		/////////////////
		//DOMAIN SHADER//
		/////////////////
		///Sets the Domain Shader on the pipeline. returns false on failure.
		bool SetDomainShader(DomainShader& domainShader);
		///Sets a Constant Buffer to the Domain Shader
		bool DSSetConstantBuffer(ConstantBuffer& cBuffer, uint32 startSlot, uint32 numBuffers);
		///Compile and create a Domain Shader
		bool CompileAndCreateDomainShader(DomainShader& out_DomainShader, const UNICHAR* pszShaderFile);
		///Creates a Domain Shader
		bool CreateDomainShader(DomainShader& out_ShaderObj);


		///////////////////
		//GEOMETRY SHADER//
		///////////////////
		///Sets the Geometry Shader on the pipeline. returns false on failure.
		bool SetGeometryShader(GeometryShader& geometryShader);
		///Sets a Constant Buffer to the Geometry Shader
		bool GSSetConstantBuffer(ConstantBuffer& cBuffer, uint32 startSlot, uint32 numBuffers);
		///Compile and create a Domain Shader
		bool CompileAndCreateGeometryShader(GeometryShader& out_GeometryShader, const UNICHAR* pszShaderFile);
		///Creates a Domain Shader
		bool CreateGeometryShader(GeometryShader& out_ShaderObj);

		void ClearGeometryShader();

		////////////////////
		//RASTERIZER STATE//
		////////////////////
		///Sets the Rasterizer State
		bool SetRSState(const RasterizerState& rasterizerState);
		///Set the default Rasterizer State
		void SetRSStateDefault();
		///Creates a rasterizer State
		bool CreateRasterizerState(RasterizerState& out_rasterizerState);

		////////////////////
		/////BLEND STATE////
		////////////////////
		///Create a Blend State
		bool CreateBlendState(BlendState& blendState);
		///Sets the Blend State
		bool SetBlendState(const BlendState& blendState);
		///Sets a default blend state
		void SetBlendStateDefault();

		/////////////////////////////////////////////////
		//////////////////DRAW METHODS///////////////////
		/////////////////////////////////////////////////

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
		///Resize the window
		void Resize(uint32 Width, uint32 Height);
		///Get the size of the backbuffer
		void GetScreenSize(int32& out_width, int32& out_height);
		///Get the width
		int32 GetWidth() { return m_ScreenWidth; };
		///Get the height
		int32 GetHeight() { return m_ScreenHeight; };

		/////////////////////////////////////////////////
		///////////////OBJECT CREATORS///////////////////
		/////////////////////////////////////////////////

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

		////////////////////
		/////TEXTURE 2D/////
		////////////////////
		///Create a Texture from a file
		bool CreateTextureFromFile(const char* pszFilePath, TextureCore2D& out_Texture, eBUFFER_USAGE usage, eBIND_FLAGS bind_flags,
			eCPU_ACCESS_FLAGS cpu_access_flags);

		///Create a Texture from memory
		bool CreateTextureFromMemory(bool bCompressed, int width, int height, void* memory, TextureCore2D& out_Texture, eBUFFER_USAGE usage, eBIND_FLAGS bind_flags,
			eCPU_ACCESS_FLAGS cpu_access_flags);

		///Create a Texture void
		bool CreateTexture(void* memory, int32 pitch, int32 slice, TextureCore2D & out_Texture, int width, int height, eBIND_FLAGS bind_flags, eFORMAT format, int MipLevels = 1, eBUFFER_USAGE usage = USAGE_DEFAULT, eCPU_ACCESS_FLAGS cpu_access_flags = eCPU_ACCESS_FLAGS::CPU_DEFAULT);
		
		
		////////////////////
		/////TEXTURE 3D/////
		////////////////////
		bool CreateTexture3D(void* memory, int32 pitch, int32 slice, TextureCore3D & out_Texture, int width, int height, eBIND_FLAGS bind_flags, eFORMAT format, int MipLevels = 1, eBUFFER_USAGE usage = USAGE_DEFAULT, eCPU_ACCESS_FLAGS cpu_access_flags = eCPU_ACCESS_FLAGS::CPU_DEFAULT);
		///Create a Texture from a file
		bool CreateTexture3DFromFile(const char* pszFilePath1, const char* pszFilePath2, const char* pszFilePath3, const char* pszFilePath4, TextureCore3D& out_Texture, eBUFFER_USAGE usage, eBIND_FLAGS bind_flags,
			eCPU_ACCESS_FLAGS cpu_access_flags);

		bool CreateCubeMapFrom6Files(char * front, char * back, char * left, char * right, char * top, char * bottom, TextureCore2D & out_Cubemap, eFORMAT format, eBUFFER_USAGE usage = USAGE_DEFAULT, eBIND_FLAGS bind_flags = eBIND_FLAGS::BIND_SHADER_RESOURCE, eCPU_ACCESS_FLAGS cpu_access_flags = eCPU_ACCESS_FLAGS::CPU_DEFAULT);
		bool CreateCubeMapFrom6MemoryPtr(void * front, void * back, void * left, void * right, void * top, void * bottom, TextureCore2D & out_Cubemap, eFORMAT format, eBUFFER_USAGE usage = USAGE_DEFAULT, eBIND_FLAGS bind_flags = eBIND_FLAGS::BIND_SHADER_RESOURCE, eCPU_ACCESS_FLAGS cpu_access_flags = eCPU_ACCESS_FLAGS::CPU_DEFAULT);
		bool CreateCubeMapFromDDS(char* pszFilePath, TextureCore2D& out_Cubemap, eFORMAT format = FORMAT_R8G8B8A8_UNORM, eBUFFER_USAGE usage = USAGE_DEFAULT, eBIND_FLAGS bind_flags = eBIND_FLAGS::BIND_SHADER_RESOURCE, eCPU_ACCESS_FLAGS cpu_access_flags = eCPU_ACCESS_FLAGS::CPU_DEFAULT);

		ID3D11Device* GetDevice() { return m_Device; };

		/////////////////////

		ConstantBuffer WorldBuffer;
		ConstantBuffer ProjBuffer;
		ConstantBuffer ViewBuffer;

		String m_LoadedVerticesName;

	private:

		friend FontRender;

		///The Device it is using
		VkInstance m_Instance;
		VkDevice m_Device;
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

}