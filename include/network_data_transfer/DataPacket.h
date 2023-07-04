#pragma once
#include "Core_TCP.hpp"
#include <memory>
class TCP_API DataPacket
{
private:
	int size;
	void* val_ptr;
public:
	DataPacket(void* value_ptr,int size) :val_ptr(value_ptr),size(size) {}
	DataPacket() = delete;
	template<typename t>
	const t* GetValue()const { return (t*)val_ptr; }
	int GetSize()const { return size; }
};
