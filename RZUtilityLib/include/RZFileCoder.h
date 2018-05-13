#pragma once

namespace rczEngine
{
	///A class that manages files, can open them, write text or binary to them and close them.
	class RZ_UTILITY_EXPORT FileManager
	{
	public:
		///Opens a text File and returns true on success. Receives File name, and FileManager::f_File* flags
		void OpenFile(const char* pszFileName, uint32 fileFlags);
		///Opens a file as binary and returns true on success.
		void OpenBinaryFile(const char* pszFileName, uint32 fileFlags);
		///Writes chars into the file
		void WriteTextInFile(char* pszTextIn);
		//Writes binary into the file
		void WriteBinaryInFile(void* memory, SIZE_T size);
		///Reads chars from the file
		void ReadTextInFile(char* outBuffer, uint32 charsToRead);
		///Reads Binary from the file
		void ReadBinaryInFile(void* outBuffer, SIZE_T size);
		///Seeks a position on the file
		void SeekInFile(uint32 fileIndex);
		///Returns the position of the file
		uint32 GetIndexPos();
		///Closes the file
		void CloseFile();

	private:
		String m_strFileName;
		uint32 m_iFileIndex;
		uint32 m_fFlags;
		FileStream File;
	};


};