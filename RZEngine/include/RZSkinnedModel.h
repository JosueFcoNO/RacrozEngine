#pragma once

namespace rczEngine
{
	///Contains an animation, a skeleton and a vector of SkinnedMeshes.
	class RZ_EXP SkinnedModel : public Resource
	{
	public:
		SkinnedModel() { m_Type = ResourceType::RES_SKINNEDMODEL; };
		~SkinnedModel() { Release(); };

		void DrawModel(Gfx::GfxCore* gfx, void* res, Map<String, ResourceHandle>* materialOverride, MATERIAL_TYPE matType = MAT_ANY);

		void Load(const String& filePath, const String& resName);
		void Release() { m_MeshSkeleton.DestroySkeletonBuffers(); m_VertexBuffer.Destroy(); m_IndexBuffer.Destroy(); };

		virtual void Serialize();
		virtual void DeSerialize();


#ifndef RZ_EDITOR
		virtual void RenderResourceGUI()
		{
			ImGui::Text("Skinned Model Resource");
		}
#endif

		Gfx::VertexBuffer<Gfx::SkinnedVertex> m_VertexBuffer;
		Gfx::IndexBuffer m_IndexBuffer;

		Vector<SkinnedMesh> m_VectorMeshes;
		Map<String, ResourceHandle> m_MaterialMap;

		ResourceHandle m_SetAnimation = INVALID_RESOURCE;

		Skeleton m_MeshSkeleton;

	private:
		Vector<Texture2D*> LoadTextures(const String& filePath);

		Vector<StrPtr<Animation>> LoadAnimations(char* filePath);

		Vector<Material*> LoadMaterials(const String& filePath);
	};
}