#pragma once

namespace rczEngine
{
	class RZ_EXP EditorSettings
	{
	public:
		void InitEditorSettings();

		void LoadEditorSettings();
		
		void SaveEditorSettings();

	private:
		Vector<String> m_ProjectFilesPaths;

	};
};