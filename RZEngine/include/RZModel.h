#pragma once
namespace rczEngine
{
	///A class Mesh that contains Gfx::Vertex and Index buffers.
	class Model : public Resource
	{
	public:
		RZ_EXP Model() { m_Type = ResourceType::RES_3DMODEL; };
		RZ_EXP void DrawModel(Map<String, ResourceHandle>* materialOverride, int renderHash = -1);

		RZ_EXP virtual void Serialize();
		RZ_EXP virtual void DeSerialize();
		
		RZ_EXP virtual void Load(const String& filePath, const String& resName);
		RZ_EXP virtual void Release() { m_VertexBuffer.Destroy(); m_IndexBuffer.Destroy(); m_VectorMeshes.clear(); };
		
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