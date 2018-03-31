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
        void Clear() { memset(this, 0, sizeof(OverlappedEx)); }     // 전체 초기화        
        void Reset() { memset(this, 0, sizeof(OVERLAPPED)); }       // 부모만 초기화 (operation과 owner는 고정이므로)
	};
	
	class NetUnit : public suho::idioms::NonCopyable
	{
	public:
		NetUnit() {}
		explicit NetUnit(int index);
		virtual ~NetUnit();
        
		void Init(Direction direction);		// 생성시 한번 실행		
		
		// iocp에 요청
        void AcceptRequest();					// AcceptEx
        void RecieveRequest();					// WSARecv
        int SendRequest(void* buffer, int size);// WSASend		

		void Accept(DWORD recvbytes);			// 통지 후 처리
		void Recieve(DWORD recvbytes);
		void Send(DWORD sendbytes);		

		void DisconnectRequest();				// postqueued 로 직접 넘김
		void Disconnect();						// 즉시 종료 처리
				
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
		// 서브클래싱용 각 시점에 맞게 하위클래스에서 override
		// Init() 이후 호출
		virtual void OnInit() {};          
		// 연결 성공 후 호출
		virtual void OnConnect() {};       
		// Disconnect() 이후 호출		
		virtual void OnDisconnect() {};    

		// 패킷크기 리턴. 패킷크기에 도달하지 못하면 추가 데이터 요청
		// 이함수 진입시 _recv_buffer의 _read_pos 가 패킷의 시작점을 가리키고 있음
		virtual DWORD HeaderParsing(void* packet, DWORD recvbytes) { UNREFERENCED(packet); UNREFERENCED(recvbytes); return 0; }
		// 패킷 처리.  로직 대부분이 이 함수안에서 실행. packet 데이터는 복사해서 써야한다
		virtual void PacketProcessing(const void* packet, DWORD packetsize) { UNREFERENCED(packet); UNREFERENCED(packetsize); }

		void IoStart();                                             // 연결시 초기화

        void Cleanup();                                             // 연결종료시 초기화
		void BindIocp();											// Iocp에 연결

	protected:
		const int					                                _index = -1;		// mem pool 상에서 인덱스
		int															_connect_id = -1;	// connect 시 json 파일에 있는 ID값

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


