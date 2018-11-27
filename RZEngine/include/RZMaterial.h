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
		MAT_PBR_SpecSmoothAlpha,
		MAT_PBR_SpecSmoothBlend,

		MAT_DISNEY_PBR,
		MAT_PBR_TESS,
		MAT_PBR_TRANSPARENT,
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
		float paddings[2];
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

		virtual void Load(const char* filePath, const char* resName);
		virtual void Release() { m_MaterialCB.Destroy(); };

		virtual void Serialize();
		///DeSerializes the material and fills it up. Don't call InitMaterial on a material deserialized.
		virtual void DeSerialize();

#ifndef RZ_EDITOR
		virtual void RenderResourceGUI()
		{
			ImGui::Text("Material Resource");

			if (ImGui::Button("Tess"))
			{
				m_MatType = MAT_PBR_TESS;
			}

			if (ImGui::Button("MetRough"))
			{
				m_MatType = MAT_PBR_MetRough;
			}

			if (ImGui::Button("Transparent"))
			{
				m_MatType = MAT_PBR_TRANSPARENT;
			}

			auto ptrRes = ResVault::Pointer();

			ImVec2 texSize(100, 100);

			/// ALBEDO ///

			ResourceHandle Albedo;

			(m_TextureAlbedo != INVALID_RESOURCE) ? Albedo = m_TextureAlbedo : Albedo = ptrRes->m_WhiteTex;
			
			if (ImGui::ImageButton(ptrRes->GetResource<Texture2D>(Albedo).lock().get()->m_TextureCore.m_ShaderResource,
				texSize))
			{
				m_TextureAlbedo = LoadFile("Albedo Texture", (m_Name + String("_Albedo")).c_str(), ptrRes);
			}

			/// NORMALS ///

			ResourceHandle Normals;

			(m_TextureNormal != INVALID_RESOURCE) ? Normals = m_TextureNormal : Normals = ptrRes->m_NormalTex;
			
			ImGui::SameLine();
			if (ImGui::ImageButton(ptrRes->GetResource<Texture2D>(Normals).lock().get()->m_TextureCore.m_ShaderResource,
				texSize))
			{
				m_TextureNormal = LoadFile("Normals Texture", (m_Name + String("_Normals")).c_str(), ptrRes);
			}
			
			if (m_MatType != MAT_PBR_SpecSmoothAlpha)
			{

				/// ROUGHNESS ///

				ResourceHandle Roughness;

				(m_TextureRoughSmooth != INVALID_RESOURCE) ? Roughness = m_TextureRoughSmooth : Roughness = ptrRes->m_GreyTex;
				
				ImGui::SameLine();
				if (ImGui::ImageButton(ptrRes->GetResource<Texture2D>(Roughness).lock().get()->m_TextureCore.m_ShaderResource,
					texSize))
				{
					m_TextureRoughSmooth = LoadFile("Roughness Texture", (m_Name + String("_Roughness")).c_str(), ptrRes);
				}
			}

			/// METALNESS ///

			ResourceHandle Metallic;

			(m_TextureMetSpec != INVALID_RESOURCE) ? Metallic = m_TextureMetSpec : Metallic = ptrRes->m_BlackTex;

			if (ImGui::ImageButton(ptrRes->GetResource<Texture2D>(Metallic).lock().get()->m_TextureCore.m_ShaderResource,
				texSize))
			{
				m_TextureMetSpec = LoadFile("Metallic Texture", (m_Name + String("_Metallic")).c_str(), ptrRes);
			}

			/// AO ///

			ResourceHandle Ao;

			(m_TextureAO != INVALID_RESOURCE) ? Ao = m_TextureAO : Ao = ptrRes->m_WhiteTex;

			ImGui::SameLine();
			if (ImGui::ImageButton(ptrRes->GetResource<Texture2D>(Ao).lock().get()->m_TextureCore.m_ShaderResource,
				texSize))
			{
				m_TextureAO = LoadFile("Ao Texture", (m_Name + String("_AO")).c_str(), ptrRes);
			}

			/// EM ///

			ResourceHandle Em;

			(m_TextureEm != INVALID_RESOURCE) ? Em = m_TextureEm : Em = ptrRes->m_BlackTex;

			ImGui::SameLine();
			if (ImGui::ImageButton(ptrRes->GetResource<Texture2D>(Em).lock().get()->m_TextureCore.m_ShaderResource,
				texSize))
			{
				m_TextureEm = LoadFile("Em Texture", (m_Name + String("_EM")).c_str(), ptrRes);
			}

			if (m_MatType == MAT_PBR_TESS)
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

			if (m_MatType == MAT_PBR_TRANSPARENT)
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

			ImGui::ColorEdit3("Albedo", (float*)&m_core.g_Albedo);
			ImGui::DragFloat("Override Albedo", &m_core.OverrideAlbedo, 0.05f, 0.0f, 1.0f);

			if (m_MatType == MAT_PBR_MetRough || m_MatType == MAT_PBR_TESS)
			{
				ImGui::DragFloat("Roughness", &m_core.g_RoughGloss, 0.05f, 0.0f, 1.0f);
				ImGui::DragFloat("Override Rough", &m_core.OverrideRoughGloss, 0.05f, 0.0f, 1.0f);

				ImGui::DragFloat("Metallic", &m_core.g_Metallic, 0.05f, 0.0f, 1.0f);
				ImGui::DragFloat("Override Met", &m_core.OverrideMetallicSpecular, 0.05f, 0.0f, 1.0f);
			}
			else if (m_MatType == MAT_PBR_SpecSmooth)
			{
				ImGui::ColorEdit3("Specular", (float*)&m_core.g_Specular);
				ImGui::DragFloat("Override Spec", &m_core.OverrideMetallicSpecular, 0.05f, 0.0f, 1.0f);

				ImGui::DragFloat("Glossiness", &m_core.g_RoughGloss, 0.05f, 0.0f, 1.0f);
				ImGui::DragFloat("Override Gloss", &m_core.OverrideRoughGloss, 0.05f, 0.0f, 1.0f);
			}

			ImGui::ColorEdit3("Emmisive", (float*)&m_core.g_Emmisive);
			ImGui::DragFloat("Override Emmisive", &m_core.OverriveEmmisive, 0.05f, 0.0f, 1.0f);
			
			ImGui::DragFloat("Opacity", &m_core.g_Opacity, 0.05f, 0.0f, 1.0f);
			ImGui::DragFloat("Normal Strength", &m_core.OverrideNormal, 0.05f, 0.0f, 1.0f);

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