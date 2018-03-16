#define LOGGING 1

namespace rczEngine
{
	//A logger class that can write to multiple logs and is accesible everywhere. 
	class RZ_UTILITY_EXPORT Logger 
	{
	public:
		static void Start();
		static Logger* Pointer();
		static void ShutDown();

		enum e_MessageType
		{
			e_MESSAGE, e_WARNING, e_ERROR, e_CRITICALERROR
		};

		///Create a Log File and load it into this object
		void StartLog(const char* pszFileName);

		///Print a Log Message onto the output
		void LogMessage(const char* pszFileName, String strMessage, e_MessageType messageType = e_MESSAGE);
		void LogMessage(const char* pszFileName, const char* strMessage, e_MessageType messageType = e_MESSAGE);
		void LogMessage(const char* pszFileName, const char* strMessage, int32 i, e_MessageType messageType = e_MESSAGE);
		void LogMessage(const char* pszFileName, const char* strMessage, float f, e_MessageType messageType = e_MESSAGE);

		///Closes the current Log open
		void CloseLog(const char* pszFileName);

	private:
		static Logger*& _Instance();

		Map<String, FileStream*> m_Logs;
	};
};