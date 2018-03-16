#pragma once

namespace rczEngine
{
	class RZ_UTILITY_EXPORT Serializer
	{
	public:
		void StartFile(const char* pszFilePath);
		void LoadFile(const char* pszFilePath);

		void SerializeObject(Serializable& object);
		void SetNextObjectSerial(int serial);

		int GetNextObjectSerial();
		void DeSerializeObject(Serializable& object);
		
		void CloseFile();

	private:
		FileManager m_fileManager;

		friend class Serializable;
	};
}