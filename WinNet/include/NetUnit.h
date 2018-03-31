#pragma once

#include "WinNet.h"
#include "ListenSocket.h"
#include "OverlappedSocket.h"
#include "StreamBuffer.h"
#include "Noncopyable.h"
#include "atomic_shared_ptr.hpp"
#include "Common.h"

#include <string>
#include <memory>
#include <atomic>

namespace suho {
namespace winnet {
namespace iocp
{
    enum OperatonType
    {
        OP_ACCEPT = 0,
        OP_RECIEVE,
        OP_SEND,
		OP_DISCONNECT,
    };

	enum Direction
	{
		DIR_ACCEPT_FROM,
		DIR_CONNECT_TO,
	};
	
    struct OverlappedEx : OVERLAPPED
    {
		OperatonType operation;
        void* owner = 0;

		OverlappedEx() { Clear(); }
        void Clear() { memset(this, 0, sizeof(OverlappedEx)); }     // ��ü �ʱ�ȭ        
        void Reset() { memset(this, 0, sizeof(OVERLAPPED)); }       // �θ� �ʱ�ȭ (operation�� owner�� �����̹Ƿ�)
	};
	
	class NetUnit : public suho::idioms::NonCopyable
	{
	public:
		NetUnit() {}
		explicit NetUnit(int index);
		virtual ~NetUnit();
        
		void Init(Direction direction);		// ������ �ѹ� ����		
		
		// iocp�� ��û
        void AcceptRequest();					// AcceptEx
        void RecieveRequest();					// WSARecv
        int SendRequest(void* buffer, int size);// WSASend		

		void Accept(DWORD recvbytes);			// ���� �� ó��
		void Recieve(DWORD recvbytes);
		void Send(DWORD sendbytes);		

		void DisconnectRequest();				// postqueued �� ���� �ѱ�
		void Disconnect();						// ��� ���� ó��
				
		bool ConnectTo(const suho::winnet::SocketAddress& sockaddr); // blocking connect

		bool IsConnected();		
        bool IsActive() const { return _is_active; }
        void SetListenSocket(const jss::atomic_shared_ptr<ListenSocket>& listen_sock) { _listen_socket = listen_sock; }		
		void SetConnectID(int connect_id) { _connect_id = connect_id; }
		int GetConnectID() const { _connect_id; }

		int GetIndex() const { return _index; }
		const SocketAddress& GetLoaclAddress() { return _local_address; }
		const SocketAddress& GetRemoteAddress() { return _remote_address; }		
		
        SOCKET GetSocket() const { return _own_socket->GetSocket(); }

	private:
		// ����Ŭ���̿� �� ������ �°� ����Ŭ�������� override
		// Init() ���� ȣ��
		virtual void OnInit() {};          
		// ���� ���� �� ȣ��
		virtual void OnConnect() {};       
		// Disconnect() ���� ȣ��		
		virtual void OnDisconnect() {};    

		// ��Ŷũ�� ����. ��Ŷũ�⿡ �������� ���ϸ� �߰� ������ ��û
		// ���Լ� ���Խ� _recv_buffer�� _read_pos �� ��Ŷ�� �������� ����Ű�� ����
		virtual DWORD HeaderParsing(void* packet, DWORD recvbytes) { UNREFERENCED(packet); UNREFERENCED(recvbytes); return 0; }
		// ��Ŷ ó��.  ���� ��κ��� �� �Լ��ȿ��� ����. packet �����ʹ� �����ؼ� ����Ѵ�
		virtual void PacketProcessing(const void* packet, DWORD packetsize) { UNREFERENCED(packet); UNREFERENCED(packetsize); }

		void IoStart();                                             // ����� �ʱ�ȭ

        void Cleanup();                                             // ��������� �ʱ�ȭ
		void BindIocp();											// Iocp�� ����

	protected:
		const int					                                _index = -1;		// mem pool �󿡼� �ε���
		int															_connect_id = -1;	// connect �� json ���Ͽ� �ִ� ID��

        std::atomic<bool>                                           _is_active = false;

		int                                                         _recv_buffersize = 1024;
		int															_recv_socket_buffersize = 1024 * 4;
		int															_send_socket_buffersize = 1024 * 8;

		std::unique_ptr<suho::winnet::sock::OverlappedSocket>       _own_socket;

	private:
		suho::buffer::StreamBuffer									_accept_buffer;
		suho::buffer::StreamBuffer									_recv_buffer;

		int															_processing_size = 0;
	
        jss::atomic_shared_ptr<ListenSocket>                        _listen_socket;

		SocketAddress												_local_address;
		SocketAddress												_remote_address;

        OverlappedEx                                                _overlapped_accept = {};
        OverlappedEx                                                _overlapped_recv = {};

		Direction													_direction;

		std::mutex													_mutex;
	};
}
}
}


