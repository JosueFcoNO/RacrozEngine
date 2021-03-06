#include <RZUtilityPCH.h>
#include "RZVulkanPCH.h"
#include "..\include\rzgraphicsapi.h"
#define STB_IMAGE_IMPLEMENTATION 1
#include "../../Plug-Ins/stb-master/stb_image.h"

namespace rczEngine
{
	/////////////////////////////////////////////////
	///////////////INIT FUNCTIONS////////////////////
	/////////////////////////////////////////////////

	void GfxCore::Destroy()
	{
		//TODO: Destroy Everything
		vkDestroyInstance(m_Instance, nullptr);
	}

	bool GfxCore::CreateDevice(HWND hWnd, int32 width, int32 height, int BufferCount = 1, int RefreshRate = 60, int SampleCount = 1, int Quality = 0, bool IsWindowed = true)
	{
		//DXGI_SWAP_CHAIN_DESC sd;
		//ZeroMemory(&sd, sizeof(sd));
		//RECT Rc;
		//GetClientRect(hWnd, &Rc);
		//sd.BufferCount = BufferCount;
		//sd.BufferDesc.Width = m_ScreenWidth = width;
		//sd.BufferDesc.Height = m_ScreenHeight = height;
		//sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		//sd.BufferDesc.RefreshRate.Numerator = RefreshRate;
		//sd.BufferDesc.RefreshRate.Denominator = 1;
		//sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		//sd.OutputWindow = hWnd;
		//sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		//sd.SampleDesc.Count = SampleCount;
		//sd.SampleDesc.Quality = Quality;
		//sd.Windowed = IsWindowed;

		//TODO:
		//Create the instance and physical device.
		VkApplicationInfo appInfo = {};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Racroz Engine";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "Racroz Engine";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0;

		VkInstanceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;
		createInfo.enabledLayerCount = 0;
		createInfo.enabledExtensionCount = 0;

		uint32_t extensionCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

		std::vector<VkExtensionProperties> extensions(extensionCount);

		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

		std::cout << "available extensions:" << std::endl;

		Logger ExtensionLog;
		ExtensionLog.StartLog("ExtLog.html");

		for (const auto& extension : extensions) {
			std::cout << "\t" << extension.extensionName << std::endl;
		}

		if (vkCreateInstance(&createInfo, nullptr, &m_Instance) != VK_SUCCESS) {
			throw std::runtime_error("failed to create instance!");
		}


	}

	bool GfxCore::CreateDeviceContext()
	{
		//TODO:
		//Create the logical device
		///Creates the DeviceContext
		return false;
	}

	bool GfxCore::CreateSwapChain(HWND hWnd)
	{
		//TODO:
		//Create the frame buffer.
		///Create the swapChain
		return true;
	}

	void GfxCore::GetBackBufferInterface()
	{
		//TODO:
		//Save the frame buffer internally.
		/////Configure the Texture2D Descriptor
		//D3D11_TEXTURE2D_DESC BackBufferDesc;
		//BackBufferDesc.BindFlags = D3D11_BIND_RENDER_TARGET;
		//BackBufferDesc.ArraySize = 1;
		//BackBufferDesc.Height = m_ScreenHeight;
		//BackBufferDesc.Width = m_ScreenWidth;
		//BackBufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		//BackBufferDesc.MipLevels = 1;
		//BackBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		//BackBufferDesc.CPUAccessFlags = 0;
		//BackBufferDesc.MiscFlags = 0;
		//BackBufferDesc.SampleDesc.Count = 1;
		//BackBufferDesc.SampleDesc.Quality = 0;
	}

	bool GfxCore::SetRenderTargetViewAndDepthStencil()
	{
		//TODO:
		//Start and set the main frame buffer and depth stencil
		
		return true;
	}

	bool GfxCore::CreateRenderTarget(RenderTarget & out_renderTarget, char* name, bool drawToBackBuffer, int mipMaps, int32 width, int32 height, eFORMAT format, eBUFFER_USAGE usage, eCPU_ACCESS_FLAGS cpu_access, int32 sample_count, int32 sample_quality)
	{
		///Configure the Texture2D Descriptor
		D3D11_TEXTURE2D_DESC TexDesc;
		TexDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		TexDesc.ArraySize = 1;

		out_renderTarget.m_Name = name;

		if (height != -1)
		{
			TexDesc.Height = height;
		}
		else
		{
			TexDesc.Height = m_ScreenHeight;
		}
		
		if (width != -1)
		{
			TexDesc.Width = width;
		}
		else
		{
			TexDesc.Width = m_ScreenWidth;
		}
			
		TexDesc.Format = (DXGI_FORMAT)format;
		TexDesc.MipLevels = mipMaps;
		TexDesc.Usage = (D3D11_USAGE)usage;
		TexDesc.CPUAccessFlags = cpu_access;
		TexDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
		TexDesc.SampleDesc.Count = sample_count;
		TexDesc.SampleDesc.Quality = sample_quality;


		HRESULT hr;

		if (drawToBackBuffer)
		{
			m_BackBuffer->GetDesc(&TexDesc);
			TexDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		}

		///Create a Texture2D into the Texture in the Render Target
		hr = m_Device->CreateTexture2D(&TexDesc, NULL, out_renderTarget.GetTextureCore()->GetTexture());

		if (hr != S_OK)
		{
			return false;
		}

		D3D11_SHADER_RESOURCE_VIEW_DESC SRV;
		ZeroMemory(&SRV, sizeof(SRV));
		SRV.Format = TexDesc.Format;
		SRV.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		SRV.Texture2D.MostDetailedMip = 0;
		SRV.Texture2D.MipLevels = mipMaps;

		hr = m_Device->CreateShaderResourceView(*out_renderTarget.GetTextureCore()->GetTexture(), &SRV, out_renderTarget.GetTextureCore()->GetShaderResourceView());

		if (hr != S_OK)
		{
			return false;
		}

		if (drawToBackBuffer)
		{
			SetTheRenderTargetToTheSwapChain(out_renderTarget);
		}

		///Create the RenderTargetView from the backbufffer
		hr = m_Device->CreateRenderTargetView(out_renderTarget.GetTextureCore()->m_Texture, NULL, &out_renderTarget.GetRenderTargetView()->m_RenderTargetView);

		return S_OK(hr);
	}

