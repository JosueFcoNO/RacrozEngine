#pragma once

namespace rczEngine
{
	class RZ_EXP ConsoleWindow : public BaseWindow
	{
	public:

	protected:
		virtual void Render() override
		{
			auto& ptr = Logger::Pointer()->GetLoggedStrings();

			for (int32 i = ptr.size() - 1; i > -1; --i)
			{
				ImGui::Text("%s", ptr[i].GetString().c_str());
			}
		};
	};
}
