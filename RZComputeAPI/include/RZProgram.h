#pragma once

namespace rczEngine
{
	class RZ_EXP Program
	{
	public:
		~Program() { Destroy(); };

		bool CreateProgramFromFile(char* pszFilePath, ComputeAPI* capi);
		bool CreateProgramFromSource(char* source, size_t sourceSize, ComputeAPI* capi);
		bool BuildProgram(ComputeAPI* capi);
		void Destroy();

		cl_program m_Program = NULL;
	};
}