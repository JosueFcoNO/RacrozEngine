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
			Vector<LogObject> ptr = Logger::Pointer()->GetLoggedStrings();

			for (size_t i = ptr.size() - 1; i > -1; --i)
			{
				ImGui::Text("%s", ptr[i].GetString().c_str());
			}
		}
	}
}