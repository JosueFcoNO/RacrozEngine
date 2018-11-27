#pragma once
#include "RZUtilityPCH.h"

namespace rczEngine
{
	/////////Containers//////////

	template<typename t>
	using List = RZ_EXP std::list<t>;

	template <class t>
	using Vector = RZ_EXP std::vector<t>;
	
	template<class t>
	using Stack = RZ_EXP std::stack<t>;
	
	template<class t>
	using Deque = RZ_EXP std::deque<t>;

	template<class t>
	using Queue = RZ_EXP std::queue<t>;

	template<class t>
	using Set = RZ_EXP std::set<t>;

	template<class t>
	using PriorityQueue = RZ_EXP std::priority_queue<t>;

	template<class t, class k>
	using Map = RZ_EXP std::map<t,k>;

	template <typename Key, typename Type, typename Hash = std::hash<Key>, typename Pred = std::equal_to<Key>, typename Alloc = std::allocator<std::pair<const Key, Type>>>
	using UoMap = RZ_EXP std::unordered_map<Key,Type, Hash, Pred, Alloc>;

	template <class t, class k>
	using UoSet = RZ_EXP std::unordered_set<t, k>;

	template<class t>
	using MSet = RZ_EXP std::multiset<t>;

	template<class t, class k>
	using MMap = RZ_EXP  std::multimap<t, k>;

	using FileStream = RZ_EXP std::fstream;

	//////////Strings//////////////

	template<typename Char_Ty>
	using BasicString = RZ_EXP std::basic_string<Char_Ty>;

	using String = RZ_EXP BasicString<ANSICHAR>;
	
	using StringW = RZ_EXP BasicString<UNICHAR>;

	using StringStream = RZ_EXP std::stringstream;

	////////Smart Pointers////////

	template <class k, class t>
	using Pair = RZ_EXP std::pair<k, t>;

	template <class c>
	using StrPtr = RZ_EXP std::shared_ptr<c>;

	template <class c>
	using WeakPtr = RZ_EXP std::weak_ptr<c>;

	template <class c>
	using UniquePtr = RZ_EXP std::unique_ptr<c>;

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