#include <RZEnginePCH.h>

namespace rczEngine
{
	void Octree::InsertModel(GameObject* gameObj)
	{
		auto modelPtr = gameObj->GetComponent<ModelRenderer>(CMP_MODEL_RENDERER).lock();
		auto model = ResVault::Pointer()->GetResource<Model>(modelPtr->m_Model).lock();

		for (int i = 0; i < model->m_VectorMeshes.size(); ++i)
		{
			const auto& currentMesh = model->m_VectorMeshes[i];
			const auto& meshCore = model->m_VectorMeshes[i].m_MeshCore;   

			for (int k = 0; k < meshCore.m_IndexCount; k += 3)
			{
				auto i1 = model->m_IndexBuffer.GetIndex(meshCore.m_IndexOffset + i);
				auto i2 = model->m_IndexBuffer.GetIndex(meshCore.m_IndexOffset + i + 1);
				auto i3 = model->m_IndexBuffer.GetIndex(meshCore.m_IndexOffset + i + 2);

				const auto& v1 = model->m_VertexBuffer.GetVertex(meshCore.m_VertexOffset + i1);
				const auto& v2 = model->m_VertexBuffer.GetVertex(meshCore.m_VertexOffset + i2);
				const auto& v3 = model->m_VertexBuffer.GetVertex(meshCore.m_VertexOffset + i3);

				OctreeTriangle newTriangle;
				newTriangle.V1 = v1;
				newTriangle.V2 = v2;
				newTriangle.V3 = v3;

				const auto materialHandle = model->m_MaterialMap[currentMesh.m_Material];

				newTriangle.Material = materialHandle;

				PreProcessedData[materialHandle].push_back(newTriangle);
			}
		}

	}
}