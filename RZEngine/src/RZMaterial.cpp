#include <RZEnginePCH.h>

namespace rczEngine
{
	void Material::InitMaterial(MATERIAL_TYPE mat_type, Gfx::GfxCore* gfx)
	{
		m_MaterialCB.CreateConstantBuffer(sizeof(MaterialCore), Gfx::USAGE_DEFAULT, gfx);

		m_MatType = mat_type;

		if (m_MatType == MAT_PBR_MetRough)
		{
			SetAttributesMet(Vector4(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, 0.5f);
		}
		else if (m_MatType == MAT_PBR_SpecSmooth)
		{
			SetAttributesSpec(Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), 0.5f);
		}
	}

	void Material::SetThisMaterial(Gfx::GfxCore* gfx, void* res)
	{
		ResVault* Res = (ResVault*)res;

		ResourceHandle handle = GetHandle();

		if (handle != INVALID_RESOURCE)
		{
			gfx->m_SetMaterial = handle;

			m_MaterialCB.UpdateConstantBuffer(&m_core, gfx);
			m_MaterialCB.SetBufferInPS(MATERIAL_SLOT, gfx);

			if (m_TextureAlbedo != INVALID_RESOURCE)
			{
				Res->GetResource<Texture2D>(m_TextureAlbedo).lock()->SetThisTextureInPS(TEX_ALBEDO, 1, gfx);
			}
			else
			{
				Res->GetResource<Texture2D>(Res->m_WhiteTex).lock()->SetThisTextureInPS(TEX_ALBEDO, 1, gfx);
			}

			if (m_TextureNormal != INVALID_RESOURCE)
			{
				Res->GetResource<Texture2D>(m_TextureNormal).lock()->SetThisTextureInPS(TEX_NORMAL, 1, gfx);
			}
			else
			{
				Res->GetResource<Texture2D>(Res->m_NormalTex).lock()->SetThisTextureInPS(TEX_NORMAL, 1, gfx);
			}

			if (m_TextureMetSpec != INVALID_RESOURCE)
			{
				Res->GetResource<Texture2D>(m_TextureMetSpec).lock()->SetThisTextureInPS(TEX_MET_SPEC, 1, gfx);
			}
			else
			{
				if (m_MatType == MAT_PBR_MetRough || m_MatType == MAT_PBR_MetRough_Tess)
					Res->GetResource<Texture2D>(Res->m_BlackTex).lock()->SetThisTextureInPS(TEX_MET_SPEC, 1, gfx);
				else
					Res->GetResource<Texture2D>(Res->m_GreyTex).lock()->SetThisTextureInPS(TEX_MET_SPEC, 1, gfx);
			}

			if (m_MatType != MAT_PBR_SpecSmooth_Alpha)
			{
				if (m_TextureRoughSmooth != INVALID_RESOURCE)
				{
					Res->GetResource<Texture2D>(m_TextureRoughSmooth).lock()->SetThisTextureInPS(TEX_ROUGH_SMOOTH, 1, gfx);
				}
				else
				{
					Res->GetResource<Texture2D>(Res->m_GreyTex).lock()->SetThisTextureInPS(TEX_ROUGH_SMOOTH, 1, gfx);
				}
			}

			if (m_TextureAO != INVALID_RESOURCE)
			{
				Res->GetResource<Texture2D>(m_TextureAO).lock()->SetThisTextureInPS(TEX_AMBIENT_OCCLUSION, 1, gfx);
			}
			else
			{
				Res->GetResource<Texture2D>(Res->m_WhiteTex).lock()->SetThisTextureInPS(TEX_AMBIENT_OCCLUSION, 1, gfx);
			}

			if (m_TextureEm != INVALID_RESOURCE)
			{
				Res->GetResource<Texture2D>(m_TextureEm).lock()->SetThisTextureInPS(TEX_EMISSIVE, 1, gfx);
			}
			else
			{
				Res->GetResource<Texture2D>(Res->m_BlackTex).lock()->SetThisTextureInPS(TEX_EMISSIVE, 1, gfx);
			}

			if (m_MatType == MATERIAL_TYPE::MAT_PBR_MetRough_Tess)
			{
				if (m_TextureH != INVALID_RESOURCE)
				{
					Res->GetResource<Texture2D>(m_TextureH).lock()->SetThisTextureInDS(0, 1, gfx);
				}
				else
				{
					Res->GetResource<Texture2D>(Res->m_GreyTex).lock()->SetThisTextureInDS(0, 1, gfx);
				}
			}

			if (m_MatType == MATERIAL_TYPE::MAT_PBR_MetRough_Trans)
			{
				if (m_TextureO != INVALID_RESOURCE)
				{
					Res->GetResource<Texture2D>(m_TextureO).lock()->SetThisTextureInPS(TEX_OPACITY, 1, gfx);
				}
				else
				{
					Res->GetResource<Texture2D>(Res->m_WhiteTex).lock()->SetThisTextureInPS(TEX_OPACITY, 1, gfx);
				}
			}
		}
	}

	void Material::SetTextures(ResourceHandle albedo, ResourceHandle normals, ResourceHandle metallic, ResourceHandle roughness, ResourceHandle ao, ResourceHandle em, ResourceHandle h)
	{
		m_TextureAlbedo = albedo;
		m_TextureNormal = normals;
		m_TextureMetSpec = metallic;
		m_TextureRoughSmooth = roughness;
		m_TextureAO = ao;
		m_TextureEm = em;
		m_TextureH = h;
	}

	void Material::SetAttributesMet(Vector4 albedo, float metallic, float roughness)
	{
		m_core.g_Albedo = albedo;
		m_core.g_Metallic = metallic;
		m_core.g_RoughGloss = roughness;
	}

