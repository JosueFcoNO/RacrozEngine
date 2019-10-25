#pragma once

namespace rczEngine
{
	///A class that manages Paths. Can be serialized, hashed and get the extension from them.
	class FilePath : public Serializable
	{
	public:
		RZ_EXP FilePath(const String& str);
		RZ_EXP FilePath() : m_FilePath(""), m_Hash(0ull) {};

		virtual ~FilePath() = default;            // destructor (virtual if X is meant to be a base class)
		FilePath(const FilePath&) = default;             // copy constructor
		FilePath& operator=(const FilePath&) = default;  // copy assignment
		FilePath(FilePath&&) = default;                  // move constructor
		FilePath& operator=(FilePath&&) = default;       // move assignment

		//Reset the FilePath and Hash.
		RZ_EXP void ResetPath(const String& filePath);
		RZ_EXP void ResetPath();

		//Get the file path string.
		RZ_EXP FORCEINLINE String GetFilePath() const { return m_FilePath; };
		//Get the hash of the file path string.
		RZ_EXP FORCEINLINE size_t GetHash() const noexcept { return m_Hash; };
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
		RZ_EXP static bool FileExists(const String& filePath);

		//Overloaded operators to compare FilePath objects using their hashes. The hash can't change if the filePath does not change.
		RZ_EXP bool operator==(const FilePath& other) const noexcept;
		RZ_EXP bool operator!=(const FilePath& other) const noexcept;

		//Serializes the path and hash.
		RZ_EXP void Serialize() override;
		//DeSerializes the path and hash.
		RZ_EXP void DeSerialize() override;

	private:
		//The file path string.
		String m_FilePath;
		//The hash of the file path. Used to compare FilePath objects and as handles.
		size_t m_Hash;
	};


};