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

	ImGui::SetCursorPos(imagePos);
	ImGui::Image(m_LinkedRenderPipeline.lock()->GetFinalRenderTarget().lock()->GetTextureCore()->m_ShaderResource, windowSize);

}