	bool GfxCore::CreateDepthStencyl(DepthStencyl & out_depthStencyl, int32 width, int32 height, eBUFFER_USAGE usage, eCPU_ACCESS_FLAGS cpu_access, int32 sample_count, int32 sample_quality )
	{
		///Configure the Texture2D Descriptor
		D3D11_TEXTURE2D_DESC TexDesc;
		TexDesc.ArraySize = 1;

		if (height != -1)
		{
			TexDesc.Height = height;
		}
		else
		{
			TexDesc.Height = m_ScreenHeight;
		}

		if (width != -1)
		{
			TexDesc.Width = width;
		}
		else
		{
			TexDesc.Width = m_ScreenWidth;
		}

		TexDesc.MipLevels = 1;
		TexDesc.Usage = (D3D11_USAGE)usage;
		TexDesc.CPUAccessFlags = cpu_access;
		TexDesc.MiscFlags = 0;
		TexDesc.SampleDesc.Count = sample_count;
		TexDesc.SampleDesc.Quality = sample_quality;
		TexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
		TexDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

		///Create the resource and save it in m_DepthStencil
		m_Device->CreateTexture2D(&TexDesc, NULL, out_depthStencyl.GetTexture()->GetTexture());

		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
		ZeroMemory(&descDSV, sizeof(descDSV));
		descDSV.Format = TexDesc.Format;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

		///Create the Depth Stencil View and save it int m_DepthStencilView
		m_Device->CreateDepthStencilView(*out_depthStencyl.GetTexture()->GetTexture(), &descDSV, &out_depthStencyl.GetDepthStencylView()->m_DepthStencylView);
		return true;
	}

	bool GfxCore::AddRenderTarget(RenderTarget & renderTarget, int32 renderTargetSlot)
	{
		m_RenderTargetView[renderTargetSlot] = renderTarget.GetRenderTargetView()->m_RenderTargetView;
		return true;
	}

	bool GfxCore::SetRenderTargets()
	{
		///Set the render target and the depth stecil
		m_DeviceContext->OMSetRenderTargets(m_RenderTargetNumber, m_RenderTargetView, m_DepthStencilView);
		return true;
	}

	void GfxCore::UnbindRenderTargets()
	{
		ID3D11RenderTargetView *pSRV[1] = { nullptr };
		m_DeviceContext->OMSetRenderTargets(1, pSRV, NULL);
	}

	void GfxCore::UnbindPSShaderResource(int32 slot)
	{
		ID3D11ShaderResourceView *pSRV[1] = { nullptr };
		m_DeviceContext->PSSetShaderResources(slot, 1, pSRV);
	}

	void GfxCore::SetNumberOfRenderTargets(int32 numberRenderTargets)
	{
		m_RenderTargetNumber = byte(numberRenderTargets);
	}

