#include <RZEnginePCH.h>

namespace rczEngine
{
	void Material::InitMaterial(MATERIAL_TYPE mat_type, Gfx::GfxCore* gfx)
	{
		m_MaterialCB.CreateConstantBuffer(sizeof(MaterialCore), Gfx::USAGE_DEFAULT, gfx);

		SetAttributes(Vector4(1, 1, 1, 1), Vector4(1, 1, 1, 1), Vector3(1, 1, 1), .5f);
		SetDisneyAttributes(0.5f, 0.0f, 0.8f, 0.4f, 0.0f, 0.0f, 0.0f, 0.1f, 0.0f);

		m_MatType = mat_type;
	}

	void Material::SetThisMaterial(Gfx::GfxCore* gfx, void* res)
	{
		ResVault* Res = (ResVault*)res;

		//if (gfx->m_SetMaterial == m_Handle)
		//{
		//	return;
		//}

		if (m_Handle != INVALID_RESOURCE)
		{
			gfx->m_SetMaterial = m_Handle;

			m_MaterialCB.UpdateConstantBuffer(&m_core, gfx);
			m_MaterialCB.SetBufferInPS(MATERIAL_SLOT, gfx);

			if (m_TextureAlbedo != -1)
			{
				Res->GetResource<Texture2D>(m_TextureAlbedo).lock()->SetThisTextureInPS(TEX_ALBEDO, 1, gfx);
			}
			else
			{
				Res->GetResource<Texture2D>(Res->m_WhiteTex).lock()->SetThisTextureInPS(TEX_ALBEDO, 1, gfx);
			}

			if (m_TextureNormal != -1)
			{
				Res->GetResource<Texture2D>(m_TextureNormal).lock()->SetThisTextureInPS(TEX_NORMAL, 1, gfx);
			}
			else
			{
				Res->GetResource<Texture2D>(Res->m_NormalTex).lock()->SetThisTextureInPS(TEX_NORMAL, 1, gfx);
			}

			if (m_TextureMetSpec != -1)
			{
				Res->GetResource<Texture2D>(m_TextureMetSpec).lock()->SetThisTextureInPS(TEX_MET_SPEC, 1, gfx);
			}
			else
			{
				if (m_MatType == MAT_PBR_MetRough || m_MatType == MAT_PBR_TESS)
					Res->GetResource<Texture2D>(Res->m_BlackTex).lock()->SetThisTextureInPS(TEX_MET_SPEC, 1, gfx);
				else
					Res->GetResource<Texture2D>(Res->m_GreyTex).lock()->SetThisTextureInPS(TEX_MET_SPEC, 1, gfx);
			}

			if (m_MatType != MAT_PBR_SpecSmoothAlpha)
			{
				if (m_TextureRoughSmooth != -1)
				{
					Res->GetResource<Texture2D>(m_TextureRoughSmooth).lock()->SetThisTextureInPS(TEX_ROUGH_SMOOTH, 1, gfx);
				}
				else
				{
					Res->GetResource<Texture2D>(Res->m_GreyTex).lock()->SetThisTextureInPS(TEX_ROUGH_SMOOTH, 1, gfx);
				}
			}

			if (m_TextureAO != -1)
			{
				Res->GetResource<Texture2D>(m_TextureAO).lock()->SetThisTextureInPS(TEX_AMBIENT_OCCLUSION, 1, gfx);
			}
			else
			{
				Res->GetResource<Texture2D>(Res->m_WhiteTex).lock()->SetThisTextureInPS(TEX_AMBIENT_OCCLUSION, 1, gfx);
			}

			if (m_TextureEm != -1)
			{
				Res->GetResource<Texture2D>(m_TextureEm).lock()->SetThisTextureInPS(TEX_EMISSIVE, 1, gfx);
			}
			else
			{
				Res->GetResource<Texture2D>(Res->m_BlackTex).lock()->SetThisTextureInPS(TEX_EMISSIVE, 1, gfx);
			}

			if (m_MatType == MATERIAL_TYPE::MAT_PBR_TESS)
			{
				if (m_TextureH != -1)
				{
					Res->GetResource<Texture2D>(m_TextureH).lock()->SetThisTextureInDS(0, 1, gfx);
				}
				else
				{
					Res->GetResource<Texture2D>(Res->m_GreyTex).lock()->SetThisTextureInDS(0, 1, gfx);
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
		}
	}


	void Material::Load(const char* filePath, const char* resName, bool addToResourceManager)
	{
		m_Name = resName;

		///Create an Importer and Read the file in fileName
		Assimp::Importer B;
		const aiScene* Scene = B.ReadFile(filePath, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);

		if (addToResourceManager)
		{
			ResVault::Pointer()->InsertResource(this);
		}
		else
		{
			m_Handle = INVALID_RESOURCE;
		}

		auto ptr = Gfx::GfxCore::Pointer();

		aiColor3D Color = { 0,0,0 };
		Scene->mMaterials[0]->Get(AI_MATKEY_COLOR_DIFFUSE, Color);
		m_core.m_Diffuse.m_x = Color.r;
		m_core.m_Diffuse.m_y = Color.g;
		m_core.m_Diffuse.m_z = Color.b;

		Scene->mMaterials[0]->Get(AI_MATKEY_COLOR_AMBIENT, Color);
		m_core.m_Ambient.m_x = Color.r;
		m_core.m_Ambient.m_y = Color.g;
		m_core.m_Ambient.m_z = Color.b;

		Scene->mMaterials[0]->Get(AI_MATKEY_COLOR_SPECULAR, Color);
		m_core.m_Specular.m_x = Color.r;
		m_core.m_Specular.m_y = Color.g;
		m_core.m_Specular.m_z = Color.b;

		m_core.m_SpecularStrength = 0;
		Scene->mMaterials[0]->Get(AI_MATKEY_SHININESS_STRENGTH, m_core.m_SpecularStrength);

	}

	void Material::SetAttributes(Vector4 diffuseColor, Vector4 ambientColor, Vector3 specularColor, float specularPower)
	{
		m_core.m_Diffuse = diffuseColor;
		m_core.m_Ambient = ambientColor;
		m_core.m_Specular = specularColor;
		m_core.m_SpecularStrength = specularPower;
	}

	void Material::SetDisneyAttributes(float specularStrength, float subsurface, float specular, float specularTint, float anisotropic, float sheen, float sheenTint, float clearCoat, float clearCoatGloss)
	{
		m_core.g_Anisotropic = anisotropic;
		m_core.g_Clearcoat = clearCoat;
		m_core.g_ClearcoatGloss = clearCoatGloss;
		m_core.g_Sheen = sheen;
		m_core.g_SheenTint = sheenTint;
		m_core.g_Specular = specular;
		m_core.g_SpecularTint = specularTint;
		m_core.g_Subsurface = subsurface;
	}
}