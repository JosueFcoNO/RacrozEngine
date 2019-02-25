#pragma once

namespace rczEngine
{
	class SceneManager
	{
	private:
		static SceneManager*& _Instance();

	public:
		RZ_EXP static void Start();
		RZ_EXP static SceneManager* Pointer();
		RZ_EXP static void ShutDown();

		RZ_EXP StrPtr<Scene> CreateDefaultScene(const String& name);
		RZ_EXP StrPtr<Scene> CreateEmptyScene(const String& name);
		RZ_EXP StrPtr<Scene> GetActiveScene();

		RZ_EXP StrPtr<Scene> LoadScene(const String&filePath);
		RZ_EXP void SaveScene(const String& filePath);

	private:
		StrPtr<Resource> getNewResource(SERIALS serials);

		StrPtr<Scene> m_ActiveScene;
		Vector<StrPtr<Scene>> m_SavedScenes;

	};
}