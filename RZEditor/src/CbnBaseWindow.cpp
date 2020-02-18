#include <RZEditorPCH.h>

namespace rczEngine
{
	void BaseWindow::InitWindow(const String && name)
	{
		m_Name = std::move(name);
	}

	void BaseWindow::UpdateWindow(float deltaTime)
	{
	}

	void BaseWindow::RenderWindow()
	{
		ConfigRender();
		BeginRender();
		Render();
		EndRender();
	}

	void BaseWindow::ConfigRender()
	{

	}

	void BaseWindow::BeginRender()
	{
		ImGui::Begin(m_Name.c_str());
	}

	void BaseWindow::EndRender()
	{
		ImGui::End();
	}
}