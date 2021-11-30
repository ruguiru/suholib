#pragma once

#include <map>
#include <vector>

#include "WinNet.h"
#include "NetUnit.h"
#include "Singleton.hpp"
#include "atomic_shared_ptr.hpp"
#include "AutoConnector.h"

namespace suho {
namespace winnet {
namespace iocp
{
	class ConnectionManager : public suho::pattern::singleton::Singleton<ConnectionManager>
	{
	public:
		ConnectionManager()
		{
			_netunit_storage.reserve(1024);
		}

		~ConnectionManager() override
		{
			_auto_connector.Terminate();
		}

		template<typename T> inline void InitAcceptConnections(
			int size, const jss::atomic_shared_ptr<ListenSocket>& listen_sock);
		template<typename T> inline void InitConnectConnection(
			int interval_msec, int id, const std::string& address, short port);

		const jss::atomic_shared_ptr<NetUnit>& Get(int index)
		{
			return _netunit_storage[index];
		}

		void StartConnect()	{ _auto_connector.Start(); }
		void StopConnect() { _auto_connector.Terminate(); }

		void Destroy()
		{
			_netunit_storage.clear();
			_auto_connector.Destroy();
		}

		int GetSize() const { return _netunit_storage.size(); }

	private:
		//typedef std::vector<jss::atomic_shared_ptr<NetUnit>>			Storage;
		using Storage = std::vector<jss::atomic_shared_ptr<NetUnit>>;
		// 서버 종료전까지 보관하여 sptr에 의해 메모리 삭제되는 일이 없게 한다
		Storage															_netunit_storage;
		
		AutoConnector													_auto_connector;
	};

	template<typename T>
	inline void ConnectionManager::InitAcceptConnections(
		int size, const jss::atomic_shared_ptr<ListenSocket>& listen_sock)
	{
		int index = _netunit_storage.size();
		while (size--)
		{
			jss::atomic_shared_ptr<NetUnit> netunit = std::make_shared<T>(index++);
			netunit->Init( Direction::DIR_ACCEPT_FROM);
			netunit->SetListenSocket(listen_sock);
			netunit->AcceptRequest();

			_netunit_storage.push_back(std::move(netunit));
		}		
	}

	template<typename T>
	inline void ConnectionManager::InitConnectConnection(
		int interval_msec, int id, const std::string& address, short port)
	{
		int index = _netunit_storage.size();
		jss::atomic_shared_ptr<NetUnit> netunit = std::make_shared<T>(index++);
		netunit->Init( Direction::DIR_CONNECT_TO);
		netunit->SetConnectID(id);

		_netunit_storage.push_back(netunit);

		_auto_connector.Register(netunit, interval_msec, address, port );
	}
}
}
}



