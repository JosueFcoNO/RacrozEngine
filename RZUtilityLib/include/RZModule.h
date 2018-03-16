#pragma once
#include <RZUtilityPCH.h>

namespace rczEngine
{
	template <class T>
	class RZ_UTILITY_EXPORT Module
	{
	public:
		static T& Instance()
		{
			return *_Instance();
		}

		static T* InstancePtr()
		{
			return _Instance();
		}

		template<class... Args>
		static void StartUp(Args&&... args)
		{
			
			_Instance() = new T(std::forward<Args>(args)...);
			
			//IsShutDown() = false;

			((Module*)_Instance())->OnStartUp();
		}

		static void ShutDown()
		{
			((Module*)_Instance())->OnShutDown();

			delete(_Instance());
			IsShutDown() = true;
		}

	protected:
		Module()
		{
		}

		virtual ~Module() {}

		virtual void OnStartUp()
		{

		}

		virtual void OnShutDown()
		{

		}

		static T*& _Instance()
		{
			static T* inst = nullptr;
			return inst;
		}

		static bool& IsShutDown()
		{
			static bool inst = true;
			return inst;
		}
	};

};

