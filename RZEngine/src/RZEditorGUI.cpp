#include <RZEnginePCH.h>

namespace rczEngine
{
#pragma region Static Functions

	GUIEditor*& GUIEditor::_Instance()
	{
		static GUIEditor* instance = nullptr;
		return instance;
	}

	void GUIEditor::Start()
	{
		(_Instance()) = new GUIEditor;
	}

	GUIEditor* GUIEditor::Pointer()
	{
		return _Instance();
	}

	void GUIEditor::ShutDown()
	{
		delete _Instance();
	}

	bool IsAnyMouseButtonDown()
	{
		ImGuiIO& io = ImGui::GetIO();
		for (int n = 0; n < ARRAYSIZE(io.MouseDown); n++)
			if (io.MouseDown[n])
				return true;
		return false;
	}

	IMGUI_API LRESULT ImguiWndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		ImGuiIO& io = ImGui::GetIO();
		switch (msg)
		{
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_MBUTTONDOWN:
		{
			int button = 0;
			if (msg == WM_LBUTTONDOWN) button = 0;
			if (msg == WM_RBUTTONDOWN) button = 1;
			if (msg == WM_MBUTTONDOWN) button = 2;
			if (!IsAnyMouseButtonDown() && GetCapture() == NULL)
				SetCapture(hwnd);
			io.MouseDown[button] = true;
			return 0;
		}
		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
		case WM_MBUTTONUP:
		{
			int button = 0;
			if (msg == WM_LBUTTONUP) button = 0;
			if (msg == WM_RBUTTONUP) button = 1;
			if (msg == WM_MBUTTONUP) button = 2;
			io.MouseDown[button] = false;
			if (!IsAnyMouseButtonDown() && GetCapture() == hwnd)
				ReleaseCapture();
			return 0;
		}
		case WM_MOUSEWHEEL:
			io.MouseWheel += GET_WHEEL_DELTA_WPARAM(wParam) > 0 ? +1.0f : -1.0f;
			return 0;
		case WM_MOUSEMOVE:
			io.MousePos.x = (signed short)(lParam);
			io.MousePos.y = (signed short)(lParam >> 16);
			return 0;
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			if (wParam < 256)
				io.KeysDown[wParam] = 1;
			return 0;
		case WM_KEYUP:
		case WM_SYSKEYUP:
			if (wParam < 256)
				io.KeysDown[wParam] = 0;
			return 0;
		case WM_CHAR:
			// You can also use ToAscii()+GetKeyboardState() to retrieve characters.
			if (wParam > 0 && wParam < 0x10000)
				io.AddInputCharacter((unsigned short)wParam);
			return 0;
		}
		return 0;
	}

