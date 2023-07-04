#pragma once

#include "Core_TCP.hpp"

#include "Client.h"
#include "Server.h"


class TCP_API TCPConnectionFactory 
{
public:
	static Client* CreateClient();
	//if host is null, then host is default
	static Server* CreateServer(const char* host,const char* port);
};