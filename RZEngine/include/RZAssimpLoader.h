#pragma once
#include "fbxsdk.h"
//TODO: ver si tengo que encerrar en un ifdef de plataforma ese include.

namespace rczEngine
{
	class RZ_UTILITY_EXPORT AssimpLoader : public ModelLoader
	{
	public:
		virtual StrPtr<Model> LoadModel(const char* filePath);
		virtual bool LoadModelWithHierarchy(StrGameObjectPtr gameObject, const char* filePath);
		virtual StrPtr<SkinnedModel> LoadSkinnedModel(const char* filePath);

	private:
		bool AddNodeMesh(aiNode* pNode, const char* parent);
		void LoadMaterials(Map<String, ResourceHandle>& materialMap);

		Assimp::Importer* m_Manager = NULL;
		const aiScene* m_Scene = NULL;
		ResVault* m_Res = NULL;
	};
}