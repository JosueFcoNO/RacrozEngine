#pragma once
#include <RZUtilityPCH.h>

namespace rczEngine
{
	
	class MemAllocator
	{
	public:
		template <class T>
		T* allocate(SIZE_T num);

		template <class T>
		void deallocate(T* ptr, SIZE_T num);
	};

	class StackAllocator : public MemAllocator
	{
		StackAllocator(SIZE_T stackSize);
		~StackAllocator();

		template <class T>
		T* allocate(SIZE_T num);

		template <class T>
		void deallocate(T* ptr, SIZE_T num);


		void* m_Pointer;
		uint32 m_ByteIndex;
		SIZE_T m_StackSize;
	};

	class HeapAllocator : public MemAllocator
	{
		struct HeapNode
		{
			void* m_Pointer;
			String m_Scope;
			uint32 m_AllocatedBytes;
			uint32 m_ObjType;
		};

		template <class T>
		T* allocate(SIZE_T num);

		template <class T>
		void deallocate(T* ptr, SIZE_T num);

		template<class T>
		T* HeapNew(uint32 nObjects, std::string Scope, uint32 ObjectType);

		template<class T>
		void HeapDelete(T* pointer, uint32 nObjects);

		List<HeapNode> AllocationData;
	};

	class FrameAllocator : public MemAllocator
	{
		FrameAllocator(SIZE_T stackSize);
		~FrameAllocator();

		template <class T>
		T* allocate(SIZE_T num);

		template <class T>
		void deallocate(T* ptr, SIZE_T num);

		void FrameStart();

		void* m_Pointer;
		uint32 m_ByteIndex;
		SIZE_T m_StackSize;
	};

	template <class T>
	class MemoryPoolAllocator : public MemAllocator
	{
		MemoryPoolAllocator(uint32 PoolSize);

		T* allocate(SIZE_T num);

		void deallocate(T* ptr, SIZE_T num);

		T* m_Pointer;
		SIZE_T m_PoolSize;
		uint32 m_PoolIndex;

	};
};