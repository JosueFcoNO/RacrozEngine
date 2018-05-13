#pragma once

namespace rczEngine
{
	class RZ_UTILITY_EXPORT ModelLoader
	{
	public:
		virtual StrPtr<Model> LoadModel(const char* filePath) = 0;
		virtual bool LoadModelWithHierarchy(StrGameObjectPtr gameObject, const char* filePath) = 0;
		virtual StrPtr<SkinnedModel> LoadSkinnedModel(const char* filePath) = 0;

	private:

	};
}