#pragma once

namespace rczEngine
{
	typedef Vector<KeyFrame> KeyFrameVector;
	typedef Map<String, KeyFrameVector> AnimationTimeline;

	class RZ_UTILITY_EXPORT Animation : public Resource
	{
	public:
		void AddKeyFrame(KeyFrame& keyFrame, String boneName);
		KeyFrameVector& GetChannel(String boneName);
		KeyFrame GetKeyFrame(String& boneName, float time);
		KeyFrame GetNextKeyFrame(String& boneName, float time);

		virtual void Load(const char* filePath, const char* resName, bool addToResourceManager = true);
		virtual void Release() { m_Timeline.clear(); };

#ifndef RZ_EDITOR
		virtual void RenderResourceGUI()
		{
			ImGui::Text("Animation Resource");
		}
#endif

		AnimationTimeline m_Timeline;
		float m_Duration;
		float m_TicksPerSecond;
	};
}