#pragma once

namespace rczEngine
{
	class ResVault;

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
	class RZ_UTILITY_EXPORT Resource : public std::enable_shared_from_this<Resource>, public Serializable
	{
	public:
		Resource() {};
		virtual ~Resource() {};

		//Return the handle of the resource. If it is equal to NULL. It is not resource managed.
		ResourceHandle GetHandle() const { return m_FilePath.GetHash(); };

		//Returns the name.
		const char* GetName() const { return m_Name.c_str(); };
		//Sets the name
		void SetName(const char* name) { m_Name = name; };

		//Returns the FilePath const char
		const char* GetFilePath() const { return m_FilePath.GetFilePath().c_str(); };
		//Returns the FilePath Object
		const Path& GetPath() { return m_FilePath; };

		//Init the FilePath object.
		void SetFilePath(const char* filePath) { m_FilePath.ResetPath(filePath); };
		//Init the FilePath object.
		void SetFilePath(String filePath) { m_FilePath.ResetPath(filePath.c_str()); };

		//Load the resource.
		virtual void Load(const char* filePath, const char* resName) = 0;
		//Release the Resource.
		virtual void Release() = 0;

		ResourceType m_Type;

#ifndef RZ_EDITOR
		//Render the resource GUI.
		virtual void RenderResourceGUI() = 0;
#endif
	protected:
		Path m_FilePath;
		String m_Name = "Unnamed Resource";
	};
};