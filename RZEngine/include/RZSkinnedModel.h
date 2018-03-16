#pragma once

namespace rczEngine
{
	///Contains an animation, a skeleton and a vector of SkinnedMeshes.
	class RZ_UTILITY_EXPORT SkinnedModel : public Resource
	{
	public:
		SkinnedModel() {};
		~SkinnedModel() { Release(); };

		void DrawModel(Gfx::GfxCore* gfx, void* res, Map<String, ResourceHandle>* materialOverride, MATERIAL_TYPE matType = MAT_ANY);

		void Load(const char* filePath, const char* resName, bool addToResourceManager = true);
		void Release() { m_MeshSkeleton.DestroySkeletonBuffers(); m_VertexBuffer.Destroy(); m_IndexBuffer.Destroy(); };

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
		Vector<Texture2D*> LoadTextures(char* filePath);

		Vector<Animation*> LoadAnimations(char* filePath);

		Vector<Material*> LoadMaterials(char* filePath);
	};
}