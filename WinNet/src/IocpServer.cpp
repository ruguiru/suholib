#include "IocpServer.h"
#include "ThreadManager.hpp"
#include "Timer.h"
#include "Tokenizer.h"
#include "FileReader.h"
#include "OverlappedPool.h"

using namespace suho::winnet;
using namespace suho::winnet::iocp;
using namespace suho::log;


IocpServer::IocpServer()
{	
}


IocpServer::~IocpServer()
{
	if( _iocp)
		CloseHandle(_iocp);

	_iocp = INVALID_HANDLE_VALUE;
}

bool IocpServer::InitIocp(const std::string& config_rootpath, int worker_thread_count)
{	
    _iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
    if (!_iocp)
        return false;

    CreateWorkerThread(worker_thread_count);

	_configfile_rootpath = config_rootpath;

    return true;
}

void IocpServer::Start()
{	
    IocpLog(level::INFO, "Start Server");    

	OverlappedPool::GetInstance()->Create(ConnectionManager::GetInstance()->GetSize() * 2);

    StartWorkerThread();

	ConnectionManager::GetInstance()->StartConnect();
}

void IocpServer::Wait()
{
	suho::thread::ThreadManager::GetInstance()->JoinAll();	// 모든 스레드 종료를 기다림

	_running.Wait();
}

void IocpServer::Terminate()
{
	// 돌고있는 스레드들 종료시켜주자.
	ConnectionManager::GetInstance()->StopConnect();
    TerminateWorkerThread();

	// 메모리도 정리
	ConnectionManager::GetInstance()->Destroy();

    _running.Set();
}

bool IocpServer::Associate(HANDLE handle, ULONG_PTR comp_key)
{
	return CreateIoCompletionPort(handle, _iocp, comp_key, 0) == _iocp;
}

void IocpServer::CreateWorkerThread(int worker_thread_count)
{
    if (worker_thread_count == 0)
    {
        SYSTEM_INFO systemInfo;

        GetSystemInfo(&systemInfo);
        _worker_thread_count = systemInfo.dwNumberOfProcessors * 2;
    }
    else
        _worker_thread_count = worker_thread_count;

    //_worker_threads.reset(new WorkerThread[_worker_thread_count]);	
	_worker_thread_array = std::make_unique<WorkerThread[]>(_worker_thread_count);
}

jss::atomic_shared_ptr<ListenSocket> IocpServer::Listen(unsigned short port)
{
	auto sock = std::make_shared<ListenSocket>();

	sock->Bind({ INADDR_ANY, port });
    sock->SetConditionalAccept(true);
    sock->Listen();

	Associate(reinterpret_cast<HANDLE>(sock->GetSocket()), 0);

	return std::move(sock);
}

void IocpServer::StartWorkerThread()
{
    for (DWORD i = 0; i<_worker_thread_count; i++)
    {
		_worker_thread_array[i].Start(_iocp);
    }

    IocpLog(level::INFO, "WorkerThreads Start count : %d", _worker_thread_count);
}

void IocpServer::TerminateWorkerThread()
{
    for (DWORD i = 0; i<_worker_thread_count; i++)
    {
        _worker_thread_array[i].Terminate();

		// 루프를 빠져나가도록 getqueued 블로킹을 풀어준다
		PostQueuedCompletionStatus(_iocp, 0, NULL, NULL);
    }
}

json IocpServer::LoadConnectionInfo(const std::string& direction, const std::string & connection_name)
{
	std::string fullpath(_configfile_rootpath);
	fullpath += ConnectionInfoFile;

	suho::file::FileReader fr(fullpath);
	int filesize = fr.GetFileSize();

	char* buf = new char[filesize];
	memset(buf, 0, filesize);

	fr.Read(buf, filesize);

	json root = json::parse(buf);

	delete[] buf;

	json acceptconn = root[direction];
	json conn = acceptconn[connection_name];

	return conn;
}


std::string IocpServer::EliminateFirstString(const std::string & fullname)
{
	suho::util::Tokenizer parser(fullname, ' ');
	parser.GetToken();
	if (parser.HasNext())
	{
		return parser.GetToken();
	}

	return "";
}
