#include "test_common.h"
#include "IocpServer.h"
#include "ClientManager.h"
#include "Client.h"
#include "TimeServer.h"
#include "Statistic.h"

#include <stdlib.h>

using namespace std;
using namespace suho::winnet;
using namespace suho::winnet::iocp;
using namespace suho::log;


void Terminator()
{
    ClientManager::GetInstance()->Destroy();
	Statistic::GetInstance()->Terminate();
	Iocp->Terminate();

	Iocp->Wait();						// BaseThread 상속받아 구현한 모든 스레드 기다림

    system("pause");
}

// 통계 클래스를 제외하고는 아래 형식을 지켜야 한다.
int main()
{
	atexit(Terminator);

    Iocp->InitIocp("../", 0);           // configfile path, create workerthread, 0이면 코어개수*2

	Iocp->InitAccepts<Client>();        // bind(), listen(), acceptex()
	// 또 다른 타입의 Accepts 커넥션들 등록
	// ...
	
    Iocp->InitConnects<TimeServer>();	// bind(), connectex()
	// 또 다른 타입의 Connect 커넥션들 등록
	// ...
	
	Iocp->Start();                      // start workerthread, start auto connect

	Statistic::GetInstance()->Start();	// 통계 (초당 평균 send, recv)

	Iocp->Wait();						// Iocp 조건변수 및 TreadManager로 등록한 모든 스레드 기다림

	Terminator();
}