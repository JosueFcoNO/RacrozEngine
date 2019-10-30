#pragma once

namespace rczEngine
{
	enum ResourceType
	{
		RES_UNKNOWN,
		RES_3DMODEL,
		RES_SKINNEDMODEL,
		RES_TEXTURE,
		RES_AUDIO,
		RES_MATERIAL,
		RES_ANIMATION,
		RES_CUBEMAP,
		RES_MISSING
	};

	///The resource base class. 
	class Resource : public std::enable_shared_from_this<Resource>, public Serializable
	{
	public:
		RZ_EXP Resource() {};
		RZ_EXP virtual ~Resource() {};

		//Return the handle of the resource. If it is equal to NULL. It is not resource managed.
		RZ_EXP ResourceHandle GetHandle() const { return m_FilePath.GetHash(); };

		//Returns the name.
		RZ_EXP const String GetName() const { return m_Name.c_str(); };
		//Sets the name
		RZ_EXP void SetName(const String& name) { m_Name = name; };

		//Returns the FilePath const char
		RZ_EXP String GetFilePath() const { return m_FilePath.GetFilePath(); };
		//Returns the FilePath Object
		RZ_EXP const Path& GetPath() { return m_FilePath; };

		//Init the FilePath object.
		RZ_EXP void SetFilePath(const String& filePath) { m_FilePath.ResetPath(filePath); };

		//Load the resource.
		RZ_EXP virtual void Load(const String& filePath, const String& resName) = 0;
		//Release the Resource.
		RZ_EXP virtual void Release() = 0;

		ResourceType m_Type;

#ifndef RZ_EDITOR
		//Render the resource GUI.
		RZ_EXP virtual void RenderResourceGUI() = 0;
#endif
	protected:
		Path m_FilePath;
		String m_Name = "Unnamed Resource";
	};
};