#pragma once

namespace rczEngine
{
	class RZ_EXP SceneManager
	{
	private:
		static SceneManager*& _Instance();

	public:
		static void Start();
		static SceneManager* Pointer();
		static void ShutDown();

		StrPtr<Scene> CreateDefaultScene(const char* name);
		StrPtr<Scene> CreateEmptyScene(const char* name);
		StrPtr<Scene> GetActiveScene();

		StrPtr<Scene> LoadScene(const char* filePath);
		void SaveScene(const char* filePath);

	private:
		StrPtr<Resource> getNewResource(SERIALS serials);

		StrPtr<Scene> m_ActiveScene;
		Vector<StrPtr<Scene>> m_SavedScenes;

	};
}