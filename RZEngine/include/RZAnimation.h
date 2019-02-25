#pragma once

namespace rczEngine
{
	typedef Vector<KeyFrame> KeyFrameVector;
	typedef Map<String, KeyFrameVector> AnimationTimeline; //TODO: turn animationTimeline into a class of its own?

	class Animation : public Resource
	{
	public:
		RZ_EXP void AddKeyFrame(KeyFrame& keyFrame, String boneName);
		RZ_EXP KeyFrameVector& GetChannel(String boneName);
		RZ_EXP KeyFrame GetKeyFrame(String& boneName, float time);
		RZ_EXP KeyFrame GetNextKeyFrame(String& boneName, float time);

		RZ_EXP virtual void Load(const String& filePath, const String& resName);
		RZ_EXP virtual void Release() { m_Timeline.clear(); };

		RZ_EXP virtual void Serialize();
		RZ_EXP virtual void DeSerialize();

#ifndef RZ_EDITOR
		RZ_EXP virtual void RenderResourceGUI()
		{
			ImGui::Text("Animation Resource");
		}
#endif

		AnimationTimeline m_Timeline;
		float m_Duration;
		float m_TicksPerSecond;
	};
}