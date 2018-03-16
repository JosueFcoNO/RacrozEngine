#pragma once
namespace rczEngine
{
	///A class Mesh that contains Gfx::Vertex and Index buffers.
	class RZ_UTILITY_EXPORT Model : public Resource
	{
	public:
		Model() {};
		void DrawModel(Gfx::GfxCore* gfx, void* res, Map<String, ResourceHandle>* materialOverride, MATERIAL_TYPE matType = MAT_ANY);
		
		virtual void Load(const char* filePath, const char* resName, bool addToResourceManager);
		virtual void Release() { m_VertexBuffer.Destroy(); m_IndexBuffer.Destroy(); m_VectorMeshes.clear(); };
		
		Gfx::VertexBuffer<Gfx::Vertex> m_VertexBuffer;
		Gfx::IndexBuffer m_IndexBuffer;

		Vector<Mesh> m_VectorMeshes;
		Map<String, ResourceHandle> m_MaterialMap;

#ifndef RZ_EDITOR
		virtual void RenderResourceGUI()
		{
			ImGui::Text("Model Resource");
		}
#endif

	};
}