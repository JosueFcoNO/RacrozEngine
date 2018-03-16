#include <RZComputePCH.h>

namespace rczEngine
{
	bool Program::CreateProgramFromFile(char * pszFilePath, ComputeAPI * capi)
	{
		return capi->LoadAndCreateProgramFromFile(*this, pszFilePath);
	}

	bool Program::CreateProgramFromSource(char * source, size_t sourceSize, ComputeAPI * capi)
	{
		return capi->CreateProgramFromSource(*this, source, sourceSize);
	}

	bool Program::BuildProgram(ComputeAPI * capi)
	{
		return capi->BuildProgram(*this);
	}

	void Program::Destroy()
	{
		clReleaseProgram(m_Program);
	}
}