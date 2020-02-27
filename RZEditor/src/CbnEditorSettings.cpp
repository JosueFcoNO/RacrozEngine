#include <RZEditorPCH.h>

namespace rczEngine
{
	void EditorSettings::InitEditorSettings()
	{
		m_ProjectFilesPaths.clear();

		if (!Path::FileExists(m_SettingsFile))
		{
			SaveEditorSettings();
		}
		else
		{
			LoadEditorSettings();
		}
	}

	void EditorSettings::LoadEditorSettings()
	{
		std::ifstream i("EditorSettings.cbn");
		nlohmann::json inJson;
		i >> inJson;

		if (inJson.contains("ProjectsKnown"))
		for (auto element : inJson["ProjectsKnown"])
		{
			m_ProjectFilesPaths.push_back(element);
		}

	}

	void EditorSettings::SaveEditorSettings()
	{
		FileStream o("EditorSettings.cbn", FileStream::out);

		nlohmann::json outJson;

		outJson["ProjectsKnown"] = m_ProjectFilesPaths;

		o << outJson;
	}
};