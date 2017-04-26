#include "socket.h"

Socket::Socket()
{
	sock = new OSSocket();
}

Socket::Socket(ISocket *socket)
{
	sock = (OSSocket*)socket;
	//sock = new OSSocket();
}


Socket::Socket(short port)
{
	sock = new OSSocket(port);
}

Socket::Socket(int port, char* ip)
{
	sock = new OSSocket(port, ip);
}

Socket::Socket(int port, char *ip, SocketFamily family, SocketType type, SocketProtocol)
{

}

Socket::~Socket()
{
	delete sock;
}

int Socket::RecvAll(char *buf)
{
	return sock->RecvAll(buf);
}

int Socket::SendAll(char *buf, int size)
{
	int sended = sock->SendAll(buf, size);
	return sended;
}

void Socket::Connect()
{
	sock->Connect();
}

void Socket::Listen(int len)
{
	sock->Listen(len);
}

Socket* Socket::AcceptOSSocket()
{
	//OSSocket *newOSSocket = (OSSocket*)Accept();
//	Socket *newSocket = new Socket()
	return (Socket*)Accept();
}

Socket* Socket::AcceptSocket()
{
	Socket* newSocket = new Socket(Accept());
	if (!newSocket)
	{
		ThrowSocketException("Cant Allocate new socke");
	}

	return newSocket;
}

ISocket* Socket::Accept()
{
	return sock->Accept();
}

void Socket::Close()
{
	sock->Close();
}

void Socket::Bind()
{
	sock->Bind();
}


// TODO EPOLL socket
//int Socket::Select()
//{
//	return sock->Select();
//}

int Socket::Recv(char *buf, int size)
{
	return sock->Recv(buf, size);
}

int Socket::Send(char *buf, int size)
{
	return sock->Send(buf, size);
}

void Socket::ShutDown(How shutHow)
{
	sock->ShutDown(shutHow);
}

void Socket::SetDirectPort(short port)
{
	sock->SetDirectPort(port);
}

int Socket::RecvFrom()
{
	sock->RecvFrom();
	return 0;
}

int Socket::SendTo()
{
	sock->SendTo();
	return 0;
}

