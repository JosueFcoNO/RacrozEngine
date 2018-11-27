#define LOGGING 1

namespace rczEngine
{
	enum eLogMessageType
	{
		log_MESSAGE, log_WARNING, log_ERROR, log_CRITICALERROR
	};

	class LogObject
	{
	public:
		void Init(const String& string, eLogMessageType type = log_MESSAGE, const String& logFile = "")
		{
			m_String = string;
			m_LogFile = logFile;
			m_Type = type;
		};

		FORCEINLINE String GetString() { return m_String; };
		FORCEINLINE eLogMessageType GetType() { return m_Type; };

	private:
		String m_LogFile = "";
		String m_String = "";
		eLogMessageType m_Type = log_MESSAGE;
	};

	//A logger class that can write to multiple logs and is accesible everywhere. 
	class Logger
	{
	public:
		RZ_EXP static void Start();
		RZ_EXP static Logger* Pointer();
		RZ_EXP static void ShutDown();

		RZ_EXP void Log(const String& string, eLogMessageType type = log_MESSAGE);
		RZ_EXP void ClearLog() { m_LoggedStrings.clear(); };
		RZ_EXP Vector<LogObject>& GetLoggedStrings() { return m_LoggedStrings; };
	
		///Create a Log File and load it into this object
		RZ_EXP void StartLog(const char* pszFileName);

		///Print a Log Message onto the output
		RZ_EXP void LogMessageToFileLog(String pszFileName, String strMessage, eLogMessageType messageType = log_MESSAGE);
		RZ_EXP void LogMessageToFileLog(const char* pszFileName, const char* strMessage, eLogMessageType messageType = log_MESSAGE);
		RZ_EXP void LogMessageToFileLog(const char* pszFileName, const char* strMessage, int32 i, eLogMessageType messageType = log_MESSAGE);
		RZ_EXP void LogMessageToFileLog(const char* pszFileName, const char* strMessage, float f, eLogMessageType messageType = log_MESSAGE);

		///Closes the current Log open
		RZ_EXP void CloseLog(const char* pszFileName);

	private:
		static Logger*& _Instance();

		Map<String, FileStream*> m_Logs;
		Vector<LogObject> m_LoggedStrings;
	};
};