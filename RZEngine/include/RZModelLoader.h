#pragma once

namespace rczEngine
{
	class RZ_EXP ModelLoader
	{
	public:
		virtual StrPtr<Model> LoadModel(const char* filePath) = 0;
		virtual bool LoadModelWithHierarchy(StrGameObjectPtr gameObject, const char* filePath) = 0;
		virtual bool LoadSkinnedModel(StrGameObjectPtr gameObject, const char* filePath) = 0;

	private:

	};
}