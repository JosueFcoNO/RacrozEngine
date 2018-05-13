#pragma once
#include "fbxsdk.h"
//TODO: ver si tengo que encerrar en un ifdef de plataforma ese include.

namespace rczEngine
{
	class RZ_UTILITY_EXPORT FbxLoader : public ModelLoader
	{
	public:
		virtual bool LoadModel();
		virtual bool LoadModelWithHierarchy() { return false; };
		virtual bool LoadSkinnedModel() { return false; };

	private:
		bool AddNodeMesh(FbxNode* pNode, const char* parent);

		fbxsdk::FbxManager* m_Manager;
	};
}