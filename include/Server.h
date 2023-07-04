#pragma once
#include "Core_TCP.hpp"
#include "network_data_transfer/DataPacket.h"
#include <functional>
class TCP_API Server 
{
public:
	virtual void Start() = 0;
	virtual bool Send(DataPacket&) = 0;
	virtual void Stop() = 0;
	virtual void Update() = 0;
	virtual bool IsRuning()const = 0;
	virtual void SetReceiveCallback(std::function<void(void*)> OnReceive) = 0;
};
