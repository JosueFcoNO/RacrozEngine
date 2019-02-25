#include <RZUtilityPCH.h>

namespace rczEngine
{
	Path::Path(const String& str) : m_Hash(size_t{ 0 })
	{
		ResetPath(str);
	}

	void Path::ResetPath(const String& filePath)
	{
		m_FilePath = filePath;

		auto pos = m_FilePath.find("\\");
		while (pos != std::string::npos)
		{
			m_FilePath[pos] = '/';
			pos = m_FilePath.find("\\");
		}

		std::hash<String> hash;
		m_Hash = hash(m_FilePath);
	}

	void Path::ResetPath()
	{
		ResetPath(m_FilePath);
	}

	String Path::GetFileName() const
	{
		///Parse the filepath to find the extension
		Vector<String> parsedFilePath = Parser::ParseToStrings<ANSICHAR>(m_FilePath, ".", 0);
		//Get the extension substring into fileExtension
		String fileExtension = parsedFilePath[parsedFilePath.size() - 1];

		///Parse the filepath withouth the extension to find the name
		parsedFilePath = Parser::ParseToStrings<ANSICHAR>(m_FilePath, "/", 0);

		//Get the extension name into fileName
		return parsedFilePath[parsedFilePath.size() - 1];
	}

	String Path::GetFileDir() const
	{
		///Parse the filepath to find the extension
		Vector<String> parsedFilePath = Parser::ParseToStrings<ANSICHAR>(m_FilePath, ".", 0);
		//Get the extension substring into fileExtension
		String fileExtension = parsedFilePath[parsedFilePath.size() - 1];

		///Parse the filepath withouth the extension to find the name
		parsedFilePath = Parser::ParseToStrings<ANSICHAR>(m_FilePath, "/", 0);

		String pathDir = m_FilePath;

		for (int i = 0; i < parsedFilePath[parsedFilePath.size() - 1].length(); ++i)
			pathDir.pop_back();

		//Get the extension name into fileName
		return pathDir;
	}

	String Path::GetFileExtension() const
	{
		///Parse the filepath to find the extension
		Vector<String> parsedFilePath = Parser::ParseToStrings<ANSICHAR>(m_FilePath, ".", 0);
		//Get the extension substring into fileExtension
		String fileExtension = parsedFilePath[parsedFilePath.size() - 1];

		//If the string was not divided, it did not contain any . or file extension
		if (parsedFilePath.size() < 2)
		{
			return "null";
		}

		return fileExtension;
	}

	bool Path::IsDirectory() const
	{
		//If there is no path extension. It means it is a directory path.
		return (GetFileExtension() == "null");
	}

	bool Path::FileExists() const
	{
		std::ifstream infile(m_FilePath);
		const bool FileExists = infile.good();
		infile.close();
		return FileExists;
	}

	bool Path::FileExists(const String& filePath)
	{
		std::ifstream infile(filePath);
		const bool FileExists = infile.good();
		infile.close();
		return FileExists;
	}

	bool Path::operator==(const Path & other) const noexcept
	{
		return (m_Hash == other.m_Hash);
	}

	bool Path::operator!=(const Path & other) const noexcept
	{
		return !(m_Hash == other.m_Hash);
	}

	void Path::Serialize()
	{
		auto ser = Serializer::Pointer();
		//Write the path serial.
		ser->SetNextObjectSerial(SERIAL_PATH);
		//Serialize the filePath String.
		ser->SerializeString(m_FilePath);
		//Serialize the hash.
		ser->WriteData((char*)&m_Hash, sizeof(m_Hash));
	}

	void Path::DeSerialize()
	{
		auto ser = Serializer::Pointer();
		//Read the path serial.
		ser->GetNextObjectSerial();
		//Serialize the filePath String.
		ser->DeSerializeString(m_FilePath);
		//DeSerialize the hash.
		ser->ReadData((char*)&m_Hash, sizeof(m_Hash));
		ResetPath();
	}

};