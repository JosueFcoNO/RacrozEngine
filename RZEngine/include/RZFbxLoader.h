#pragma once
#include "fbxsdk.h"
//TODO: ver si tengo que encerrar en un ifdef de plataforma ese include.

namespace rczEngine
{
	class RZ_EXP FbxLoader : public ModelLoader
	{
	public:
		virtual StrPtr<Model> LoadModel(const char* filePath);
		virtual bool LoadModelWithHierarchy(StrGameObjectPtr gameObject, const char* filePath) { return false; };
		virtual bool LoadSkinnedModel(StrGameObjectPtr gameObject, const char* filePath) { return false; };

	private:
		bool AddNodeMesh(FbxNode* pNode, const char* parent);

		fbxsdk::FbxManager* m_Manager;

		ResVault* m_Res = NULL;
		Gfx::GfxCore* m_Gfx = NULL;
	};
}