#pragma once
//#include <windows.h>
//#include <WinSock2.h>
#pragma comment(lib,"Ws2_32.lib")
//#include <winsock2.h>
//#include <ws2tcpip.h>
#include "../isocket.h"

#include "windows.h"

#define SIZE_FIRST_MESSAGE sizeof(long)

class WinSocket : public ISocket
{
	SOCKET sock;
	WSADATA wsaData;
	sockaddr_in sAddr;
	FD_SET set;
	static FD_SET& SetSocketEpoll();
public:

	WinSocket();
	WinSocket(SOCKET descriptor);
	WinSocket(short port, char *ip, int aFamily = AF_INET, int type = SOCK_STREAM, int protocol = IPPROTO_TCP);
	WinSocket(short port, int aFamily = AF_INET, int type = SOCK_STREAM, int protocol = IPPROTO_TCP);
	WinSocket(short port, char*ip, SocketFamily family, SocketType type, SocketProtocol protocol);

	void InitSocket(int aFamily, int type, int protocol);
	void SetSocketPort(short port, char* srvAddr, int aFamily = AF_INET);
	void SetDirectPort(short port); // I think we no need this function
	void SetSocketPort(short port, int srvAddr, int aFamily = AF_INET);
	int Select(); // TODO EPOLL SOCKETS	
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
