#include <CbnUtilitiesPCH.h>
#include "RZDirectXPCH.h"
#include "..\include\rzgraphicsapi.h"
#define STB_IMAGE_IMPLEMENTATION 1
#include "../../Plug-Ins/stb-master/stb_image.h"

namespace rczEngine
{
	namespace Gfx
	{
		GfxCore*& GfxCore::_Instance()
		{
			static GfxCore* instance = nullptr;
			return instance;
		}

		void GfxCore::Start()
		{
			(_Instance()) = new GfxCore;
		}

		GfxCore* GfxCore::Pointer()
		{
			return _Instance();
		}

		void GfxCore::ShutDown()
		{
			delete _Instance();
		}

#pragma region =	| Init Functions |

		void GfxCore::InitAndStart(bool isWindowed)
		{
#ifdef LOGGING
			Logger::Pointer()->StartLog("Gfx");
#endif
			auto window = OSLayer::Pointer()->GetWindow();
			auto getRect = window.GetRect();

			CreateDevice(getRect.right + getRect.left, getRect.bottom + getRect.top, 2, 144, 1, 0, isWindowed);
			GetBackBufferInterface();
			SetRenderTargetViewAndDepthStencil();
			SetViewPortDefault();
			CreateDefaulDepthStencylState();
		}

		void GfxCore::Destroy()
		{
#ifdef LOGGING
			auto Log = Logger::Pointer();
			//Log->LogMessageToFileLog("Gfx", "Releasing BackBuffer, DeviceContext, Device and Swapchain");
			//Log->CloseLog("Gfx");
#endif

			if (m_BackBuffer) m_BackBuffer->Release();
			if (m_SwapChain) m_SwapChain->Release();
			if (m_DeviceContext) m_DeviceContext->Release();
			if (m_Device) m_Device->Release();
		}

		bool GfxCore::CreateDevice(int32 width, int32 height, int BufferCount = 1, int RefreshRate = 60, int SampleCount = 1, int Quality = 0, bool IsWindowed = true)
		{
#ifdef LOGGING
			Logger::Pointer()->LogMessageToFileLog("Gfx", "Creating Device, Context and Swapchain. DirectX11.");
#endif

			///Set the Swap Chain Descriptor
			DXGI_SWAP_CHAIN_DESC SwapChainDesc;

			SwapChainDesc.BufferCount = BufferCount;

			SwapChainDesc.BufferDesc.Width = m_ScreenWidth = width;
			SwapChainDesc.BufferDesc.Height = m_ScreenHeight = height;
			SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			SwapChainDesc.BufferDesc.RefreshRate.Numerator = RefreshRate;
			SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;

			SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			SwapChainDesc.OutputWindow = OSLayer::Pointer()->GetWindowHandle();
			SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;

			SwapChainDesc.SampleDesc.Count = SampleCount;
			SwapChainDesc.SampleDesc.Quality = Quality;

			SwapChainDesc.Windowed = IsWindowed;
			SwapChainDesc.Flags = 0;

			///Create the device and context. If succeful return true, else return false.
			return D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_HARDWARE, nullptr, D3D11_CREATE_DEVICE_DEBUG,
				nullptr, 0, D3D11_SDK_VERSION, &SwapChainDesc, &m_SwapChain, &m_Device, nullptr, &m_DeviceContext) == S_OK;
		}

		bool GfxCore::CreateDeviceContext()
		{
#ifdef LOGGING
			Logger::Pointer()->LogMessageToFileLog("Gfx", "Creating Device Context.");
#endif

			///Creates the DeviceContext
			return false;
		}

		bool GfxCore::CreateSwapChain()
		{
			///Create the swapChain
			return true;
		}

