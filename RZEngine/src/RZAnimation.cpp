#include <RZEnginePCH.h>

namespace rczEngine
{
	void Animation::AddKeyFrame(KeyFrame & keyFrame, String boneName)
	{
		auto tempKeyframe = m_Timeline.find(boneName);
		if (tempKeyframe == m_Timeline.end())
		{
			m_Timeline[boneName].push_back(keyFrame);
		}
		else
		{
			m_Timeline[boneName].push_back(keyFrame);
		}
	}

	KeyFrameVector & Animation::GetChannel(String boneName)
	{
		return m_Timeline[boneName];
	}

	KeyFrame Animation::GetKeyFrame(String & boneName, float time)
	{
		KeyFrame Temp;
		auto tempKeyframe = m_Timeline.find(boneName);
		if (tempKeyframe != m_Timeline.end())
		{
			KeyFrameVector* BoneKeyFrame = &tempKeyframe->second;

			for (int32 i = 0; i < BoneKeyFrame->size(); ++i)
			{
				if (time > BoneKeyFrame->at(i).m_Time)
				{
					Temp = BoneKeyFrame->at(i);
				}
				else
				{
					break;
				}
			}
		}

		return Temp;
	}

	KeyFrame Animation::GetNextKeyFrame(String & boneName, float time)
	{
		KeyFrame Temp;
		auto tempKeyframe = m_Timeline.find(boneName);
		if (tempKeyframe != m_Timeline.end())
		{
			KeyFrameVector* BoneKeyFrame = &tempKeyframe->second;

			for (int32 i = 0; i < BoneKeyFrame->size(); ++i)
			{
				if (time > BoneKeyFrame->at(i).m_Time)
				{
					if (i + 1 < BoneKeyFrame->size())
					{
						Temp = BoneKeyFrame->at(i + 1);
					}
					else
					{
						Temp = BoneKeyFrame->at(i);
					}
				}
				else
				{
					break;
				}
			}
		}

		return Temp;
	}

