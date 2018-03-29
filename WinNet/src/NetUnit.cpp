#include "NetUnit.h"
#include "Tokenizer.h"
#include "IocpServer.h"
#include "ConnectSocket.h"
#include "OverlappedPool.h"

#include <iostream>

using namespace suho::winnet;
using namespace suho::winnet::iocp;
using namespace suho::log;

NetUnit::NetUnit(int index):
	_index(index)
{
}


NetUnit::~NetUnit()
{
	_overlapped_accept.Clear();
	_overlapped_recv.Clear();
	//_overlapped_send.Clear();
}

void NetUnit::Init(Direction direction)
{	
	_direction = direction;

	if( _direction == DIR_CONNECT_TO )
		_own_socket = std::make_unique<suho::winnet::iocp::ConnectSocket>();
	else
		_own_socket = std::make_unique<suho::winnet::sock::OverlappedSocket>();

	_accept_buffer.Create(128);
	_recv_buffer.Create(_recv_buffersize);

    _overlapped_accept.operation = OP_ACCEPT;
    _overlapped_accept.owner = this;

    _overlapped_recv.operation = OP_RECIEVE;
    _overlapped_recv.owner = this;

    //_overlapped_send.operation = OP_SEND;
    //_overlapped_send.owner = this;

    OnInit();
}

void NetUnit::Reset()
{
	_is_active = true;
}

void NetUnit::Cleanup()
{
	_is_active = false;

	_accept_buffer.Clear();
	_recv_buffer.Clear();

	_local_address.Clear();
	_remote_address.Clear();

	_overlapped_accept.Reset();
	_overlapped_recv.Reset();
	//_overlapped_send.Reset();

	_processing_size = 0;
}

void NetUnit::Disconnect()
{
	std::lock_guard<std::mutex> lock(_mutex);

    //NetLog(level::INFO, "[%d] Disconnected!", _index);		// TEST

	_own_socket->Close();

    Cleanup();
    OnDisconnect();

	if(_direction == DIR_ACCEPT_FROM )
		Accept();
}

void NetUnit::DisconnectRequest()
{
	std::lock_guard<std::mutex> lock(_mutex);

    //NetLog(level::INFO, "[%d] Request Disconnect", _index);		// TEST

	_own_socket->ShutDown(SD_BOTH);

    PostQueuedCompletionStatus(Iocp->GetIocpHandle(), 0, reinterpret_cast<ULONG_PTR>(this), NULL);
}

bool NetUnit::ConnectTo(const suho::winnet::SocketAddress & sockaddr)
{
	bool success = _own_socket->Connect(sockaddr);
	if (success)
		ConnectPost();

	return success;
}

void NetUnit::ConnectPost()
{
	Reset();

	Iocp->Associate(reinterpret_cast<HANDLE>(_own_socket->GetSocket()), reinterpret_cast<ULONG_PTR>(this));

	OnConnect();

	Recieve();
}

void NetUnit::Accept()
{
	if (!_listen_socket->IsValidSocket())
	{
		NetLog(level::ERR, "Accept() listensocket is not allocate");
		return;
	}

    _own_socket->AsyncAccept(_listen_socket->GetSocket(), _accept_buffer.GetWritePos(), _accept_buffer.GetWritableSize(), &_overlapped_accept);
}

void NetUnit::Recieve()
{
    _overlapped_recv.Reset();      

	if (!_own_socket->AsyncRecieve(_recv_buffer.GetWritePos(), _recv_buffer.GetWritableSize(), &_overlapped_recv))
	{
		DisconnectRequest();
	}
}

int NetUnit::Send(void * buffer, int size)
{
	//std::lock_guard<std::mutex> lock(_mutex);

    //_overlapped_send.Reset();
	OverlappedEx* over_send = OverlappedPool::GetInstance()->GetOverlapped(OP_SEND, this);

    int sendbytes = _own_socket->AsyncSend(reinterpret_cast<char*>(buffer), size, over_send);
	if (sendbytes == -1)
	{
		DisconnectRequest();
		return -1;
	}

	return sendbytes;
}

void NetUnit::AcceptPost()
{
	//std::lock_guard<std::mutex> lock(_mutex);

    //NetLog(level::INFO, "[%d] AcceptPost!", _index);		// TEST

	INT localaddr_len = 0;
	INT remoteaddr_len = 0;

	SOCKADDR* localaddr = NULL;
	SOCKADDR* remoteaddr = NULL;

	GetAcceptExSockaddrs(_accept_buffer.GetReadPos(), 0,
		sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16,
		&localaddr, &localaddr_len, &remoteaddr, &remoteaddr_len);

	if (remoteaddr_len == 0)
	{
		NetLog(level::ERR, "GetAcceptExSockaddrs Fail");
	}
	else
	{
		_local_address.Set(localaddr);
		_remote_address.Set(remoteaddr);
	}

	_own_socket->SetUpdateAcceptContext(_listen_socket->GetSocket());

	Iocp->Associate(reinterpret_cast<HANDLE>(_own_socket->GetSocket()), reinterpret_cast<ULONG_PTR>(this));

	Reset();

	OnConnect();

	Recieve();
}

void NetUnit::RecievePost(DWORD recvbytes)
{
	//std::lock_guard<std::mutex> lock(_mutex);

    //NetLog(level::INFO, "[%d] RecvPost %d bytes recieve", _index, recvbytes);	// TEST
    
	_recv_buffer.ShiftWritePos(recvbytes);
	_processing_size += recvbytes;

	while (_processing_size > 0)
	{
		DWORD packetsize = HeaderParsing(_recv_buffer.GetReadPos(), _processing_size);
		if (packetsize == 0)
			break;
				
		PacketProcessing(_recv_buffer.GetReadPos(), packetsize);
		
		if (!_is_active)	// 패킷처리 수행중 연결끊어졌을 경우
			return;

		// read_pos 를 패킷크기단위로 이동시키므로 HeaderParsing에서는 항상 패킷 시작점을 가리킴
		_recv_buffer.ShiftReadPos(packetsize);
		_processing_size -= packetsize;
	}

	_recv_buffer.CarriageReturn();

    Recieve();
}

void NetUnit::SendPost(DWORD sendbytes)
{
    //NetLog(level::INFO, "[%d] SendPost %d bytes send", _index, sendbytes);	// TEST
}

bool NetUnit::IsConnected()
{
	std::lock_guard<std::mutex> lock(_mutex);

	bool is_connect = _own_socket->IsConnected();

	if (is_connect == false)
		_is_active = false;

	return is_connect;
}
