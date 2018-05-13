#pragma once
namespace rczEngine
{
	///A class Mesh that contains Gfx::Vertex and Index buffers.
	class RZ_UTILITY_EXPORT Model : public Resource
	{
	public:
		Model() { m_Type = ResourceType::RES_3DMODEL; };
		void DrawModel(Gfx::GfxCore* gfx, void* res, Map<String, ResourceHandle>* materialOverride, MATERIAL_TYPE matType = MAT_ANY);

		virtual void Serialize();
		virtual void DeSerialize();
		
		virtual void Load(const char* filePath, const char* resName);
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