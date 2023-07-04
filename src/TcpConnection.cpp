#include "TcpConnection.hpp"
#include "tcp/tcp.h"



Client* TCPConnectionFactory::CreateClient()
{
	return new winsock::TcpClient;
}

Server* TCPConnectionFactory::CreateServer(const char* host, const char* port)
{
	return new winsock::TcpServer{host,port};
}
