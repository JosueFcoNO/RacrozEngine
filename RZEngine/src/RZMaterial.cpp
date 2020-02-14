#include <RZEnginePCH.h>

namespace rczEngine
{
	void Material::InitMaterial(eMaterialType mat_type)
	{
		const auto gfx = Gfx::GfxCore::Pointer();
		const auto Res = ResVault::Pointer();

		m_MaterialCB.CreateConstantBuffer(sizeof(MaterialCore), Gfx::USAGE_DEFAULT, gfx);

		m_MatType = mat_type;
		m_ShadingType = eShadingType::PBR;
		m_BlendType = eBlendType::Opaque;

		m_TwoMaterialBlend = false;
		m_Tesselation = false;
		m_TwoSided = false;

		m_TextureAlbedo = Res->m_WhiteTex;
		m_TextureNormal = Res->m_NormalTex;
		m_TextureRoughSmooth = Res->m_GreyTex;
		m_TextureAO = Res->m_WhiteTex;
		m_TextureEm = Res->m_BlackTex;
		m_TextureH = Res->m_GreyTex;
		m_TextureO = Res->m_WhiteTex;

		if (m_MatType == eMaterialType::PBR_MetRough)
		{
			SetAttributesMet(Vector4(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, 0.5f);
			
			m_TextureMetSpec = Res->m_BlackTex;
		}
		else if (m_MatType == eMaterialType::PBR_SpecSmooth)
		{
			SetAttributesSpec(Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), 0.5f);

			m_TextureMetSpec = Res->m_GreyTex;
		}
	}

	void Material::SetThisMaterial()
	{
		auto gfx = Gfx::GfxCore::Pointer();
		auto Res = ResVault::Pointer();

		ResourceHandle handle = GetHandle();

		if (handle != INVALID_RESOURCE)
		{
			gfx->m_SetMaterial = handle;

			m_MaterialCB.UpdateConstantBuffer(&m_core, gfx);
			m_MaterialCB.SetBufferInPS(MATERIAL_SLOT, gfx);

			Res->GetResource<Texture2D>(m_TextureAlbedo).lock()->SetThisTextureInPS((int)eTextureType::BaseColor, 1, gfx);
			Res->GetResource<Texture2D>(m_TextureNormal).lock()->SetThisTextureInPS((int)eTextureType::Normals, 1, gfx);
			Res->GetResource<Texture2D>(m_TextureMetSpec).lock()->SetThisTextureInPS((int)eTextureType::MetallicSpecular, 1, gfx);
			Res->GetResource<Texture2D>(m_TextureRoughSmooth).lock()->SetThisTextureInPS((int)eTextureType::RoughGloss, 1, gfx);
			Res->GetResource<Texture2D>(m_TextureAO).lock()->SetThisTextureInPS((int)eTextureType::AmbientOcclusion, 1, gfx);
			Res->GetResource<Texture2D>(m_TextureEm).lock()->SetThisTextureInPS((int)eTextureType::Emissive, 1, gfx);

			if (m_Tesselation)
			{
				Res->GetResource<Texture2D>(m_TextureH).lock()->SetThisTextureInDS(0, 1, gfx);
			}

			if (m_BlendType != eBlendType::Opaque)
			{
				Res->GetResource<Texture2D>(m_TextureO).lock()->SetThisTextureInPS((int)eTextureType::OpacityMask, 1, gfx);
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
			throw std::exception();

			auto gfx = Gfx::GfxCore::Pointer();

			m_MaterialCB.UpdateConstantBuffer(&m_core, gfx);
			m_MaterialCB.SetBufferInPS(MATERIAL_SLOT, gfx);

			if (m_TextureAlbedo != INVALID_RESOURCE)
			{
				Res->GetResource<Texture3D>(m_TextureAlbedo).lock()->SetThisTextureInPS((int)eTextureType::BaseColor, 1, gfx);
			}

			if (m_TextureNormal != INVALID_RESOURCE)
			{
				Res->GetResource<Texture3D>(m_TextureNormal).lock()->SetThisTextureInPS((int)eTextureType::Normals, 1, gfx);
			}

			if (m_TextureMetSpec != INVALID_RESOURCE)
			{
				Res->GetResource<Texture3D>(m_TextureMetSpec).lock()->SetThisTextureInPS((int)eTextureType::MetallicSpecular, 1, gfx);
			}


			if (m_TextureRoughSmooth != INVALID_RESOURCE)
			{
				Res->GetResource<Texture3D>(m_TextureRoughSmooth).lock()->SetThisTextureInPS((int)eTextureType::RoughGloss, 1, gfx);
			}

			if (m_TextureAO != INVALID_RESOURCE)
			{
				Res->GetResource<Texture3D>(m_TextureAO).lock()->SetThisTextureInPS((int)eTextureType::AmbientOcclusion, 1, gfx);
			}


			if (m_TextureEm != INVALID_RESOURCE)
			{
				Res->GetResource<Texture3D>(m_TextureEm).lock()->SetThisTextureInPS((int)eTextureType::Emissive, 1, gfx);
			}
		}
	}

	void Material::SetTexture(eTextureType texType, ResourceHandle rHandle)
	{
		switch (texType)
		{
		default:
		case eTextureType::BaseColor:
			m_TextureAlbedo = rHandle;
			break;
		case eTextureType::Normals:
			m_TextureNormal = rHandle;
			break;
		case eTextureType::MetallicSpecular:
			m_TextureMetSpec = rHandle;
			break;
		case eTextureType::RoughGloss:
			m_TextureRoughSmooth = rHandle;
			break;
		case eTextureType::AmbientOcclusion:
			m_TextureAO = rHandle;
			break;
		case eTextureType::Emissive:
			m_TextureEm = rHandle;
			break;
		case eTextureType::HeightMap:
			m_TextureH = rHandle;
			break;
		case eTextureType::OpacityMask:
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


	////////////////////////////
	/// Material Render Info
	////////////////////////////

	int MaterialRenderInfo::CalculateRenderHash(eComponentID componentID, eMaterialType matType, eShadingType shading, eBlendType blendType, bool Tesselated, bool TwoSided, bool blendedMaterial, bool wireframe)
	{
		int hash = 0;

		hash |= (int)componentID;
		hash |= (int)matType << 5;
		hash |= (int)shading << 9;
		hash |= (int)blendType << 13;
		hash |= (int)Tesselated << 14;
		hash |= (int)TwoSided << 15;
		hash |= (int)blendedMaterial << 16;
		hash |= (int)wireframe << 17;

		return hash;
	}

	int MaterialRenderInfo::CalculateRenderHash(MaterialRenderInfo rInfo)
	{
		return CalculateRenderHash(
			rInfo.componentID,
			rInfo.matType,
			rInfo.shading,
			rInfo.blendType,
			rInfo.Tesselated,
			rInfo.TwoSided,
			rInfo.blendedMaterial,
			rInfo.wireframe);
	}

	MaterialRenderInfo MaterialRenderInfo::DecodeRenderHash(int hash)
	{
		MaterialRenderInfo info;

		info.componentID = (eComponentID)(hash & 0b11111);
		info.matType = (eMaterialType)(hash >> 5 & 0b1111);
		info.shading = (eShadingType)(hash >> 9 & 0b1111);
		info.blendType = (eBlendType)(hash >> 13 & 0b1111);
		info.Tesselated = (bool)(hash >> 14 & 0b1);
		info.TwoSided = (bool)(hash >> 15 & 0b1);
		info.blendedMaterial = (bool)(hash >> 16 & 0b1);
		info.wireframe = (bool)(hash >> 17 & 0b1);

		return info;
	}


}