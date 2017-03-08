#include "stdafx.h"
#include "unixsocket.h"
#ifdef __unix__
UnixSocket::UnixSocket()
{
	sock = -1;
}

UnixSocket::UnixSocket(int descriptor)
{
	sock = descriptor;
}

UnixSocket::UnixSocket(short port, char *ip, int aFamily, int type, int protocol)
{
	sock = -1;
	InitSocket(aFamily, type, protocol);
	SetSocketPort(port, ip);
}

UnixSocket::UnixSocket(short port, int aFamily, int type, int protocol)
{
	sock = -1;
	InitSocket(aFamily, type, protocol);
	SetSocketPort(port, 0);
}

void UnixSocket::InitSocket(int aFamily, int type, int protocol)
{
	sock = socket(aFamily, type, protocol);
	if (sock < 0)
		ThrowSocketExceptionWithCode("Error socket, errno", errno);
}

void UnixSocket::SetSocketPort(short port, char* srvAddr, int aFamily)
{
	sAddr.sin_family = aFamily;
	sAddr.sin_port = htons(port);
	sAddr.sin_addr.s_addr = inet_addr(srvAddr);
}

void UnixSocket::SetDirectPort(short port)
{
	sAddr.sin_port = port;//htons(port);
}

void UnixSocket::SetSocketPort(short port, int srvAddr, int aFamily)
{
	sAddr.sin_family = aFamily;
	sAddr.sin_port = htons(port);
	sAddr.sin_addr.s_addr = srvAddr;
}

int UnixSocket::RecvAll(char *buf)
{
	int sizeBuffer = 0;
	int lenght = 0;
	int init = 1;

	while (sizeBuffer < SIZE_FIRST_MESSAGE && init)
	{
		int recieved = recv(sock, (char*)&lenght + sizeBuffer, SIZE_FIRST_MESSAGE - sizeBuffer, 0);
		if (recieved < 0)
			ThrowSocketExceptionWithCode("Error recv, errno ", errno);
		sizeBuffer += recieved;
		if (!sizeBuffer) init = 0;
	}

	if (!lenght) return -1;

	sizeBuffer = 0;
	while (sizeBuffer < lenght)
	{
		int recieved = recv(sock, buf + sizeBuffer, lenght - sizeBuffer, 0);
		sizeBuffer += recieved;
		if (recieved < 0)
			ThrowSocketExceptionWithCode("Error recv, errno ", errno);
	}
	return sizeBuffer;
}

int UnixSocket::Recv(char *buf, int size)
{
	int recieved = recv(sock, buf, size, 0);
	if (recieved < 0)
		ThrowSocketExceptionWithCode("Error recv, errno ", errno);

	return recieved;
}

int UnixSocket::Send(char *buf, int size)
{
	int sended = send(sock, buf, size, 0);
	if (sended < 0)
		ThrowSocketExceptionWithCode("Error recv, errno ", errno);

	return sended;
}

int UnixSocket::SendAll(char *buf, int bufLen)
{
	int sizeBuffer = 0;
	int lenght = bufLen;

	while (sizeBuffer < SIZE_FIRST_MESSAGE)
	{
		int sended = send(sock, (char*)&lenght + sizeBuffer, SIZE_FIRST_MESSAGE - sizeBuffer, 0);
		if (sended < 0)
			ThrowSocketExceptionWithCode("Error recv, errno ", errno);
		sizeBuffer += sended;
	}

	sizeBuffer = 0;
	while (sizeBuffer < lenght)
	{
		int sended = send(sock, buf + sizeBuffer, bufLen - sizeBuffer, 0);
		if (sended < 0)
			ThrowSocketExceptionWithCode("Error recv, errno ", errno);

		sizeBuffer += sended;
	}

	return sizeBuffer;
}

void UnixSocket::Connect()
{
	int len = sizeof(sAddr);
	if (connect(sock, (sockaddr*)&sAddr, len))
		ThrowSocketExceptionWithCode("Error connect, errno ", errno);
}


void UnixSocket::ShutDown(How shutHow)
{
	if (shutdown(sock, shutHow) == -1)
		ThrowSocketExceptionWithCode("Error shutdown, errno ", errno);
}

void UnixSocket::Close()
{
	close(sock);
}

void UnixSocket::Bind()
{
	if (bind(sock, (sockaddr *)& sAddr, sizeof(sAddr)) == -1)
		ThrowSocketExceptionWithCode("Error bind, errno ", errno);

}

void UnixSocket::Listen(int len)
{
	if (listen(sock, len) == -1)
		ThrowSocketExceptionWithCode("Error listen, errno ", errno);
}

ISocket* UnixSocket::Accept()
{
	socklen_t sizeSAddr = sizeof(sAddr);
	int newS = accept(sock, (sockaddr*)&sAddr, &sizeSAddr);
	if (!newS) return NULL;
	if (newS < 0)
		ThrowSocketExceptionWithCode("Error accept, errno ", errno);

	UnixSocket *newSocket = new UnixSocket(newS);
	newSocket->sAddr = sAddr;
	return (ISocket*)newSocket;
}

int UnixSocket::RecvFrom()
{
	return NoError;
}

int UnixSocket::SendTo()
{
	return NoError;
}

#endif