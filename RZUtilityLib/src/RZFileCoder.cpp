#include "RZUtilityPCH.h"
#include "RZFileCoder.h"

namespace rczEngine
{
	struct FileFlags
	{
		static const uint32 f_FileWrite = std::ios_base::out;
		static const uint32 f_FileRead = std::ios_base::in;
		static const uint32 f_FileTrunc = std::ios_base::trunc;
		static const uint32 f_FileBinary = std::ios_base::binary;
		static const uint32 f_FileAppend = std::ios_base::app;
	};

	FileManager::FileManager()
	{
		m_strFileName = std::make_unique<String>();
		m_File = std::make_unique<FileStream>();
	}

	void FileManager::OpenFile(const char * pszFileName, uint32 fileFlags)
	{
		m_fFlags = fileFlags;
		m_File->open(pszFileName, fileFlags);

		*m_strFileName = pszFileName;
		m_iFileIndex = 0;
	}

	void FileManager::OpenBinaryFile(const char * pszFileName, uint32 fileFlags)
	{
		return OpenFile(pszFileName, fileFlags | FileFlags::f_FileBinary);
	}

	void FileManager::WriteTextInFile(char* pszTextIn)
	{	
		std::string S = pszTextIn;
		*m_File << S;
		m_iFileIndex += static_cast<uint32>(S.size());
	}

	void FileManager::WriteBinaryInFile(void * memory, SIZE_T size)
	{
		m_File->write((const char*)memory, size);
		m_iFileIndex += size;
	}

	void FileManager::ReadTextInFile(char * outBuffer, uint32 charsToRead)
	{
		m_File->read(outBuffer, charsToRead);
	}

	void FileManager::ReadBinaryInFile(void * outBuffer, SIZE_T size)
	{
		m_File->read((char*)outBuffer, size);
	}

	void FileManager::SeekInFile(uint32 fileIndex)
	{
		m_File->seekp(fileIndex);
	}

	uint32 FileManager::GetIndexPos()
	{
		return m_iFileIndex;
	}

	void FileManager::CloseFile()
	{
		m_File->close();
	}
};