	void GfxCore::SetTheRenderTargetToTheSwapChain(RenderTarget & renderTarget)
	{
		///Get the BackBuffer from the swapchain and into the Texture
		HRESULT hr = m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&renderTarget.GetTextureCore()->m_Texture);
	}

	bool GfxCore::SetViewPort()
	{
		///Create the viewport
		D3D11_VIEWPORT ViewPort;
		///Set its Width and height to the ones stored on this object.
		ViewPort.Width = (float)m_ScreenWidth;
		ViewPort.Height = (float)m_ScreenHeight;
		ViewPort.TopLeftX = 0;
		ViewPort.TopLeftY = 0;
		ViewPort.MaxDepth = 1.0f;
		ViewPort.MinDepth = 0.0f;
		///Set the viewport on the context.
		m_DeviceContext->RSSetViewports(1, &ViewPort);

		m_DefaultRS.Init();
		m_DefaultRS.CreateRasterizerState(this);

		return true;
	}

	void GfxCore::GenerateMipMaps(TextureCore2D & texCore)
	{
		m_DeviceContext->GenerateMips(*texCore.GetShaderResourceView());
	}

	void GfxCore::GenerateMipMaps(TextureCore3D & texCore)
	{
		m_DeviceContext->GenerateMips(*texCore.GetShaderResourceView());
	}

	bool GfxCore::VSSetConstantBuffer(ConstantBuffer& cBuffer, uint32 startSlot, uint32 numBuffers)
	{
		m_DeviceContext->VSSetConstantBuffers(startSlot, numBuffers, &cBuffer.m_Buffer);
		return true;
	}

	void GfxCore::UpdateSubResource(BasicBuffer * buffer, void * ptrToData, int32 pitch, int32 slice, int32 size)
	{
		if (buffer->m_Usage == USAGE_DEFAULT)
		{
			m_DeviceContext->UpdateSubresource(buffer->m_Buffer, 0, NULL, ptrToData, pitch, slice);
		}
		else if (buffer->m_Usage == USAGE_DYNAMIC)
		{
			D3D11_MAPPED_SUBRESOURCE mappedResource;
			ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

			m_DeviceContext->Map(buffer->m_Buffer, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &mappedResource);
			//	Update the vertex buffer here
			memcpy(mappedResource.pData, ptrToData, size);
			//	Reenable GPU access to the vertex buffer data.
			m_DeviceContext->Unmap(buffer->m_Buffer, 0);
		}
	}

	void GfxCore::SetIndexBuffer(IndexBuffer & IndexBuffer, uint32 bufferOffset)
	{
		m_DeviceContext->IASetIndexBuffer(IndexBuffer.m_Buffer, DXGI_FORMAT_R32_UINT, bufferOffset);
	}

	void GfxCore::UpdateConstantBuffer(ConstantBuffer& cBuffer, void* srcData)
	{
		m_DeviceContext->UpdateSubresource(cBuffer.m_Buffer, 0, 0, srcData, 0, 0);
	}

	bool GfxCore::SetPixelShader(PixelShader & pixelShader)
	{
		m_DeviceContext->PSSetShader(pixelShader.m_PixelShader, 0, 0);
		return true;
	}

	bool GfxCore::SetVertexShader(VertexShader & vertexShader)
	{
		m_DeviceContext->VSSetShader(vertexShader.m_VertexShader, 0, 0);
		return true;
	}

	bool GfxCore::PSSetConstantBuffer(ConstantBuffer & cBuffer, uint32 startSlot, uint32 numBuffers)
	{
		m_DeviceContext->PSSetConstantBuffers(startSlot, numBuffers, &cBuffer.m_Buffer);
		return true;
	}

	bool GfxCore::SetPSShaderResource(uint32 startSlot, uint32 NumView, ID3D11ShaderResourceView * shaderResource)
	{
		m_DeviceContext->PSSetShaderResources(startSlot, NumView, &shaderResource);

		return false;
	}

	bool GfxCore::SetVSShaderResource(uint32 startSlot, uint32 NumView, ID3D11ShaderResourceView * shaderResource)
	{
		m_DeviceContext->VSSetShaderResources(startSlot, NumView, &shaderResource);

		return true;
	}

	bool GfxCore::SetPSSamplers(const SamplerState& samplerState, uint32 startSlot, uint32 NumView)
	{
		m_DeviceContext->PSSetSamplers(startSlot, NumView, &samplerState.m_SamplerState);

		return false;
	}

	bool GfxCore::SetHullShader(HullShader & hullShader)
	{
		m_DeviceContext->HSSetShader(hullShader.m_HullShader, 0, 0);
		return true;
	}

	bool GfxCore::HSSetConstantBuffer(ConstantBuffer & cBuffer, uint32 startSlot, uint32 numBuffers)
	{
		m_DeviceContext->HSSetConstantBuffers(startSlot, numBuffers, &cBuffer.m_Buffer);
		return true;
	}

	bool GfxCore::SetDomainShader(DomainShader & domainShader)
	{
		m_DeviceContext->DSSetShader(domainShader.m_DomainShader, 0, 0);
		return true;
	}

	bool GfxCore::DSSetConstantBuffer(ConstantBuffer & cBuffer, uint32 startSlot, uint32 numBuffers)
	{
		m_DeviceContext->DSSetConstantBuffers(startSlot, numBuffers, &cBuffer.m_Buffer);
		return true;
	}

	///////////////////
	//GEOMETRY SHADER//
	///////////////////

	bool GfxCore::SetGeometryShader(GeometryShader & geometryShader)
	{
		m_DeviceContext->GSSetShader(geometryShader.m_GeometryShader, 0, 0);

		return true;
	}

	bool GfxCore::GSSetConstantBuffer(ConstantBuffer & cBuffer, uint32 startSlot, uint32 numBuffers)
	{
		m_DeviceContext->GSSetConstantBuffers(startSlot, numBuffers, &cBuffer.m_Buffer);
		return true;
	}

	bool GfxCore::CompileAndCreateGeometryShader(GeometryShader & out_GeometryShader, const UNICHAR * pszShaderFile)
	{
		CompileShader(out_GeometryShader, pszShaderFile, "gs_5_0", "GS_Main");

		return CreateGeometryShader(out_GeometryShader);;
	}

	bool GfxCore::CreateGeometryShader(GeometryShader & out_ShaderObj)
	{
		HRESULT result;

		result = m_Device->CreateGeometryShader(out_ShaderObj.m_ShaderBlob->GetBufferPointer(), out_ShaderObj.m_ShaderBlob->GetBufferSize(), 0, &out_ShaderObj.m_GeometryShader);

		if (FAILED(result))
		{
			if (out_ShaderObj.m_GeometryShader)
				out_ShaderObj.m_GeometryShader->Release();
			return false;
		}

		return true;
	}

	void GfxCore::ClearGeometryShader()
	{
		m_DeviceContext->GSSetShader(NULL, 0, 0);
	}



	bool GfxCore::SetRSState(const RasterizerState & rasterizerState)
	{
		m_DeviceContext->RSSetState(rasterizerState.m_RasterizerState);
		return true;
	}

	void GfxCore::SetRSStateDefault()
	{
		m_DefaultRS.SetThisRasterizerState(this);
	}

	bool GfxCore::CreateBlendState(BlendState & blendState)
	{
		D3D11_BLEND_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.AlphaToCoverageEnable = false;
		desc.IndependentBlendEnable = false;

		desc.RenderTarget[0].BlendEnable = blendState.m_BlendEnable;
		desc.RenderTarget[0].BlendOp = (D3D11_BLEND_OP)blendState.m_BlendOp;
		desc.RenderTarget[0].BlendOpAlpha = (D3D11_BLEND_OP)blendState.m_BlendOpAlpha;
		desc.RenderTarget[0].DestBlend = (D3D11_BLEND)blendState.m_DestBlend;
		desc.RenderTarget[0].DestBlendAlpha = (D3D11_BLEND)blendState.m_DestBlendAlpha;
		desc.RenderTarget[0].SrcBlend = (D3D11_BLEND)blendState.m_SrcBlend;
		desc.RenderTarget[0].SrcBlendAlpha = (D3D11_BLEND)blendState.m_SrcBlendAlpha;
		desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;


		HRESULT hr = m_Device->CreateBlendState(&desc, &blendState.m_BlendState);

		return !FAILED(hr);
	}

	bool GfxCore::SetBlendState(const BlendState & blendState)
	{
		m_DeviceContext->OMSetBlendState(blendState.m_BlendState, NULL, 0xffffffff);
		return true;
	}

	void GfxCore::SetBlendStateDefault()
	{
		m_DeviceContext->OMSetBlendState(NULL, NULL, 0xffffffff);
	}

	void GfxCore::ClearRenderTargetView(int32 renderTargetSlot, const float Red, const float Blue, const float Green, const float Alpha)
	{
		float Color[4];
		Color[0] = Red;
		Color[1] = Blue;
		Color[2] = Green;
		Color[3] = Alpha;

		m_DeviceContext->ClearRenderTargetView(m_RenderTargetView[renderTargetSlot], Color);
	}


	void GfxCore::ClearDepthTargetView()
	{
		m_DeviceContext->ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 255);
	}

	void GfxCore::Present()
	{
		m_SwapChain->Present(0, 0);
	}

	bool GfxCore::Draw(uint32 vertexCount, uint32 vertex_start)
	{
		m_DeviceContext->Draw(vertexCount, vertex_start);
		return false;
	}

	bool GfxCore::DrawIndexed(uint32 indexCount, uint32 startIndex, uint32 startVertex)
	{
		m_DeviceContext->DrawIndexed(indexCount, startIndex, startVertex);
		return false;
	}

	void GfxCore::GetScreenSize(int32 & out_width, int32 & out_height)
	{
		out_width = m_ScreenWidth;
		out_height = m_ScreenHeight;
	}

	void GfxCore::Resize(uint32 Width, uint32 Height)
	{
		if (m_SwapChain)
		{
			m_ScreenHeight = Height;
			m_ScreenWidth = Width;

			if (m_DepthStencil) { m_DepthStencil->Release(); }
			if (m_DepthStencilView) { m_DepthStencilView->Release(); }
			if (m_RenderTargetView[0]) { m_RenderTargetView[0]->Release(); }

			m_DeviceContext->ClearState();
			m_SwapChain->ResizeBuffers(2, Width, Height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
			///Get the BackBuffer from the swapchain and into the Texture
			GetBackBufferInterface();
			//m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&m_BackBuffer);
			SetRenderTargetViewAndDepthStencil();
			SetViewPort();

			return;
		}

	}

	bool GfxCore::CreateInputLayout(InputLayout& out_ILayout, const VertexShader& vertexShader)
	{
		uint32 nLayoutElements = out_ILayout.GetElementSize();

		D3D11_INPUT_ELEMENT_DESC* LayoutData = new D3D11_INPUT_ELEMENT_DESC[nLayoutElements];
		
		for (uint32 i = 0; i < nLayoutElements; i++)
		{
			LayoutData[i].SemanticName = out_ILayout.m_VectorElements[i].SemanticName.c_str();
			LayoutData[i].SemanticIndex = out_ILayout.m_VectorElements[i].SemanticIndex;
			LayoutData[i].InstanceDataStepRate = out_ILayout.m_VectorElements[i].InstanceDataStepRate;
			LayoutData[i].InputSlotClass = (D3D11_INPUT_CLASSIFICATION)out_ILayout.m_VectorElements[i].InputSlotClass;
			LayoutData[i].InputSlot = out_ILayout.m_VectorElements[i].InputSlot;
			LayoutData[i].Format = (DXGI_FORMAT)out_ILayout.m_VectorElements[i].Format;
			LayoutData[i].AlignedByteOffset = out_ILayout.m_VectorElements[i].AlignedByteOffset;
		}


		HRESULT result = m_Device->CreateInputLayout(LayoutData,
			nLayoutElements, vertexShader.m_ShaderBlob->GetBufferPointer(),
			vertexShader.m_ShaderBlob->GetBufferSize(), &out_ILayout.m_InputLayout);

		delete[] LayoutData;

		if (FAILED(result))
		{
			return false;
		}


		return true;
	}

	////////////////////////////////////////////////////////////////////////
	//////////////////////// TEXTURE 2D ////////////////////////////////////
	////////////////////////////////////////////////////////////////////////

	bool GfxCore::CreateTextureFromFile(const char * pszFilePath, TextureCore2D & out_Texture, eBUFFER_USAGE usage, eBIND_FLAGS bind_flags, eCPU_ACCESS_FLAGS cpu_access_flags)
	{
		// Basic usage (see HDR discussion below for HDR usage):
		//    int x,y,n;
		//    unsigned char *data = stbi_load(filename, &x, &y, &n, 0);
		//    // ... process data if not NULL ...
		//    // ... x = width, y = height, n = # 8-bit components per pixel ...
		//    // ... replace '0' with '1'..'4' to force that many components per pixel
		//    // ... but 'n' will always be the number that it would have been if you said 0
		//    stbi_image_free(data)
		int Width, Height, OriginalNumComponents;
		UCHAR* Data = stbi_load(pszFilePath, &Width, &Height, &OriginalNumComponents, 4);

		D3D11_SUBRESOURCE_DATA SubData;
		ZeroMemory(&SubData, sizeof(SubData));
		SubData.pSysMem = Data;
		SubData.SysMemPitch = Width * 4;
		SubData.SysMemSlicePitch = 1;

		DXGI_SAMPLE_DESC SamplerDesc;
		ZeroMemory(&SamplerDesc, sizeof(SamplerDesc));
		SamplerDesc.Count = 1;
		SamplerDesc.Quality = 0;
	
		D3D11_TEXTURE2D_DESC Descriptor;
		ZeroMemory(&Descriptor, sizeof(Descriptor));
		Descriptor.ArraySize = 1;
		Descriptor.BindFlags = bind_flags | D3D11_BIND_RENDER_TARGET;
		Descriptor.CPUAccessFlags = cpu_access_flags;
		Descriptor.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		Descriptor.Height = Height;
		Descriptor.Width = Width;
		Descriptor.MipLevels = 0;
		Descriptor.Usage = (D3D11_USAGE)usage;
		Descriptor.SampleDesc = SamplerDesc;
		Descriptor.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

		HRESULT result = m_Device->CreateTexture2D(&Descriptor, NULL, out_Texture.GetTexture());

		D3D11_BOX box;
		box.left = 0;
		box.right = Width;
		box.bottom = Height;
		box.top = 0;
		box.front = 0;
		box.back = 1;

		m_DeviceContext->UpdateSubresource(
			*out_Texture.GetTexture(),
			0,
			&box,
			Data, 
			Width * 4,
			0
		);
		
		D3D11_SHADER_RESOURCE_VIEW_DESC SRV;
		ZeroMemory(&SRV, sizeof(SRV));
		SRV.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		SRV.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		SRV.Texture2D.MipLevels = 0;

		result = m_Device->CreateShaderResourceView(*out_Texture.GetTexture(), NULL, out_Texture.GetShaderResourceView());

		STBI_FREE(Data);
		GenerateMipMaps(out_Texture);

		return true;
	}

	bool GfxCore::CreateTextureFromMemory(bool bCompressed, int width, int height, void * memory, TextureCore2D & out_Texture, eBUFFER_USAGE usage, eBIND_FLAGS bind_flags, eCPU_ACCESS_FLAGS cpu_access_flags)
	{

		int Width = width, Height = height, OriginalNumComponents;
		stbi_load_from_memory((stbi_uc*)memory, width, &Width, &Height, &OriginalNumComponents, 4);

		D3D11_SUBRESOURCE_DATA SubData;
		ZeroMemory(&SubData, sizeof(SubData));
		SubData.pSysMem = memory;
		SubData.SysMemPitch = Width * 4;
		SubData.SysMemSlicePitch = 1;

		DXGI_SAMPLE_DESC SamplerDesc;
		ZeroMemory(&SamplerDesc, sizeof(SamplerDesc));
		SamplerDesc.Count = 1;
		SamplerDesc.Quality = 0;

		D3D11_TEXTURE2D_DESC Descriptor;
		ZeroMemory(&Descriptor, sizeof(Descriptor));
		Descriptor.ArraySize = 1;
		Descriptor.BindFlags = bind_flags;
		Descriptor.CPUAccessFlags = cpu_access_flags;
		Descriptor.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		Descriptor.Height = Height;
		Descriptor.Width = Width;
		Descriptor.MipLevels = 1;
		Descriptor.Usage = (D3D11_USAGE)usage;
		Descriptor.SampleDesc = SamplerDesc;
		Descriptor.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

		HRESULT result = m_Device->CreateTexture2D(&Descriptor, &SubData, out_Texture.GetTexture());

		D3D11_SHADER_RESOURCE_VIEW_DESC SRV;
		ZeroMemory(&SRV, sizeof(SRV));
		SRV.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		SRV.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		SRV.Texture2D.MostDetailedMip = 0;
		SRV.Texture2D.MipLevels = -1;

		result = m_Device->CreateShaderResourceView(*out_Texture.GetTexture(), &SRV, out_Texture.GetShaderResourceView());
		if (S_OK(result))
		{
			return true;
		}

		return false;
	}

	bool GfxCore::CreateTexture(void* memory, int32 pitch, int32 slice, TextureCore2D & out_Texture, int width, int height, eBIND_FLAGS bind_flags, eFORMAT format, int MipLevels, eBUFFER_USAGE usage,  eCPU_ACCESS_FLAGS cpu_access_flags)
	{
		D3D11_SUBRESOURCE_DATA SubData;
		ZeroMemory(&SubData, sizeof(SubData));
		SubData.pSysMem = memory;
		SubData.SysMemPitch = pitch;
		SubData.SysMemSlicePitch = slice;

		DXGI_SAMPLE_DESC SamplerDesc;
		ZeroMemory(&SamplerDesc, sizeof(SamplerDesc));
		SamplerDesc.Count = 1;
		SamplerDesc.Quality = 0;

		D3D11_TEXTURE2D_DESC Descriptor;
		ZeroMemory(&Descriptor, sizeof(Descriptor));
		Descriptor.ArraySize = 1;
		Descriptor.BindFlags = bind_flags;
		Descriptor.CPUAccessFlags = cpu_access_flags;
		Descriptor.Format = (DXGI_FORMAT)format;
		Descriptor.Height = height;
		Descriptor.Width = width;
		Descriptor.MipLevels = 1;
		Descriptor.Usage = (D3D11_USAGE)usage;
		Descriptor.SampleDesc = SamplerDesc;

		HRESULT result = m_Device->CreateTexture2D(&Descriptor, &SubData, out_Texture.GetTexture());

		D3D11_SHADER_RESOURCE_VIEW_DESC SRV;
		ZeroMemory(&SRV, sizeof(SRV));
		SRV.Format = (DXGI_FORMAT)format;
		SRV.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		SRV.Texture2D.MostDetailedMip = 0;
		SRV.Texture2D.MipLevels = 1;

		result = m_Device->CreateShaderResourceView(*out_Texture.GetTexture(), &SRV, out_Texture.GetShaderResourceView());

		if (S_OK(result))
		{
			return true;
		}

		return false;
	}

	////////////////////////////////////////////////////////////////////////
	//////////////////////// TEXTURE 3D ////////////////////////////////////
	////////////////////////////////////////////////////////////////////////

	bool GfxCore::CreateTexture3D(void * memory, int32 pitch, int32 slice, TextureCore3D & out_Texture, int width, int height, eBIND_FLAGS bind_flags, eFORMAT format, int MipLevels, eBUFFER_USAGE usage, eCPU_ACCESS_FLAGS cpu_access_flags)
	{
		D3D11_SUBRESOURCE_DATA SubData;
		ZeroMemory(&SubData, sizeof(SubData));
		SubData.pSysMem = memory;
		SubData.SysMemPitch = pitch;
		SubData.SysMemSlicePitch = slice;

		D3D11_TEXTURE3D_DESC Descriptor;
		ZeroMemory(&Descriptor, sizeof(Descriptor));
		Descriptor.BindFlags = bind_flags;
		Descriptor.CPUAccessFlags = cpu_access_flags;
		Descriptor.Format = (DXGI_FORMAT)format;
		Descriptor.Height = height;
		Descriptor.Width = width;
		Descriptor.MipLevels = 1;
		Descriptor.Usage = (D3D11_USAGE)usage;

		HRESULT result = m_Device->CreateTexture3D(&Descriptor, &SubData, out_Texture.GetTexture());

		D3D11_SHADER_RESOURCE_VIEW_DESC SRV;
		ZeroMemory(&SRV, sizeof(SRV));
		SRV.Format = (DXGI_FORMAT)format;
		SRV.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE3D;
		SRV.Texture2D.MostDetailedMip = 0;
		SRV.Texture2D.MipLevels = 1;

		result = m_Device->CreateShaderResourceView(*out_Texture.GetTexture(), &SRV, out_Texture.GetShaderResourceView());

		if (S_OK(result))
		{
			return true;
		}

		return false;
	}

	bool GfxCore::CreateTexture3DFromFile(const char * pszFilePath1, const char * pszFilePath2, const char * pszFilePath3, const char * pszFilePath4, TextureCore3D & out_Texture, eBUFFER_USAGE usage, eBIND_FLAGS bind_flags, eCPU_ACCESS_FLAGS cpu_access_flags)
	{
		int Width, Height, OriginalNumComponents;
		UCHAR* Data1 = stbi_load(pszFilePath1, &Width, &Height, &OriginalNumComponents, 4);
		UCHAR* Data2 = stbi_load(pszFilePath2, &Width, &Height, &OriginalNumComponents, 4);
		UCHAR* Data3 = stbi_load(pszFilePath3, &Width, &Height, &OriginalNumComponents, 4);
		UCHAR* Data4 = stbi_load(pszFilePath4, &Width, &Height, &OriginalNumComponents, 4);

		UCHAR* Data = (UCHAR*)malloc(Width*Height*4*4);

		int32 Slice = Width*Height * 4;

		memcpy_s(Data, Slice, Data1, Slice);
		memcpy_s(Data + Slice, Slice, Data2, Slice);
		memcpy_s(Data + Slice*2, Slice, Data3, Slice);
		memcpy_s(Data + Slice*3, Slice, Data4, Slice);

		STBI_FREE(Data1);
		STBI_FREE(Data2);
		STBI_FREE(Data3);
		STBI_FREE(Data4);


		D3D11_SUBRESOURCE_DATA SubData;
		ZeroMemory(&SubData, sizeof(SubData));
		SubData.pSysMem = Data;
		SubData.SysMemPitch = Width * 4;
		SubData.SysMemSlicePitch = Slice;

		DXGI_SAMPLE_DESC SamplerDesc;
		ZeroMemory(&SamplerDesc, sizeof(SamplerDesc));
		SamplerDesc.Count = 1;
		SamplerDesc.Quality = 0;

		D3D11_TEXTURE3D_DESC Descriptor;
		ZeroMemory(&Descriptor, sizeof(Descriptor));
		Descriptor.BindFlags = bind_flags | D3D11_BIND_RENDER_TARGET;
		Descriptor.CPUAccessFlags = cpu_access_flags;
		Descriptor.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		Descriptor.Height = Height;
		Descriptor.Width = Width;
		Descriptor.Depth = 4;
		Descriptor.MipLevels = 0;
		Descriptor.Usage = (D3D11_USAGE)usage;
		Descriptor.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

		HRESULT result = m_Device->CreateTexture3D(&Descriptor, NULL, out_Texture.GetTexture());

		D3D11_BOX box;
		box.left = 0;
		box.right = Width;
		box.bottom = Height;
		box.top = 0;
		box.front = 0;
		box.back = 4;

		m_DeviceContext->UpdateSubresource(
			*out_Texture.GetTexture(),
			0,
			&box,
			Data,
			Width * 4,
			Slice
		);

		D3D11_SHADER_RESOURCE_VIEW_DESC SRV;
		ZeroMemory(&SRV, sizeof(SRV));
		SRV.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		SRV.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE3D;
		SRV.Texture2D.MipLevels = 0;

		result = m_Device->CreateShaderResourceView(*out_Texture.GetTexture(), NULL, out_Texture.GetShaderResourceView());

		STBI_FREE(Data);
		GenerateMipMaps(out_Texture);

		return true;
	}

	bool GfxCore::CreateCubeMapFrom6Files(char * front, char * back, char * left, char * right, char * top, char * bottom, TextureCore2D & out_Cubemap, eFORMAT format, eBUFFER_USAGE usage, eBIND_FLAGS bind_flags, eCPU_ACCESS_FLAGS cpu_access_flags)
	{
		int Width, Height, OriginalNumComponents;
		UCHAR* Data[6];
		Data[0] = stbi_load(front, &Width, &Height, &OriginalNumComponents, 4);
		Data[1] = stbi_load(back, &Width, &Height, &OriginalNumComponents, 4);
		Data[2] = stbi_load(left, &Width, &Height, &OriginalNumComponents, 4);
		Data[3] = stbi_load(right, &Width, &Height, &OriginalNumComponents, 4);
		Data[4] = stbi_load(top, &Width, &Height, &OriginalNumComponents, 4);
		Data[5] = stbi_load(bottom, &Width, &Height, &OriginalNumComponents, 4);

		D3D11_TEXTURE2D_DESC Descriptor;
		ZeroMemory(&Descriptor, sizeof(Descriptor));
		Descriptor.ArraySize = 6;
		Descriptor.BindFlags = bind_flags;
		Descriptor.CPUAccessFlags = cpu_access_flags;
		Descriptor.Format = (DXGI_FORMAT)format;
		Descriptor.Height = Height;
		Descriptor.Width = Width;
		Descriptor.MipLevels = 9;
		Descriptor.Usage = (D3D11_USAGE)usage;

		ID3D11Texture2D* textures[6];
		HRESULT result;

		for (int i = 0; i < 6; ++i)
		{
			D3D11_SUBRESOURCE_DATA SubData;
			ZeroMemory(&SubData, sizeof(SubData));
			SubData.pSysMem = Data[i];
			SubData.SysMemPitch = Width * 4;
			SubData.SysMemSlicePitch = 1;

			DXGI_SAMPLE_DESC SamplerDesc;
			ZeroMemory(&SamplerDesc, sizeof(SamplerDesc));
			SamplerDesc.Count = 1;
			SamplerDesc.Quality = 0;

			Descriptor.SampleDesc = SamplerDesc;

			result = m_Device->CreateTexture2D(&Descriptor, &SubData, &textures[i]);
		}

		Descriptor.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

		result = m_Device->CreateTexture2D(&Descriptor, NULL, textures);

		D3D11_SHADER_RESOURCE_VIEW_DESC SRV;
		ZeroMemory(&SRV, sizeof(SRV));
		SRV.Format = (DXGI_FORMAT)format;
		SRV.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
		SRV.Texture2D.MostDetailedMip = 0;
		SRV.Texture2D.MipLevels = 9;

		result = m_Device->CreateShaderResourceView(*textures, &SRV, out_Cubemap.GetShaderResourceView());

		if (S_OK(result))
		{
			return true;
		}

		return false;
	}

	bool GfxCore::CreateCubeMapFrom6MemoryPtr(void * front, void * back, void * left, void * right, void * top, void * bottom, TextureCore2D & out_Cubemap, eFORMAT format, eBUFFER_USAGE usage, eBIND_FLAGS bind_flags, eCPU_ACCESS_FLAGS cpu_access_flags)
	{
		return false;
	}

	bool GfxCore::CreateCubeMapFromDDS(char* pszFilePath, TextureCore2D& out_Cubemap, eFORMAT format, eBUFFER_USAGE usage, eBIND_FLAGS bind_flags, eCPU_ACCESS_FLAGS cpu_access_flags)
	{
		wchar_t  ws[100];
		swprintf(ws, 100, L"%hs", pszFilePath);

		HRESULT hr = DirectX::CreateDDSTextureFromFile(m_Device, m_DeviceContext, ws, (ID3D11Resource**)&out_Cubemap.m_Texture, &out_Cubemap.m_ShaderResource);
		
		if (FAILED(hr))
		{
			return false;
		}

		return true;
	}
	

	bool GfxCore::SetInputLayout(const InputLayout& iLayout)
	{
		if (iLayout.m_InputLayout == NULL)
		{
			return false;
		}

		m_DeviceContext->IASetInputLayout(iLayout.m_InputLayout);
		return true;
	}	

	void GfxCore::SetPrimitiveTopology(eTOPOLOGY usedTopology)
	{
		m_DeviceContext->IASetPrimitiveTopology(static_cast<D3D11_PRIMITIVE_TOPOLOGY>(usedTopology));
	}

	bool GfxCore::CreateSamplerState(SamplerState& out_samplerState)
	{
		D3D11_SAMPLER_DESC SamplerDesc;
		ZeroMemory(&SamplerDesc, sizeof(SamplerDesc));
		SamplerDesc.AddressU = (D3D11_TEXTURE_ADDRESS_MODE)out_samplerState.m_TexAddressU;
		SamplerDesc.AddressV = (D3D11_TEXTURE_ADDRESS_MODE)out_samplerState.m_TexAddressV;
		SamplerDesc.AddressW = (D3D11_TEXTURE_ADDRESS_MODE)out_samplerState.m_TexAddressW;
		SamplerDesc.ComparisonFunc = (D3D11_COMPARISON_FUNC)out_samplerState.m_ComparisonFunc;
		SamplerDesc.Filter = (D3D11_FILTER)out_samplerState.m_Filter;
		SamplerDesc.MaxAnisotropy = out_samplerState.m_MaxAnisotropy;
		SamplerDesc.MaxLOD = out_samplerState.m_MaxLOD;
		SamplerDesc.MinLOD = out_samplerState.m_MinLOD;
		SamplerDesc.MipLODBias = out_samplerState.m_MipLODBias;
		SamplerDesc.BorderColor[0] = out_samplerState.m_BorderColor.x;
		SamplerDesc.BorderColor[1] = out_samplerState.m_BorderColor.y;
		SamplerDesc.BorderColor[2] = out_samplerState.m_BorderColor.z;
		SamplerDesc.BorderColor[3] = out_samplerState.m_BorderColor.m_w;

		HRESULT hr = m_Device->CreateSamplerState(&SamplerDesc, &out_samplerState.m_SamplerState);

		return S_OK(hr);
	}

	bool GfxCore::CreateRasterizerState(RasterizerState & out_rasterizerState)
	{
		D3D11_RASTERIZER_DESC RasterDesc;
		ZeroMemory(&RasterDesc, sizeof(RasterDesc));
		RasterDesc.AntialiasedLineEnable = out_rasterizerState.m_AntialiasedLineEnable;
		RasterDesc.CullMode = (D3D11_CULL_MODE)out_rasterizerState.m_CullMode;
		RasterDesc.FillMode = (D3D11_FILL_MODE)out_rasterizerState.m_FillMode;
		RasterDesc.DepthBias = out_rasterizerState.m_DepthBias;
		RasterDesc.DepthBiasClamp = out_rasterizerState.m_DepthBiasClamp;
		RasterDesc.DepthClipEnable = out_rasterizerState.m_DepthClipEnable;
		RasterDesc.FrontCounterClockwise = out_rasterizerState.m_FrontCounterClockwise;
		RasterDesc.MultisampleEnable = out_rasterizerState.m_MultisampleEnable;
		RasterDesc.ScissorEnable = out_rasterizerState.m_ScissorEnable;
		RasterDesc.SlopeScaledDepthBias = out_rasterizerState.m_SlopeScaledDepthBias;
		
		HRESULT hr = m_Device->CreateRasterizerState(&RasterDesc, &out_rasterizerState.m_RasterizerState);

		return S_OK(hr);
	}

	bool GfxCore::CompileShader(Shader & out_ShaderObj, const UNICHAR * pszShaderFile, const char * version, const char * entryPoint)
	{
		ID3D10Blob* errorBuffer = 0;
		HRESULT result;

		String EntryPoint;
		String Version;
		Version = version;
		EntryPoint = entryPoint;

		result = D3DCompileFromFile(pszShaderFile, 0, 0, EntryPoint.c_str(), Version.c_str(), D3DCOMPILE_DEBUG, 0, &out_ShaderObj.m_ShaderBlob, &errorBuffer);

		if (FAILED(result))
		{
			if (errorBuffer != 0)
			{
				String Temp = ((char*)errorBuffer->GetBufferPointer());
				Temp;
				errorBuffer->Release();
			}
			return false;
		}

		return true;
	}

	bool GfxCore::CompileAndCreateVertexShader(VertexShader & out_VertexShader, const UNICHAR * pszShaderFile)
	{
		CompileShader(out_VertexShader, pszShaderFile, "vs_5_0", "VS_Main");

		return CreateVertexShader(out_VertexShader);;
	}

	bool GfxCore::CreateVertexShader(VertexShader& out_ShaderObj)
	{
		HRESULT result;

		result = m_Device->CreateVertexShader(out_ShaderObj.m_ShaderBlob->GetBufferPointer(), out_ShaderObj.m_ShaderBlob->GetBufferSize(), 0, &out_ShaderObj.m_VertexShader);

			if (FAILED(result))
			{
				if (out_ShaderObj.m_VertexShader)
					out_ShaderObj.m_VertexShader->Release();
				return false;
			}

		return true;
	}

	bool GfxCore::CompileAndCreatePixelShader(PixelShader & out_PixelShader, const UNICHAR * pszShaderFile)
	{
		CompileShader(out_PixelShader, pszShaderFile, "ps_5_0", "PS_Main");

		return CreatePixelShader(out_PixelShader);
	}

	bool GfxCore::CreatePixelShader(PixelShader& out_ShaderObj)
	{
		HRESULT result;

		result = m_Device->CreatePixelShader(out_ShaderObj.m_ShaderBlob->GetBufferPointer(), out_ShaderObj.m_ShaderBlob->GetBufferSize(), 0, &out_ShaderObj.m_PixelShader);

		if (FAILED(result))
			{
				if (out_ShaderObj.m_PixelShader)
					out_ShaderObj.m_PixelShader->Release();
				return false;
			}

		return true;
	}

	bool GfxCore::CompileAndCreateHullShader(HullShader & out_HullShader, const UNICHAR * pszShaderFile)
	{
		CompileShader(out_HullShader, pszShaderFile, "hs_5_0", "HS_Main");

		return CreateHullShader(out_HullShader);
	}

	bool GfxCore::CreateHullShader(HullShader & out_ShaderObj)
	{
		HRESULT result;

		result = m_Device->CreateHullShader(out_ShaderObj.m_ShaderBlob->GetBufferPointer(), out_ShaderObj.m_ShaderBlob->GetBufferSize(), 0, &out_ShaderObj.m_HullShader);

		if (FAILED(result))
		{
			if (out_ShaderObj.m_HullShader)
				out_ShaderObj.m_HullShader->Release();
			return false;
		}

		return true;
	}

	bool GfxCore::CompileAndCreateDomainShader(DomainShader & out_DomainShader, const UNICHAR * pszShaderFile)
	{
		CompileShader(out_DomainShader, pszShaderFile, "ds_5_0", "DS_Main");

		return CreateDomainShader(out_DomainShader);
	}

	bool GfxCore::CreateDomainShader(DomainShader & out_ShaderObj)
	{
		HRESULT result;

		result = m_Device->CreateDomainShader(out_ShaderObj.m_ShaderBlob->GetBufferPointer(), out_ShaderObj.m_ShaderBlob->GetBufferSize(), 0, &out_ShaderObj.m_DomainShader);

		if (FAILED(result))
		{
			if (out_ShaderObj.m_DomainShader)
				out_ShaderObj.m_DomainShader->Release();
			return false;
		}

		return true;
	}

	bool GfxCore::ReflectShader(InputLayout & out_InputLayout, Shader & shaderToReflect)
	{
		///Reflect the shader using its stored blob
		ID3D11ShaderReflection* Reflection = NULL;
		HRESULT result;
		result = D3DReflect(shaderToReflect.m_ShaderBlob->GetBufferPointer(), shaderToReflect.m_ShaderBlob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&Reflection);

		///Check for failure
		if (FAILED(result))
		{
			if (Reflection)
			{
				Reflection->Release();
			}

			return false;
		}

		D3D11_SHADER_DESC desc;
		Reflection->GetDesc(&desc);
		
		///Clear the input layout
		out_InputLayout.ClearElementList();

		for (UINT i = 0; i < desc.InputParameters; i++)
		{
			D3D11_SIGNATURE_PARAMETER_DESC d;
			Reflection->GetInputParameterDesc(i, &d);
			
			//out_InputLayout.AddElement((ANSICHAR*)d.SemanticName, d.SemanticIndex, );
		}

		return true;
	}

	bool GfxCore::CreateBuffer(BasicBuffer& out_Buffer, uint32 sizeOfElement, uint32 numOfElements, void* arrayOfElements, uint32 elementsPitch, uint32 elementsSlice, eBUFFER_USAGE usage, eBIND_FLAGS bindflags)
	{
		D3D11_BUFFER_DESC BufferDesc;
		ZeroMemory(&BufferDesc, sizeof(BufferDesc));
		BufferDesc.Usage = (D3D11_USAGE)usage;
		BufferDesc.BindFlags = bindflags;
		BufferDesc.ByteWidth = sizeOfElement*numOfElements;
		BufferDesc.CPUAccessFlags = usage == USAGE_DYNAMIC ? CPU_WRITE : 0;

		out_Buffer.m_Usage = usage;

		HRESULT hr;

		if (arrayOfElements != NULL)
		{
			D3D11_SUBRESOURCE_DATA BufferData;
			ZeroMemory(&BufferData, sizeof(BufferData));
			BufferData.pSysMem = arrayOfElements;
			BufferData.SysMemPitch = elementsPitch;
			BufferData.SysMemSlicePitch = elementsSlice;

			hr = m_Device->CreateBuffer(&BufferDesc, &BufferData, &out_Buffer.m_Buffer);
		}
		else
		{
			hr = m_Device->CreateBuffer(&BufferDesc, NULL, &out_Buffer.m_Buffer);
		}

		if (!FAILED(hr))
		{
			return true;
		}

		return false;
	};

}