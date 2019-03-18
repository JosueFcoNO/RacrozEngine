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

			m_Fov = m_CameraCore.GetFov();
			m_AspectRatio = m_CameraCore.GetAspectRatio();
			m_FarClip = m_CameraCore.GetFarClip();
			m_NearClip = m_CameraCore.GetNearClip();

			ImGui::DragFloat("FOV", &m_Fov, 1.0f, 10.0f, 180.0f);
			ImGui::DragFloat("Aspect Ratio", &m_AspectRatio, 0.05f);
			ImGui::DragFloat("Far Clip", &m_FarClip, 10.0f, 1.0f, 1000.0f);
			ImGui::DragFloat("Near Clip", &m_NearClip, 0.0005f, 0.0005f, 1000.0f);

			m_CameraCore.SetFov(m_Fov);
			m_CameraCore.SetAspectRatio(m_AspectRatio);
			m_CameraCore.SetFarClip(m_FarClip);
			m_CameraCore.SetNearClip(m_NearClip);

			//static bool once = false;
			//
			//Vector<Vector3> vertices =
			//{
			//	m_CameraCore.m_Frustum.NearPoints[0],
			//	m_CameraCore.m_Frustum.NearPoints[1],
			//	m_CameraCore.m_Frustum.NearPoints[2],
			//	m_CameraCore.m_Frustum.NearPoints[3],
			//	m_CameraCore.m_Frustum.FarPoints[0],
			//	m_CameraCore.m_Frustum.FarPoints[1],
			//	m_CameraCore.m_Frustum.FarPoints[2],
			//	m_CameraCore.m_Frustum.FarPoints[3]
			//};
			//
			//Vector<uint32> indices =
			//{
			//	0, 1,
			//	0, 3,
			//	1, 2,
			//	2, 3,
			//
			//	4, 5,
			//	4, 7,
			//	5, 6,
			//	6, 7,
			//
			//	0, 4,
			//	1, 5,
			//	2, 6,
			//	3, 7
			//};
			//
			//if (!once)
			//{
			//	lines = GraphicDebugger::Pointer()->AddLineListIndex("Frustum", vertices, indices, Color(1, 1, 1, 1), -1.0f);
			//	once = true;
			//}
			//else
			//{
			//	if (!lines.expired())
			//		lines.lock()->SetLineListIndex(vertices, indices);
			//}

		}

		float m_Fov = 0.0f;
		float m_AspectRatio = 0.0f;
		float m_FarClip = 0.0f;
		float m_NearClip = 0.0f;
#endif

		Camera m_CameraCore;

	protected:
		void CreateCamera();

		WeakPtr<DebuggerLineList> lines;
	};
}