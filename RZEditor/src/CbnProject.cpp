#include <RZEditorPCH.h>

namespace rczEngine
{
	////////////////////////////
	/// Renderer Settings Conversions
	////////////////////////////

	void to_json(nlohmann::json& j, const RendererSettings& p)
	{
		j = nlohmann::json{ {"VXGIOn", p.UsesVXGI} };
	}

	void from_json(const nlohmann::json& j, RendererSettings& p)
	{
		p.UsesVXGI = j.at("VXGIOn");
	}


	////////////////////////////
	/// Project Json Conversions
	////////////////////////////

	void to_json(nlohmann::json& j, const Project& p)
	{
		j = nlohmann::json{ {"name", p.GetName()}, {"path", p.GetPath()}, {"rendererSettings", p.m_RendererSettings} };
	}

	void from_json(const nlohmann::json& j, Project& p)
	{
		p.SetName(j.at("name"));
		p.SetPath(j.at("path"));
		p.m_RendererSettings = j.at("rendererSettings");
	}

	////////////////////////////
	/// Project Saving
	////////////////////////////

	void Project::LoadProjectCbn(const StringW& path)
	{
		std::ifstream i(path);
		nlohmann::json inJson;
		i >> inJson;

		(*this) = inJson;
	}

	void Project::SaveCbn()
	{
		FileStream o(m_RootPath + L"/" + TextTool::AnsiToUni(m_Name.c_str()) + L".cbn", FileStream::out | FileStream::trunc);

		nlohmann::json outJson = (*this);

		o << outJson;
	}

};