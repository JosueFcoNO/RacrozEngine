#include <RZComputePCH.h>

namespace rczEngine
{
	bool MemBuffer::CreateMemBuffer(size_t memSize, eMEMORY_FLAGS flags, ComputeAPI * capi)
	{
		return capi->CreateBuffer(*this, flags, memSize);
	}

	bool MemBuffer::EnqueueReadBuffer(size_t dataSize, void * ptrToData, ComputeAPI * capi)
	{
		return capi->EnqueueReadBuffer(*this, ptrToData, dataSize);
	}

	bool MemBuffer::EnqueueWriteBuffer(size_t dataSize, void * ptrToData, ComputeAPI * capi)
	{
		return capi->EnqueueWriteBuffer(*this, dataSize, ptrToData);
	}

	void MemBuffer::Destroy()
	{
		clReleaseMemObject(m_MemBuffer);
	}
}