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
		Vector3 GetViewDir();
		Vector3 GetPosition();
		float GetNearPlane();
		float GetFarPlane();

#ifndef RZ_EDITOR
		virtual void RenderComponent()
		{
			ImGui::Separator();
			ImGui::Text("Camera Component");

			m_FarClip = m_CameraCore.GetFarClip();
			m_NearClip = m_CameraCore.GetNearClip();			

			if (m_Perspective)
			{
				m_Fov = m_CameraCore.GetFov();
				m_AspectRatio = m_CameraCore.GetAspectRatio();
				ImGui::DragFloat("FOV", &m_Fov, 1.0f, 10.0f, 180.0f);
				ImGui::DragFloat("Aspect Ratio", &m_AspectRatio, 0.05f);
				m_CameraCore.SetFov(m_Fov);
				m_CameraCore.SetAspectRatio(m_AspectRatio);
			}
			else
			{
				m_Width = m_CameraCore.GetWidth();
				m_Height = m_CameraCore.GetHeight();
				ImGui::DragFloat("Width", &m_Width, 10.0f, 10.0f, 2000.0f);
				ImGui::DragFloat("Height", &m_Height, 10.0f, 10.0f, 2000.0f);
				m_CameraCore.SetHeight(m_Height);
				m_CameraCore.SetWidth(m_Width);
			}
			ImGui::DragFloat("Far Clip", &m_FarClip, 10.0f, 1.0f, 10000.0f);
			ImGui::DragFloat("Near Clip", &m_NearClip, 0.0005f, 0.0005f, 1000.0f);
			ImGui::Checkbox("Perspective", &m_Perspective);

			
			m_CameraCore.SetFarClip(m_FarClip);
			m_CameraCore.SetNearClip(m_NearClip);
			m_CameraCore.SetPerspective(m_Perspective);
		}

		float m_Fov = 0.0f;
		float m_AspectRatio = 0.0f;
		float m_FarClip = 0.0f;
		float m_NearClip = 0.0f;
		float m_Width = 0.0f;
		float m_Height = 0.0f;
		bool m_Perspective = true;
#endif

		Camera m_CameraCore;

	protected:
		void CreateCamera();

		WeakPtr<DebuggerLineList> lines;
	};
}