	void Material::SetAttributesSpec(Vector4 albedo, Vector4 specular, float glossiness)
	{
		m_core.g_Albedo = albedo;
		m_core.g_Specular = specular;
		m_core.g_RoughGloss = glossiness;
	}

	void Material::SetOpacity(float opacity)
	{
		m_core.g_Opacity = opacity;
	}

	void Material::SetTesselationFactor(float tess)
	{
		m_core.g_TesselationFactor = tess;
	}

	void Material::SetPlanetMaterial()
	{
		ResVault* Res = ResVault::Pointer();

		ResourceHandle handle = GetHandle();

		if (handle != INVALID_RESOURCE)
		{
			auto gfx = Gfx::GfxCore::Pointer();

			m_MaterialCB.UpdateConstantBuffer(&m_core, gfx);
			m_MaterialCB.SetBufferInPS(MATERIAL_SLOT, gfx);

			if (m_TextureAlbedo != INVALID_RESOURCE)
			{
				Res->GetResource<Texture3D>(m_TextureAlbedo).lock()->SetThisTextureInPS(TEX_ALBEDO, 1, gfx);
			}

			if (m_TextureNormal != INVALID_RESOURCE)
			{
				Res->GetResource<Texture3D>(m_TextureNormal).lock()->SetThisTextureInPS(TEX_NORMAL, 1, gfx);
			}

			if (m_TextureMetSpec != INVALID_RESOURCE)
			{
				Res->GetResource<Texture3D>(m_TextureMetSpec).lock()->SetThisTextureInPS(TEX_MET_SPEC, 1, gfx);
			}


			if (m_TextureRoughSmooth != INVALID_RESOURCE)
			{
				Res->GetResource<Texture3D>(m_TextureRoughSmooth).lock()->SetThisTextureInPS(TEX_ROUGH_SMOOTH, 1, gfx);
			}

			if (m_TextureAO != INVALID_RESOURCE)
			{
				Res->GetResource<Texture3D>(m_TextureAO).lock()->SetThisTextureInPS(TEX_AMBIENT_OCCLUSION, 1, gfx);
			}


			if (m_TextureEm != INVALID_RESOURCE)
			{
				Res->GetResource<Texture3D>(m_TextureEm).lock()->SetThisTextureInPS(TEX_EMISSIVE, 1, gfx);
			}
		}
	}

	void Material::SetTexture(TEXTURE_TYPE texType, ResourceHandle rHandle)
	{
		switch (texType)
		{
		default:
		case TEX_ALBEDO:
			m_TextureAlbedo = rHandle;
			break;
		case TEX_NORMAL:
			m_TextureNormal = rHandle;
			break;
		case TEX_MET_SPEC:
			m_TextureMetSpec = rHandle;
			break;
		case TEX_ROUGH_SMOOTH:
			m_TextureRoughSmooth = rHandle;
			break;
		case TEX_AMBIENT_OCCLUSION:
			m_TextureAO = rHandle;
			break;
		case TEX_EMISSIVE:
			m_TextureEm = rHandle;
			break;
		case TEX_HEIGHT:
			m_TextureH = rHandle;
			break;
		case TEX_OPACITY:
			m_TextureO = rHandle;
			break;
		}
	}

	void Material::Load(const String& filePath, const String& resName)
	{
	}

	void Material::Serialize()
	{
		auto ser = Serializer::Pointer();

		ser->SetNextObjectSerial(SERIAL_MATERIAL);

		///Write the material name
		ser->SerializeString(m_Name);
		//Write the material file path and hash
		m_FilePath.Serialize();
		//Write the material type
		ser->WriteData(&m_MatType, sizeof(m_MatType));
		//Write the material core
		ser->WriteData(&m_core, sizeof(m_core));

		//Write all the handles to textures.
		ser->WriteData(&m_TextureAlbedo, sizeof(ResourceHandle));
		ser->WriteData(&m_TextureNormal, sizeof(ResourceHandle));
		ser->WriteData(&m_TextureRoughSmooth, sizeof(ResourceHandle));
		ser->WriteData(&m_TextureMetSpec, sizeof(ResourceHandle));
		ser->WriteData(&m_TextureAO, sizeof(ResourceHandle));
		ser->WriteData(&m_TextureEm, sizeof(ResourceHandle));
		ser->WriteData(&m_TextureH, sizeof(ResourceHandle));
		ser->WriteData(&m_TextureO, sizeof(ResourceHandle));

	}

	void Material::DeSerialize()
	{
		auto ser = Serializer::Pointer();

		///Read the material name
		ser->DeSerializeString(m_Name);
		//Read the material file path and hash
		m_FilePath.DeSerialize();
		//Read the material type
		ser->ReadData(&m_MatType, sizeof(m_MatType));
		//Read the material core
		ser->ReadData(&m_core, sizeof(m_core));

		//Write all the handles to textures.
		ser->ReadData(&m_TextureAlbedo, sizeof(ResourceHandle));
		ser->ReadData(&m_TextureNormal, sizeof(ResourceHandle));
		ser->ReadData(&m_TextureRoughSmooth, sizeof(ResourceHandle));
		ser->ReadData(&m_TextureMetSpec, sizeof(ResourceHandle));
		ser->ReadData(&m_TextureAO, sizeof(ResourceHandle));
		ser->ReadData(&m_TextureEm, sizeof(ResourceHandle));
		ser->ReadData(&m_TextureH, sizeof(ResourceHandle));
		ser->ReadData(&m_TextureO, sizeof(ResourceHandle));

		m_MaterialCB.CreateConstantBuffer(sizeof(MaterialCore), Gfx::USAGE_DEFAULT, Gfx::GfxCore::Pointer());
	}

}