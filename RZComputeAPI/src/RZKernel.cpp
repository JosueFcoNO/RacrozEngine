#include <RZComputePCH.h>

namespace rczEngine
{
	bool Kernel::CreateKernel(char * entryPoint, ComputeAPI * capi, Program * programWithKernel)
	{
		return capi->CreateKernel(*this, *programWithKernel, entryPoint);
	}

	bool Kernel::SetKernelArgument(MemBuffer & mem, size_t slot)
	{
		auto ret = clSetKernelArg(m_Kernel, slot, sizeof(cl_mem), (void*)&mem.m_MemBuffer);

		return (ret == CL_SUCCESS);
	}

	void Kernel::Destroy()
	{
		clReleaseKernel(m_Kernel);
	}
}