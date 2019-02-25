#pragma once
#include "RZUtilityPCH.h"
#include "RZMemoryAllocators.h"

namespace rczEngine
{
	///////////////////////////////////////////////////////
	////////////////////////////////////////Heap Allocator
	///////////////////////////////////////////////////////
	//
	//template<class T>
	//T * HeapAllocator::allocate(SIZE_T num)
	//{
	//	T* Temp;
	//
	//	if (num > 0)
	//	{
	//		Temp = malloc(sizeof(T)*num);
	//		return Temp;
	//	}
	//	else
	//	{
	//		return nullptr
	//	}
	//}
	//
	//template<class T>
	//void HeapAllocator::deallocate(T * ptr, SIZE_T num)
	//{
	//	for (int i = 0; i < num; i++)
	//	{
	//		free(ptr + i)
	//	}
	//}
	//
	//template<class T>
	//T * HeapAllocator::HeapNew(uint32 nObjects, std::string Scope, uint32 ObjectType)
	//{
	//	T* Temp;
	//	T* First;
	//
	//	for (int i = 0; i < nObjects; i++)
	//	{
	//			Temp = new nObjects;
	//			HeapNode New;
	//			New.m_AllocatedBytes = sizeof(T);
	//			New.m_Pointer = Temp;
	//			New.m_Scope = Scope;
	//			New.m_ObjType = ObjectType;
	//			AllocationData.push_back(New);
	//
	//			if (i == 0)
	//			{
	//				First = Temp;
	//			}
	//	
	//	}
	//
	//	return First;
	//}
	//
	//template<class T>
	//void HeapAllocator::HeapDelete(T * pointer, uint32 nObjects)
	//{
	//	for (int i = 0; i < nObjects; i++)
	//	{
	//		delete pointer + i;
	//	}
	//}
	//
	///////////////////////////////////////////////
	////////////////////////////////Frame Allocator
	///////////////////////////////////////////////
	//
	//
	//FrameAllocator::FrameAllocator(SIZE_T stackSize)
	//{
	//	m_Pointer = malloc(stackSize);
	//	m_ByteIndex = 0;
	//	m_StackSize = stackSize;
	//}
	//
	//FrameAllocator::~FrameAllocator()
	//{
	//	free(m_Pointer);
	//}
	//
	//void FrameAllocator::FrameStart()
	//{
	//	m_ByteIndex = 0;
	//}
	//
	//template<class T>
	//T * FrameAllocator::allocate(SIZE_T num)
	//{
	//	///Check if there's enough bytes for the allocation, if not return nullptr
	//	if (m_StackSize - num * sizeof(T) > 0)
	//	{
	//		///Create a temp pointer and point to the current m_Pointer+m_ByteIndex
	//		T* Pointer = m_Pointer + m_ByteIndex;
	//		///Move forward in the stack num*sizeof(T) bytes
	//		m_ByteIndex += num * sizeof(T);
	//		///Return the pointer to the new "allocated" data
	//		return Pointer;
	//	}
	//
	//	return nullptr;
	//}
	//
	//template<class T>
	//void FrameAllocator::deallocate(T * ptr, SIZE_T num)
	//{
	//}
	//
	//////////////////////////////////////////////////////
	////////////////////////////////////////////Memory Pool
	///////////////////////////////////////////////////////
	//
	//template<class T>
	//MemoryPoolAllocator<T>::MemoryPoolAllocator(uint32 PoolSize)
	//{
	//	m_Pointer = new T[PoolSize];
	//	m_PoolSize = PoolSize;
	//	m_PoolIndex = 0;
	//}
	//
	//template<class T>
	//T * MemoryPoolAllocator<T>::allocate(SIZE_T num)
	//{
	//	if (num > m_PoolIndex - m_PoolSize)
	//	{
	//		m_PoolIndex = 0;
	//	}
	//
	//	return m_Pointer + m_PoolIndex++;
	//}
	//
	//template<class T>
	//void MemoryPoolAllocator<T>::deallocate(T * ptr, SIZE_T num)
	//{
	//}

};
