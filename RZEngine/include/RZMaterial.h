#pragma once

namespace rczEngine
{
	enum { MATERIAL_SLOT = 4 };

	enum class eMaterialType
	{
		PBR_MetRough,
		PBR_SpecSmooth,
		BlinnPhong,
		PlanetSurface,
		PlanetAtmosphere,
		UserInterface,
		AnyMaterial
	};

	enum class eShadingType
	{
		Unlit,
		PBR,
		PBRClearCoat,
		BlinnPhong,
		Foliage,
		Subsurface
	};

	enum class eBlendType
	{
		Opaque,
		Translucent,
		Masked,
		Additive
	};

	///Two sided
	///Blend Material
	///Wireframe
	///Refraction
	///Tesselation

	enum class eTextureType
	{
		BaseColor = 0,

		Normals = 1,
		NormalSmooth = 1,

		MetallicSpecular = 2,
		Metallic = 2,
		Specular = 2,

		RoughGloss = 3,
		Roughness = 3,
		Gloss = 3,

		AmbientOcclusion = 4,

		OpacityMask = 6,

		Emissive = 10,

		HeightMap = 11
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
		float g_TesselationScale = 1.0f;
		float pad2 = 0.0f;

		float pad3 = 0.0f;
		float pad4 = 0.0f;
		float pad5 = 0.0f;
		float pad6 = 0.0f;

		float pad7 = 0.0f;
		float padding[3];
	};

	

	class RZ_EXP Material : public Resource
	{
	public:
		Material() { m_Type = ResourceType::RES_MATERIAL; };
		~Material() { Release(); };
		void InitMaterial(eMaterialType mat_type);
		void SetThisMaterial();

		void SetTexture(eTextureType texType, ResourceHandle rHandle);
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

		void DrawTextureSlot(eTextureType ID, const String& name, ResourceHandle& texture, const ResourceHandle& defaultTex)
		{
			static const ImVec2 texSize(100, 100);

			ImGui::PushID((int)ID);

			ImGui::Separator();
			ImGui::LabelText("", name.c_str());

			ResourceHandle handle;
			(texture != INVALID_RESOURCE) ? handle = texture : handle = defaultTex;

			auto currentResource = ResVault::Pointer()->GetResource<Texture2D>(handle).lock();

			if (ImGui::ImageButton(currentResource->GetTextureCore()->m_ShaderResource, texSize))
			{
				texture = LoadFile(name + " Texture", (m_Name + String("_" + name)).c_str(), ResVault::Pointer());
			}

			ImGui::SameLine();

			ImGui::LabelText("", currentResource->GetFilePath().c_str());

			ImGui::PopID();
		}

