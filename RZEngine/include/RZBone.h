#pragma once

namespace rczEngine
{
	class Animation;

	class RZ_UTILITY_EXPORT Bone
	{
	public:
		Bone() : m_OffsetMatrix(INIT_UNIT), m_JointMatrix(INIT_UNIT), m_AccumulatedMatrix(INIT_UNIT)
		{
		
		};
		~Bone() 
		{ 
		
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
		Matrix4 m_AccumulatedTransform;


	private:
		Matrix4 m_AccumulatedMatrix;
		Bone * m_Parent = NULL;
	};
}