		void GfxCore::GetBackBufferInterface()
		{
			///Configure the Texture2D Descriptor
			D3D11_TEXTURE2D_DESC BackBufferDesc;
			BackBufferDesc.BindFlags = D3D11_BIND_RENDER_TARGET;
			BackBufferDesc.ArraySize = 1;
			BackBufferDesc.Height = m_ScreenHeight;
			BackBufferDesc.Width = m_ScreenWidth;
			BackBufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			BackBufferDesc.MipLevels = 1;
			BackBufferDesc.Usage = D3D11_USAGE_DEFAULT;
			BackBufferDesc.CPUAccessFlags = 0;
			BackBufferDesc.MiscFlags = 0;
			BackBufferDesc.SampleDesc.Count = 1;
			BackBufferDesc.SampleDesc.Quality = 0;

			///Create a Texture2D into BackBuffer
			m_Device->CreateTexture2D(&BackBufferDesc, nullptr, &m_BackBuffer);

			///Get the BackBuffer from the swapchain and into the Texture
			m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&m_BackBuffer);
		}

		bool GfxCore::SetRenderTargetViewAndDepthStencil()
		{
#ifdef LOGGING
			Logger::Pointer()->LogMessageToFileLog("Gfx", "Setting Render Target View and Depth Stencil.");
#endif

			///Create the RenderTargetView from the backbufffer
			m_Device->CreateRenderTargetView(m_BackBuffer, nullptr, &m_BaseRenderTargetView);

			CreateDepthStencyl(m_DepthStencyl);

			///Set the render target and the depth stecil
			m_DeviceContext->OMSetRenderTargets(1, &m_BaseRenderTargetView, m_DepthStencyl.GetDepthStencylView()->m_DepthStencylView);

			return true;
		}

		bool GfxCore::SetViewPortDefault()
		{
#ifdef LOGGING
			Logger::Pointer()->LogMessageToFileLog("Gfx", "Setting View Port.");
#endif

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

		bool GfxCore::SetViewPort(int32 width, int32 height)
		{
#ifdef LOGGING
			Logger::Pointer()->LogMessageToFileLog("Gfx", "Setting View Port.");
#endif

			///Create the viewport
			D3D11_VIEWPORT ViewPort;
			///Set its Width and height to the ones stored on this object.
			ViewPort.Width = (float)width;
			ViewPort.Height = (float)height;
			ViewPort.TopLeftX = 0;
			ViewPort.TopLeftY = 0;
			ViewPort.MaxDepth = 1.0f;
			ViewPort.MinDepth = 0.0f;
			///Set the viewport on the context.
			m_DeviceContext->RSSetViewports(1, &ViewPort);

			return true;
		}

		void GfxCore::Resize(uint32 Width, uint32 Height)
		{
#ifdef LOGGING
			Logger::Pointer()->LogMessageToFileLog("Gfx", "Resize.");
#endif

			if (m_SwapChain)
			{
				m_ScreenHeight = Height;
				m_ScreenWidth = Width;

				if (m_RenderTargetView[0]) { m_RenderTargetView[0]->Release(); }

				m_DeviceContext->ClearState();
				m_SwapChain->ResizeBuffers(2, Width, Height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
				///Get the BackBuffer from the swapchain and into the Texture
				GetBackBufferInterface();
				SetRenderTargetViewAndDepthStencil();
				SetViewPortDefault();

				return;
			}

		}

		void GfxCore::GetScreenSize(int32 & out_width, int32 & out_height)
		{
			out_width = m_ScreenWidth;
			out_height = m_ScreenHeight;
		}

#pragma endregion

#pragma region =	| Pipeline Functions |

		bool GfxCore::CreateDepthStencyl(DepthStencyl & out_depthStencyl, int32 width, int32 height, eBUFFER_USAGE usage, eCPU_ACCESS_FLAGS cpu_access, int32 sample_count, int32 sample_quality)
		{
#ifdef LOGGING
			Logger::Pointer()->LogMessageToFileLog("Gfx", "Creating Depth Stencyl.");
#endif

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
			TexDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;

			D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
			ZeroMemory(&descDSV, sizeof(descDSV));
			descDSV.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

			D3D11_SHADER_RESOURCE_VIEW_DESC  descSR;
			ZeroMemory(&descSR, sizeof(descSR));
			descSR.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
			descSR.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			descSR.Texture2D.MostDetailedMip = 0;
			descSR.Texture2D.MipLevels = -1;

			///Create the resource and save it in m_DepthStencil
			m_Device->CreateTexture2D(&TexDesc, nullptr, &out_depthStencyl.GetTexture()->m_Texture);

			///Create the Depth Stencil View and save it int m_DepthStencilView
			m_Device->CreateDepthStencilView(out_depthStencyl.GetTexture()->m_Texture, &descDSV, &out_depthStencyl.GetDepthStencylView()->m_DepthStencylView);

			///Create a Shader Resource View to bind this Depth Texture to a shader stage.
			m_Device->CreateShaderResourceView(out_depthStencyl.GetTexture()->m_Texture, &descSR, &out_depthStencyl.GetShaderResouceView()->m_ShaderResource);

			return true;
		}

		void GfxCore::CreateDefaulDepthStencylState()
		{
			D3D11_DEPTH_STENCIL_DESC dsDesc;

			// Depth test parameters
			dsDesc.DepthEnable = true;
			dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
			dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

			// Stencil test parameters
			dsDesc.StencilEnable = true;
			dsDesc.StencilReadMask = 0xFF;
			dsDesc.StencilWriteMask = 0xFF;

			// Stencil operations if pixel is front-facing
			dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
			dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
			dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
			dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

			// Stencil operations if pixel is back-facing
			dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
			dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
			dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
			dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

			// Create depth stencil state
			m_Device->CreateDepthStencilState(&dsDesc, &m_DepthStencylState);
		}

		void GfxCore::SetDefaulDepthStencylState()
		{
			m_DeviceContext->OMSetDepthStencilState(m_DepthStencylState, 0);
		}

		void GfxCore::SetPrimitiveTopology(eTOPOLOGY usedTopology)
		{
			m_DeviceContext->IASetPrimitiveTopology(static_cast<D3D11_PRIMITIVE_TOPOLOGY>(usedTopology));
		}

		void GfxCore::SetInputLayout(const InputLayout& iLayout)
		{
			m_DeviceContext->IASetInputLayout(iLayout.m_InputLayout);
		}

		bool GfxCore::CreateRenderTarget(RenderTarget & out_renderTarget, const String& name, bool drawToBackBuffer, int mipMaps, int32 width, int32 height, eFORMAT format, eBUFFER_USAGE usage, eCPU_ACCESS_FLAGS cpu_access, int32 sample_count, int32 sample_quality)
		{
#ifdef LOGGING
			Logger::Pointer()->LogMessageToFileLog("Gfx", String("Creating Render Target.") + name);
#endif

			///Configure the Texture2D Descriptor
			D3D11_TEXTURE2D_DESC TexDesc;
			TexDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
			TexDesc.ArraySize = 1;

			out_renderTarget.SetName(name);
			auto core = out_renderTarget.GetTextureCore();
			core->m_Height = height;
			core->m_Width = width;

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
			hr = m_Device->CreateTexture2D(&TexDesc, nullptr, &out_renderTarget.GetTextureCore()->m_Texture);

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

			hr = m_Device->CreateShaderResourceView(out_renderTarget.GetTextureCore()->m_Texture, &SRV, &out_renderTarget.GetTextureCore()->m_ShaderResource);

			if (hr != S_OK)
			{
				return false;
			}

			if (drawToBackBuffer)
			{
				SetTheRenderTargetToTheSwapChain(out_renderTarget);
			}

			///Create the RenderTargetView from the backbufffer
			hr = m_Device->CreateRenderTargetView(out_renderTarget.GetTextureCore()->m_Texture, nullptr, &out_renderTarget.GetRenderTargetView()->m_RenderTargetView);

			return S_OK(hr);
		}

		bool GfxCore::AddRenderTarget(RenderTarget & renderTarget, int32 renderTargetSlot)
		{
			m_RenderTargetView[renderTargetSlot] = renderTarget.GetRenderTargetView()->m_RenderTargetView;
			return true;
		}

		bool GfxCore::SetRenderTargets(bool UseDepthStencyl, DepthStencyl* depth)
		{


			///Set the render target and the depth stecil
			if (UseDepthStencyl && depth == nullptr)
				m_DeviceContext->OMSetRenderTargets(m_RenderTargetNumber, m_RenderTargetView, m_DepthStencyl.GetDepthStencylView()->m_DepthStencylView);
			else if (depth != nullptr && UseDepthStencyl)
				m_DeviceContext->OMSetRenderTargets(m_RenderTargetNumber, m_RenderTargetView, depth->GetDepthStencylView()->m_DepthStencylView);
			else
				m_DeviceContext->OMSetRenderTargets(m_RenderTargetNumber, m_RenderTargetView, NULL);

			return true;
		}

		bool GfxCore::SetDefaultRenderTarget()
		{
			m_DeviceContext->OMSetRenderTargets(1, &m_BaseRenderTargetView, m_DepthStencyl.GetDepthStencylView()->m_DepthStencylView);

			return true;
		}

		void GfxCore::UnbindRenderTargets()
		{
			ID3D11RenderTargetView *pSRV[1] = { nullptr };
			m_DeviceContext->OMSetRenderTargets(1, pSRV, nullptr);
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

		void GfxCore::SetIndexBuffer(IndexBuffer & IndexBuffer, uint32 bufferOffset)
		{
			m_DeviceContext->IASetIndexBuffer(IndexBuffer.m_Buffer, DXGI_FORMAT_R32_UINT, bufferOffset);
		}

		void GfxCore::UpdateSubResource(BasicBuffer * buffer, void * ptrToData, int32 pitch, int32 slice, int32 size)
		{
			if (buffer->m_Usage == USAGE_DEFAULT)
			{
				m_DeviceContext->UpdateSubresource(buffer->m_Buffer, 0, nullptr, ptrToData, pitch, slice);
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

		void GfxCore::UpdateSubResource(TextureCore2D * buffer, void * ptrToData, int32 pitch, int32 slice, int32 size)
		{
			m_DeviceContext->UpdateSubresource(buffer->m_Texture, 0, 0, ptrToData, pitch, slice);
		}

		void GfxCore::UpdateConstantBuffer(ConstantBuffer& cBuffer, void* srcData)
		{
			m_DeviceContext->UpdateSubresource(cBuffer.m_Buffer, 0, 0, srcData, 0, 0);
		}

#pragma endregion

#pragma region =	| Draw Functions |

		void GfxCore::ClearDefaultRenderTargetView(const float Red, const float Blue, const float Green, const float Alpha)
		{
			float Color[4];
			Color[0] = Red;
			Color[1] = Blue;
			Color[2] = Green;
			Color[3] = Alpha;

			m_DeviceContext->ClearRenderTargetView(m_BaseRenderTargetView, Color);
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

		void GfxCore::ClearDepthTargetView(DepthStencyl* depthStencyl)
		{
			if (depthStencyl)
				m_DeviceContext->ClearDepthStencilView(depthStencyl->GetDepthStencylView()->m_DepthStencylView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 255);
			else
				m_DeviceContext->ClearDepthStencilView(m_DepthStencyl.GetDepthStencylView()->m_DepthStencylView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 255);
		}

		DepthStencyl* GfxCore::GetDefaultDepthStencyl()
		{
			return &m_DepthStencyl;
		}

		void GfxCore::Present()
		{
			m_SwapChain->Present(0, 0);
		}

		bool GfxCore::Draw(size_t vertexCount, size_t vertex_start)
		{
			m_DeviceContext->Draw(vertexCount, vertex_start);
			return false;
		}

		bool GfxCore::DrawIndexed(size_t indexCount, size_t startIndex, size_t startVertex)
		{
			m_DeviceContext->DrawIndexed(indexCount, startIndex, startVertex);
			return false;
		}

#pragma endregion

#pragma region =	| Vertex Shader |

		bool GfxCore::CompileAndCreateVertexShader(VertexShader & out_VertexShader, const UNICHAR * pszShaderFile)
		{
#ifdef LOGGING
			Logger::Pointer()->LogMessageToFileLog("Gfx", String("Creating Vertex Shader: ") + TextTool::UniToAnsi(pszShaderFile));
#endif
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

		void GfxCore::SetVertexShader(VertexShader & vertexShader)
		{
			m_DeviceContext->VSSetShader(vertexShader.m_VertexShader, 0, 0);
		}

		void GfxCore::VSSetConstantBuffer(ConstantBuffer& cBuffer, uint32 startSlot, uint32 numBuffers)
		{
			m_DeviceContext->VSSetConstantBuffers(startSlot, numBuffers, &cBuffer.m_Buffer);
		}

		void GfxCore::SetVSShaderResource(uint32 startSlot, uint32 NumView, ShaderResource * shaderResource)
		{
			m_DeviceContext->VSSetShaderResources(startSlot, NumView, &shaderResource->m_ShaderResource);
		}

#pragma endregion

#pragma region =	| Pixel Shader |

		bool GfxCore::CompileAndCreatePixelShader(PixelShader & out_PixelShader, const UNICHAR * pszShaderFile)
		{
#ifdef LOGGING
			Logger::Pointer()->LogMessageToFileLog("Gfx", String(" ||| Creating Pixel Shader: ") + TextTool::UniToAnsi(pszShaderFile));
#endif

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

		void GfxCore::SetPixelShader(PixelShader & pixelShader)
		{
			m_DeviceContext->PSSetShader(pixelShader.m_PixelShader, 0, 0);
		}

		void GfxCore::PSSetConstantBuffer(ConstantBuffer & cBuffer, uint32 startSlot, uint32 numBuffers)
		{
			m_DeviceContext->PSSetConstantBuffers(startSlot, numBuffers, &cBuffer.m_Buffer);
		}

		void GfxCore::SetPSShaderResource(uint32 startSlot, uint32 NumView, ShaderResource* shaderResource)
		{
			m_DeviceContext->PSSetShaderResources(startSlot, NumView, &shaderResource->m_ShaderResource);
		}

		void GfxCore::UnbindPSShaderResource(int32 slot)
		{
			ID3D11ShaderResourceView *pSRV[1] = { nullptr };
			m_DeviceContext->PSSetShaderResources(slot, 1, pSRV);
		}

		void GfxCore::SetPSSamplers(const SamplerState& samplerState, uint32 startSlot, uint32 NumView)
		{
			m_DeviceContext->PSSetSamplers(startSlot, NumView, &samplerState.m_SamplerState);
		}

#pragma endregion

#pragma region =	| Hull Shader |

		bool GfxCore::CompileAndCreateHullShader(HullShader & out_HullShader, const UNICHAR * pszShaderFile)
		{
#ifdef LOGGING
			Logger::Pointer()->LogMessageToFileLog("Gfx", String("Creating Hull Shader: ") + TextTool::UniToAnsi(pszShaderFile));
#endif

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

		bool GfxCore::SetHullShader(HullShader & hullShader)
		{
			m_DeviceContext->HSSetShader(hullShader.m_HullShader, 0, 0);

			SetPrimitiveTopology(eTOPOLOGY::TOPO_3_CONTROL_POINT_PATCHLIST);

			return true;
		}

		bool GfxCore::HSSetConstantBuffer(ConstantBuffer & cBuffer, uint32 startSlot, uint32 numBuffers)
		{
			m_DeviceContext->HSSetConstantBuffers(startSlot, numBuffers, &cBuffer.m_Buffer);
			return true;
		}

		void GfxCore::RemoveHullShader()
		{
			m_DeviceContext->HSSetShader(nullptr, 0, 0);

			SetPrimitiveTopology(eTOPOLOGY::TOPO_TRIANGLELIST);
		}

		void GfxCore::SetHSShaderResource(uint32 startSlot, uint32 NumView, ShaderResource* shaderResource)
		{
			m_DeviceContext->HSSetShaderResources(startSlot, NumView, &shaderResource->m_ShaderResource);
		}

		void GfxCore::UnbindHSShaderResource(int32 slot)
		{
			ID3D11ShaderResourceView *pSRV[1] = { nullptr };
			m_DeviceContext->HSSetShaderResources(slot, 1, pSRV);
		}

		void GfxCore::SetHSSamplers(const SamplerState & samplerState, uint32 startSlot, uint32 NumView)
		{
			m_DeviceContext->HSSetSamplers(startSlot, NumView, &samplerState.m_SamplerState);
		}

#pragma endregion

#pragma region =	| Domain Shader |

		bool GfxCore::CompileAndCreateDomainShader(DomainShader & out_DomainShader, const UNICHAR * pszShaderFile)
		{
#ifdef LOGGING
			Logger::Pointer()->LogMessageToFileLog("Gfx", String("Creating Domain Shader: ") + TextTool::UniToAnsi(pszShaderFile));
#endif

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

		void GfxCore::RemoveDomainShader()
		{
			m_DeviceContext->DSSetShader(nullptr, 0, 0);
		}

		void GfxCore::SetDSShaderResource(uint32 startSlot, uint32 NumView, ShaderResource* shaderResource)
		{
			m_DeviceContext->DSSetShaderResources(startSlot, NumView, &shaderResource->m_ShaderResource);
		}

		void GfxCore::UnbindDSShaderResource(int32 slot)
		{
			ID3D11ShaderResourceView *pSRV[1] = { nullptr };
			m_DeviceContext->DSSetShaderResources(slot, 1, pSRV);
		}

		void GfxCore::SetDSSamplers(const SamplerState & samplerState, uint32 startSlot, uint32 NumView)
		{
			m_DeviceContext->DSSetSamplers(startSlot, NumView, &samplerState.m_SamplerState);
		}
#pragma endregion

#pragma region =	| Geometry Shader |

		bool GfxCore::CompileAndCreateGeometryShader(GeometryShader & out_GeometryShader, const UNICHAR * pszShaderFile)
		{
#ifdef LOGGING
			Logger::Pointer()->LogMessageToFileLog("Gfx", String("Geometry Vertex Shader: ") + TextTool::UniToAnsi(pszShaderFile));
#endif

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

		void GfxCore::ClearGeometryShader()
		{
			m_DeviceContext->GSSetShader(nullptr, 0, 0);
		}

#pragma endregion

#pragma region =	| Rasterizer State |

		bool GfxCore::CreateRasterizerState(RasterizerState & out_rasterizerState)
		{
#ifdef LOGGING
			Logger::Pointer()->LogMessageToFileLog("Gfx", String("Creating Rasterizer State: "));
#endif

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

		bool GfxCore::SetRSState(const RasterizerState & rasterizerState)
		{
			m_DeviceContext->RSSetState(rasterizerState.m_RasterizerState);
			return true;
		}

		void GfxCore::SetRSStateDefault()
		{
			m_DefaultRS.SetThisRasterizerState(this);
		}

#pragma endregion

#pragma region =	| Blend State |


		bool GfxCore::CreateBlendState(BlendState & blendState)
		{
#ifdef LOGGING
			Logger::Pointer()->LogMessageToFileLog("Gfx", "Creating Blend State");
#endif

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
			m_DeviceContext->OMSetBlendState(blendState.m_BlendState, nullptr, 0xffffffff);
			return true;
		}

		void GfxCore::SetBlendStateDefault()
		{
			m_DeviceContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);
		}

#pragma endregion

#pragma region =	| Object Creation |

		bool GfxCore::CreateSamplerState(SamplerState& out_samplerState)
		{
#ifdef LOGGING
			Logger::Pointer()->LogMessageToFileLog("Gfx", String("Creating Sample State: "));
#endif

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
			SamplerDesc.BorderColor[0] = out_samplerState.m_BorderColor.m_x;
			SamplerDesc.BorderColor[1] = out_samplerState.m_BorderColor.m_y;
			SamplerDesc.BorderColor[2] = out_samplerState.m_BorderColor.m_z;
			SamplerDesc.BorderColor[3] = out_samplerState.m_BorderColor.m_w;

			HRESULT hr = m_Device->CreateSamplerState(&SamplerDesc, &out_samplerState.m_SamplerState);

			return S_OK(hr);
		}

		bool GfxCore::CompileShader(Shader & out_ShaderObj, const UNICHAR * pszShaderFile, const String& version, const String& entryPoint)
		{
			ID3D10Blob* errorBuffer = 0;
			HRESULT result;

			String EntryPoint;
			String Version;
			Version = version;
			EntryPoint = entryPoint;
			out_ShaderObj.ShaderPath.ResetPath(TextTool::UniToAnsi(pszShaderFile));

			result = D3DCompileFromFile(pszShaderFile, 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, EntryPoint.c_str(), Version.c_str(), D3DCOMPILE_DEBUG, 0, &out_ShaderObj.m_ShaderBlob, &errorBuffer);

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

		bool GfxCore::ReflectShader(InputLayout & out_InputLayout, Shader & shaderToReflect)
		{
			///Reflect the shader using its stored blob
			ID3D11ShaderReflection* Reflection = nullptr;
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
			BufferDesc.ByteWidth = sizeOfElement * numOfElements;

			if (usage == USAGE_DYNAMIC)
			{
				BufferDesc.CPUAccessFlags = CPU_WRITE;
			}
			else if (usage == USAGE_STAGING)
			{
				BufferDesc.CPUAccessFlags = CPU_READ | CPU_WRITE;
			}
			else
			{
				BufferDesc.CPUAccessFlags = 0;
			}

			out_Buffer.m_Usage = usage;

			HRESULT hr;

			if (arrayOfElements != nullptr)
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
				hr = m_Device->CreateBuffer(&BufferDesc, nullptr, &out_Buffer.m_Buffer);
			}

			if (!FAILED(hr))
			{
				return true;
			}

			return false;
		};

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

		void * GfxCore::GetBufferData(BasicBuffer & buffer)
		{
			//Create the new Staging buffer.
			Gfx::BasicBuffer tempBuffer;
			tempBuffer.Create(buffer.m_SizeOfElement, buffer.m_NumOfElements, nullptr,
				buffer.m_ElementsPitch, buffer.m_ElementSlice, USAGE_STAGING, (eBIND_FLAGS)0, this);

			//Copy the resource.
			m_DeviceContext->CopyResource(tempBuffer.m_Buffer, buffer.m_Buffer);

			//Allocate the memory.
			uint32 size = buffer.m_SizeOfElement * buffer.m_NumOfElements;
			void* ptr = malloc(size);

			//Create the mappedResource
			D3D11_MAPPED_SUBRESOURCE mappedResource;
			ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

			//Map the tempBuffer resource to the mapped Resource.
			m_DeviceContext->Map(tempBuffer.m_Buffer, 0, D3D11_MAP_READ, 0, &mappedResource);
			//Copy the buffer's data to the pointer.
			memcpy(ptr, mappedResource.pData, size);
			//	Reenable GPU access to the vertex buffer data.
			m_DeviceContext->Unmap(tempBuffer.m_Buffer, 0);

			tempBuffer.Destroy();

			return ptr;
		}

		void * GfxCore::GetTexture2DData(TextureCore2D & texture)
		{
			TextureCore2D tempCore;

			D3D11_SUBRESOURCE_DATA SubData;
			ZeroMemory(&SubData, sizeof(SubData));
			SubData.pSysMem = nullptr;
			SubData.SysMemPitch = texture.m_TextureBytePitch;
			SubData.SysMemSlicePitch = 1;

			DXGI_SAMPLE_DESC SamplerDesc;
			ZeroMemory(&SamplerDesc, sizeof(SamplerDesc));
			SamplerDesc.Count = 1;
			SamplerDesc.Quality = 0;

			D3D11_TEXTURE2D_DESC Descriptor;
			ZeroMemory(&Descriptor, sizeof(Descriptor));
			Descriptor.ArraySize = 1;
			Descriptor.BindFlags = 0;
			Descriptor.CPUAccessFlags = CPU_READ;
			Descriptor.Format = (DXGI_FORMAT)texture.m_Format;
			Descriptor.Height = texture.m_Height;
			Descriptor.Width = texture.m_Width;
			Descriptor.MipLevels = 0;
			Descriptor.Usage = (D3D11_USAGE)USAGE_STAGING;
			Descriptor.SampleDesc = SamplerDesc;
			Descriptor.MiscFlags = 0;

			HRESULT result = m_Device->CreateTexture2D(&Descriptor, nullptr, &tempCore.m_Texture);

			//Copy the resource.
			m_DeviceContext->CopyResource(tempCore.m_Texture, texture.m_Texture);

			//Allocate the memory.
			uint32 size = texture.m_TextureBytes;
			void* ptr = malloc(size);

			//Create the mappedResource
			D3D11_MAPPED_SUBRESOURCE mappedResource;
			ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

			//Map the tempBuffer resource to the mapped Resource.
			m_DeviceContext->Map(tempCore.m_Texture, 0, D3D11_MAP_READ, 0, &mappedResource);
			//Copy the buffer's data to the pointer.
			memcpy(ptr, mappedResource.pData, size);
			//	Reenable GPU access to the vertex buffer data.
			m_DeviceContext->Unmap(tempCore.m_Texture, 0);

			tempCore.m_Texture->Release();

			return ptr;
		}

#pragma endregion

#pragma region =	| Texture Functions |

		bool GfxCore::CreateTextureFromFile(const String& pszFilePath, TextureCore2D & out_Texture, eBUFFER_USAGE usage, eBIND_FLAGS bind_flags, eCPU_ACCESS_FLAGS cpu_access_flags)
		{
			// Basic usage (see HDR discussion below for HDR usage):
			//    int x,y,n;
			//    unsigned char *data = stbi_load(filename, &x, &y, &n, 0);
			//    // ... process data if not nullptr ...
			//    // ... x = width, y = height, n = # 8-bit components per pixel ...
			//    // ... replace '0' with '1'..'4' to force that many components per pixel
			//    // ... but 'n' will always be the number that it would have been if you said 0
			//    stbi_image_free(data)
			int Width, Height, OriginalNumComponents;
			UCHAR* Data = stbi_load(pszFilePath.c_str(), &Width, &Height, &OriginalNumComponents, 4);

			if (Data == nullptr)
			{
				return false;
			}

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

			out_Texture.m_TextureBytePitch = SubData.SysMemPitch;
			out_Texture.m_TextureBytes = SubData.SysMemPitch*Height;
			out_Texture.m_Format = FORMAT_R8G8B8A8_UNORM;
			out_Texture.m_Height = Height;
			out_Texture.m_Width = Width;
			out_Texture.m_BindFlags = (Gfx::eBIND_FLAGS)Descriptor.BindFlags;
			out_Texture.m_AccessFlags = cpu_access_flags;

			HRESULT result = m_Device->CreateTexture2D(&Descriptor, nullptr, &out_Texture.m_Texture);

			D3D11_BOX box;
			box.left = 0;
			box.right = Width;
			box.bottom = Height;
			box.top = 0;
			box.front = 0;
			box.back = 1;

			m_DeviceContext->UpdateSubresource(
				out_Texture.m_Texture,
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

			result = m_Device->CreateShaderResourceView(out_Texture.m_Texture, nullptr, &out_Texture.m_ShaderResource);
			STBI_FREE(Data);
			GenerateMipMaps(&out_Texture);

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

			HRESULT result = m_Device->CreateTexture2D(&Descriptor, &SubData, &out_Texture.m_Texture);

			D3D11_SHADER_RESOURCE_VIEW_DESC SRV;
			ZeroMemory(&SRV, sizeof(SRV));
			SRV.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			SRV.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			SRV.Texture2D.MostDetailedMip = 0;
			SRV.Texture2D.MipLevels = 1;

			result = m_Device->CreateShaderResourceView(out_Texture.m_Texture, &SRV, &out_Texture.m_ShaderResource);

			return (S_OK(result));
		}

		bool GfxCore::CreateEmptyTexture(TextureCore2D & out_Texture, int width, int height, eBIND_FLAGS bind_flags, eFORMAT format, eFORMAT SRVformat, int MipLevels, eBUFFER_USAGE usage, eCPU_ACCESS_FLAGS cpu_access_flags)
		{
			DXGI_SAMPLE_DESC SamplerDesc;
			ZeroMemory(&SamplerDesc, sizeof(SamplerDesc));
			SamplerDesc.Count = 1;
			SamplerDesc.Quality = 0;

			D3D11_TEXTURE2D_DESC Descriptor;
			ZeroMemory(&Descriptor, sizeof(Descriptor));
			Descriptor.ArraySize = 1;
			Descriptor.BindFlags = bind_flags | D3D11_BIND_RENDER_TARGET;
			Descriptor.CPUAccessFlags = cpu_access_flags;
			Descriptor.Format = (DXGI_FORMAT)format;
			Descriptor.Height = height;
			Descriptor.Width = width;
			Descriptor.MipLevels = MipLevels;
			Descriptor.Usage = (D3D11_USAGE)usage;
			Descriptor.SampleDesc = SamplerDesc;
			Descriptor.MiscFlags = (usage != USAGE_STAGING) ? D3D11_RESOURCE_MISC_GENERATE_MIPS : 0;

			out_Texture.m_TextureBytePitch = 0;
			out_Texture.m_TextureBytes = 1 * height;
			out_Texture.m_Format = format;
			out_Texture.m_Height = height;
			out_Texture.m_Width = width;
			out_Texture.m_BindFlags = (Gfx::eBIND_FLAGS)Descriptor.BindFlags;
			out_Texture.m_AccessFlags = cpu_access_flags;

			HRESULT result = m_Device->CreateTexture2D(&Descriptor, nullptr, &out_Texture.m_Texture);
			D3D11_SHADER_RESOURCE_VIEW_DESC SRV;
			ZeroMemory(&SRV, sizeof(SRV));
			SRV.Format = (DXGI_FORMAT)SRVformat;
			SRV.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			SRV.Texture2D.MipLevels = MipLevels;

			result = m_Device->CreateShaderResourceView(out_Texture.m_Texture, nullptr, &out_Texture.m_ShaderResource);

			if (MipLevels != 1)
				GenerateMipMaps(&out_Texture);

			return (S_OK(result));
		}

		bool GfxCore::CreateTexture(void* memory, int32 pitch, int32 slice, TextureCore2D & out_Texture, int width, int height, eBIND_FLAGS bind_flags, eFORMAT format, int MipLevels, eBUFFER_USAGE usage, eCPU_ACCESS_FLAGS cpu_access_flags)
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
			Descriptor.BindFlags = bind_flags | D3D11_BIND_RENDER_TARGET;
			Descriptor.CPUAccessFlags = cpu_access_flags;
			Descriptor.Format = (DXGI_FORMAT)format;
			Descriptor.Height = height;
			Descriptor.Width = width;
			Descriptor.MipLevels = MipLevels;
			Descriptor.Usage = (D3D11_USAGE)usage;
			Descriptor.SampleDesc = SamplerDesc;
			Descriptor.MiscFlags = (usage != USAGE_STAGING) ? D3D11_RESOURCE_MISC_GENERATE_MIPS : 0;

			out_Texture.m_TextureBytePitch = SubData.SysMemPitch;
			out_Texture.m_TextureBytes = SubData.SysMemPitch*height;
			out_Texture.m_Format = FORMAT_R8G8B8A8_UNORM;
			out_Texture.m_Height = height;
			out_Texture.m_Width = width;
			out_Texture.m_BindFlags = (Gfx::eBIND_FLAGS)Descriptor.BindFlags;
			out_Texture.m_AccessFlags = cpu_access_flags;

			HRESULT result = m_Device->CreateTexture2D(&Descriptor, nullptr, &out_Texture.m_Texture);

			D3D11_BOX box;
			box.left = 0;
			box.right = width;
			box.bottom = height;
			box.top = 0;
			box.front = 0;
			box.back = 1;

			if (memory)
			{
				m_DeviceContext->UpdateSubresource(
					out_Texture.m_Texture,
					0,
					&box,
					memory,
					width * 4,
					0
				);
			}

			D3D11_SHADER_RESOURCE_VIEW_DESC SRV;
			ZeroMemory(&SRV, sizeof(SRV));
			SRV.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			SRV.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			SRV.Texture2D.MipLevels = MipLevels;

			result = m_Device->CreateShaderResourceView(out_Texture.m_Texture, nullptr, &out_Texture.m_ShaderResource);
			GenerateMipMaps(&out_Texture);

			return (S_OK(result));
		}

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

			HRESULT result = m_Device->CreateTexture3D(&Descriptor, &SubData, &out_Texture.m_Texture);

			D3D11_SHADER_RESOURCE_VIEW_DESC SRV;
			ZeroMemory(&SRV, sizeof(SRV));
			SRV.Format = (DXGI_FORMAT)format;
			SRV.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE3D;
			SRV.Texture2D.MostDetailedMip = 0;
			SRV.Texture2D.MipLevels = 1;

			result = m_Device->CreateShaderResourceView(out_Texture.m_Texture, &SRV, &out_Texture.m_ShaderResource);

			return S_OK(result);
		}

		bool GfxCore::CreateTexture3DFromFile(const String& pszFilePath1, const String& pszFilePath2, const String& pszFilePath3, const String& pszFilePath4, TextureCore3D & out_Texture, eBUFFER_USAGE usage, eBIND_FLAGS bind_flags, eCPU_ACCESS_FLAGS cpu_access_flags)
		{
			int Width, Height, OriginalNumComponents;
			UCHAR* Data1 = stbi_load(pszFilePath1.c_str(), &Width, &Height, &OriginalNumComponents, 4);
			UCHAR* Data2 = stbi_load(pszFilePath2.c_str(), &Width, &Height, &OriginalNumComponents, 4);
			UCHAR* Data3 = stbi_load(pszFilePath3.c_str(), &Width, &Height, &OriginalNumComponents, 4);
			UCHAR* Data4 = stbi_load(pszFilePath4.c_str(), &Width, &Height, &OriginalNumComponents, 4);

			UCHAR* Data = (UCHAR*)malloc(Width*Height * 4 * 4);

			int32 Slice = Width * Height * 4;

			memcpy_s(Data, Slice, Data1, Slice);
			memcpy_s(Data + Slice, Slice, Data2, Slice);
			memcpy_s(Data + Slice * 2, Slice, Data3, Slice);
			memcpy_s(Data + Slice * 3, Slice, Data4, Slice);

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

			HRESULT result = m_Device->CreateTexture3D(&Descriptor, nullptr, &out_Texture.m_Texture);

			D3D11_BOX box;
			box.left = 0;
			box.right = Width;
			box.bottom = Height;
			box.top = 0;
			box.front = 0;
			box.back = 4;

			m_DeviceContext->UpdateSubresource(
				out_Texture.m_Texture,
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

			result = m_Device->CreateShaderResourceView(out_Texture.m_Texture, nullptr, &out_Texture.m_ShaderResource);

			STBI_FREE(Data);
			GenerateMipMaps(&out_Texture);

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

			result = m_Device->CreateTexture2D(&Descriptor, nullptr, textures);

			D3D11_SHADER_RESOURCE_VIEW_DESC SRV;
			ZeroMemory(&SRV, sizeof(SRV));
			SRV.Format = (DXGI_FORMAT)format;
			SRV.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
			SRV.Texture2D.MostDetailedMip = 0;
			SRV.Texture2D.MipLevels = 9;

			result = m_Device->CreateShaderResourceView(*textures, &SRV, &out_Cubemap.m_ShaderResource);

			if (S_OK(result))
			{
				return true;
			}

			return false;
		}

		bool GfxCore::CreateCubeMapFrom6Tex2DCore(TextureCore2D & front, TextureCore2D & back, TextureCore2D & left, TextureCore2D & right, TextureCore2D & top, TextureCore2D & bottom, TextureCore2D & out_Cubemap, eFORMAT format, eBUFFER_USAGE usage, eBIND_FLAGS bind_flags, eCPU_ACCESS_FLAGS cpu_access_flags)
		{
			int Width, Height;
			Width = front.m_Width;
			Height = front.m_Height;

			D3D11_TEXTURE2D_DESC Descriptor;
			ZeroMemory(&Descriptor, sizeof(Descriptor));
			Descriptor.ArraySize = 6;
			Descriptor.BindFlags = bind_flags | D3D11_BIND_RENDER_TARGET;
			Descriptor.CPUAccessFlags = cpu_access_flags;
			Descriptor.Format = (DXGI_FORMAT)format;
			Descriptor.Height = Height;
			Descriptor.Width = Width;
			Descriptor.MipLevels = 1;
			Descriptor.Usage = (D3D11_USAGE)usage;

			Descriptor.SampleDesc.Count = 1;
			Descriptor.SampleDesc.Quality = 0;

			ID3D11Texture2D* textures[6];

			textures[1] = front.m_Texture;
			textures[0] = back.m_Texture;
			textures[4] = left.m_Texture;
			textures[5] = right.m_Texture;
			textures[2] = top.m_Texture;
			textures[3] = bottom.m_Texture;

			HRESULT result;

			Descriptor.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

			result = m_Device->CreateTexture2D(&Descriptor, nullptr, &out_Cubemap.m_Texture);

			D3D11_BOX box;
			box.left = 0;
			box.right = Width;
			box.top = 0;
			box.bottom = Height;
			box.front = 0;
			box.back = 1;

			m_DeviceContext->CopySubresourceRegion(out_Cubemap.m_Texture, 0, 0, 0, 0, textures[0], 0, &box);
			m_DeviceContext->CopySubresourceRegion(out_Cubemap.m_Texture, 1, 0, 0, 0, textures[1], 0, &box);
			m_DeviceContext->CopySubresourceRegion(out_Cubemap.m_Texture, 2, 0, 0, 0, textures[2], 0, &box);
			m_DeviceContext->CopySubresourceRegion(out_Cubemap.m_Texture, 3, 0, 0, 0, textures[3], 0, &box);
			m_DeviceContext->CopySubresourceRegion(out_Cubemap.m_Texture, 4, 0, 0, 0, textures[4], 0, &box);
			m_DeviceContext->CopySubresourceRegion(out_Cubemap.m_Texture, 5, 0, 0, 0, textures[5], 0, &box);

			D3D11_SHADER_RESOURCE_VIEW_DESC SRV;
			ZeroMemory(&SRV, sizeof(SRV));
			SRV.Format = (DXGI_FORMAT)format;
			SRV.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
			SRV.TextureCube.MostDetailedMip = 0;
			SRV.TextureCube.MipLevels = 1;

			result = m_Device->CreateShaderResourceView(out_Cubemap.m_Texture, &SRV, &out_Cubemap.m_ShaderResource);

			if (S_OK(result))
			{
				return true;
			}

			return false;
		}

		bool GfxCore::CreateCubeMapFrom6MemoryPtr(void * front, void * back, void * left, void * right, void * top, void * bottom, TextureCore2D & out_Cubemap, eFORMAT format, eBUFFER_USAGE usage, eBIND_FLAGS bind_flags, eCPU_ACCESS_FLAGS cpu_access_flags)
		{
			//TODO
			return false;
		}

		bool GfxCore::CreateCubeMapFromDDS(const String& pszFilePath, TextureCore2D& out_Cubemap)
		{
			StringW str = TextTool::AnsiToUni(pszFilePath.c_str());

			HRESULT hr = DirectX::CreateDDSTextureFromFile(m_Device, m_DeviceContext, str.c_str(), (ID3D11Resource**)&out_Cubemap.m_Texture, &out_Cubemap.m_ShaderResource);

			if (FAILED(hr))
			{
				return false;
			}

			return true;
		}

		void GfxCore::GenerateMipMaps(ShaderResource* texCore)
		{
			m_DeviceContext->GenerateMips(texCore->m_ShaderResource);
		}

#pragma endregion

	}
}