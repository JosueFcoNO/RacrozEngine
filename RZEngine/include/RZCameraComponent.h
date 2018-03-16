#pragma once

namespace rczEngine
{
	class RZ_UTILITY_EXPORT CameraCmp : public Component, public std::enable_shared_from_this<CameraCmp>
	{
	public:
		CameraCmp() 
		{
		};

		static const ComponentType s_ComponentType = CMP_CAMERA;
		virtual ComponentType GetComponentType() { return CameraCmp::s_ComponentType; };
		virtual ComponentId GetComponentID() { return m_ID; };

		Matrix4 GetViewMatrix();
		Matrix4 GetProjMatrix();
		Vector3 GetOrientation();
		Vector3 GetPosition();
		float GetNearPlane();
		float GetFarPlane();

#ifndef RZ_EDITOR
		virtual void RenderComponent()
		{
			ImGui::Separator();
			ImGui::Text("Camera Component");

			ImGui::DragFloat("FOV", &m_CameraCore.m_Fov, 1.0f, 10.0f, 180.0f);
			ImGui::DragFloat("Aspect Ratio", &m_CameraCore.m_AspectRatio, 0.05f);
			ImGui::DragFloat("Far Clip", &m_CameraCore.m_FarClip, 10.0f, 1.0f, 1000.0f);
			ImGui::DragFloat("Near Clip", &m_CameraCore.m_NearClip, 10.0f, 1.0f, 1000.0f);

		}
#endif

	protected:
		void CreateCamera();

		Camera m_CameraCore;
	};
}