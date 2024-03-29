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
	IocpLog(level::LogLevel::FATAL, "[%d] NetUnit Destroy!!!!!!", _index);

	_overlapped_accept.Clear();
	_overlapped_connect.Clear();
	_overlapped_recv.Clear();
}

void NetUnit::Init(Direction direction)
{	
	_direction = direction;

	if (_direction == Direction::DIR_CONNECT_TO)
	{
		_own_socket = std::make_unique<suho::winnet::iocp::ConnectSocket>();
		PrepareAsyncConnect();
	}
	else
		_own_socket = std::make_unique<suho::winnet::sock::OverlappedSocket>();

	_accept_buffer.Create(1024);
	_recv_buffer.Create(_recv_buffersize);

    _overlapped_accept.operation = OperatonType::OP_ACCEPT;
    _overlapped_accept.owner = this;

	_overlapped_connect.operation = OperatonType::OP_CONNECT;
	_overlapped_connect.owner = this;

    _overlapped_recv.operation = OperatonType::OP_RECIEVE;
    _overlapped_recv.owner = this;	

    OnInit();
}

void NetUnit::AcceptRequest()
{
	if (atomic_load(&_is_active))
		return;

	if (!_listen_socket->IsValidSocket())
	{
		IocpLog(level::LogLevel::ERR, "Accept() listensocket is not allocate");
		return;
	}

    _own_socket->AsyncAccept(_listen_socket->GetSocket(), _accept_buffer.GetWritePos(), _accept_buffer.GetWritableSize(), &_overlapped_accept);
}

void NetUnit::Accepted(DWORD recvbytes)
{
	UNREFERENCED(recvbytes)

		INT localaddr_len = 0;
	INT remoteaddr_len = 0;

	SOCKADDR* localaddr = NULL;
	SOCKADDR* remoteaddr = NULL;

	GetAcceptExSockaddrs(_accept_buffer.GetReadPos(), 0,
		sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16,
		&localaddr, &localaddr_len, &remoteaddr, &remoteaddr_len);

	if (remoteaddr_len == 0)
	{
		IocpLog(level::LogLevel::ERR, "GetAcceptExSockaddrs Fail");
	}
	else
	{
		_local_address.Set(localaddr);
		_remote_address.Set(remoteaddr);
	}

	if (_own_socket->SetUpdateAcceptContext(_listen_socket->GetSocket()))
		IoStart();
}

bool NetUnit::ConnectRequest(const suho::winnet::SocketAddress & sockaddr)
{
	if (atomic_load(&_is_active))
	{
		return false;
	}

	ConnectSocket* socket = dynamic_cast<ConnectSocket*>(_own_socket.get());
	if (socket == nullptr)
	{
		IocpLog(level::LogLevel::FATAL, "connect() fail to [%s] ConnectSocket is null", sockaddr.GetIP().ToString().c_str());
		return false;
	}

	if (!socket->AsyncConnect(sockaddr, &_overlapped_connect))
	{
		IocpLog(level::LogLevel::ERR, "connect() fail to [%s] AsyncConnect Fail", sockaddr.GetIP().ToString().c_str());
		return false;
	}

	return true;
}

void NetUnit::Connected(DWORD recvbytes)
{
	UNREFERENCED(recvbytes)

	ConnectSocket* socket = dynamic_cast<ConnectSocket*>(_own_socket.get());
	if (socket == nullptr)
		return;

	if (socket->SetUpdateConnectContext())
		IoStart();
}

void NetUnit::RecieveRequest()
{
	if (!atomic_load(&_is_active))
		return;

    _overlapped_recv.Reset();

	if (!_own_socket->AsyncRecieve(_recv_buffer.GetWritePos(), _recv_buffer.GetWritableSize(), &_overlapped_recv))
	{
		DisconnectRequest();
	}
}

