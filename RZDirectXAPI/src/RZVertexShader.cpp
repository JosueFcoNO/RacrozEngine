#include <RZUtilityPCH.h>
#include "RZDirectXPCH.h"

namespace rczEngine
{
	namespace Gfx
	{
		VertexShader::VertexShader()
		{
			m_ShaderBlob = nullptr;
			m_VertexShader = nullptr;
		}

		VertexShader::~VertexShader()
		{
			Destroy();
		}

		bool VertexShader::CreateVertexShader(const WCHAR * pszFileString, GfxCore* gfx)
		{
			return gfx->CompileAndCreateVertexShader(*this, pszFileString);
		}

		void VertexShader::Destroy()
		{
			if (m_ShaderBlob) m_ShaderBlob->Release();
			if (m_VertexShader) m_VertexShader->Release();
		}

		void VertexShader::SetThisVertexShader(GfxCore* gfx)
		{
			if (m_VertexShader == nullptr || m_ShaderBlob == nullptr)
			{
				return;
			}

			gfx->SetVertexShader(*this);
		}

		void VertexShader::SetThisInputLayout(GfxCore* gfx)
		{
			gfx->SetInputLayout(m_InputLayout);
		}

		void VertexShader::SetThisVertexShaderAndInputLayout(GfxCore* gfx)
		{
			SetThisVertexShader(gfx);
			SetThisInputLayout(gfx);
		}

		void VertexShader::ReflectLayout(int layoutSlot, GfxCore* gfx)
		{
			HRESULT hr;

			///Do the reflection
			ID3D11ShaderReflection* reflect = nullptr;
			hr = D3DReflect(m_ShaderBlob->GetBufferPointer(), m_ShaderBlob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&reflect);

			///Get the reflection's descriptor
			D3D11_SHADER_DESC shaderDesc;
			hr = reflect->GetDesc(&shaderDesc);
			int32 nOfInputParameters = shaderDesc.InputParameters;

			///iterate through the parameters and add them to the input layout.
			D3D11_SIGNATURE_PARAMETER_DESC parameterDesc;
			int32 SemanticIndex;
			int32 ByteWidth = 0;

			for (int32 i = 0; i < nOfInputParameters; ++i)
			{
				hr = reflect->GetInputParameterDesc(i, &parameterDesc);

				SemanticIndex = parameterDesc.SemanticIndex;
				String SemanticName = parameterDesc.SemanticName;

				if (SemanticName == "POSITION")
				{
					m_InputLayout.AddElement(SemanticName.c_str(), SemanticIndex, eFORMAT::FORMAT_R32G32B32_FLOAT, layoutSlot, ByteWidth, eINPUT_CLASSIFICATION::INPUT_PER_VERTEX, 0);
					ByteWidth += 4 * 3;
				}
				else
					if (SemanticName == "TEXCOORD")
					{
						m_InputLayout.AddElement(SemanticName.c_str(), SemanticIndex, eFORMAT::FORMAT_R32G32_FLOAT, layoutSlot, ByteWidth, eINPUT_CLASSIFICATION::INPUT_PER_VERTEX, 0);
						ByteWidth += 4 * 2;
					}
					else
						if (SemanticName == "NORMAL")
						{
							m_InputLayout.AddElement(SemanticName.c_str(), SemanticIndex, eFORMAT::FORMAT_R32G32B32_FLOAT, layoutSlot, ByteWidth, eINPUT_CLASSIFICATION::INPUT_PER_VERTEX, 0);
							ByteWidth += 4 * 3;
						}
						else
							if (SemanticName == "BINORMAL")
							{
								m_InputLayout.AddElement(SemanticName.c_str(), SemanticIndex, eFORMAT::FORMAT_R32G32B32_FLOAT, layoutSlot, ByteWidth, eINPUT_CLASSIFICATION::INPUT_PER_VERTEX, 0);
								ByteWidth += 4 * 3;
							}
							else
								if (SemanticName == "TANGENT")
								{
									m_InputLayout.AddElement(SemanticName.c_str(), SemanticIndex, eFORMAT::FORMAT_R32G32B32_FLOAT, layoutSlot, ByteWidth, eINPUT_CLASSIFICATION::INPUT_PER_VERTEX, 0);
									ByteWidth += 4 * 3;
								}
								else
									if (SemanticName == "BLENDINDICES")
									{
										m_InputLayout.AddElement(SemanticName.c_str(), SemanticIndex, eFORMAT::FORMAT_R32_SINT, layoutSlot, ByteWidth, eINPUT_CLASSIFICATION::INPUT_PER_VERTEX, 0);
										ByteWidth += 4;
									}
									else
										if (SemanticName == "BLENDWEIGHT")
										{
											m_InputLayout.AddElement(SemanticName.c_str(), SemanticIndex, eFORMAT::FORMAT_R32_FLOAT, layoutSlot, ByteWidth, eINPUT_CLASSIFICATION::INPUT_PER_VERTEX, 0);
											ByteWidth += 4;
										}
			}

			reflect->Release();

			m_InputLayout.CreateInputLayout(*this, gfx);
		}
	}
}