#pragma once
#include "RZUtilityPCH.h"

namespace rczEngine
{
	/////////Containers//////////

	template<typename t>
	using List = std::list<t>;

	template <class t, typename alloc = std::allocator<t>>
	using Vector = std::vector<t, alloc>;
	
	template<class t>
	using Stack = std::stack<t>;
	
	template<class t>
	using Deque = std::deque<t>;

	template<class t>
	using Queue = std::queue<t>;

	template<class t>
	using Set = std::set<t>;

	template<class t>
	using PriorityQueue = std::priority_queue<t>;

	template<class t, class k>
	using Map = std::map<t,k>;

	template <typename Key, typename Type, typename Hash = std::hash<Key>, typename Pred = std::equal_to<Key>, typename Alloc = std::allocator<std::pair<const Key, Type>>>
	using UoMap = std::unordered_map<Key,Type, Hash, Pred, Alloc>;

	template <class t, class k>
	using UoSet = std::unordered_set<t, k>;

	template<class t>
	using MSet = std::multiset<t>;

	template<class t, class k>
	using MMap = std::multimap<t, k>;

	using FileStream = std::fstream;

	//////////Strings//////////////

	template<typename Char_Ty>
	using BasicString = std::basic_string<Char_Ty>;

	using String = BasicString<ANSICHAR>;
	
	using StringW = BasicString<UNICHAR>;

	using StringStream = std::stringstream;

	////////Smart Pointers////////

	template <class k, class t>
	using Pair = std::pair<k, t>;

	template <class c>
	using StrPtr = std::shared_ptr<c>;

	template <class c>
	using WeakPtr = std::weak_ptr<c>;

	template <class c>
	using UniquePtr = std::unique_ptr<c>;

	//////////Casts///////////

	template <class c>
	c CastStatic(c other)
	{
		return static_cast<c>(other);
	}

	template <typename t1, typename t2>
	StrPtr<t1> CastStaticPtr(StrPtr<t2> other)
	{
		return std::static_pointer_cast<t1, t2>(other);
	}

	template <typename t1, typename t2>
	StrPtr<t1> CastDynamicPtr(StrPtr<t2> other)
	{
		return std::dynamic_pointer_cast<t1, t2>(other);
	}

	template <typename t1, typename t2>
	StrPtr<t1> CastConstPtr(StrPtr<t2> other)
	{
		return std::const_pointer_cast<t1, t2>(other);
	}
};