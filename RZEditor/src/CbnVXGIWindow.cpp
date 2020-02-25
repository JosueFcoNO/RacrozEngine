#include <RZEditorPCH.h>

namespace rczEngine
{
	void VXGIWindow::Render()
	{
		auto& vxgi = RacrozRenderer::Pointer()->m_VXGI;

		ImGui::Combo("Visualization Mode", (int*)&m_RenderingMode, "NORMAL\0DIFFUSE_CHANNEL\0SPECULAR_CHANNEL\0OPACITY_VOXELS\0EMITTANCE_VOXELS\0IRRADIANCE_VOXELS\0");
		vxgi.g_RenderingMode = m_RenderingMode;

		if (ImGui::Button("Voxelize Scene"))
		{
			RacrozRenderer::Pointer()->PrepareGI();
		}

		if (ImGui::DragFloat("Voxel Size", &g_fVoxelSize, 0.1f, 1, 64.0f))
		{
			vxgi.g_fVoxelSize = g_fVoxelSize;
		}

		if (ImGui::DragFloat("Diffuse Scale", &g_fDiffuseScale, 0.1f, 0, 30.0f))
		{
			vxgi.g_fDiffuseScale = g_fDiffuseScale;
		}

		if (ImGui::DragFloat("Specular Scale", &g_fSpecularScale, 0.1f, 0, 30.0f))
		{
			vxgi.g_fSpecularScale = g_fSpecularScale;
		}

		if (ImGui::DragFloat("Ambient Scale", &g_fVxaoScale, 0.1f, 0, 30.0f))
		{
			vxgi.g_fVxaoScale = g_fVxaoScale;
		}

		if (ImGui::DragFloat("Ambient Range", &g_fVxaoRange, 20.0f, 20.0f, 1024.0f))
		{
			vxgi.g_fVxaoRange = g_fVxaoRange;
		}

		if (ImGui::DragFloat("SSAO Range", &g_ssaoRadius, 5.0f, 2.0f, 1000.0f))
		{
			vxgi.g_ssaoRadius = g_ssaoRadius;
		}

		if (ImGui::Checkbox("Multi Bounce", &g_bEnableMultiBounce))
		{
			vxgi.g_bEnableMultiBounce = g_bEnableMultiBounce;
		}

		if (ImGui::DragFloat("MultiBounce Scale", &g_fMultiBounceScale, 0.1f, 0.1f, 30.0f))
		{
			vxgi.g_fMultiBounceScale = g_fMultiBounceScale;
		}

		if (ImGui::DragFloat("Sampling Rate", &g_fSamplingRate, 0.1f, 0.1f, 30.0f))
		{
			vxgi.g_fSamplingRate = g_fSamplingRate;
		}

		if (ImGui::DragFloat("Quality", &g_fQuality, 0.1f, 1.0f, 30.0f))
		{
			vxgi.g_fQuality = g_fQuality;
		}

		if (ImGui::DragInt("Map Size", &g_nMapSize, 1, 1, 30))
		{
			vxgi.g_nMapSize = g_nMapSize;
		}

		if (ImGui::Checkbox("Temp Filtering", &g_bTemporalFiltering))
		{
			vxgi.g_bTemporalFiltering = g_bTemporalFiltering;
		}
		///	bool g_bEnableGI = true;
	}
}