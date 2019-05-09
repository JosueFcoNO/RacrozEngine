#pragma once
#include <RZGraphicDebuggerPoint.h>

namespace rczEngine
{
	class Animation;

	class RZ_EXP Bone
	{
	public:
		Bone() : m_OffsetMatrix(eInit::Unit), m_JointMatrix(eInit::Unit), m_AccumulatedMatrix(eInit::Unit)
		{

		};

		~Bone()
		{

		};

		void InitBone(StrPtr<BoneComponent> boneCmp)
		{
			m_BoneComponent = boneCmp;
		};

		void AddBoneChildren(Bone* child);
		void SetParent(Bone* parent);

		void UpdateAccumulatedMatrix(Vector<Matrix4>& matrixPalette);
		void UpdateChildren(Vector<Matrix4>& matrixPalette);
		void InterpolateBone(const KeyFrame* k0, const KeyFrame* k1, float time);

		void InterpolateBoneWithAnim(float time, WeakPtr<Animation> anim);
		void InterpolateChildren(float time, WeakPtr<Animation> anim);

		String m_Name;
		int32 m_BoneIndex = -1;
		Matrix4 m_OffsetMatrix;
		Matrix4 m_JointMatrix;
		Matrix4 m_TransformMatrix;

		Vector<Bone*> m_ChildrenBones;

		bool m_RealBone = false;

	private:
		StrPtr<BoneComponent> m_BoneComponent;
		Matrix4 m_AccumulatedMatrix;
		Bone * m_Parent = NULL;
		StrPtr<DebuggerPoint> element;
	};
}