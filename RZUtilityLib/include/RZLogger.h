#define LOGGING 1

namespace rczEngine
{
	//A logger class that can write to multiple logs and is accesible everywhere. 
	class Logger
	{
	public:
		RZ_EXP static void Start();
		RZ_EXP static gsl::not_null<Logger*> Pointer();
		RZ_EXP static gsl::not_null<Logger*> PointerOrCreate();
		RZ_EXP static void ShutDown();

		RZ_EXP void Log(const String& string, eLogMsgType type = eLogMsgType::Message);
		RZ_EXP FORCEINLINE void ClearLog() noexcept { m_LoggedStrings.clear(); };
		RZ_EXP FORCEINLINE auto& GetLoggedStrings() const noexcept { return m_LoggedStrings; };
	
		///Create a Log File and load it into this object
		RZ_EXP void StartLog(const String& pszFileName);

		///Print a Log Message onto the output
		RZ_EXP void LogMessageToFileLog(const String& pszFileName, const String& strMessage, eLogMsgType messageType = eLogMsgType::Message);

		template <typename t>
		void LogMessageToFileLog(const String& pszFileName, const String& strMessage, t value, eLogMsgType messageType = eLogMsgType::Message)
		{
			LogMessageToFileLog(pszFileName, strMessage + std::to_string(value), messageType);
		}

		///Closes the current Log open
		RZ_EXP void CloseLog(const String& pszFileName);

	private:
		static Logger*& _Instance() noexcept;

		Map<String, UniquePtr<FileStream>> m_Logs;
		Vector<LogObject> m_LoggedStrings;
	};
};