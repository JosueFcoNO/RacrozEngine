#pragma once

namespace rczEngine
{
	class RZ_EXP Kernel
	{
	public:
		~Kernel() { Destroy(); };

		bool CreateKernel(char* entryPoint, ComputeAPI* capi, Program* programWithKernel);
		bool SetKernelArgument(MemBuffer& mem, size_t slot);
		void Destroy();

		cl_kernel m_Kernel = NULL;
	};
}