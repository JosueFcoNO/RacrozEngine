#pragma once

namespace rczEngine
{
	enum class eLogMsgType
	{
		Message,
		Warning, 
		Error, 
		CriticalError
	};

	///A class used to hold and remember differents logs. Used by the console to look at log history, filter log history or write the logs to a file.
	class LogObject
	{
	public:
		LogObject(const String& string, eLogMsgType type = eLogMsgType::Message, const String& logFile = "") noexcept : 
			m_String(string), m_LogFile(logFile), m_Type(type) {};

		FORCEINLINE String GetString() const noexcept { return m_String; };
		FORCEINLINE eLogMsgType GetType() const noexcept { return m_Type; };

	private:
		///The log file that it belongs to.
		String m_LogFile;

		///The actual log string.
		String m_String;

		///The LogMsgType;
		eLogMsgType m_Type;
	};
}