	void Animation::Load(const char* filePath, const char* resName, bool addToResourceManager)
	{
		///Change the m_Name to the resource name
		m_Name = resName;

		///Load the scene.
		Assimp::Importer B;
		const aiScene* Scene = B.ReadFile(filePath, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);

		///If the scene has animations
		if (Scene->HasAnimations())
		{
			///If addToResourceManager is true, i do it.
			if (addToResourceManager)
			{
				ResVault::Pointer()->InsertResource(this);
			}
			else ///If it isn't set the Handle to Invalid
			{
				m_Handle = INVALID_RESOURCE;
			}

			///Iterate through the animations
			for (uint32 i = 0; i < Scene->mNumAnimations; ++i)
			{
				///CurrentAnim is a pointer to the current animation i'm iterating over
				aiAnimation* CurrentAnim = Scene->mAnimations[i];
				///Get the duration into m_Duration
				m_Duration = static_cast<float>(CurrentAnim->mDuration);
				///Get the Ticks per Second into m_TicksPerSecond
				m_TicksPerSecond = static_cast<float>(CurrentAnim->mTicksPerSecond);

				///Iterate through the number of channels in the CurrentAnimation
				for (uint32 o = 0; o < CurrentAnim->mNumChannels; ++o)
				{
					aiNodeAnim* CurrentChannel = CurrentAnim->mChannels[o];

					auto NodeName = Parser::ParseToStrings<ANSICHAR>(CurrentChannel->mNodeName.data, "$", 0);
					BasicString<ANSICHAR> CurrentAnimNode = CurrentChannel->mNodeName.data;
					BasicString<ANSICHAR> CurrentFbxTransform = "";

					if (NodeName.size() > 2)
					{
						if (NodeName[1] == "AssimpFbx")
						{
							CurrentAnimNode = NodeName[0];
							CurrentAnimNode.pop_back();
							CurrentFbxTransform = NodeName[2];
						}
					}

					uint32 KeysToGen = CurrentChannel->mNumPositionKeys;
					if (CurrentChannel->mNumRotationKeys > KeysToGen)
					{ 
						KeysToGen = CurrentChannel->mNumRotationKeys;
					}
					else if(CurrentChannel->mNumScalingKeys > KeysToGen)
					{
						KeysToGen = CurrentChannel->mNumScalingKeys;
					}

					int32 LastPosKey = 0;
					int32 LastRotKey = 0;
					int32 LastScaleKey = 0;

					for (uint32 k = 0; k < KeysToGen; ++k)
					{
						KeyFrame TempFrame;
						TempFrame.m_Scale.Set(1, 1, 1);
						auto TempChannel = CurrentChannel;

						TempFrame.m_Time = TempChannel->mPositionKeys[LastPosKey].mTime;
						if (CurrentChannel->mRotationKeys[LastRotKey].mTime > TempFrame.m_Time)
						{
							TempFrame.m_Time = CurrentChannel->mRotationKeys[LastRotKey].mTime;
						}
						else if (CurrentChannel->mScalingKeys[LastScaleKey].mTime > TempFrame.m_Time)
						{
							TempFrame.m_Time = CurrentChannel->mScalingKeys[LastScaleKey].mTime;
						}

						if (k < CurrentChannel->mNumPositionKeys)
						{
							TempFrame.m_Position.m_x = TempChannel->mPositionKeys[k].mValue.x;
							TempFrame.m_Position.m_y = TempChannel->mPositionKeys[k].mValue.y;
							TempFrame.m_Position.m_z = TempChannel->mPositionKeys[k].mValue.z;

							LastPosKey = k;
						}
						else
						{
							TempFrame.m_Position.m_x = TempChannel->mPositionKeys[LastPosKey].mValue.x;
							TempFrame.m_Position.m_y = TempChannel->mPositionKeys[LastPosKey].mValue.y;
							TempFrame.m_Position.m_z = TempChannel->mPositionKeys[LastPosKey].mValue.z;
						}

						if (k < CurrentChannel->mNumScalingKeys)
						{
							TempFrame.m_Scale.m_x = TempChannel->mScalingKeys[k].mValue.x;
							TempFrame.m_Scale.m_y = TempChannel->mScalingKeys[k].mValue.y;
							TempFrame.m_Scale.m_z = TempChannel->mScalingKeys[k].mValue.z;

							LastScaleKey = k;
						}
						else
						{
							TempFrame.m_Scale.m_x = TempChannel->mScalingKeys[LastScaleKey].mValue.x;
							TempFrame.m_Scale.m_y = TempChannel->mScalingKeys[LastScaleKey].mValue.y;
							TempFrame.m_Scale.m_z = TempChannel->mScalingKeys[LastScaleKey].mValue.z;
						}

						if (k < CurrentChannel->mNumRotationKeys)
						{
							TempFrame.m_Rotation.m_x = TempChannel->mRotationKeys[k].mValue.x;
							TempFrame.m_Rotation.m_y = TempChannel->mRotationKeys[k].mValue.y;
							TempFrame.m_Rotation.m_z = TempChannel->mRotationKeys[k].mValue.z;
							TempFrame.m_Rotation.m_w = TempChannel->mRotationKeys[k].mValue.w;

							LastRotKey = k;
						}
						else
						{
							TempFrame.m_Rotation.m_x = TempChannel->mRotationKeys[LastRotKey].mValue.x;
							TempFrame.m_Rotation.m_y = TempChannel->mRotationKeys[LastRotKey].mValue.y;
							TempFrame.m_Rotation.m_z = TempChannel->mRotationKeys[LastRotKey].mValue.z;
							TempFrame.m_Rotation.m_w = TempChannel->mRotationKeys[LastRotKey].mValue.w;
						}

						AddKeyFrame(TempFrame, CurrentAnimNode.data());
					}
				}

			}
		}
	}
}