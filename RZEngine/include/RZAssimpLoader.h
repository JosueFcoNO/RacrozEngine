#pragma once
#include "fbxsdk.h"
//TODO: ver si tengo que encerrar en un ifdef de plataforma ese include.

namespace rczEngine
{
	class RZ_EXP AssimpLoader : public ModelLoader
	{
	public:
		virtual StrPtr<Model> LoadModel(const String& filePath);
		virtual bool LoadModelWithHierarchy(StrGameObjectPtr gameObject, const String& filePath);
		virtual bool LoadSkinnedModel(StrGameObjectPtr gameObject, const String& filePath);

	private:
		bool AddNodeMesh(aiNode* pNode, const String& parent, bool addMeshes, bool addBones = false);
		void LoadMaterials();
		ResourceHandle LoadMesh(aiNode* pNode, bool bones = false);

		void AddVertices(Gfx::VertexBuffer<Gfx::Vertex>& vertexBuffer, aiMesh * mesh);
		void AddVertices(Gfx::VertexBuffer<Gfx::SkinnedVertex>& vertexBuffer, aiMesh * mesh);

		void AddIndices(Gfx::IndexBuffer& indexBuffer, aiMesh * mesh);

		void BuildSkeleton(StrPtr<SkinnedModel> skin);
		void AddBones(StrPtr<SkinnedModel> model, aiMesh * mesh, uint32 vertexOffset);
		void AddBoneToSkeleton(StrPtr<SkinnedModel> model, aiNode* pNode, Bone* parentBone);
		void CopyMatrix(Matrix4& dest, aiMatrix4x4& src);

		Assimp::Importer* m_Manager = NULL;
		const aiScene* m_Scene = NULL;
		
		ResVault* m_Res = NULL;
		Gfx::GfxCore* m_Gfx = NULL;

		Map<String, ResourceHandle> materialMap;
		Path m_FilePath;

		Map<String, aiBone*> Bones;
		Map<String, StrPtr<BoneComponent>> BoneComponents;
		Map<String, int> BoneIndex;
		Vector<String> BoneNames;
	};
}