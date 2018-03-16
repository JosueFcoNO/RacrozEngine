#include <RZComputePCH.h>

namespace rczEngine
{
	ComputeAPI*& ComputeAPI::_Instance()
	{
		static ComputeAPI* instance = nullptr;
		return instance;
	}

	void ComputeAPI::Start()
	{
		(_Instance()) = new ComputeAPI;
	}

	ComputeAPI* ComputeAPI::Pointer()
	{
		return _Instance();
	}

	void ComputeAPI::ShutDown()
	{
		delete _Instance();
	}

	void ComputeAPI::InitComputeAPI(Gfx::GfxCore* gfx)
	{
		cl_uint ret_num_devices;
		cl_uint ret_num_platforms;

		cl_platform_id* plat;
		cl_int ret = clGetPlatformIDs(3, &m_PlatformID, &ret_num_platforms);
		plat = (cl_platform_id*)malloc(sizeof(cl_platform_id*)*ret_num_platforms);

		ret = clGetPlatformIDs(3, plat, &ret_num_platforms);

		char extension_string[1024];
		memset(extension_string, ' ', 1024);

		clGetPlatformInfo(plat[1], CL_PLATFORM_NAME, sizeof(extension_string), extension_string, NULL);
		clGetPlatformInfo(plat[1], CL_PLATFORM_EXTENSIONS, sizeof(extension_string), extension_string,NULL);

		char *extStringStart = NULL;
		extStringStart = strstr(extension_string, "cl_khr_dx11_sharing");

		if (extStringStart != 0) { printf("Platform does support cl_khr_dx11_sharing\n"); };

		ret = clGetDeviceIDs(plat[1], CL_DEVICE_TYPE_ALL, 1, &m_DeviceID, &ret_num_devices);

		clGetDeviceInfo(m_DeviceID, CL_DEVICE_NAME, sizeof(extension_string), extension_string, NULL);

		//clGetDeviceIDsFromD3D11KHR_fn func2 = (clGetDeviceIDsFromD3D11KHR_fn)clGetExtensionFunctionAddressForPlatform(plat[1], "clGetDeviceIDsFromD3D11KHR");
		//ret = func2(plat[1], CL_D3D11_DEVICE_KHR, gfx->GetDevice(), CL_ALL_DEVICES_FOR_D3D11_KHR, 1, &m_DeviceID, &ret_num_devices);

		m_PlatformID = plat[1];

		cl_context_properties cps[] = { CL_CONTEXT_PLATFORM, (cl_context_properties)m_PlatformID, CL_CONTEXT_D3D11_DEVICE_KHR, NULL, CL_CONTEXT_INTEROP_USER_SYNC, CL_FALSE, 0 };
		m_Context = clCreateContext(NULL, 1, &m_DeviceID, NULL, NULL, &ret);

		m_CommandQueue = clCreateCommandQueue(m_Context, m_DeviceID, 0, &ret);
		
		//func = (clCreateFromD3D11BufferKHR_fn)clGetExtensionFunctionAddress("clCreateFromD3D11BufferKHR");
	}

	bool ComputeAPI::LoadAndCreateProgramFromFile(Program& out_program, char * pszFilePath)
	{
		FILE * fp;
		char* source_str;
		size_t source_size;

		fopen_s(&fp, pszFilePath, "r");

		if (!fp)
		{
			fprintf(stderr, "Failed to load kermes.\n");
			exit(1);
		}

		source_str = (char*)malloc(10000000);
		source_size = fread(source_str, 1, 10000000, fp);
		fclose(fp);

		cl_int ret;
		out_program.m_Program = clCreateProgramWithSource(m_Context, 1, (const char**)&source_str, (const size_t *)&source_size, &ret);

		free(source_str);

		return (ret == CL_SUCCESS);
	}

	bool ComputeAPI::CreateProgramFromSource(Program& out_program, char* pszProgramSource, size_t srcSize)
	{
		cl_int ret;
		out_program.m_Program = clCreateProgramWithSource(m_Context, 1, (const char**)&pszProgramSource, (const size_t *)&srcSize, &ret);

		return (ret == CL_SUCCESS);
	}

	bool ComputeAPI::BuildProgram(Program& out_program)
	{
		auto ret = clBuildProgram(out_program.m_Program, 1, &m_DeviceID, NULL, NULL, NULL);

		if (ret == CL_BUILD_PROGRAM_FAILURE) {
			// Determine the size of the log
			size_t log_size;
			clGetProgramBuildInfo(out_program.m_Program, m_DeviceID, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);

			// Allocate memory for the log
			char *logg = (char *)malloc(log_size);

			// Get the log
			clGetProgramBuildInfo(out_program.m_Program, m_DeviceID, CL_PROGRAM_BUILD_LOG, log_size, logg, NULL);

			// Print the log
			printf("%s\n", logg);
		}

		return (ret == CL_SUCCESS);
	}

	bool ComputeAPI::CreateKernel(Kernel & out_kernel, Program & programWithKernel, char * entryPoint)
	{
		cl_int ret;
		out_kernel.m_Kernel = clCreateKernel(programWithKernel.m_Program, entryPoint, &ret);

		return (ret == CL_SUCCESS);
	}

	bool ComputeAPI::RunKernel(Kernel & kernel, size_t globalItemSize, size_t localItemSize)
	{
		auto ret = clEnqueueNDRangeKernel(m_CommandQueue, kernel.m_Kernel, 1, NULL, &globalItemSize, &localItemSize, 0, NULL, NULL);

		return (ret == CL_SUCCESS);
	}

	bool ComputeAPI::CreateBuffer(MemBuffer& out_memBuffer, eMEMORY_FLAGS flags, size_t memorySize)
	{
		cl_int ret;
		out_memBuffer.m_MemBuffer = clCreateBuffer(m_Context, flags, memorySize, NULL, &ret);

		return (ret == CL_SUCCESS);
	}

	bool ComputeAPI::CreateBufferFromD3D11(MemBuffer & out_memBuffer, Gfx::BasicBuffer * buffer, eMEMORY_FLAGS flags)
	{
		cl_int ret;

		out_memBuffer.m_MemBuffer = func(m_Context, flags, buffer->m_Buffer, &ret);

		return (ret == CL_SUCCESS);
	}

	bool ComputeAPI::EnqueueWriteBuffer(MemBuffer & out_MemBuffer, size_t dataSize, void* ptrToData)
	{
		auto ret = clEnqueueWriteBuffer(m_CommandQueue, out_MemBuffer.m_MemBuffer, CL_TRUE, 0, dataSize, ptrToData, 0, NULL, NULL);

		return (ret == CL_SUCCESS);
	}

	bool ComputeAPI::EnqueueReadBuffer(MemBuffer& memBuffer, void* destBuffer, size_t sizeOfBuffer)
	{
		auto ret = clEnqueueReadBuffer(m_CommandQueue, memBuffer.m_MemBuffer, CL_TRUE, 0, sizeOfBuffer, destBuffer, 0, NULL, NULL);

		return (ret == CL_SUCCESS);
	}

	void ComputeAPI::Destroy()
	{
		clFlush(m_CommandQueue);
		clFinish(m_CommandQueue);
		//clRetainMemObject(a_mem_obj);
		//clRetainMemObject(b_mem_obj);
		//clRetainMemObject(c_mem_obj);
		clReleaseCommandQueue(m_CommandQueue);
		clReleaseContext(m_Context);
	}
}
