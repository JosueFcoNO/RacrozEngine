#include <RZEditorPCH.h>

namespace rczEngine
{
	void to_json(nlohmann::json& j, const ProjectRecord& p)
	{
		j = nlohmann::json{ {"name", p.Name}, {"path", p.Path} };
	}

	void from_json(const nlohmann::json& j, ProjectRecord& p)
	{
		j.at("name").get_to(p.Name);
		j.at("path").get_to(p.Path);
	}

	void EditorSettings::InitEditorSettings()
	{
		m_ProjectFilesScores.clear();

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
		std::ifstream i(m_SettingsFile);
		nlohmann::json inJson;
		i >> inJson;

		if (inJson.contains("ProjectsKnown"))
		for (auto element : inJson["ProjectsKnown"])
		{
			m_ProjectFilesScores[element["name"]] = (element);
		}

	}

	void EditorSettings::SaveEditorSettings()
	{
		FileStream o(m_SettingsFile, FileStream::out);

		nlohmann::json outJson;

		outJson["ProjectsKnown"] = m_ProjectFilesScores;

		o << outJson;
	}
};