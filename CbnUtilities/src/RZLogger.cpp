#include "CbnUtilitiesPCH.h"

namespace rczEngine
{
	Logger*& Logger::_Instance() noexcept
	{
		static Logger* instance = nullptr;
		return instance;
	}

	void Logger::Start()
	{
		(_Instance()) = new Logger;
	}

	gsl::not_null<Logger*> Logger::Pointer() noexcept
	{
		return _Instance();
	}

	gsl::not_null<Logger*> Logger::PointerOrCreate()
	{
		if (_Instance() == nullptr)
		{
			Start();
		}

		return Pointer();
	}

	void Logger::ShutDown() noexcept
	{
		delete _Instance();
	}

	RZ_EXP void Logger::Log(const String & string, eLogMsgType type)
	{
		m_LoggedStrings.push_back(LogObject{ string, type });
	}

	void Logger::StartLog(const String& pszFileName)
	{
		m_LoggedStrings.reserve(128);

		///Create the fstream obj
		m_Logs[pszFileName] = std::make_unique<FileStream>();

		///Create and open the new file as pszFileName+HH:MM:SS/DD/MM/YYYY
		auto file = pszFileName + __DATE__ + " " + __TIME__ + ".html";
		
		for (auto& c : file)
		{
			if (c == ':') c = '-';
		}

		m_Logs[pszFileName]->open(file, std::ios_base::out | std::ios_base::trunc);

		///Write the header and html labels of the Log
		*m_Logs[pszFileName] << "<html><header><h3>Racroz Engine Logger</h3>  <link rel=\"stylesheet\" type=\"text/css\" href=\"Log.css\"></header><body><p>Body Start</p>";
	}

	void Logger::LogMessageToFileLog(const String& pszFileName, const String& strMessage, eLogMsgType messageType)
	{
		const auto it = m_Logs.find(pszFileName);

		if (it == m_Logs.end()) return;

		///Write the message over the </body and </html labels
		///Sum the message size to the index and re-write the html end labels
		switch (messageType)
		{
		default:
		case eLogMsgType::Message:
			*m_Logs[pszFileName] << "<div class = \"Message\">";
			break;
		case eLogMsgType::Warning:
			*m_Logs[pszFileName] << "<div class = \"Warning\"> ";
			break;
		case eLogMsgType::Error:
			*m_Logs[pszFileName] << "<div class = \"Error\"> ";
			break;
		case eLogMsgType::CriticalError:
			*m_Logs[pszFileName] << "<div class = \"FError\"> ";
			break;
		}

		*m_Logs[pszFileName] << strMessage;
		*m_Logs[pszFileName] << "</div>";

		Log(strMessage, messageType);
	}

	void Logger::CloseLog(const String& pszFileName)
	{
		*m_Logs[pszFileName] << "</table></body></html>";

		m_Logs[pszFileName]->close();
	}
};
