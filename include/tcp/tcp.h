#pragma once
#include "Client.h"
#include "Server.h"



namespace winsock
{
	/// <summary>
	/// return value is addrinfo pointer information about the host.
	/// </summary>
	/// <param name="hints">:	A pointer to an addrinfo structure that provides hints about the type of socket the caller supports</param>
	/// <param name="nodeName">:	host (node) name or a numeric host address string.</param>
	/// <param name="service_name">:	service name or port number represented as a string.</param>
	/// <returns></returns>
	TCP_API void* GetAddressInfo(void* hints,const char* nodeName, const char* service_name);
	TCP_API void GetError(const char* error_msg);
	class TCP_API TcpClient : public Client
	{
	private:
		void* hints;
		unsigned long long ConnectSocket;
		
	public: 
		~TcpClient();
		TcpClient();
		//parameters description in GetAddressInfo
		bool Connect(const char* nodeName, const char* service_name)override;
		bool Send(DataPacket&) override;
		bool Disconnect() override;
		DataPacket* Receive()override;
	};

	class TCP_API TcpServer : public Server
	{
	private:
		using recv_event_callback = std::function<void(void*)>;
		recv_event_callback ReceiveCallback;
		bool isRunning;
		unsigned long long ClientSocket, ListenSocket;
	public:
		~TcpServer();
		TcpServer(const char* host_address,const char* port);
		void Start() override;
		void Stop() override;
		bool Send(DataPacket&)override;
		void Update()override;
		bool IsRuning()const override;
		void SetReceiveCallback(std::function<void(void*)>)override;
	};
}
