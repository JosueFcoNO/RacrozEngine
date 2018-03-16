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

	void Logger::StartLog(const char * pszFileName)
	{
		///Create the fstream obj
		auto ptr = Pair<String, FileStream*>(pszFileName, new FileStream());
		m_Logs.insert(ptr);

		///Create and open the new file as pszFileName+HH:MM:SS/DD/MM/YYYY
		m_Logs[pszFileName]->open(String(pszFileName)+".html", std::ios_base::out | std::ios_base::trunc);

		///Write the header and html labels of the Log
		*m_Logs[pszFileName] << "<html><header><h3>Racroz Engine Logger</h3>  <link rel=\"stylesheet\" type=\"text/css\" href=\"Log.css\"></header><body><p>Body Start</p>";
	}

	void Logger::LogMessage(const char* pszFileName, const char* strMessage, e_MessageType messageType)
	{
		LogMessage(pszFileName, String(strMessage), messageType);
	}

	void Logger::LogMessage(const char* pszFileName, String strMessage, e_MessageType messageType)
	{
		///Write the message over the </body and </html labels
		///Sum the message size to the index and re-write the html end labels
		switch (messageType)
		{
		case e_MESSAGE:
			*m_Logs[pszFileName] << "<div class = \"Message\">";
			*m_Logs[pszFileName] << strMessage;
			*m_Logs[pszFileName] << "</div>";
			break;
		case e_WARNING:
			*m_Logs[pszFileName] << "<div class = \"Warning\"> ";
			*m_Logs[pszFileName] << strMessage;
			*m_Logs[pszFileName] << "</div>";
			break;
		case e_ERROR:
			*m_Logs[pszFileName] << "<div class = \"Error\"> ";
			*m_Logs[pszFileName] << strMessage;
			*m_Logs[pszFileName] << "</div>";
			break;
		case e_CRITICALERROR:
			*m_Logs[pszFileName] << "<div class = \"FError\"> ";
			*m_Logs[pszFileName] << strMessage;
			*m_Logs[pszFileName] << "</div>";
			break;
		}
	}

	void Logger::LogMessage(const char* pszFileName, const char* strMessage, int32 i, e_MessageType messageType)
	{
		LogMessage(pszFileName, String(strMessage) + TextTool::IntTo(i), messageType);
	}

	void Logger::LogMessage(const char* pszFileName, const char* strMessage, float f, e_MessageType messageType)
	{
		LogMessage(pszFileName, String(strMessage) + TextTool::FloatTo(f), messageType);
	}

	void Logger::CloseLog(const char* pszFileName)
	{
		*m_Logs[pszFileName] << "</table></body></html>";

		m_Logs[pszFileName]->close();
	}
};
