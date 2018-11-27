#include "RZUtilityPCH.h"

namespace rczEngine
{
	Logger*& Logger::_Instance()
	{
		static Logger* instance = nullptr;
		return instance;
	}

	void Logger::Start()
	{
		(_Instance()) = new Logger;
	}

	Logger* Logger::Pointer()
	{
		return _Instance();
	}

	void Logger::ShutDown()
	{
		delete _Instance();
	}

	RZ_EXP void Logger::Log(const String & string, eLogMessageType type)
	{
		LogObject newLog;
		newLog.Init(string, type);
		m_LoggedStrings.push_back(newLog);
	}

	void Logger::StartLog(const char * pszFileName)
	{
		///Create the fstream obj
		auto ptr = Pair<String, FileStream*>(pszFileName, new FileStream());
		m_Logs.insert(ptr);

		///Create and open the new file as pszFileName+HH:MM:SS/DD/MM/YYYY
		m_Logs[pszFileName]->open(String(pszFileName) + ".html", std::ios_base::out | std::ios_base::trunc);

		///Write the header and html labels of the Log
		*m_Logs[pszFileName] << "<html><header><h3>Racroz Engine Logger</h3>  <link rel=\"stylesheet\" type=\"text/css\" href=\"Log.css\"></header><body><p>Body Start</p>";
	}

	void Logger::LogMessageToFileLog(const char* pszFileName, const char* strMessage, eLogMessageType messageType)
	{
		LogMessageToFileLog(pszFileName, String(strMessage), messageType);
	}

	void Logger::LogMessageToFileLog(String pszFileName, String strMessage, eLogMessageType messageType)
	{
		auto it = m_Logs.find(pszFileName);

		if (it == m_Logs.end()) return;

		///Write the message over the </body and </html labels
		///Sum the message size to the index and re-write the html end labels
		switch (messageType)
		{
		default:
		case log_MESSAGE:
			*m_Logs[pszFileName] << "<div class = \"Message\">";
			break;
		case log_WARNING:
			*m_Logs[pszFileName] << "<div class = \"Warning\"> ";
			break;
		case log_ERROR:
			*m_Logs[pszFileName] << "<div class = \"Error\"> ";
			break;
		case log_CRITICALERROR:
			*m_Logs[pszFileName] << "<div class = \"FError\"> ";
			break;
		}

		*m_Logs[pszFileName] << strMessage;
		*m_Logs[pszFileName] << "</div>";

		Log(strMessage, messageType);
	}

	void Logger::LogMessageToFileLog(const char* pszFileName, const char* strMessage, int32 i, eLogMessageType messageType)
	{
		LogMessageToFileLog(pszFileName, String(strMessage) + TextTool::IntTo(i), messageType);
	}

	void Logger::LogMessageToFileLog(const char* pszFileName, const char* strMessage, float f, eLogMessageType messageType)
	{
		LogMessageToFileLog(pszFileName, String(strMessage) + TextTool::FloatTo(f), messageType);
	}

	void Logger::CloseLog(const char* pszFileName)
	{
		*m_Logs[pszFileName] << "</table></body></html>";

		m_Logs[pszFileName]->close();
	}
};
