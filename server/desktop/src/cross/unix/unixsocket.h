#pragma once
#ifdef  __unix__
#include <sys/socket.h>
#include "socket.h"
#include "exeption.h"
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SIZE_FIRST_MESSAGE sizeof(long)
class UnixSocket : public ISocket
{
	int sock;
	sockaddr_in sAddr;
public:
	UnixSocket();
	UnixSocket(int descriptor);
	UnixSocket(short port, char *ip, int aFamily = AF_INET, int type = SOCK_STREAM, int protocol = IPPROTO_TCP);
	UnixSocket(short port, int aFamily = AF_INET, int type = SOCK_STREAM, int protocol = IPPROTO_TCP);
	void InitSocket(int aFamily, int type, int protocol);
	void SetSocketPort(short port, char* srvAddr, int aFamily = AF_INET);
	void SetDirectPort(short port);
	void SetSocketPort(short port, int srvAddr, int aFamily = AF_INET);
	int RecvAll(char *buf);
	int Recv(char *buf, int size);
	int Send(char *buf, int size);
	int SendAll(char *buf, int bufLen);
	void Connect();
	void ShutDown(How shutHow);
	void Close();
	void Bind();
	void Listen(int len);
	ISocket* Accept();
	int RecvFrom();
	int SendTo();
};


#endif