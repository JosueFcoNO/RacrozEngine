#include <RZEditorPCH.h>

namespace rczEngine
{
	void EditorSettings::InitEditorSettings()
	{
		m_ProjectFilesPaths.clear();

		LoadEditorSettings();
	}

	void EditorSettings::LoadEditorSettings()
	{
		nlohmann::json json;
		json["Test"]["Possible"] = 1444;

		FileStream o("EditorSettings.cbn", FileStream::in);
		o << std::setw(4) << json << std::endl;

	}
};