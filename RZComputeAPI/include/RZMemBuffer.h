#pragma once

namespace rczEngine
{
	class RZ_UTILITY_EXPORT MemBuffer
	{
	public:
		~MemBuffer() { Destroy(); };

		bool CreateMemBuffer(size_t memSize, eMEMORY_FLAGS flags, ComputeAPI* capi);
		bool EnqueueReadBuffer(size_t dataSize, void* ptrToData, ComputeAPI* capi);
		bool EnqueueWriteBuffer(size_t dataSize, void* ptrToData, ComputeAPI* capi);
		void Destroy();

		cl_mem m_MemBuffer = NULL;
	};
}