#include <RZEditorPCH.h>

namespace rczEngine
{
	void BaseWindow::InitWindow(const String && name, int width, int height)
	{
		m_Width = 0;
		m_Height = 0;
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
		if (m_Width != 0 && m_Height != 0 && m_FirstOpen)
		{
			m_FirstOpen = false;
			ImGui::SetNextWindowSize(ImVec2(m_Width, m_Height));
		}

		ImGui::Begin(m_Name.c_str(), &m_Open);
	}

	void BaseWindow::EndRender()
	{
		ImGui::End();
	}
}