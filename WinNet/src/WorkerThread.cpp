#include "WorkerThread.h"
#include "NetUnit.h"
#include "IocpServer.h"
#include "OverlappedPool.h"

using namespace suho::winnet::iocp;

WorkerThread::WorkerThread()
{
}


WorkerThread::~WorkerThread()
{
}

void WorkerThread::Run(void * param)
{
    HANDLE iocp = reinterpret_cast<HANDLE>(param);
    
    while (_is_started)
    {
        DWORD transferred_bytes = 0;
        DWORD completion_key = 0;
        LPOVERLAPPED overlapped = nullptr;

        BOOL result = GetQueuedCompletionStatus(iocp, &transferred_bytes, &completion_key, &overlapped, INFINITE);
        if (result == FALSE)
        {   
            DWORD errorcode = GetLastError();

			if (overlapped != 0)
			{
				OverlappedEx* over_ex = static_cast<OverlappedEx*>(overlapped);
				if (over_ex->operation == OP_SEND || over_ex->operation == OP_DISCONNECT)
				{
					OverlappedPool::GetInstance()->ReturnObj(over_ex);
				}
			}

            if (transferred_bytes == 0 || errorcode == ERROR_NETNAME_DELETED)
            {
				// 종료 처리
				if (completion_key != 0)
				{
					NetUnit* connection = reinterpret_cast<NetUnit*>(completion_key);
					if (connection)
						connection->DisconnectRequest();
				}
            }

			continue;
        }

		if (overlapped == 0)
			continue;

		OverlappedEx* over_ex = static_cast<OverlappedEx*>(overlapped);		
        if (over_ex->operation == OP_ACCEPT)
        {
            NetUnit* new_connection = reinterpret_cast<NetUnit*>(over_ex->owner);
            if (new_connection)
                new_connection->Accepted(transferred_bytes);
        }
		else if (over_ex->operation == OP_CONNECT)
		{
			NetUnit* new_connection = reinterpret_cast<NetUnit*>(over_ex->owner);
			if (new_connection)
				new_connection->Connected(transferred_bytes);
		}
		else
		{
			OperatonType optype = over_ex->operation;
			if (over_ex->operation == OP_SEND || over_ex->operation == OP_DISCONNECT)
				OverlappedPool::GetInstance()->ReturnObj(over_ex);

			if (completion_key != 0)
			{
				NetUnit* connection = reinterpret_cast<NetUnit*>(completion_key);
				if (connection)
				{
					if (optype == OP_RECIEVE && transferred_bytes != 0)
					{
						connection->Recieved(transferred_bytes);
					}
					else if (optype == OP_SEND && transferred_bytes != 0)
					{
						connection->Sent(transferred_bytes);
					}
					else if (optype == OP_DISCONNECT)
					{
						connection->Disconnect();
					}
					else if (transferred_bytes == 0)
					{
						connection->Disconnect();
					}
				}
			}
		}
    }  
}
