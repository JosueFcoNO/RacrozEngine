#pragma once

namespace rczEngine
{
	///A class that manages Paths. Can be serialized, hashed and get the extension from them.
	class RZ_UTILITY_EXPORT Path : public Serializable
	{
	public:
		Path() {};
		Path(String str);
		Path(const char* str);
		
		//Reset the FilePath and Hash.
		void ResetPath(const char* filePath);
		void ResetPath(String filePath);
		void ResetPath();

		//Get the file path string.
		FORCEINLINE String GetFilePath() const;
		//Get the hash of the file path string.
		FORCEINLINE size_t GetHash() const { return m_Hash; };
		//Returns the File's Extension
		String GetFileExtension() const;
		//Returns the file name.
		String GetFileName() const;

		//Returns if it is a Directory or not.
		bool IsDirectory() const;
		//Returns if the file referenced in the path file exists or not.
		bool FileExists() const;
		//Returns if the file referenced in the path file exists or not.
		static bool FileExists(const char* filePath);

		//Overloaded operators to compare Path objects using their hashes. The hash can't change if the filePath does not change.
		bool operator==(const Path& other) const;
		bool operator!=(const Path& other) const;

		//Serializes the path and hash.
		virtual void Serialize() const;
		virtual void Serialize();
		//DeSerializes the path and hash.
		virtual void DeSerialize();

	private:
		//Theh file path string.
		String m_FilePath;
		//The hash of the file path. Used to compare Path objects and as handles.
		size_t m_Hash;
	};


};