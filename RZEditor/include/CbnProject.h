#pragma once

namespace rczEngine
{
	struct RendererSettings
	{
		bool UsesVXGI;
	};

	class RZ_EXP Project
	{
	public:

		void SaveCbn();

		void SetName(const String& name) {
			m_Name = name;
		};

		String GetName() const { return m_Name; };

		void SetPath(const StringW path)
		{
			m_RootPath = path;
		}

		StringW GetPath() const { return m_RootPath; };

		RendererSettings m_RendererSettings;

	private:
		String m_Name;
		StringW m_RootPath;

		void LoadProjectCbn(const StringW & path);

	};
};