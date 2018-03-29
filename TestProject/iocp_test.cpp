#include "test_common.h"
#include "IocpServer.h"
#include "ClientManager.h"
#include "Client.h"
#include "TimeServer.h"
#include "Statistic.h"
#include "LogContainer.h"

#include <thread>
#include <chrono>
#include <future>
#include <list>
#include <array>
#include <type_traits>
#include <stdlib.h>

using namespace std;
using namespace suho::winnet;
using namespace suho::winnet::iocp;
using namespace suho::log;


void Terminator()
{
	SystemLog(level::INFO, "atexit()");

    ClientManager::GetInstance()->Destroy();
	Statistic::GetInstance()->Terminate();
	Iocp->Terminate();

	Iocp->Wait();						// BaseThread ��ӹ޾� ������ ��� ������ ��ٸ�

    system("pause");
}

int main()
{
	atexit(Terminator);

    Iocp->InitIocp("../", 0);           // create workerthread

	Iocp->InitAccepts<Client>();        // listen(), acceptex()
    Iocp->InitConnects<TimeServer>();	
	
	Iocp->Start();                      // start workerthread, start auto connect
	Statistic::GetInstance()->Start();

	Iocp->Wait();						// BaseThread ��ӹ޾� ������ ��� ������ ��ٸ�

	Terminator();
}