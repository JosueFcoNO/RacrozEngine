#pragma once

namespace rczEngine
{
	class Animation;

	class RZ_EXP Bone
	{
	public:
		Bone() : m_OffsetMatrix(INIT_UNIT), m_JointMatrix(INIT_UNIT), m_AccumulatedMatrix(INIT_UNIT)
		{

		};

		~Bone()
		{

		};

		void InitBone(StrPtr<BoneComponent> boneCmp)
		{
			m_BoneComponent = boneCmp;
			m_AccumulatedTransform.Identity();
			m_TransformMatrix = m_BoneComponent->GetOwner().lock()->GetLocalMatrix().GetTransposed();
			m_OffsetInverse = m_OffsetMatrix.GetInverse();
		};

		void AddBoneChildren(Bone* child);
		void SetParent(Bone* parent);

		void UpdateAccumulatedMatrix(Vector<Matrix4>& matrixPalette);
		void UpdateChildren(Vector<Matrix4>& matrixPalette);
		void InterpolateBone(KeyFrame& k0, KeyFrame& k1, float time);

		void InterpolateBoneWithAnim(float time, WeakPtr<Animation> anim);
		void InterpolateChildren(float time, WeakPtr<Animation> anim);

		String m_Name;
		int32 m_BoneIndex = -1;
		Matrix4 m_OffsetMatrix;
		Vector<Bone*> m_ChildrenBones;
		Matrix4 m_JointMatrix;
		Matrix4 m_TransformMatrix;

	private:
		StrPtr<BoneComponent> m_BoneComponent;
		Matrix4 m_OffsetInverse;
		Matrix4 m_AccumulatedMatrix;
		Matrix4 m_AccumulatedTransform;
		Bone * m_Parent = NULL;
	};
}