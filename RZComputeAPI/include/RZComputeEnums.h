#pragma once

namespace rczEngine
{
	enum eMEMORY_FLAGS
	{
		MEM_READ_WRITE = 1,
		MEM_WRITE_ONLY = 2,
		MEM_READ_ONLY = 4,
		MEM_USE_HOST_PTR = 8,
		MEM_ALLOC_HOST_PTR = 16,
		MEM_COPY_HOST_PTR = 32
	};
}