void NetUnit::Recieved(DWORD recvbytes)
{
	_recv_buffer.ShiftWritePos(recvbytes);
	_processing_size += recvbytes;

	while (_processing_size > 0)
	{
		if (!atomic_load(&_is_active))
			return;

		DWORD packetsize = HeaderParsing(_recv_buffer.GetReadPos(), _processing_size);
		if (packetsize == 0)
			break;

		PacketProcessing(_recv_buffer.GetReadPos(), packetsize);

		if (!atomic_load(&_is_active))
			return;

		// read_pos 를 패킷크기단위로 이동시키므로 HeaderParsing에서는 항상 패킷 시작점을 가리킴
		_recv_buffer.ShiftReadPos(packetsize);
		_processing_size -= packetsize;
	}

	if (!atomic_load(&_is_active))
		return;

	_recv_buffer.CarriageReturn();

	RecieveRequest();
}

int NetUnit::SendRequest(void * buffer, int size)
{
	if (!atomic_load(&_is_active))
		return -1;

	OverlappedEx* over_send = OverlappedPool::GetInstance()->GetOverlapped( OperatonType::OP_SEND, this);

    int sendbytes = _own_socket->AsyncSend(reinterpret_cast<char*>(buffer), size, over_send);
	if (sendbytes == -1)
	{
		OverlappedPool::GetInstance()->ReturnObj(over_send);
		DisconnectRequest();
		return -1;
	}

	return sendbytes;
}

void NetUnit::Sent(DWORD sendbytes)
{
	UNREFERENCED(sendbytes)
}

void NetUnit::DisconnectRequest()
{
	bool flag = true;
	if (_is_active.compare_exchange_strong(flag, false))
	{
		_own_socket->SetLinger(true, 0);
		_own_socket->ShutDown(SD_BOTH);

		if (_direction == Direction::DIR_CONNECT_TO)
			_own_socket->Close();
		else
			_own_socket->Disable();

		OverlappedEx* over_disconnect = OverlappedPool::GetInstance()->GetOverlapped( OperatonType::OP_DISCONNECT, this);

		PostQueuedCompletionStatus(Iocp->GetIocpHandle(), 0, reinterpret_cast<ULONG_PTR>(this), over_disconnect);
	}
}

void NetUnit::Disconnect()
{
	OnDisconnect();

	Cleanup();

	if (_direction == Direction::DIR_ACCEPT_FROM)
	{		
		_own_socket->Reuse();
		AcceptRequest();
	}
	else
	{
		PrepareAsyncConnect();
	}
}

bool NetUnit::ConnectTo(const suho::winnet::SocketAddress & sockaddr)
{
	if (atomic_load(&_is_active))
		return false;

	bool success = _own_socket->Connect(sockaddr);
	if (success)
	{
		IoStart();
	}

	return success;
}

void NetUnit::IoStart()
{
	bool flag = false;
	if (_is_active.compare_exchange_strong(flag, true))
	{
		if( _direction == Direction::DIR_ACCEPT_FROM)
			BindIocp();

		OnConnect();

		_own_socket->Enable();

		RecieveRequest();
	}
}

void NetUnit::Cleanup()
{
	_connect_id = -1;

	_accept_buffer.Clear();
	_recv_buffer.Clear();

	_local_address.Clear();
	_remote_address.Clear();

	_overlapped_accept.Reset();
	_overlapped_connect.Reset();
	_overlapped_recv.Reset();

	_processing_size = 0;
}

void NetUnit::BindIocp()
{
	if (!_own_socket->IsValidSocket())
		return;

	Iocp->Associate(reinterpret_cast<HANDLE>(_own_socket->GetSocket()), reinterpret_cast<ULONG_PTR>(this));
}

void NetUnit::PrepareAsyncConnect()
{
	_own_socket->Bind(SocketAddress(INADDR_ANY, 0));
	BindIocp();
}

bool NetUnit::IsConnected()
{
	return _own_socket->IsConnected();
}
