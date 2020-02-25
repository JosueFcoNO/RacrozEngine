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

	auto pass = m_LinkedRenderPipeline.lock()->GetPass(m_CurrentPass);

	if (ImGui::Button("Next Pass"))
	{
		m_CurrentPass++;
		m_CurrentRT = 0;
	}
	
	ImGui::SameLine();

	if (!pass) return;

	int size = pass->GetRenderTargetCount();

	if (ImGui::Button("Next RT"))
	{
		m_CurrentRT = (m_CurrentRT + 1) % size;
	}

	ImGui::SetCursorPos(imagePos);

	auto getRT = pass->GetRenderTarget(m_CurrentRT);

	ImGui::SetNextWindowBgAlpha(1);
	ImGui::Image(getRT->GetTextureCore()->m_ShaderResource, windowSize);

}
