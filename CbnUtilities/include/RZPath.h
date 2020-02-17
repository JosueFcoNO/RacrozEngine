#pragma once

namespace rczEngine
{
	///A class that manages Paths. Can be serialized, hashed and get the extension from them.
	class Path : public Serializable
	{
	public:
		RZ_EXP Path(const String& str);
		RZ_EXP Path() noexcept : m_Hash(0ull) {};

		virtual ~Path() = default;            // destructor (virtual if X is meant to be a base class)
		Path(const Path&) = default;             // copy constructor
		Path& operator=(const Path&) = default;  // copy assignment
		Path(Path&&) = default;                  // move constructor
		Path& operator=(Path&&) = default;       // move assignment

		//Reset the FilePath and Hash.
		RZ_EXP void ResetPath(const String& filePath);
		RZ_EXP void ResetPath();

		//Get the file path string.
		RZ_EXP String GetFilePath() const { return m_FilePath; };
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

		//Overloaded operators to compare Path objects using their hashes. The hash can't change if the filePath does not change.
		RZ_EXP bool operator==(const Path& other) const noexcept;
		RZ_EXP bool operator!=(const Path& other) const noexcept;

		//Serializes the path and hash.
		RZ_EXP void Serialize() override;
		//DeSerializes the path and hash.
		RZ_EXP void DeSerialize() override;

	private:
		//The file path string.
		String m_FilePath;
		//The hash of the file path. Used to compare Path objects and as handles.
		size_t m_Hash;
	};


};