#pragma endregion

	void GUIEditor::InitEditor(PlatformData* platformData)
	{
		m_gfx = Gfx::GfxCore::Pointer();

		m_Hwnd = platformData->WindowHandle;

		if (!QueryPerformanceFrequency((LARGE_INTEGER *)&m_TicksPerSecond))
			return;
		if (!QueryPerformanceCounter((LARGE_INTEGER *)&m_Time))
			return;

		ImGuiIO& io = ImGui::GetIO();
		io.KeyMap[ImGuiKey_Tab] = VK_TAB;                       // Keyboard mapping. ImGui will use those indices to peek into the io.KeyDown[] array that we will update during the application lifetime.
		io.KeyMap[ImGuiKey_LeftArrow] = VK_LEFT;
		io.KeyMap[ImGuiKey_RightArrow] = VK_RIGHT;
		io.KeyMap[ImGuiKey_UpArrow] = VK_UP;
		io.KeyMap[ImGuiKey_DownArrow] = VK_DOWN;
		io.KeyMap[ImGuiKey_PageUp] = VK_PRIOR;
		io.KeyMap[ImGuiKey_PageDown] = VK_NEXT;
		io.KeyMap[ImGuiKey_Home] = VK_HOME;
		io.KeyMap[ImGuiKey_End] = VK_END;
		io.KeyMap[ImGuiKey_Delete] = VK_DELETE;
		io.KeyMap[ImGuiKey_Backspace] = VK_BACK;
		io.KeyMap[ImGuiKey_Enter] = VK_RETURN;
		io.KeyMap[ImGuiKey_Escape] = VK_ESCAPE;
		io.KeyMap[ImGuiKey_A] = 'A';
		io.KeyMap[ImGuiKey_C] = 'C';
		io.KeyMap[ImGuiKey_V] = 'V';
		io.KeyMap[ImGuiKey_X] = 'X';
		io.KeyMap[ImGuiKey_Y] = 'Y';
		io.KeyMap[ImGuiKey_Z] = 'Z';

		io.RenderDrawListsFn = NULL;  // Alternatively you can set this to NULL and call ImGui::GetDrawData() after ImGui::Render() to get the same ImDrawData pointer.
		io.ImeWindowHandle = m_Hwnd;

		m_SceneGraphGUI.InitSceneGraphGUI();

		return;
	}

	void GUIEditor::PreRender(ImDrawData * draw_data)
	{
		static int g_VertexBufferSize;
		static int g_IndexBufferSize;

		// Create and grow vertex/index buffers if needed
		if (!g_pVB || g_VertexBufferSize < draw_data->TotalVtxCount)
		{
			if (g_pVB) { g_pVB->Release(); g_pVB = NULL; }
			g_VertexBufferSize = draw_data->TotalVtxCount + 5000;
			D3D11_BUFFER_DESC desc;
			memset(&desc, 0, sizeof(D3D11_BUFFER_DESC));
			desc.Usage = D3D11_USAGE_DYNAMIC;
			desc.ByteWidth = g_VertexBufferSize * sizeof(ImDrawVert);
			desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			desc.MiscFlags = 0;
			if (m_gfx->m_Device->CreateBuffer(&desc, NULL, &g_pVB) < 0)
				return;
		}

		if (!g_pIB || g_IndexBufferSize < draw_data->TotalIdxCount)
		{
			if (g_pIB) { g_pIB->Release(); g_pIB = NULL; }
			g_IndexBufferSize = draw_data->TotalIdxCount + 10000;
			D3D11_BUFFER_DESC desc;
			memset(&desc, 0, sizeof(D3D11_BUFFER_DESC));
			desc.Usage = D3D11_USAGE_DYNAMIC;
			desc.ByteWidth = g_IndexBufferSize * sizeof(ImDrawIdx);
			desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			if (m_gfx->m_Device->CreateBuffer(&desc, NULL, &g_pIB) < 0)
				return;
		}

		// Copy and convert all vertices into a single contiguous buffer
		D3D11_MAPPED_SUBRESOURCE vtx_resource, idx_resource;
		if (m_gfx->m_DeviceContext->Map(g_pVB, 0, D3D11_MAP_WRITE_DISCARD, 0, &vtx_resource) != S_OK)
			return;
		if (m_gfx->m_DeviceContext->Map(g_pIB, 0, D3D11_MAP_WRITE_DISCARD, 0, &idx_resource) != S_OK)
			return;
		ImDrawVert* vtx_dst = (ImDrawVert*)vtx_resource.pData;
		ImDrawIdx* idx_dst = (ImDrawIdx*)idx_resource.pData;
		for (int n = 0; n < draw_data->CmdListsCount; n++)
		{
			const ImDrawList* cmd_list = draw_data->CmdLists[n];
			memcpy(vtx_dst, cmd_list->VtxBuffer.Data, cmd_list->VtxBuffer.Size * sizeof(ImDrawVert));
			memcpy(idx_dst, cmd_list->IdxBuffer.Data, cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx));
			vtx_dst += cmd_list->VtxBuffer.Size;
			idx_dst += cmd_list->IdxBuffer.Size;
		}
		m_gfx->m_DeviceContext->Unmap(g_pVB, 0);
		m_gfx->m_DeviceContext->Unmap(g_pIB, 0);

		// Setup orthographic projection matrix into our constant buffer
		{
			D3D11_MAPPED_SUBRESOURCE mapped_resource;
			if (m_gfx->m_DeviceContext->Map(g_pVertexConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource) != S_OK)
				return;
			VERTEX_CONSTANT_BUFFER* constant_buffer = (VERTEX_CONSTANT_BUFFER*)mapped_resource.pData;
			float L = 0.0f;
			float R = ImGui::GetIO().DisplaySize.x;
			float B = ImGui::GetIO().DisplaySize.y;
			float T = 0.0f;
			float mvp[4][4] =
			{
				{ 2.0f / (R - L),   0.0f,           0.0f,       0.0f },
				{ 0.0f,         2.0f / (T - B),     0.0f,       0.0f },
				{ 0.0f,         0.0f,           0.5f,       0.0f },
				{ (R + L) / (L - R),  (T + B) / (B - T),    0.5f,       1.0f },
			};
			memcpy(&constant_buffer->mvp, mvp, sizeof(mvp));
			m_gfx->m_DeviceContext->Unmap(g_pVertexConstantBuffer, 0);
		}

		// Backup DX state that will be modified to restore it afterwards (unfortunately this is very ugly looking and verbose. Close your eyes!)
		struct BACKUP_DX11_STATE
		{
			UINT                        ScissorRectsCount, ViewportsCount;
			D3D11_RECT                  ScissorRects[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE];
			D3D11_VIEWPORT              Viewports[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE];
			ID3D11RasterizerState*      RS;
			ID3D11BlendState*           BlendState;
			FLOAT                       BlendFactor[4];
			UINT                        SampleMask;
			UINT                        StencilRef;
			ID3D11DepthStencilState*    DepthStencilState;
			ID3D11ShaderResourceView*   PSShaderResource;
			ID3D11SamplerState*         PSSampler;
			ID3D11PixelShader*          PS;
			ID3D11VertexShader*         VS;
			UINT                        PSInstancesCount, VSInstancesCount;
			ID3D11ClassInstance*        PSInstances[256], *VSInstances[256];   // 256 is max according to PSSetShader documentation
			D3D11_PRIMITIVE_TOPOLOGY    PrimitiveTopology;
			ID3D11Buffer*               IndexBuffer, *VertexBuffer, *VSConstantBuffer;
			UINT                        IndexBufferOffset, VertexBufferStride, VertexBufferOffset;
			DXGI_FORMAT                 IndexBufferFormat;
			ID3D11InputLayout*          InputLayout;
		};
		BACKUP_DX11_STATE old;
		old.ScissorRectsCount = old.ViewportsCount = D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE;
		m_gfx->m_DeviceContext->RSGetScissorRects(&old.ScissorRectsCount, old.ScissorRects);
		m_gfx->m_DeviceContext->RSGetViewports(&old.ViewportsCount, old.Viewports);
		m_gfx->m_DeviceContext->RSGetState(&old.RS);
		m_gfx->m_DeviceContext->OMGetBlendState(&old.BlendState, old.BlendFactor, &old.SampleMask);
		m_gfx->m_DeviceContext->OMGetDepthStencilState(&old.DepthStencilState, &old.StencilRef);
		m_gfx->m_DeviceContext->PSGetShaderResources(0, 1, &old.PSShaderResource);
		m_gfx->m_DeviceContext->PSGetSamplers(0, 1, &old.PSSampler);
		old.PSInstancesCount = old.VSInstancesCount = 256;
		m_gfx->m_DeviceContext->PSGetShader(&old.PS, old.PSInstances, &old.PSInstancesCount);
		m_gfx->m_DeviceContext->VSGetShader(&old.VS, old.VSInstances, &old.VSInstancesCount);
		m_gfx->m_DeviceContext->VSGetConstantBuffers(0, 1, &old.VSConstantBuffer);
		m_gfx->m_DeviceContext->IAGetPrimitiveTopology(&old.PrimitiveTopology);
		m_gfx->m_DeviceContext->IAGetIndexBuffer(&old.IndexBuffer, &old.IndexBufferFormat, &old.IndexBufferOffset);
		m_gfx->m_DeviceContext->IAGetVertexBuffers(0, 1, &old.VertexBuffer, &old.VertexBufferStride, &old.VertexBufferOffset);
		m_gfx->m_DeviceContext->IAGetInputLayout(&old.InputLayout);

		// Bind shader and vertex buffers
		unsigned int stride = sizeof(ImDrawVert);
		unsigned int offset = 0;
		m_gfx->m_DeviceContext->IASetVertexBuffers(0, 1, &g_pVB, &stride, &offset);
		m_gfx->m_DeviceContext->IASetIndexBuffer(g_pIB, sizeof(ImDrawIdx) == 2 ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT, 0);
		m_gfx->m_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_gfx->m_DeviceContext->VSSetConstantBuffers(0, 1, &g_pVertexConstantBuffer);
		m_gfx->m_DeviceContext->PSSetSamplers(0, 1, &g_pFontSampler);

		g_VertexShader->SetThisVertexShaderAndInputLayout(m_gfx);
		g_PixelShader->SetThisPixelShader(m_gfx);

		// Setup render state
		const float blend_factor[4] = { 0.3f, 0.3f, 0.3f, 0.3f };
		m_gfx->m_DeviceContext->OMSetBlendState(g_pBlendState, blend_factor, 0xffffffff);
		m_gfx->m_DeviceContext->OMSetDepthStencilState(g_pDepthStencilState, 0);
		m_gfx->m_DeviceContext->RSSetState(g_pRasterizerState);

		// Render command lists
		int vtx_offset = 0;
		int idx_offset = 0;
		for (int n = 0; n < draw_data->CmdListsCount; n++)
		{
			const ImDrawList* cmd_list = draw_data->CmdLists[n];
			for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
			{
				const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
				if (pcmd->UserCallback)
				{
					pcmd->UserCallback(cmd_list, pcmd);
				}
				else
				{
					const D3D11_RECT r = { (LONG)pcmd->ClipRect.x, (LONG)pcmd->ClipRect.y, (LONG)pcmd->ClipRect.z, (LONG)pcmd->ClipRect.w };
					m_gfx->m_DeviceContext->PSSetShaderResources(0, 1, (ID3D11ShaderResourceView**)&pcmd->TextureId);
					m_gfx->m_DeviceContext->RSSetScissorRects(1, &r);
					m_gfx->m_DeviceContext->DrawIndexed(pcmd->ElemCount, idx_offset, vtx_offset);
				}
				idx_offset += pcmd->ElemCount;
			}
			vtx_offset += cmd_list->VtxBuffer.Size;
		}

		// Restore modified DX state
		m_gfx->m_DeviceContext->RSSetScissorRects(old.ScissorRectsCount, old.ScissorRects);
		m_gfx->m_DeviceContext->RSSetViewports(old.ViewportsCount, old.Viewports);
		m_gfx->m_DeviceContext->RSSetState(old.RS); if (old.RS) old.RS->Release();
		m_gfx->m_DeviceContext->OMSetBlendState(old.BlendState, old.BlendFactor, old.SampleMask); if (old.BlendState) old.BlendState->Release();
		m_gfx->m_DeviceContext->OMSetDepthStencilState(old.DepthStencilState, old.StencilRef); if (old.DepthStencilState) old.DepthStencilState->Release();
		m_gfx->m_DeviceContext->PSSetShaderResources(0, 1, &old.PSShaderResource); if (old.PSShaderResource) old.PSShaderResource->Release();
		m_gfx->m_DeviceContext->PSSetSamplers(0, 1, &old.PSSampler); if (old.PSSampler) old.PSSampler->Release();
		m_gfx->m_DeviceContext->PSSetShader(old.PS, old.PSInstances, old.PSInstancesCount); if (old.PS) old.PS->Release();
		for (UINT i = 0; i < old.PSInstancesCount; i++) if (old.PSInstances[i]) old.PSInstances[i]->Release();
		m_gfx->m_DeviceContext->VSSetShader(old.VS, old.VSInstances, old.VSInstancesCount); if (old.VS) old.VS->Release();
		m_gfx->m_DeviceContext->VSSetConstantBuffers(0, 1, &old.VSConstantBuffer); if (old.VSConstantBuffer) old.VSConstantBuffer->Release();
		for (UINT i = 0; i < old.VSInstancesCount; i++) if (old.VSInstances[i]) old.VSInstances[i]->Release();
		m_gfx->m_DeviceContext->IASetPrimitiveTopology(old.PrimitiveTopology);
		m_gfx->m_DeviceContext->IASetIndexBuffer(old.IndexBuffer, old.IndexBufferFormat, old.IndexBufferOffset); if (old.IndexBuffer) old.IndexBuffer->Release();
		m_gfx->m_DeviceContext->IASetVertexBuffers(0, 1, &old.VertexBuffer, &old.VertexBufferStride, &old.VertexBufferOffset); if (old.VertexBuffer) old.VertexBuffer->Release();
		m_gfx->m_DeviceContext->IASetInputLayout(old.InputLayout); if (old.InputLayout) old.InputLayout->Release();

	}

	void GUIEditor::DestroyEditor()
	{
		InvalidateDeviceObjects();
		ImGui::Shutdown();
		m_Hwnd = (HWND)0;
	}

	void GUIEditor::RenderEditor(RendererConfig * user)
	{
		NewFrame(m_gfx);

		{
			ImGui::Text("PBR Config");

			ImGui::SliderFloat("Light", &user->LightIntensity, 0.0f, 1.0f);
			ImGui::SliderFloat("Ambient", &user->AmbientLightIntensity, 0.0f, 1.0f);
			ImGui::SliderFloat("Reflection", &user->ReflectionIntensity, 0.0f, 1.0f);

			ImGui::SliderInt("R", &user->R, 0, 1);
			ImGui::SliderInt("G", &user->G, 0, 1);
			ImGui::SliderInt("B", &user->B, 0, 1);

			ImGui::Checkbox("Wireframe", &user->Wireframe);

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

			ImGui::SliderInt("Correction Mode:", &ColorMode, 0, 3);
			ImGui::SliderFloat("Exposure", &Exposure, 0.1f, 5.0f);
		}

		ImGui::Begin("Play Mode");
		{
			static bool PlayMode = false;

			if (ImGui::Checkbox("", &PlayMode))
			{
				if (PlayMode)
					Input::Pointer()->RegainDevices();
				else
					Input::Pointer()->ReleaseDevices();
			}
		}

		m_SceneGraphGUI.RenderSceneGraphGUI();
		if (m_SceneGraphGUI.m_ActiveGameObject)
			m_GameObjectGUI.SetNewGameObject(m_SceneGraphGUI.m_ActiveGameObject);
		m_GameObjectGUI.RenderWindow();
		m_ResourcesGUI.RenderResources();


	}

	void GUIEditor::CreateFontsTexture()
	{
		// Build texture atlas
		ImGuiIO& io = ImGui::GetIO();
		unsigned char* pixels;
		int width, height;
		io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

		// Upload texture to graphics system
		{
			D3D11_TEXTURE2D_DESC desc;
			ZeroMemory(&desc, sizeof(desc));
			desc.Width = width;
			desc.Height = height;
			desc.MipLevels = 1;
			desc.ArraySize = 1;
			desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			desc.SampleDesc.Count = 1;
			desc.Usage = D3D11_USAGE_DEFAULT;
			desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
			desc.CPUAccessFlags = 0;

			ID3D11Texture2D *pTexture = NULL;
			D3D11_SUBRESOURCE_DATA subResource;
			subResource.pSysMem = pixels;
			subResource.SysMemPitch = desc.Width * 4;
			subResource.SysMemSlicePitch = 0;
			m_gfx->GetDevice()->CreateTexture2D(&desc, &subResource, &pTexture);

			// Create texture view
			D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
			ZeroMemory(&srvDesc, sizeof(srvDesc));
			srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			srvDesc.Texture2D.MipLevels = desc.MipLevels;
			srvDesc.Texture2D.MostDetailedMip = 0;
			m_gfx->GetDevice()->CreateShaderResourceView(pTexture, &srvDesc, &g_pFontTextureView);
			pTexture->Release();
		}

		// Store our identifier
		io.Fonts->TexID = (void *)g_pFontTextureView;

		// Create texture sampler
		{
			D3D11_SAMPLER_DESC desc;
			ZeroMemory(&desc, sizeof(desc));
			desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
			desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
			desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
			desc.MipLODBias = 0.f;
			desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
			desc.MinLOD = 0.f;
			desc.MaxLOD = 0.f;
			m_gfx->GetDevice()->CreateSamplerState(&desc, &g_pFontSampler);
		}
	}

	void GUIEditor::InvalidateDeviceObjects()
	{
		if (!m_gfx->m_Device)
			return;

		if (g_pFontSampler) { g_pFontSampler->Release(); g_pFontSampler = NULL; }
		if (g_pFontTextureView) { g_pFontTextureView->Release(); g_pFontTextureView = NULL; ImGui::GetIO().Fonts->TexID = NULL; } // We copied g_pFontTextureView to io.Fonts->TexID so let's clear that as well.
		if (g_pIB) { g_pIB->Release(); g_pIB = NULL; }
		if (g_pVB) { g_pVB->Release(); g_pVB = NULL; }

		if (g_pBlendState) { g_pBlendState->Release(); g_pBlendState = NULL; }
		if (g_pDepthStencilState) { g_pDepthStencilState->Release(); g_pDepthStencilState = NULL; }
		if (g_pRasterizerState) { g_pRasterizerState->Release(); g_pRasterizerState = NULL; }
		if (g_PixelShader->m_PixelShader) { g_PixelShader->m_PixelShader->Release(); g_PixelShader->m_PixelShader = NULL; }
		if (g_pVertexConstantBuffer) { g_pVertexConstantBuffer->Release(); g_pVertexConstantBuffer = NULL; }
		if (g_VertexShader->m_InputLayout.m_InputLayout) { g_VertexShader->m_InputLayout.m_InputLayout->Release(); g_VertexShader->m_InputLayout.m_InputLayout = NULL; }
		if (g_VertexShader->m_VertexShader) { g_VertexShader->m_VertexShader->Release(); g_VertexShader->m_VertexShader = NULL; }
	}

	bool GUIEditor::CreateDeviceObjects(rczEngine::Gfx::GfxCore * gfx)
	{
		//if (g_pFontSampler)
		//	ImGui_ImplDX11_InvalidateDeviceObjects();

		// Create the vertex shader

		g_VertexShader = new Gfx::VertexShader();
		gfx->CompileShader(*g_VertexShader, L"Shaders/imgui/VS.hlsl", "vs_5_0", "main");
		gfx->CreateVertexShader(*g_VertexShader);

		g_VertexShader->m_InputLayout.
			AddElement("POSITION", 0, Gfx::FORMAT_R32G32_FLOAT, 0, 0, Gfx::INPUT_PER_VERTEX, 0);

		g_VertexShader->m_InputLayout.
			AddElement("TEXCOORD", 0, Gfx::FORMAT_R32G32_FLOAT, 0, 8, Gfx::INPUT_PER_VERTEX, 0);

		g_VertexShader->m_InputLayout.
			AddElement("COLOR", 0, Gfx::FORMAT_R8G8B8A8_UNORM, 0, 16, Gfx::INPUT_PER_VERTEX, 0);

		g_VertexShader->m_InputLayout.CreateInputLayout(*g_VertexShader, gfx);

		// Create the constant buffer

		m_ConstantBuffer.Create(sizeof(VERTEX_CONSTANT_BUFFER), 1, NULL, 0, 0, Gfx::USAGE_DYNAMIC, Gfx::BIND_CONSTANT_BUFFER, gfx);
		g_pVertexConstantBuffer = m_ConstantBuffer.m_Buffer;

		// Create the pixel shader
		g_PixelShader = new Gfx::PixelShader();
		gfx->CompileShader(*g_PixelShader, L"Shaders/imgui/PS.hlsl", "ps_5_0", "main");

		gfx->CreatePixelShader(*g_PixelShader);


		// Create the blending setup
		m_BlendState.InitBlendState(true,
			Gfx::BLEND_OP_ADD, Gfx::BLEND_OP_ADD, Gfx::BLEND_INV_SRC_ALPHA, Gfx::BLEND_ZERO, Gfx::BLEND_SRC_ALPHA, Gfx::BLEND_INV_SRC_ALPHA);

		m_BlendState.CreateBlendState(gfx);

		g_pBlendState = m_BlendState.m_BlendState;

		// Create the rasterizer state
		{
			D3D11_RASTERIZER_DESC desc;
			ZeroMemory(&desc, sizeof(desc));
			desc.FillMode = D3D11_FILL_SOLID;
			desc.CullMode = D3D11_CULL_NONE;
			desc.ScissorEnable = true;
			desc.DepthClipEnable = true;
			m_gfx->GetDevice()->CreateRasterizerState(&desc, &g_pRasterizerState);
		}

		// Create depth-stencil State
		{
			D3D11_DEPTH_STENCIL_DESC desc;
			ZeroMemory(&desc, sizeof(desc));
			desc.DepthEnable = false;
			desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
			desc.DepthFunc = D3D11_COMPARISON_ALWAYS;
			desc.StencilEnable = false;
			desc.FrontFace.StencilFailOp = desc.FrontFace.StencilDepthFailOp = desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
			desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
			desc.BackFace = desc.FrontFace;
			m_gfx->GetDevice()->CreateDepthStencilState(&desc, &g_pDepthStencilState);
		}

		CreateFontsTexture();

		return true;
	}

	void GUIEditor::NewFrame(Gfx::GfxCore * gfx)
	{
		if (!g_pFontSampler)
			CreateDeviceObjects(gfx);

		ImGuiIO& io = ImGui::GetIO();

		// Setup display size (every frame to accommodate for window resizing)
		RECT rect;
		GetClientRect(m_Hwnd, &rect);
		io.DisplaySize = ImVec2((float)(rect.right - rect.left), (float)(rect.bottom - rect.top));

		// Setup time step
		INT64 current_time;
		QueryPerformanceCounter((LARGE_INTEGER *)&current_time);
		io.DeltaTime = (float)(current_time - m_Time) / m_TicksPerSecond;
		m_Time = current_time;

		// Read keyboard modifiers inputs
		io.KeyCtrl = (GetKeyState(VK_CONTROL) & 0x8000) != 0;
		io.KeyShift = (GetKeyState(VK_SHIFT) & 0x8000) != 0;
		io.KeyAlt = (GetKeyState(VK_MENU) & 0x8000) != 0;
		io.KeySuper = false;
		// io.KeysDown : filled by WM_KEYDOWN/WM_KEYUP events
		// io.MousePos : filled by WM_MOUSEMOVE events
		// io.MouseDown : filled by WM_*BUTTON* events
		// io.MouseWheel : filled by WM_MOUSEWHEEL events

		// Set OS mouse position if requested last frame by io.WantMoveMouse flag (used when io.NavMovesTrue is enabled by user and using directional navigation)
		if (io.WantMoveMouse)
		{
			POINT pos = { (int)io.MousePos.x, (int)io.MousePos.y };
			ClientToScreen(m_Hwnd, &pos);
			SetCursorPos(pos.x, pos.y);
		}

		// Hide OS mouse cursor if ImGui is drawing it
		if (io.MouseDrawCursor)
			SetCursor(NULL);

		// Start the frame
		ImGui::NewFrame();
	}

};