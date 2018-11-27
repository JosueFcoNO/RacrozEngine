#include <RZEnginePCH.h>

namespace rczEngine
{
	void Bone::UpdateAccumulatedMatrix(Vector<Matrix4>& matrixPalette)
	{
		if (m_BoneIndex >= 0)
		{
			if (m_Parent)
			{
				m_AccumulatedTransform = m_Parent->m_AccumulatedTransform*m_JointMatrix;
				m_AccumulatedMatrix = (m_Parent->m_AccumulatedMatrix*m_JointMatrix);
			}
			else
			{
				m_AccumulatedTransform = m_TransformMatrix;
				m_AccumulatedMatrix = m_TransformMatrix;
			}

			matrixPalette[m_BoneIndex] = (m_AccumulatedTransform)*m_OffsetMatrix;
		}
		else
		{
			assert(false);
		}

		UpdateChildren(matrixPalette);
	}

	void Bone::UpdateChildren(Vector<Matrix4>& matrixPalette)
	{
		if (m_ChildrenBones.size())
		{
			for (int i = 0; i < m_ChildrenBones.size(); ++i)
			{
				m_ChildrenBones[i]->UpdateAccumulatedMatrix(matrixPalette);
			}
		}
	}

	void Bone::InterpolateBone(KeyFrame & k0, KeyFrame & k1, float time)
	{
		///Calculate the current time normalized between k0 and k1.
		float t0 = k0.m_Time;
		float t1 = k1.m_Time;
		float currentTime = (time - t0) / (t1 - t0);

		if (t0 == t1)
		{
			currentTime = 0.0f;
		}

		/// Lerp the position, scale and slerp rotation
		Vector3 newPos = Math::Lerp(k0.m_Position, k1.m_Position, currentTime);
		Vector3 newScale = Math::Lerp(k0.m_Scale, k1.m_Scale, currentTime);
		Quaternion newRot = Quaternion::Slerp(k0.m_Rotation, k1.m_Rotation, currentTime);

		GraphicDebugger::Pointer()->AddFrameDebuggerPoint(m_OffsetInverse*newPos, 0.1f, 1, 0, 0);

		///Create the new local joint matrix for the bone.
		m_JointMatrix = newRot.GetAsMatrix4()*Matrix4::Scale3D(newScale.m_x, newScale.m_y, newScale.m_z)*Matrix4::Translate3D(newPos.m_x, newPos.m_y, newPos.m_z);
		m_JointMatrix.Transpose();

		if (m_BoneComponent)
		{
			m_TransformMatrix = m_BoneComponent->GetOwner().lock()->GetLocalMatrix().GetTransposed();
			//m_JointMatrix *= m_TransformMatrix;
		}

	}

	void Bone::InterpolateBoneWithAnim(float time, WeakPtr<Animation> anim)
	{
		KeyFrame k0, k1;
		k0.m_Time = -1;

		StrPtr<Animation> animPtr(anim);

		k0 = animPtr->GetKeyFrame(m_Name, time);
		k1 = animPtr->GetNextKeyFrame(m_Name, time);

		if (k0.m_Time >= 0)
		{
			InterpolateBone(k0, k1, time);
		}
		else
		{
			m_TransformMatrix = m_BoneComponent->GetOwner().lock()->GetLocalMatrix().GetTransposed();
			m_JointMatrix = m_TransformMatrix;
		}


		for (int32 i = 0; i < m_ChildrenBones.size(); ++i)
		{
			m_ChildrenBones[i]->InterpolateBoneWithAnim(time, anim);
		}
	}

	void Bone::InterpolateChildren(float time, WeakPtr<Animation> anim)
	{
		for (int32 i = 0; i < m_ChildrenBones.size(); ++i)
		{
			m_ChildrenBones[i]->InterpolateBoneWithAnim(time, anim);
		}
	}

	void Bone::AddBoneChildren(Bone* child)
	{
		m_ChildrenBones.push_back(child);
	}

	void Bone::SetParent(Bone * parent)
	{
		m_Parent = parent;
	}
}