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

            //NetLog(level::INFO, "GQCS FALSE ErrorCode:%d Bytes:%d", errorcode, transferred_bytes);	// TEST
            if (transferred_bytes == 0 || errorcode == ERROR_NETNAME_DELETED)
            {
				// 종료 처리
				if (completion_key != 0)
				{
					NetUnit* connection = reinterpret_cast<NetUnit*>(completion_key);
					if (connection)
						connection->Disconnect();
				}
            }

			if (overlapped != 0)           // operation 이 있는 경우
			{
				OverlappedEx* over_ex = static_cast<OverlappedEx*>(overlapped);
				if (over_ex->operation == OP_SEND)
				{
					OverlappedPool::GetInstance()->ReturnObj(over_ex);
				}
			}

            continue;
        }

        // return true 인 경우
        if (overlapped != 0 )           // operation 이 있는 경우
        {
            OverlappedEx* over_ex = static_cast<OverlappedEx*>(overlapped);
            if (over_ex->operation == OP_ACCEPT && transferred_bytes == 0)
            {
                NetUnit* new_connection = reinterpret_cast<NetUnit*>(over_ex->owner);
                if (new_connection)
                    new_connection->AcceptPost();
            }
            else if (over_ex->operation == OP_RECIEVE && transferred_bytes != 0)
            {
                if (completion_key == 0)
                    continue;

                NetUnit* connection = reinterpret_cast<NetUnit*>(completion_key);
                if (connection)
                    connection->RecievePost(transferred_bytes);
            }
            else if(over_ex->operation == OP_SEND)
            {
				OverlappedPool::GetInstance()->ReturnObj(over_ex);

				if (transferred_bytes != 0)
				{
					if (completion_key == 0)
						continue;

					NetUnit* connection = reinterpret_cast<NetUnit*>(completion_key);
					if (connection)
						connection->SendPost(transferred_bytes);
				}
            }
            else if(transferred_bytes == 0)
            {
                // 종료 처리
                if (completion_key == 0)
                    continue;

                NetUnit* connection = reinterpret_cast<NetUnit*>(completion_key);
                if (connection)
					connection->Disconnect();
            }
        }
        else
        {
            if (transferred_bytes == 0)
            {
                // 종료 처리
                if (completion_key == 0)
                    continue;

                NetUnit* connection = reinterpret_cast<NetUnit*>(completion_key);
                if (connection)
                    connection->Disconnect();
            }
        }
     
    }  
}
