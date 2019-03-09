#pragma once

namespace rczEngine
{
	enum { MATERIAL_SLOT = 4 };

	enum MATERIAL_TYPE
	{
		MAT_PBR_MetRough,
		MAT_PBR_MetRoughAlpha,
		MAT_PBR_MetRoughBlend,

		MAT_PBR_SpecSmooth,
		MAT_PBR_SpecSmooth_Alpha,
		MAT_PBR_SpecSmoothBlend,

		MAT_PBR_MetRough_Tess,
		MAT_PBR_MetRough_Trans,
		MAT_PBR_SpecSmooth_Tess,
		MAT_PBR_SpecSmooth_Trans,
		MAT_PLANET,
		MAT_ANY,
		MAT_UI
	};

	enum TEXTURE_TYPE
	{
		TEX_ALBEDO = 0,

		TEX_NORMAL = 1,
		TEX_NORMAL_SMOOTH = 1,

		TEX_MET_SPEC = 2,
		TEX_METALLIC = 2,
		TEX_SPECULAR = 2,

		TEX_ROUGH_SMOOTH = 3,
		TEX_ROUGHNESS = 3,
		TEX_SMOOTHNESS = 3,

		TEX_AMBIENT_OCCLUSION = 4,

		TEX_OPACITY = 6,

		TEX_EMISSIVE = 10,

		TEX_HEIGHT = 11
	};

	struct MaterialCore
	{
		Vector3 g_Albedo;
		float OverrideAlbedo = 0.0f;

		Vector3 g_Specular;
		float OverrideMetallicSpecular = 0.0f;

		Vector3 g_Emmisive;
		float OverriveEmmisive = 0.0f;

		float g_RoughGloss = 0.0f;
		float g_Metallic = 0.0f;
		float g_Opacity = 1.0f;
		float g_TesselationFactor = 0.0f;

		float OverrideOpacity = 0.0f;
		float AOStrength = 1.0f;
		float tileX = 1.0f;
		float tileY = 1.0f;

		float OverrideNormal = 0.0f;
		float OverrideRoughGloss = 0.0f;
		float g_SpecularTint = 0.0f;
		float g_SheenTint = 0.0f;

		float g_Anisotropic = 0.0f;
		float g_Sheen = 0.0f;
		float g_ClearcoatGloss = 0.0f;
		float g_Subsurface = 0.0f;

		float g_Clearcoat = 0.0f;
		float padding[3];
	};

	class RZ_EXP Material : public Resource
	{
	public:
		Material() { m_Type = ResourceType::RES_MATERIAL; };
		~Material() { Release(); };
		void InitMaterial(MATERIAL_TYPE mat_type, Gfx::GfxCore* gfx = NULL);
		void SetThisMaterial(Gfx::GfxCore* gfx = NULL, void* res = NULL);

		void SetTexture(TEXTURE_TYPE texType, ResourceHandle rHandle);
		void SetTextures(ResourceHandle albedo = INVALID_RESOURCE, ResourceHandle normals = INVALID_RESOURCE, ResourceHandle metallic = INVALID_RESOURCE, ResourceHandle roughness = INVALID_RESOURCE, ResourceHandle AO = INVALID_RESOURCE, ResourceHandle em = INVALID_RESOURCE, ResourceHandle h = INVALID_RESOURCE);

		void SetAttributesMet(Vector4 albedo, float metallic, float roughness);
		void SetAttributesSpec(Vector4 albedo, Vector4 specular, float glossiness);
		void SetOpacity(float opacity);
		void SetTesselationFactor(float tess);

		void SetPlanetMaterial();

		virtual void Load(const String& filePath, const String& resName);
		virtual void Release() { m_MaterialCB.Destroy(); };

