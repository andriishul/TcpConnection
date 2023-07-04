#include "tcp/tcp.h"
#include <stdexcept>
#include <string>
#include <future>
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
namespace winsock {
	TcpClient::~TcpClient()
	{
		delete hints;
	}

	TcpClient::TcpClient()
	{
		int s_Error = DEFAULT_SIZE;
		addrinfo hints;
		WSADATA data;
		s_Error = WSAStartup(MAKEWORD(2, 2), &data);

		if (s_Error == WSASYSNOTREADY)
			throw std::runtime_error{ "WSAStartup error: WSASYSNOTREADY" };
		if (s_Error == WSAVERNOTSUPPORTED)
			throw std::runtime_error{ "WSAStartup error: WSAVERNOTSUPPORTED" };
		if (s_Error == WSAEINPROGRESS)
			throw std::runtime_error{ "WSAStartup error: WSAEINPROGRESS" };
		if (s_Error == WSAEPROCLIM)
			throw std::runtime_error{ "WSAStartup error: WSAEPROCLIM" };
		if (s_Error == WSAEFAULT)
			throw std::runtime_error{ "WSAStartup error: WSAEFAULT" };
		if (!!s_Error)
			return;

		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;
		this->hints = new addrinfo;
		memcpy(this->hints, &hints, sizeof(hints));
	}

	bool TcpClient::Connect(const char* nodeName, const char* service_name)
	{
		addrinfo* res = static_cast<addrinfo*>(GetAddressInfo(hints, nodeName, service_name)), * ptr = 0;

		for (ptr = res; ptr != 0; ptr = ptr->ai_next)
		{
			//socket creating
			ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
			if (ConnectSocket == INVALID_SOCKET) {
				GetError("socket()");
				WSACleanup();
				return false;
			}
			if (connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen) == SOCKET_ERROR)
			{
				GetError("connect()");
				closesocket(ConnectSocket);
				ConnectSocket = INVALID_SOCKET;
				continue;
			}
			break;
		}


