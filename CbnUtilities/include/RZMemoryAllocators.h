#pragma once
#include <CbnUtilitiesPCH.h>

namespace rczEngine
{
	//template <typename T>
	//class BaseAllocator
	//{
	//public:
	//	typedef size_t size_type;
	//	typedef ptrdiff_t difference_type;
	//	typedef T* pointer;
	//	typedef const T* const_pointer;
	//	typedef T& reference;
	//	typedef const T& const_reference;
	//	typedef T value_type;
	//
	//	BaseAllocator() {}
	//	virtual ~BaseAllocator() {}
	//
	//	template <class U> struct rebind { typedef BaseAllocator<U> other; };
	//	template <class U> BaseAllocator(const BaseAllocator<U>&) {}
	//
	//	pointer address(reference x) const { return &x; };
	//	const_pointer address(const_reference x) const { return &x; };
	//	size_type max_size() const throw() { return size_t(-1) / sizeof(value_type); }
	//
	//	virtual pointer allocate(size_type n, BaseAllocator<void>::const_pointer hint = 0)
	//	{
	//		return static_cast<pointer>(malloc(sizeof(T)*n));
	//	}
	//
	//	virtual void deallocate(pointer p, size_type n)
	//	{
	//		free(p);
	//	}
	//
	//	void construct(pointer p, const T& val)
	//	{
	//		new(static_cast<void*>(p)) T(val);
	//	}
	//
	//	void construct(pointer p)
	//	{
	//		new(static_cast<void*>(p)) T();
	//	}
	//
	//	void destroy(pointer p)
	//	{
	//		p->~T();
	//	}
	//};
	//
	//template <class T>
	//class StackAllocator : public BaseAllocator<T>
	//{
	//	StackAllocator() = delete;
	//	virtual StackAllocator()
	//	{
	//		//pointer = malloc(1024);
	//		//m_ByteIndex = 0;
	//		//m_StackSize = stackSize;
	//	}
	//	
	//	virtual ~StackAllocator()
	//	{
	//		//free(pointer);
	//	}
	//
	//	virtual pointer allocate(size_type n, BaseAllocator<void>::const_pointer hint = 0) override
	//	{
	//		///Check if there's enough bytes for the allocation, if not throw bad alloc.
	//		if (m_StackSize - n * sizeof(T) > 0)
	//		{
	//			///Create a temp pointer and point to the current m_Pointer+m_ByteIndex
	//			T* Pointer = m_Pointer + m_ByteIndex;
	//			///Move forward in the stack num*sizeof(T) bytes
	//			m_ByteIndex += n * sizeof(T);
	//			///Return the pointer to the new "allocated" data
	//			return static_cast<pointer>(Pointer);
	//		}
	//
	//		throw new std::bad_alloc();
	//	}
	//
	//	virtual void deallocate(pointer p, size_type n)
	//	{
	//		m_ByteIndex -= n * sizeof(T);
	//	}
	//
	//	uint32 m_ByteIndex;
	//	SIZE_T m_StackSize;
	//};

	//template <class T>
	//class HeapAllocator : public BaseAllocator<T>
	//{
	//	struct HeapNode
	//	{
	//		void* m_Pointer;
	//		String m_Scope;
	//		uint32 m_AllocatedBytes;
	//		uint32 m_ObjType;
	//	};
	//
	//	T* allocate(SIZE_T num) override;
	//	void deallocate(T* ptr, SIZE_T num) override;
	//
	//	T* HeapNew(uint32 nObjects, std::string Scope, uint32 ObjectType);
	//	void HeapDelete(T* pointer, uint32 nObjects);
	//
	//	List<HeapNode> AllocationData;
	//};
	//
	//template <class T>
	//class FrameAllocator : public BaseAllocator<T>
	//{
	//	FrameAllocator(SIZE_T stackSize);
	//	~FrameAllocator();
	//
	//	T* allocate(SIZE_T num) override;
	//	void deallocate(T* ptr, SIZE_T num) override;
	//
	//	void FrameStart();
	//
	//	void* m_Pointer;
	//	uint32 m_ByteIndex;
	//	SIZE_T m_StackSize;
	//};
	//
	//template <class T>
	//class MemoryPoolAllocator : public BaseAllocator<T>
	//{
	//	MemoryPoolAllocator(uint32 PoolSize);
	//
	//	T* allocate(SIZE_T num);
	//	void deallocate(T* ptr, SIZE_T num);
	//
	//	T* m_Pointer;
	//	SIZE_T m_PoolSize;
	//	uint32 m_PoolIndex;
	//
	//};
};