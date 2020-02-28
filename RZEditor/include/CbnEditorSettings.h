#pragma once

namespace rczEngine
{
	class ProjectRecord
	{
	public:
		ProjectRecord() {};

		ProjectRecord(const String& name, const StringW& path) { Name = name; Path = path; };

		String Name;
		StringW Path;
	};

	class RZ_EXP EditorSettings
	{
	public:
		void InitEditorSettings();

		void LoadEditorSettings();
		
		void SaveEditorSettings();

		Map<String, ProjectRecord> m_ProjectFilesScores;

		String m_SettingsFile = "EditorSettings.cbn";

	};
};