		FreeAddrInfo(res);
		if (ConnectSocket == INVALID_SOCKET)
		{
			throw std::runtime_error{ "invalid socket generated" };
			WSACleanup();
			return false;
		}
		return true;
	}

	bool TcpClient::Send(DataPacket & data)
	{  
		 return send(ConnectSocket, data.GetValue<char>(), data.GetSize(), 0) == data.GetSize();
	}


	bool winsock::TcpClient::Disconnect()
	{
		return closesocket(ConnectSocket) == 0;
	}

	DataPacket* TcpClient::Receive()
	{
		
		char* buffer = new char[DEFAULT_SIZE];
		int bytes_num = recv(ConnectSocket, buffer, DEFAULT_SIZE, MSG_PEEK);
		if (bytes_num < 1) {
			delete[] buffer;
			return 0;
		}
		delete[] buffer;
		std::thread t{ [&]() {
		buffer = new char[bytes_num];
		bytes_num = recv(ConnectSocket, buffer, bytes_num, 0);
		} };
		Sleep(1000);
		t.detach();
		return bytes_num > 1 ? new DataPacket(buffer, bytes_num) : 0;
	}


	void* GetAddressInfo(void* hints, const char* nodeName, const char* service_name)
	{
		ADDRINFOA* result = 0;
		int erCode = getaddrinfo(nodeName, service_name, (ADDRINFOA*)hints, &result);
		const char* err = gai_strerrorA(erCode);
		if (!!erCode)
			throw std::runtime_error{ err };

		return result;
	}

	void GetError(const char* error_msg)
	{
		int errCode = WSAGetLastError();
		if (errCode == WSANOTINITIALISED)
			throw std::runtime_error{ std::string{error_msg} + ". error code is WSANOTINITIALISED" };
		if (errCode == WSAENETDOWN)
			throw std::runtime_error{ std::string{error_msg} + ". error code is WSAENETDOWN" };
		if (errCode == WSAEADDRINUSE)
			throw std::runtime_error{ std::string{error_msg} + ". error code is WSAEADDRINUSE" };
		if (errCode == WSAEINTR)
			throw std::runtime_error{ std::string{error_msg} + ". error code is WSAEINTR" };
		if (errCode == WSAEINPROGRESS)
			throw std::runtime_error{ std::string{error_msg} + ". error code is WSAEINPROGRESS" };
		if (errCode == WSAEALREADY)
			throw std::runtime_error{ std::string{error_msg} + ". error code is WSAEALREADY" };
		if (errCode == WSAEADDRNOTAVAIL)
			throw std::runtime_error{ std::string{error_msg} + ". error code is WSAEADDRNOTAVAIL" };
		if (errCode == WSAEAFNOSUPPORT)
			throw std::runtime_error{ std::string{error_msg} + ". error code is WSAEAFNOSUPPORT" };
		if (errCode == WSAECONNREFUSED)
			throw std::runtime_error{ std::string{error_msg} + ". error code is WSAECONNREFUSED" };
		if (errCode == WSAEFAULT)
			throw std::runtime_error{ std::string{error_msg} + ". error code is WSAEFAULT" };
		if (errCode == WSAEISCONN)
			throw std::runtime_error{ std::string{error_msg} + ". error code is WSAEISCONN" };
		if (errCode == WSAENETUNREACH)
			throw std::runtime_error{ std::string{error_msg} + ". error code is WSAENETUNREACH" };
		if (errCode == WSAEHOSTUNREACH)
			throw std::runtime_error{ std::string{error_msg} + ". error code is WSAEHOSTUNREACH" };
		if (errCode == WSAENOBUFS)
			throw std::runtime_error{ std::string{error_msg} + ". error code is WSAENOBUFS" };
		if (errCode == WSAENOTSOCK)
			throw std::runtime_error{ std::string{error_msg} + ". error code is WSAENOTSOCK" };
		if (errCode == WSAETIMEDOUT)
			throw std::runtime_error{ std::string{error_msg} + ". error code is WSAETIMEDOUT" };
		if (errCode == WSAEWOULDBLOCK)
			throw std::runtime_error{ std::string{error_msg} + ". error code is WSAEWOULDBLOCK" };
		if (errCode == WSAEACCES)
			throw std::runtime_error{ std::string{error_msg} + ". error code is WSAEACCES" };
		if (errCode == 10054)
			throw std::runtime_error{ std::string{error_msg}+"connection lost"};
		else
			throw std::runtime_error{ std::string{error_msg} + ". error code is " + std::to_string(errCode) };
	}
	bool TcpServer::IsRuning()const { return isRunning; }
	void TcpServer::Start()
	{
		isRunning = true;
	}

	void TcpServer::Stop()
	{
		isRunning = false;
		if (shutdown(ClientSocket, SD_BOTH) != 0)
			throw std::runtime_error{ "Error while shutting down on a socket" };
	}

	TcpServer::TcpServer(const char* host_address,const char* port) :ClientSocket(INVALID_SOCKET),
		ListenSocket(INVALID_SOCKET)
	{
		addrinfo hints;
		WSADATA data;
		WSAStartup(MAKEWORD(2, 2), &data);

		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;
		hints.ai_flags = AI_PASSIVE;

		addrinfo* result = (addrinfo*)GetAddressInfo(&hints, host_address, port);

		ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

		if (ListenSocket == INVALID_SOCKET) {
			GetError("socket()");

			FreeAddrInfo(result);
			closesocket(ListenSocket);
			return;
		}

		if (bind(ListenSocket, result->ai_addr, result->ai_addrlen) == SOCKET_ERROR)
		{
			GetError("bind()");
			FreeAddrInfo(result);
			closesocket(ListenSocket);
			return;
		}
		FreeAddrInfo(result);

		if (listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR)
		{
			GetError("listen()");
			closesocket(ListenSocket);
			return;
		}

		ClientSocket = accept(ListenSocket, nullptr, nullptr);
		if (ClientSocket == INVALID_SOCKET)
		{
			GetError("accept()");
			closesocket(ListenSocket);
			return;
		}
		closesocket(ListenSocket);
	}

	TcpServer::~TcpServer()
	{
		closesocket(ClientSocket);
		WSACleanup();
	}

	void TcpServer::SetReceiveCallback(std::function<void(void*)> OnReceive)
	{
		ReceiveCallback = OnReceive;
	}

	

	void TcpServer::Update()
	{
		if (!isRunning)
			return;
		char* copy_buffer = 0;
		char* buffer = (char*)malloc(DEFAULT_SIZE);
		int numBytes = 0;
		numBytes = recv(ClientSocket, (char*)buffer, DEFAULT_SIZE, MSG_PEEK);
		if (numBytes > 0)
		{
			copy_buffer = (char*)malloc(numBytes);
			if (recv(ClientSocket, copy_buffer, numBytes, 0) == numBytes) {
				ReceiveCallback(copy_buffer);
				delete copy_buffer;
			}
			else abort();
		}
		//if (numBytes == SOCKET_ERROR) {
			//GetError("Server recv()");
		//}
		delete buffer;
	}
	bool TcpServer::Send(DataPacket& data)
	{
		return send(ClientSocket,data.GetValue<char>(),data.GetSize(),0) == data.GetSize();
	}
}

	
