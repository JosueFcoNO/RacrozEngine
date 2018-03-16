#include <RZEnginePCH.h>

namespace rczEngine
{
	void Skeleton::UpdateSkeleton(WeakPtr<Animation> anim, float animationTime)
	{
		m_NumBones = m_Bones.size();

		if (!anim.expired())
		{
			StrPtr<Animation> animation(anim);

			///if the animation is not null, update the skeleton with the animation.
			if (animation)
			{
				///Interpolate froom the root bone all the children
				m_RootBone->InterpolateBoneWithAnim(animationTime, anim);
			}
		}
		
		///Go through the skeleton and update the transform using the hierarchy.
		m_RootBone->UpdateAccumulatedMatrix(m_BoneFinalMatrixVector);

	}

	void Skeleton::AddBone(Bone bone, String name)
	{
		///Add a bone to the UoMap by its name and Bone Data
		Pair<String, Bone> Value;
		Value.first = name;
		Value.second = bone;
		m_Bones.insert(Value);
	}

	Bone* Skeleton::GetBone(String name)
	{
		///Find the bone by name and return a simple pointer to it. Or a NULL in case it's not found.
		auto p = m_Bones.find(name);
		if (p != m_Bones.end())
		{
			Bone* pointer = &p->second;
			return pointer;
		}
		else
		{
			return NULL;
		}
	}

	void Skeleton::InitSkeletonBuffer(Gfx::GfxCore* gfx)
	{
		///Create the constant buffer for the FinalBoneMatrix
		m_BoneMatrices.CreateConstantBuffer(m_Bones.size()*sizeof(Matrix4), Gfx::USAGE_DEFAULT, gfx);
	}

	void Skeleton::SetSkeletonBuffers(Gfx::GfxCore* gfx)
	{
		///Update the constant buffer and set it in the Gfx::Vertex Shader
		m_BoneMatrices.UpdateConstantBuffer(&m_BoneFinalMatrixVector[0], gfx);

		m_BoneMatrices.SetBufferInVS(6, gfx);
	}

	void Skeleton::DestroySkeletonBuffers()
	{
		///Destroy the skeleton and its buffers
		m_BoneMatrices.Destroy();
	}
	const Matrix4 & Skeleton::GetRootMotion()
	{
		///Get the root bone's final transform. Sirve? Profe prins.
		return m_BoneFinalMatrixVector[0];
	}
}