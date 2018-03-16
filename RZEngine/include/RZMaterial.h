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

		TEX_EMISSIVE = 10,

		TEX_HEIGHT = 11
	};

	struct MaterialCore
	{
		Vector4 m_Diffuse;
		Vector4 m_Ambient;
		Vector3 m_Specular;
		float m_SpecularStrength;
		float g_Subsurface;
		float g_Specular;
		float g_SpecularTint;
		float g_Anisotropic;
		float g_Sheen;
		float g_SheenTint;
		float g_Clearcoat;
		float g_ClearcoatGloss;
	};

	class RZ_UTILITY_EXPORT Material : public Resource
	{
	public:
		~Material() { Release(); };
		void InitMaterial(MATERIAL_TYPE mat_type, Gfx::GfxCore* gfx = NULL);
		void SetThisMaterial(Gfx::GfxCore* gfx = NULL, void* res = NULL);

		void SetTexture(TEXTURE_TYPE texType, ResourceHandle rHandle);

		void SetTextures(ResourceHandle albedo = INVALID_RESOURCE, ResourceHandle normals = INVALID_RESOURCE, ResourceHandle metallic = INVALID_RESOURCE, ResourceHandle roughness = INVALID_RESOURCE, ResourceHandle AO = INVALID_RESOURCE, ResourceHandle em = INVALID_RESOURCE, ResourceHandle h = INVALID_RESOURCE);

		void SetAttributes(Vector4 diffuseColor, Vector4 ambientColor, Vector3 specularColor, float specularPower);
		void SetDisneyAttributes(float specularStrength, float subsurface, float specular, float specularTint, float anisotropic, float sheen, float sheenTint, float clearCoat, float clearCoatGloss);

		virtual void Load(const char* filePath, const char* resName, bool addToResourceManager = true);
		virtual void Release() { m_MaterialCB.Destroy(); };

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

			if (ImGui::Button("SpecSmooth"))
			{
				m_MatType = MAT_PBR_SpecSmooth;
			}

			if (ImGui::Button("SpecSmoothAlpha"))
			{
				m_MatType = MAT_PBR_SpecSmoothAlpha;
			}

			auto ptrRes = ResVault::Pointer();
			auto gfx = Gfx::GfxCore::Pointer();

			/// ALBEDO ///

			ResourceHandle Albedo = INVALID_RESOURCE;

			(m_TextureAlbedo != -1) ? Albedo = m_TextureAlbedo : Albedo = ptrRes->m_WhiteTex;
			
			if (ImGui::ImageButton(ptrRes->GetResource<Texture2D>(Albedo).lock().get()->m_TextureCore.m_ShaderResource,
				ImVec2(150, 150)))
			{
				m_TextureAlbedo = LoadFile("Albedo Texture", (m_Name + String("_Albedo")).c_str(), ptrRes);
			}

			/// NORMALS ///

			ResourceHandle Normals = INVALID_RESOURCE;

			(m_TextureNormal != -1) ? Normals = m_TextureNormal : Normals = ptrRes->m_NormalTex;
			
			ImGui::SameLine();
			if (ImGui::ImageButton(ptrRes->GetResource<Texture2D>(Normals).lock().get()->m_TextureCore.m_ShaderResource,
				ImVec2(150, 150)))
			{
				m_TextureNormal = LoadFile("Normals Texture", (m_Name + String("_Normals")).c_str(), ptrRes);
			}
			
			if (m_MatType != MAT_PBR_SpecSmoothAlpha)
			{

				/// ROUGHNESS ///

				ResourceHandle Roughness = INVALID_RESOURCE;

				(m_TextureRoughSmooth != -1) ? Roughness = m_TextureRoughSmooth : Roughness = ptrRes->m_GreyTex;
				
				ImGui::SameLine();
				if (ImGui::ImageButton(ptrRes->GetResource<Texture2D>(Roughness).lock().get()->m_TextureCore.m_ShaderResource,
					ImVec2(150, 150)))
				{
					m_TextureRoughSmooth = LoadFile("Roughness Texture", (m_Name + String("_Roughness")).c_str(), ptrRes);
				}
			}

			/// METALNESS ///

			ResourceHandle Metallic = INVALID_RESOURCE;

			(m_TextureMetSpec != -1) ? Metallic = m_TextureMetSpec : Metallic = ptrRes->m_BlackTex;

			if (ImGui::ImageButton(ptrRes->GetResource<Texture2D>(Metallic).lock().get()->m_TextureCore.m_ShaderResource,
				ImVec2(150, 150)))
			{
				m_TextureMetSpec = LoadFile("Metallic Texture", (m_Name + String("_Metallic")).c_str(), ptrRes);
			}

			/// AO ///

			ResourceHandle Ao = INVALID_RESOURCE;

			(m_TextureAO != -1) ? Ao = m_TextureAO : Ao = ptrRes->m_WhiteTex;

			ImGui::SameLine();
			if (ImGui::ImageButton(ptrRes->GetResource<Texture2D>(Ao).lock().get()->m_TextureCore.m_ShaderResource,
				ImVec2(150, 150)))
			{
				m_TextureAO = LoadFile("Ao Texture", (m_Name + String("_AO")).c_str(), ptrRes);
			}

			/// EM ///

			ResourceHandle Em = INVALID_RESOURCE;

			(m_TextureEm != -1) ? Em = m_TextureEm : Em = ptrRes->m_BlackTex;

			ImGui::SameLine();
			if (ImGui::ImageButton(ptrRes->GetResource<Texture2D>(Em).lock().get()->m_TextureCore.m_ShaderResource,
				ImVec2(150, 150)))
			{
				m_TextureEm = LoadFile("Em Texture", (m_Name + String("_EM")).c_str(), ptrRes);
			}

			if (m_MatType == MAT_PBR_TESS)
			{
				/// HEIGHT ///

				ResourceHandle He = INVALID_RESOURCE;

				(m_TextureH != -1) ? He = m_TextureH : He = ptrRes->m_GreyTex;

				if (ImGui::ImageButton(ptrRes->GetResource<Texture2D>(He).lock().get()->m_TextureCore.m_ShaderResource,
					ImVec2(150, 150)))
				{
					m_TextureH = LoadFile("Height Texture", (m_Name + String("_He")).c_str(), ptrRes);
				}
			}
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

		MATERIAL_TYPE m_MatType;

	private:
		Gfx::ConstantBuffer m_MaterialCB;

	};
};