#pragma once
#include "../cross/isocket.h"
#ifdef _WIN32
#include "../cross/windows/winsocket.h"
typedef WinSocket OSSocket;
#elif __unix__
#include "../cross/unix/unixsocket.h"
typedef UnixSocket OSSocket;
#endif

class Socket : public ISocket
{
	ISocket *sock;
public:
	Socket();
	Socket(ISocket*);
	Socket(short port);
	Socket(int port, char* ip);
	Socket(int port, char *ip, SocketFamily family, SocketType type, SocketProtocol);
	~Socket();
	int RecvAll(char *buf);
	int SendAll(char *buf, int size);
	void Connect();
	void Listen(int len);
	Socket* AcceptOSSocket();
	Socket* AcceptSocket();
	ISocket* Accept();
	void Close();
	void Bind();
	//int Select(); // TODO EPOLL socket
	int Recv(char *buf, int size);
	int Send(char *buf, int size);
	void ShutDown(How shutHow);
	void SetDirectPort(short port);
	int RecvFrom();
	int SendTo();
};


