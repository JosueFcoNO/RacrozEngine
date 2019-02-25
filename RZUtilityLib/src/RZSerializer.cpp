#include <RZUtilityPCH.h>

namespace rczEngine
{

	Serializer*& Serializer::_Instance() noexcept
	{
		static Serializer* instance = nullptr;
		return instance;
	}

	void Serializer::Start()
	{
		(_Instance()) = new Serializer;
	}

	Serializer* Serializer::Pointer() noexcept
	{
		return _Instance();
	}

	void Serializer::ShutDown()
	{
		delete _Instance();
	}

	void Serializer::StartFile(const String& pszFilePath)
	{
		m_fileManager.OpenBinaryFile(pszFilePath, FileStream::out);
	}

	void Serializer::LoadFile(const String& pszFilePath)
	{
		m_fileManager.OpenBinaryFile(pszFilePath, FileStream::in);
	}

	void Serializer::CloseFile()
	{
		m_fileManager.CloseFile();
	}

	void Serializer::SetNextObjectSerial(int serial)
	{
		const int serialToWrite = serial;
		WriteData((char*)&serialToWrite, sizeof(int));
	}

	void Serializer::SerializeString(const String & str)
	{
		//Create a new object to shrink the string and get the size.
		auto saved = str;
		saved.shrink_to_fit();
		auto stringSize = gsl::narrow_cast<uint32>( saved.size() );

		//Set the string serial.
		SetNextObjectSerial(SERIAL_STRING);
		//Write the number of bytes.
		WriteData((char*)&stringSize, sizeof(stringSize));

		//Save every char.
		WriteData(&saved[0], stringSize);

	}

	void Serializer::DeSerializeString(String& str)
	{
		const gsl::not_null<String*> ptr = &str;

		//Create a new object to shrink the string and get the size.
		ptr->clear();
		int32 stringSize;

		//Read the string serial.
		GetNextObjectSerial();
		//Read the number of bytes.
		ReadData((char*)&stringSize, sizeof(stringSize));
		
		//Read every char.
		auto tempChar = ' ';
		for (int32 i = 0; i < stringSize; ++i)
		{
			ReadData(&tempChar, sizeof(tempChar));

			ptr->push_back(tempChar);
		}
	}

	int Serializer::GetNextObjectSerial()
	{
		int serialRead;

		ReadData((char*)&serialRead, sizeof(int));

		return serialRead;
	}
}