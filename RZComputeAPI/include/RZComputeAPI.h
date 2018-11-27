#pragma once

namespace rczEngine
{
	class RZ_EXP ComputeAPI
	{
	private:
		static ComputeAPI*& _Instance();

	public:
		static void Start();
		static ComputeAPI* Pointer();
		static void ShutDown();

	public:
		~ComputeAPI() { Destroy(); }

		void InitComputeAPI(Gfx::GfxCore* gfx);

		bool LoadAndCreateProgramFromFile(Program& out_program, char* pszFilePath);
		bool CreateProgramFromSource(Program& out_program, char* pszProgramSource, size_t srcSize);
		bool BuildProgram(Program& out_program);

		bool CreateKernel(Kernel& out_kernel, Program& programWithKernel, char* entryPoint);
		bool RunKernel(Kernel& kernel, size_t globalItemSize, size_t localItemSize);

		bool CreateBuffer(MemBuffer& out_memBuffer, eMEMORY_FLAGS flags, size_t memorySize);
		bool CreateBufferFromD3D11(MemBuffer& out_memBuffer, Gfx::BasicBuffer* buffer,  eMEMORY_FLAGS flags);
		bool EnqueueWriteBuffer(MemBuffer& out_MemBuffer, size_t dataSize, void* ptrToData);
		bool EnqueueReadBuffer(MemBuffer& memBuffer, void* destBuffer, size_t sizeOfBuffer);

		void Destroy();

	private:
		cl_context m_Context;
		cl_command_queue m_CommandQueue;
		cl_device_id m_DeviceID = NULL;
		cl_platform_id m_PlatformID = NULL;
		clCreateFromD3D11BufferKHR_fn func;
	};

}