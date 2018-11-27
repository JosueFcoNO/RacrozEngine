#pragma once

namespace rczEngine
{
	class RZ_EXP CameraCmp : public Component, public std::enable_shared_from_this<CameraCmp>
	{
	public:
		CameraCmp()
		{
		};

		virtual ~CameraCmp()
		{
			Destroy();
		};

		virtual void Init()
		{
			CreateCamera();
		}
		void Destroy();

		static const ComponentType s_ComponentType = CMP_CAMERA;
		virtual ComponentType GetComponentType() { return CameraCmp::s_ComponentType; };
		virtual ComponentId GetComponentID() { return m_ID; };

		virtual void Serialize();
		virtual void DeSerialize();

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
			ImGui::DragFloat("Near Clip", &m_CameraCore.m_NearClip, 0.05f, 0.001f, 1000.0f);

		}
#endif

		Camera m_CameraCore;

	protected:
		void CreateCamera();

	};
}