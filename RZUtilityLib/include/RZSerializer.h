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
	class RZ_UTILITY_EXPORT Serializer
	{
	public:
		static void Start();
		static Serializer* Pointer();
		static void ShutDown();

		//Open and truncate the new file.
		void StartFile(const char* pszFilePath);
		//Load the file.
		void LoadFile(const char* pszFilePath);
		//Close the file. It is not automatic.
		void CloseFile();

		//Set and write the serial for the next serialized object
		void SetNextObjectSerial(int serial);
		//Read and get the serial for the next deserialized object.
		int GetNextObjectSerial();

		//Serialize a String.
		void SerializeString(const String& str);
		//DeSerialize a String.
		void DeSerializeString(String& str);

		//Write data.
		void WriteData(void* pointer, size_t size);
		//Read Data.
		void ReadData(void* pointer, size_t size);

	private:
		static Serializer*& _Instance();

		FileManager m_fileManager;
	};
}