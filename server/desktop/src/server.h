#pragma once
#include "net/socket.h"

class Server
{
public:

	Server();
	~Server();
	 
	void Start();

private:
	Socket* listener;
};
