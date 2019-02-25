#pragma once

namespace rczEngine
{
	class RZ_EXP ModelLoader
	{
	public:
		virtual StrPtr<Model> LoadModel(const String& filePath) = 0;
		virtual bool LoadModelWithHierarchy(StrGameObjectPtr gameObject, const String& filePath) = 0;
		virtual bool LoadSkinnedModel(StrGameObjectPtr gameObject, const String& filePath) = 0;

	private:

	};
}