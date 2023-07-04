#pragma once
#include "Core_TCP.hpp"
#include "network_data_transfer/DataPacket.h"

class TCP_API Client 
{
public:
	virtual ~Client() = default;
	Client() = default;
	virtual bool Connect(const char* nodeName, const char* service_name) = 0;
	virtual bool Send(DataPacket&) = 0;
	virtual bool Disconnect() = 0;
	virtual DataPacket* Receive() = 0;
};