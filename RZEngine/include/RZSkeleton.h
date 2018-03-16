#pragma once

namespace rczEngine
{
	///Contains all the bones and the skeleton hierarchy.
	class RZ_UTILITY_EXPORT Skeleton
	{
	public:
		///Updates the skeleton transform if given an animation, if not, just updates the bone transforms.
		void UpdateSkeleton(WeakPtr<Animation> anim, float animationTime);
		///Adds a bone the hierarchy by its name and data.
		void AddBone(Bone bone, String name);
		///Retuns a pointer to a bone by name
		Bone* GetBone(String name);

		///Set the constant buffer for the skeleton data
		void InitSkeletonBuffer(Gfx::GfxCore* gfx);
		///Set the skeleton's data in the pipeline
		void SetSkeletonBuffers(Gfx::GfxCore* gfx);
		///Destroys the skeleton's data.
		void DestroySkeletonBuffers();
		///Returns the root bone's joint transform.
		const Matrix4& GetRootMotion();
		///The root of the skeleton
		Bone* m_RootBone;

		///The number of bones in the skeleton
		int32 m_NumBones;
		///A Vector of the final bone transform matrices.
		Vector<Matrix4> m_BoneFinalMatrixVector;
		///A Map of Bones
		Map<String, Bone> m_Bones;

	private: 
		///The bone matrices constant buffer
		Gfx::ConstantBuffer m_BoneMatrices;
		///The bone number constant buffer
		Gfx::ConstantBuffer m_BoneNumber;
	};
}