#pragma once

namespace rczEngine
{
	///A class that manages files, can open them, write text or binary to them and close them.
	class FileManager
	{
	public:
		RZ_EXP FileManager();

		///Opens a text File and returns true on success. Receives File name, and FileManager::f_File* flags
		RZ_EXP void OpenFile(const char* pszFileName, uint32 fileFlags);
		///Opens a file as binary and returns true on success.
		RZ_EXP void OpenBinaryFile(const char* pszFileName, uint32 fileFlags);
		///Writes chars into the file
		RZ_EXP void WriteTextInFile(char* pszTextIn);
		//Writes binary into the file
		RZ_EXP void WriteBinaryInFile(void* memory, SIZE_T size);
		///Reads chars from the file
		RZ_EXP void ReadTextInFile(char* outBuffer, uint32 charsToRead);
		///Reads Binary from the file
		RZ_EXP void ReadBinaryInFile(void* outBuffer, SIZE_T size);
		///Seeks a position on the file
		RZ_EXP void SeekInFile(uint32 fileIndex);
		///Returns the position of the file
		RZ_EXP uint32 GetIndexPos();
		///Closes the file
		RZ_EXP void CloseFile();

	private:
		UniquePtr<String> m_strFileName;
		UniquePtr<FileStream> m_File;
		uint32 m_iFileIndex;
		uint32 m_fFlags;
	};


};