#include <RZUtilityPCH.h>

namespace rczEngine
{

	Serializer*& Serializer::_Instance()
	{
		static Serializer* instance = nullptr;
		return instance;
	}

	void Serializer::Start()
	{
		(_Instance()) = new Serializer;
	}

	Serializer* Serializer::Pointer()
	{
		return _Instance();
	}

	void Serializer::ShutDown()
	{
		delete _Instance();
	}

	void Serializer::StartFile(const char * pszFilePath)
	{
		m_fileManager.OpenBinaryFile(pszFilePath, FileStream::out);
	}

	void Serializer::LoadFile(const char * pszFilePath)
	{
		m_fileManager.OpenBinaryFile(pszFilePath, FileStream::in);
	}

	void Serializer::CloseFile()
	{
		m_fileManager.CloseFile();
	}

	void Serializer::SetNextObjectSerial(int serial)
	{
		int serialToWrite = serial;
		WriteData(&serialToWrite, sizeof(int));
	}

	void Serializer::SerializeString(const String & str)
	{
		//Create a new object to shrink the string and get the size.
		String saved = str;
		saved.shrink_to_fit();
		uint32 stringSize = (uint32)saved.size();

		//Set the string serial.
		SetNextObjectSerial(SERIAL_STRING);
		//Write the number of bytes.
		WriteData(&stringSize, sizeof(stringSize));

		//Save every char.
		WriteData(&saved[0], stringSize);

	}

	void Serializer::DeSerializeString(String & str)
	{
		//Create a new object to shrink the string and get the size.
		str.clear();
		int32 stringSize;

		//Read the string serial.
		GetNextObjectSerial();
		//Read the number of bytes.
		ReadData(&stringSize, sizeof(stringSize));

		//Read every char.
		char tempChar;
		for (int32 i = 0; i < stringSize; ++i)
		{
			ReadData(&tempChar, sizeof(tempChar));
			str.push_back(tempChar);
		}
	}

	void Serializer::WriteData(void * pointer, size_t size)
	{
		m_fileManager.WriteBinaryInFile(pointer, (SIZE_T)size);
	}

	void Serializer::ReadData(void * pointer, size_t size)
	{
		m_fileManager.ReadBinaryInFile(pointer, (SIZE_T)size);
	}

	int Serializer::GetNextObjectSerial()
	{
		int serialRead;

		ReadData(&serialRead, sizeof(int));

		return serialRead;
	}
}