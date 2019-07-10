#include <RZEnginePCH.h>

namespace rczEngine
{
	void GUIConsole::InitWindow()
	{
	}

	void GUIConsole::RenderWindow()
	{
		ImGui::Begin("Console");
		{
			auto& ptr = Logger::Pointer()->GetLoggedStrings();

			for (int32 i = ptr.size() - 1; i > -1; --i)
			{
				ImGui::Text("%s", ptr[i].GetString().c_str());
			}
		}
	}
}