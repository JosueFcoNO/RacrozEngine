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

	void FileManager::OpenFile(const String& pszFileName, uint32 fileFlags)
	{
		m_fFlags = fileFlags;
		m_File->open(pszFileName, fileFlags);

		*m_strFileName = pszFileName;
		m_iFileIndex = 0;
	}

	void FileManager::OpenBinaryFile(const String& pszFileName, uint32 fileFlags)
	{
		return OpenFile(pszFileName, fileFlags | FileFlags::f_FileBinary);
	}

	void FileManager::WriteTextInFile(const String& textIn)
	{	
		*m_File << textIn;
		m_iFileIndex += gsl::narrow_cast<uint32>( textIn.size() );
	}

	void FileManager::WriteBinaryInFile(const char * memory, SIZE_T size)
	{
		m_File->write(memory, size);
		m_iFileIndex += gsl::narrow_cast<uint32>(size);
	}

	void FileManager::ReadTextInFile(char * outBuffer, uint32 charsToRead)
	{
		m_File->read(outBuffer, charsToRead);
	}

	void FileManager::ReadBinaryInFile(char * outBuffer, SIZE_T size)
	{
		m_File->read(outBuffer, size);
	}

	void FileManager::SeekInFile(uint32 fileIndex)
	{
		m_File->seekp(fileIndex);
	}

	uint32 FileManager::GetIndexPos() const noexcept
	{
		return m_iFileIndex;
	}

	void FileManager::CloseFile()
	{
		m_File->close();
	}
};
