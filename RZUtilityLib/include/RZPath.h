#pragma once

namespace rczEngine
{
	///A class that manages Paths. Can be serialized, hashed and get the extension from them.
	class Path : public Serializable
	{
	public:
		RZ_EXP Path() {};
		RZ_EXP Path(String str);
		RZ_EXP Path(const char* str);
		
		//Reset the FilePath and Hash.
		RZ_EXP void ResetPath(const char* filePath);
		RZ_EXP void ResetPath(String filePath);
		RZ_EXP void ResetPath();

		//Get the file path string.
		RZ_EXP String GetFilePath() const { return m_FilePath; };
		//Get the hash of the file path string.
		RZ_EXP FORCEINLINE size_t GetHash() const { return m_Hash; };
		//Returns the File's Extension
		RZ_EXP String GetFileExtension() const;
		//Returns the file name.
		RZ_EXP String GetFileName() const;
		//Returns the directory of the path.
		RZ_EXP String GetFileDir() const;

		//Returns if it is a Directory or not.
		RZ_EXP bool IsDirectory() const;
		//Returns if the file referenced in the path file exists or not.
		RZ_EXP bool FileExists() const;
		//Returns if the file referenced in the path file exists or not.
		RZ_EXP static bool FileExists(const char* filePath);

		//Overloaded operators to compare Path objects using their hashes. The hash can't change if the filePath does not change.
		RZ_EXP bool operator==(const Path& other) const;
		RZ_EXP bool operator!=(const Path& other) const;

		//Serializes the path and hash.
		RZ_EXP virtual void Serialize() const;
		RZ_EXP virtual void Serialize();
		//DeSerializes the path and hash.
		RZ_EXP virtual void DeSerialize();

	private:
		//Theh file path string.
		String m_FilePath;
		//The hash of the file path. Used to compare Path objects and as handles.
		size_t m_Hash;
	};


};