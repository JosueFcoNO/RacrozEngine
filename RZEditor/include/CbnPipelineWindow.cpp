#include "RZEditorPCH.h"

void rczEngine::PipelineWindow::Render()
{
	if (m_LinkedRenderPipeline.expired()) return;

	auto windowSize = ImVec2(ImGui::GetWindowSize());

	const auto targetY = windowSize.x * 1080 / 1920;
	const auto targetX = windowSize.y * 1920 / 1080;

	if (windowSize.y > targetY)
		windowSize.y = targetY;

	if (windowSize.x > targetX)
		windowSize.x = targetX;

	ImVec2 imagePos = ImGui::GetWindowSize();
	imagePos.x -= windowSize.x;
	imagePos.y -= windowSize.y;

	imagePos.x *= 0.5f;
	imagePos.y *= 0.5f;

	imagePos.y += 40;

	const auto pipeline = m_LinkedRenderPipeline.lock();
	auto pass = pipeline->GetPass(m_CurrentPass);
	const auto passNum = pipeline->GetPassesNum();
	const auto& passID = pass->m_Name;

	if (ImGui::Button("Next Pass"))
	{
		m_CurrentPass++;

		m_CurrentPass %= passNum;

		m_CurrentRT = 0;
	}
	
	ImGui::SameLine();

	if (!pass) return;

	int size = pass->GetRenderTargetCount();

	if (ImGui::Button("Next RT"))
	{
		m_CurrentRT = (m_CurrentRT + 1) % size;
	}

	if (ImGui::Button("Final Pass"))
	{
		m_CurrentPass = passNum - 1;
		m_CurrentRT = 0;
	}

	auto getRT = pass->GetRenderTarget(m_CurrentRT);

	ImGui::Text(passID.c_str());
	ImGui::Text(getRT->m_Name.c_str());

	ImGui::SetCursorPos(imagePos);

	ImGui::SetNextWindowBgAlpha(1);
	ImGui::Image(getRT->GetTextureCore()->m_ShaderResource, windowSize);

}
