#pragma once

#include <stdlib.h>
#include <assert.h>
#include <atomic>
#include <mutex>
#include "Noncopyable.h"

namespace suho {
namespace pattern {
namespace singleton 
{
    namespace type
    {
        template<typename T>
        class BasicSingleton : private suho::idioms::NonCopyable
        {
        private:
            BasicSingleton() = default;
            ~BasicSingleton() = default;

        public:
            static T* GetInstance()
            {
                return &_instance;
            }

        private:
            static T                _instance;
        };

        template<typename T> T BasicSingleton<T>::_instance;

        template<typename T>
        class DynamicSingleton : private suho::idioms::NonCopyable
        {
        private:
            DynamicSingleton() = default;
            ~DynamicSingleton() = default;

            static void Destroy()
            {
                if (_instance)
                {
                    delete _instance;
                    _instance = nullptr;
                }
            }

        public:
            static T* GetInstance()
            {
                if (_instance == nullptr)
                {
					std::call_once(_flag, []() {_instance = new T(); });                    
                    atexit(Destroy);
                }

                return _instance;
            }

        private:
            static T*               _instance;
			static std::once_flag	_flag;
        };

        template<typename T> T* DynamicSingleton<T>::_instance = nullptr;

        template<typename T>
        class StaticSingleton : private suho::idioms::NonCopyable
        {
        private:
            StaticSingleton() = default;
            ~StaticSingleton() = default;

        public:
            static T* GetInstance()
            {
                static T _instance;
                return &_instance;
            }
        };

    }   // end namespace type

    template<typename T, typename Impl = suho::pattern::singleton::type::BasicSingleton<T>>
    class Singleton
    {
    protected:
        Singleton()
        {
            assert(!_is_create && "You Violated One Instance Rules Of Singleton");
            _is_create = true;
        }

        virtual ~Singleton()
        {
        }

    public:
        static T* GetInstance()
        {
            return  Impl::GetInstance();
        }        

    private:
        static std::atomic<bool> _is_create;// = false;
    };

    template<typename T, typename Impl> std::atomic<bool> Singleton<T, Impl>::_is_create = false;

}   // end namespace singleton
}   // end namespace pattern
}   // end namespace suho