		virtual void RenderResourceGUI()
		{
			ImGui::Text("Material Resource");

			ImGui::Combo("Material Type", (int*)(&m_MatType), "Met Rough\0Spec Smooth\0BlinnPhong\0Planet Surface\0Planet Atmosphere\0User Interface", 6);
			ImGui::Combo("Shading Type", (int*)(&m_ShadingType), "Unlit\0PBR\0PBR ClearCoat\0BlinnPhong\0Foliage\0Subsurface");
			ImGui::Combo("Blend Type", (int*)(&m_BlendType), "Opaque\0Translucent\0Masked\0Additive");
			
			ImGui::Checkbox("Blend Material", &m_TwoMaterialBlend);
			ImGui::Checkbox("Tesselation", &m_Tesselation);
			ImGui::Checkbox("Two Sided", &m_TwoSided);

			///Wireframe
			///Refraction

			auto ptrRes = ResVault::Pointer();
			

			/// ALBEDO ///

			DrawTextureSlot(eTextureType::BaseColor, "Base Color", m_TextureAlbedo, ptrRes->m_WhiteTex);
			ImGui::ColorEdit3("Base Color", (float*)&m_core.g_Albedo, ImGuiColorEditFlags_::ImGuiColorEditFlags_Uint8);
			ImGui::DragFloat("Override Base Color", &m_core.OverrideAlbedo, 0.05f, 0.0f, 1.0f);

			/// NORMALS ///

			if (m_ShadingType != eShadingType::Unlit)
			{
				DrawTextureSlot(eTextureType::Normals, "Normals", m_TextureNormal, ptrRes->m_NormalTex);

				switch (m_MatType)
				{
				case rczEngine::eMaterialType::PBR_MetRough:
					DrawTextureSlot(eTextureType::Roughness, "Roughness", m_TextureRoughSmooth, ptrRes->m_GreyTex);
					ImGui::DragFloat("Rough", &m_core.g_RoughGloss, 0.05f, 0.0f, 1.0f);
					ImGui::DragFloat("Override Rough", &m_core.OverrideRoughGloss, 0.05f, 0.0f, 1.0f);

					DrawTextureSlot(eTextureType::Metallic, "Metallic", m_TextureMetSpec, ptrRes->m_BlackTex);
					ImGui::DragFloat("Metallic", &m_core.g_Metallic, 0.05f, 0.0f, 1.0f);
					ImGui::DragFloat("Override Metallic", &m_core.OverrideMetallicSpecular, 0.05f, 0.0f, 1.0f);
					break;
				case rczEngine::eMaterialType::PBR_SpecSmooth:
					DrawTextureSlot(eTextureType::Gloss, "Gloss", m_TextureRoughSmooth, ptrRes->m_GreyTex);
					ImGui::DragFloat("Gloss", &m_core.g_RoughGloss, 0.05f, 0.0f, 1.0f);
					ImGui::DragFloat("Override Gloss", &m_core.OverrideRoughGloss, 0.05f, 0.0f, 1.0f);

					DrawTextureSlot(eTextureType::Specular, "Specular", m_TextureMetSpec, ptrRes->m_BlackTex);
					ImGui::ColorEdit3("Specular", (float*)&m_core.g_Specular, ImGuiColorEditFlags_::ImGuiColorEditFlags_Uint8);
					ImGui::DragFloat("Override Spec", &m_core.OverrideMetallicSpecular, 0.05f, 0.0f, 1.0f);
					break;
				default:
					break;
				}
			}

			/// AO ///

			DrawTextureSlot(eTextureType::AmbientOcclusion, "AmbientOcclusion", m_TextureAO, ptrRes->m_WhiteTex);
			ImGui::DragFloat("AO Strength", &m_core.AOStrength, 0.05f, 0.0f, 1.0f);

			/// EM ///

			DrawTextureSlot(eTextureType::Emissive, "Emissive", m_TextureEm, ptrRes->m_BlackTex);

			/// Height ///

			if (m_Tesselation)
				DrawTextureSlot(eTextureType::HeightMap, "Height", m_TextureH, ptrRes->m_GreyTex);

			/// Opacity ///

			if (m_BlendType != eBlendType::Opaque)
			{
				DrawTextureSlot(eTextureType::OpacityMask, "Opacity", m_TextureO, ptrRes->m_GreyTex);
				ImGui::DragFloat("Opacity", &m_core.g_Opacity, 0.05f, 0.0f, 1.0f);
			}

			if (m_ShadingType != eShadingType::Unlit)
			{
				ImGui::ColorEdit3("Emmisive", (float*)&m_core.g_Emmisive, ImGuiColorEditFlags_::ImGuiColorEditFlags_Float | ImGuiColorEditFlags_::ImGuiColorEditFlags_HDR);
				ImGui::DragFloat("Override Emmisive", &m_core.OverriveEmmisive, 0.05f, 0.0f, 1.0f);
			}

			ImGui::DragFloat("Tile X", &m_core.tileX, 0.1f, 0.0f, 32.0f);
			ImGui::DragFloat("Tile Y", &m_core.tileY, 0.1f, 0.0f, 32.0f);

		}
#endif

		MaterialCore m_core;

		ResourceHandle m_TextureAlbedo;
		ResourceHandle m_TextureNormal;
		ResourceHandle m_TextureMetSpec;
		ResourceHandle m_TextureRoughSmooth;
		ResourceHandle m_TextureAO;
		ResourceHandle m_TextureEm;
		ResourceHandle m_TextureH;
		ResourceHandle m_TextureO;

		eMaterialType m_MatType;
		eShadingType m_ShadingType;
		eBlendType m_BlendType;

		bool m_TwoMaterialBlend = false;
		bool m_TwoSided = false;
		bool m_Tesselation = false;

	private:
		Gfx::ConstantBuffer m_MaterialCB;

	};


	class RZ_EXP MaterialRenderInfo
	{
	public:

		void InitFromMaterial(const Material& material)
		{
			blendType = material.m_BlendType;
			shading = material.m_ShadingType;
			matType = material.m_MatType;
			Tesselated = material.m_Tesselation;
			TwoSided = material.m_TwoSided;
			blendedMaterial = material.m_TwoMaterialBlend;
			wireframe = false;
		};

		static int CalculateRenderHash(
			eComponentID componentID,
			eMaterialType matType,
			eShadingType shading,
			eBlendType blendType,
			bool Tesselated,
			bool TwoSided,
			bool blendedMaterial,
			bool wireframe);

		static int CalculateRenderHash(MaterialRenderInfo rInfo);

		static MaterialRenderInfo DecodeRenderHash(int hash);

		eComponentID componentID;
		eMaterialType matType;
		eShadingType shading;
		eBlendType blendType;
		bool Tesselated;
		bool TwoSided;
		bool blendedMaterial;
		bool wireframe;
	};
};