		virtual void Serialize();
		///DeSerializes the material and fills it up. Don't call InitMaterial on a material deserialized.
		virtual void DeSerialize();

#ifndef RZ_EDITOR
		virtual void RenderResourceGUI()
		{
			ImGui::Text("Material Resource");

			if (ImGui::Button("MetRough"))
			{
				m_MatType = MAT_PBR_MetRough;
			}
			ImGui::SameLine();
			if (ImGui::Button("Transparent"))
			{
				m_MatType = MAT_PBR_MetRough_Trans;
			}
			ImGui::SameLine();
			if (ImGui::Button("Tess"))
			{
				m_MatType = MAT_PBR_MetRough_Tess;
			}

			if (ImGui::Button("SpecGloss"))
			{
				m_MatType = MAT_PBR_SpecSmooth;
			}
			ImGui::SameLine();
			if (ImGui::Button("Alpha"))
			{
				m_MatType = MAT_PBR_SpecSmooth_Alpha;
			}
			ImGui::SameLine();
			if (ImGui::Button("Transparent"))
			{
				m_MatType = MAT_PBR_SpecSmooth_Trans;
			}

			auto ptrRes = ResVault::Pointer();

			ImVec2 texSize(100, 100);

			/// ALBEDO ///

			ImGui::PushID(TEX_ALBEDO);

			ResourceHandle Albedo;
			(m_TextureAlbedo != INVALID_RESOURCE) ? Albedo = m_TextureAlbedo : Albedo = ptrRes->m_WhiteTex;
			if (ImGui::ImageButton(ptrRes->GetResource<Texture2D>(Albedo).lock().get()->m_TextureCore.m_ShaderResource,
				texSize))
			{
				m_TextureAlbedo = LoadFile("Albedo Texture", (m_Name + String("_Albedo")).c_str(), ptrRes);
			}
			ImGui::PopID();

			/// NORMALS ///
			ImGui::SameLine();

			ImGui::PushID(TEX_NORMAL);

			ResourceHandle Normals;
			(m_TextureNormal != INVALID_RESOURCE) ? Normals = m_TextureNormal : Normals = ptrRes->m_NormalTex;
			if (ImGui::ImageButton(ptrRes->GetResource<Texture2D>(Normals).lock().get()->m_TextureCore.m_ShaderResource,
				texSize))
			{
				m_TextureNormal = LoadFile("Normals Texture", (m_Name + String("_Normals")).c_str(), ptrRes);
			}

			ImGui::PopID();

			ImGui::PushID(TEX_ROUGHNESS);
			
			if (m_MatType != MAT_PBR_SpecSmooth_Alpha)
			{
				ImGui::SameLine();

				/// ROUGHNESS ///
				ResourceHandle RoughGloss;
				(m_TextureRoughSmooth != INVALID_RESOURCE) ? RoughGloss = m_TextureRoughSmooth : RoughGloss = ptrRes->m_GreyTex;
				if (ImGui::ImageButton(ptrRes->GetResource<Texture2D>(RoughGloss).lock().get()->m_TextureCore.m_ShaderResource,
					texSize))
				{
					m_TextureRoughSmooth = LoadFile("RoughGloss Texture", (m_Name + String("_Roughness")).c_str(), ptrRes);
				}
			}

			ImGui::PopID();

			ImGui::PushID(TEX_MET_SPEC);

			/// METALNESS ///
			ResourceHandle MetSpec;
			(m_TextureMetSpec != INVALID_RESOURCE) ? MetSpec = m_TextureMetSpec : MetSpec = ptrRes->m_BlackTex;
			if (ImGui::ImageButton(ptrRes->GetResource<Texture2D>(MetSpec).lock().get()->m_TextureCore.m_ShaderResource,
				texSize))
			{
				m_TextureMetSpec = LoadFile("MetSpec Texture", (m_Name + String("_Metallic")).c_str(), ptrRes);
			}

			ImGui::PopID();

			ImGui::PushID(TEX_AMBIENT_OCCLUSION);

			/// AO ///

			ResourceHandle Ao;

			(m_TextureAO != INVALID_RESOURCE) ? Ao = m_TextureAO : Ao = ptrRes->m_WhiteTex;

			ImGui::SameLine();
			if (ImGui::ImageButton(ptrRes->GetResource<Texture2D>(Ao).lock().get()->m_TextureCore.m_ShaderResource,
				texSize))
			{
				m_TextureAO = LoadFile("Ao Texture", (m_Name + String("_AO")).c_str(), ptrRes);
			}

			ImGui::PopID();

			ImGui::PushID(TEX_EMISSIVE);

			/// EM ///

			ImGui::SameLine();

			ResourceHandle Em;
			(m_TextureEm != INVALID_RESOURCE) ? Em = m_TextureEm : Em = ptrRes->m_BlackTex;
			if (ImGui::ImageButton(ptrRes->GetResource<Texture2D>(Em).lock().get()->m_TextureCore.m_ShaderResource,
				texSize))
			{
				m_TextureEm = LoadFile("Em Texture", (m_Name + String("_EM")).c_str(), ptrRes);
			}

			ImGui::PopID();

			ImGui::PushID(TEX_HEIGHT);

			if (m_MatType == MAT_PBR_MetRough_Tess || m_MatType == MAT_PBR_SpecSmooth_Tess)
			{
				/// HEIGHT ///
				ResourceHandle He;
				(m_TextureH != INVALID_RESOURCE) ? He = m_TextureH : He = ptrRes->m_GreyTex;
				if (ImGui::ImageButton(ptrRes->GetResource<Texture2D>(He).lock().get()->m_TextureCore.m_ShaderResource,
					texSize))
				{
					m_TextureH = LoadFile("Height Texture", (m_Name + String("_He")).c_str(), ptrRes);
				}
			}

			ImGui::PopID();

			ImGui::PushID(TEX_OPACITY);

			if (m_MatType == MAT_PBR_MetRough_Trans || m_MatType == MAT_PBR_SpecSmooth_Trans)
			{
				/// OPACITY ///
				ResourceHandle O;
				(m_TextureO != INVALID_RESOURCE) ? O = m_TextureO : O = ptrRes->m_WhiteTex;
				if (ImGui::ImageButton(ptrRes->GetResource<Texture2D>(O).lock().get()->m_TextureCore.m_ShaderResource,
					texSize))
				{
					m_TextureO = LoadFile("Opacity Texture", (m_Name + String("_Opa")).c_str(), ptrRes);
				}
			}

			ImGui::PopID();

			ImGui::ColorEdit3("Albedo", (float*)&m_core.g_Albedo);
			ImGui::DragFloat("Override Albedo", &m_core.OverrideAlbedo, 0.05f, 0.0f, 1.0f);

			ImGui::DragFloat("Rough/Gloss", &m_core.g_RoughGloss, 0.05f, 0.0f, 1.0f);
			ImGui::DragFloat("Override Rough/Gloss", &m_core.OverrideRoughGloss, 0.05f, 0.0f, 1.0f);

			if (m_MatType == MAT_PBR_MetRough || 
				m_MatType == MAT_PBR_MetRough_Tess || 
				m_MatType == MAT_PBR_MetRough_Trans ||
				m_MatType == MAT_PBR_MetRoughAlpha ||
				m_MatType == MAT_PBR_MetRoughBlend)
			{
				ImGui::DragFloat("Metallic", &m_core.g_Metallic, 0.05f, 0.0f, 1.0f);
				ImGui::DragFloat("Override Metallic", &m_core.OverrideMetallicSpecular, 0.05f, 0.0f, 1.0f);
			}
			else 
				if (m_MatType == MAT_PBR_SpecSmooth || 
					m_MatType == MAT_PBR_SpecSmooth_Alpha ||
					m_MatType == MAT_PBR_SpecSmooth_Tess ||
					m_MatType == MAT_PBR_SpecSmooth_Trans ||
					m_MatType == MAT_PBR_SpecSmoothBlend)
			{
				ImGui::ColorEdit3("Specular", (float*)&m_core.g_Specular);
				ImGui::DragFloat("Override Spec", &m_core.OverrideMetallicSpecular, 0.05f, 0.0f, 1.0f);
			}

			ImGui::ColorEdit3("Emmisive", (float*)&m_core.g_Emmisive);
			ImGui::DragFloat("Override Emmisive", &m_core.OverriveEmmisive, 0.05f, 0.0f, 1.0f);
			
			ImGui::DragFloat("Opacity", &m_core.g_Opacity, 0.05f, 0.0f, 1.0f);
			//ImGui::DragFloat("Normal Strength", &m_core.OverrideNormal, 0.05f, 0.0f, 1.0f);
			//ImGui::DragFloat("SpecularTint", &m_core.g_SpecularTint, 0.05f, 0.0f, 1.0f);
			//ImGui::DragFloat("SheenTint", &m_core.g_SheenTint, 0.05f, 0.0f, 1.0f);
			//ImGui::DragFloat("Anisotropic", &m_core.g_Anisotropic, 0.05f, 0.0f, 1.0f);
			//ImGui::DragFloat("Sheen", &m_core.g_Sheen, 0.05f, 0.0f, 1.0f);
			//ImGui::DragFloat("ClearCoatGloss", &m_core.g_ClearcoatGloss, 0.05f, 0.0f, 1.0f);
			//ImGui::DragFloat("Subsurface", &m_core.g_Subsurface, 0.05f, 0.0f, 1.0f);
			//ImGui::DragFloat("Clearcoat", &m_core.g_Clearcoat, 0.05f, 0.0f, 1.0f);

			ImGui::DragFloat("AO Strength", &m_core.AOStrength, 0.05f, 0.0f, 1.0f);

			ImGui::DragFloat("Tile X", &m_core.tileX, 0.1f, 0.0f, 32.0f);
			ImGui::DragFloat("Tile Y", &m_core.tileY, 0.1f, 0.0f, 32.0f);

		}
#endif

		MaterialCore m_core;

		ResourceHandle m_TextureAlbedo = INVALID_RESOURCE;
		ResourceHandle m_TextureNormal = INVALID_RESOURCE;
		ResourceHandle m_TextureMetSpec = INVALID_RESOURCE;
		ResourceHandle m_TextureRoughSmooth = INVALID_RESOURCE;
		ResourceHandle m_TextureAO = INVALID_RESOURCE;
		ResourceHandle m_TextureEm = INVALID_RESOURCE;
		ResourceHandle m_TextureH = INVALID_RESOURCE;
		ResourceHandle m_TextureO = INVALID_RESOURCE;

		MATERIAL_TYPE m_MatType;

	private:
		Gfx::ConstantBuffer m_MaterialCB;

	};
};