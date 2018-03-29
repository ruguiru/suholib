#pragma once

#include <type_traits>
#include <memory>

#include "WinNet.h"
#include "Singleton.hpp"
#include "EventSync.h"
#include "Chrono.h"
#include "WorkerThread.h"
#include "json.hpp"
#include "ConnectionManager.hpp"
#include "ListenSocket.h"
#include "atomic_shared_ptr.hpp"

#define TypeToString(classname) std::string(#classname)
#define ConnectionInfoFile	"ConnectionConfig.json"

using namespace nlohmann;			// json lib


namespace suho {
namespace winnet {
namespace iocp
{
    class IocpServer : public suho::pattern::singleton::Singleton<IocpServer>
    {
    public:       
        IocpServer();
        ~IocpServer();

		// workerthread count == 0 이면 코어개수 * 2
        bool InitIocp(const std::string& config_rootpath, int worker_thread_count = 0);

        template<typename T>	inline void InitAccepts();
        template<typename T>	inline void InitConnects();        
		      
		void Start();
		void Wait();
        void Terminate();

		bool Associate(HANDLE handle, ULONG_PTR comp_key);

        HANDLE GetIocpHandle() const { return _iocp; }        
		
    private:
        jss::atomic_shared_ptr<ListenSocket> Listen(unsigned short port);		// listenSocket 생성 후 리턴 

        void CreateWorkerThread(int worker_thread_count);        
        void StartWorkerThread();
        void TerminateWorkerThread();

		json LoadConnectionInfo(const std::string& direction, const std::string& connection_name);

		std::string EliminateFirstString(const std::string& fullname);


	private:
		suho::thread::EventSync									_running;

        HANDLE													_iocp = INVALID_HANDLE_VALUE;
        DWORD													_worker_thread_count = 0;
		std::string												_configfile_rootpath;
        std::unique_ptr<WorkerThread[]>							_worker_thread_array;
    };


	template<typename T>
	inline void IocpServer::InitAccepts()
	{
		static_assert(std::is_base_of<NetUnit, T>::value, "InitAccepts() T is not NetUnit or Child of NetUnit");

		std::string type_name = EliminateFirstString(typeid(T).name());

		json connection = LoadConnectionInfo("AcceptConnections", type_name);		
		short port = connection["Port"];
		int max_accept = connection["MaxAccepts"];		

        auto listensock = Listen(port);															// bind() and listen()

		ConnectionManager::GetInstance()->InitAcceptConnections<T>(max_accept, listensock);		// create and acceptex

        NetLog(level::INFO, "Init [%s] Accept type! Count:%d Port:%d", type_name.c_str(), max_accept, port);
	}

	template<typename T>
	inline void IocpServer::InitConnects()
	{
		static_assert(std::is_base_of<NetUnit, T>::value, "InitConnects() T is not NetUnit or Child of NetUnit");

		std::string type_name = EliminateFirstString(typeid(T).name());

		json connectinfo = LoadConnectionInfo("ConnectConnections", type_name);

		int interval_msec = connectinfo["RetryIntervalmsec"];
		json dest_array = connectinfo["Destinations"];
		int arrcnt = dest_array.size();
		for (int i = 0; i < arrcnt; i++)
		{
			json dest = dest_array[i];
			int id = dest["ID"];
			std::string address = dest["Address"];
			short port = dest["Port"];			

			ConnectionManager::GetInstance()->InitConnectConnection<T>(interval_msec, id, address, port);

            NetLog(level::INFO, "Init [%s] Connect type! Address:%s Port:%d", type_name.c_str(), address.c_str(), port);
		}
	}

    

}
}
}

#define Iocp suho::winnet::iocp::IocpServer::GetInstance()