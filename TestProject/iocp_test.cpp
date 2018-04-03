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

	Iocp->Wait();						// BaseThread ��ӹ޾� ������ ��� ������ ��ٸ�

    system("pause");
}

// ��� Ŭ������ �����ϰ�� �Ʒ� ������ ���Ѿ� �Ѵ�.
int main()
{
	atexit(Terminator);

    Iocp->InitIocp("../", 0);           // configfile path, create workerthread, 0�̸� �ھ��*2

	Iocp->InitAccepts<Client>();        // bind(), listen(), acceptex()
	// �� �ٸ� Ÿ���� Accepts Ŀ�ؼǵ� ���
	// ...
	
    Iocp->InitConnects<TimeServer>();	// bind(), connectex()
	// �� �ٸ� Ÿ���� Connect Ŀ�ؼǵ� ���
	// ...
	
	Iocp->Start();                      // start workerthread, start auto connect

	Statistic::GetInstance()->Start();	// ��� (�ʴ� ��� send, recv)

	Iocp->Wait();						// Iocp ���Ǻ��� �� TreadManager�� ����� ��� ������ ��ٸ�

	Terminator();
}