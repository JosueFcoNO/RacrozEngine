#pragma once

namespace rczEngine
{
	enum SERIALS
	{
		SERIAL_SCENE,
		SERIAL_ACTOR,
		SERIAL_COMPONENT,
		SERIAL_RESOURCE,
		SERIAL_PATH,
		SERIAL_STRING,

		SERIAL_MODEL = 2000,
		SERIAL_SKINNED_MODEL,
		SERIAL_TEXTURE1D,
		SERIAL_TEXTURE2D,
		SERIAL_TEXTURE3D,
		SERIAL_MATERIAL,
		SERIAL_CUBEMAP,
		SERIAL_ANIMATION,
		SERIAL_AUDIO,
		SERIAL_MESH,
		SERIAL_KEYFRAME,
		SERIAL_ANIMATION_TIMELINE
	};

	//A class to serialize and deserialize objects. It internally manages the file manager.
	class Serializer
	{
	public:
		RZ_EXP static void Start();
		RZ_EXP static Serializer* Pointer();
		RZ_EXP static void ShutDown();

		//Open and truncate the new file.
		RZ_EXP void StartFile(const char* pszFilePath);
		//Load the file.
		RZ_EXP void LoadFile(const char* pszFilePath);
		//Close the file. It is not automatic.
		RZ_EXP void CloseFile();

		//Set and write the serial for the next serialized object
		RZ_EXP void SetNextObjectSerial(int serial);
		//Read and get the serial for the next deserialized object.
		RZ_EXP int GetNextObjectSerial();

		//Serialize a String.
		RZ_EXP void SerializeString(const String& str);
		//DeSerialize a String.
		RZ_EXP void DeSerializeString(String& str);

		//Write data.
		RZ_EXP void WriteData(void* pointer, size_t size);
		//Read Data.
		RZ_EXP void ReadData(void* pointer, size_t size);

	private:
		static Serializer*& _Instance();

		FileManager m_fileManager;
	};
}