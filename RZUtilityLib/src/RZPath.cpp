#include <RZUtilityPCH.h>

namespace rczEngine
{
	Path::Path(String str)
	{
		ResetPath(str);
	}

	Path::Path(const char * str)
	{
		ResetPath(str);
	}

	void Path::ResetPath(const char * filePath)
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

	void Path::ResetPath(String filePath)
	{
		ResetPath(filePath.c_str());
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
		bool FileExists = infile.good();
		infile.close();
		return FileExists;
	}

	bool Path::FileExists(const char * filePath)
	{
		std::ifstream infile(filePath);
		bool FileExists = infile.good();
		infile.close();
		return FileExists;
	}

	bool Path::operator==(const Path & other) const
	{
		return (m_Hash == other.m_Hash);
	}

	bool Path::operator!=(const Path & other) const
	{
		return !(m_Hash == other.m_Hash);
	}

	void Path::Serialize() const
	{
		size_t hash = m_Hash;
		auto ser = Serializer::Pointer();
		//Write the path serial.
		ser->SetNextObjectSerial(SERIAL_PATH);
		//Serialize the filePath String.
		ser->SerializeString(m_FilePath);
		//Serialize the hash.
		ser->WriteData(&hash, sizeof(m_Hash));
	}

	void Path::Serialize()
	{
		auto ser = Serializer::Pointer();
		//Write the path serial.
		ser->SetNextObjectSerial(SERIAL_PATH);
		//Serialize the filePath String.
		ser->SerializeString(m_FilePath);
		//Serialize the hash.
		ser->WriteData(&m_Hash, sizeof(m_Hash));
	}

	void Path::DeSerialize()
	{
		auto ser = Serializer::Pointer();
		//Read the path serial.
		ser->GetNextObjectSerial();
		//Serialize the filePath String.
		ser->DeSerializeString(m_FilePath);
		//DeSerialize the hash.
		ser->ReadData(&m_Hash, sizeof(m_Hash));
		ResetPath();
	}

};