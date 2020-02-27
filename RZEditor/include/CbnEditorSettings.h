#pragma once

namespace rczEngine
{
	class RZ_EXP EditorSettings
	{
	public:
		void InitEditorSettings();

		void LoadEditorSettings();
		
		void SaveEditorSettings();

		Vector<String> m_ProjectFilesPaths;


		String m_SettingsFile = "EditorSettings.cbn";

	};
};