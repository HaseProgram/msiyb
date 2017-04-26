#include "winsocket.h"

WinSocket::WinSocket()
{
	sock = INVALID_SOCKET;
}

WinSocket::WinSocket(SOCKET descriptor)
{
	sock = descriptor;
}

WinSocket::WinSocket(short port, char *ip, int aFamily, int type, int protocol)
{
	sock = INVALID_SOCKET;
	InitSocket(aFamily, type, protocol);
	SetSocketPort(port, ip);
}

WinSocket::WinSocket(short port, int aFamily, int type, int protocol)
{
	sock = INVALID_SOCKET;
	InitSocket(aFamily, type, protocol);
	SetSocketPort(port, 0);
}


static FD_SET& SetSocketEpoll()
{
	static FD_SET *set = new FD_SET;
	static ULONG g = FD_ZERO(set);
	return *set;
}

void WinSocket::InitSocket(int aFamily, int type, int protocol)
{
	int iRes = WSAStartup(0x202, &wsaData);
	if (iRes)
	{
		ThrowSocketExceptionWithCode("WsaStartup failed. WsaGetLastError:", WSAGetLastError());
	}

	sock = socket(aFamily, type, protocol);
	if (sock == INVALID_SOCKET)
	{
		ThrowSocketExceptionWithCode("WsaStartup failed. WsaGetLastError:", WSAGetLastError());
	}
}

void WinSocket::SetSocketPort(short port, char* srvAddr, int aFamily)
{
	sAddr.sin_family = aFamily;
	sAddr.sin_port = htons(port);
	sAddr.sin_addr.s_addr = inet_addr(srvAddr);
}


// I think we no need this function
void WinSocket::SetDirectPort(short port)
{
	sAddr.sin_port = port;//htons(port);
}

void WinSocket::SetSocketPort(short port, int srvAddr, int aFamily)
{
	sAddr.sin_family = aFamily;
	sAddr.sin_port = htons(port);
	sAddr.sin_addr.s_addr = srvAddr;
}


// TODO EPOOL SOCKETS
int WinSocket::Select()
{
	static FD_SET set;
	static ULONG c = FD_ZERO(&set);
	return 0;
}


int WinSocket::RecvAll(char *buf)
{
	int sizeBuffer = 0;
	int lenght = 0;
	int init = 1;

	while (sizeBuffer < SIZE_FIRST_MESSAGE && init)
	{
		int recieved = recv(sock, (char*)&lenght + sizeBuffer, SIZE_FIRST_MESSAGE - sizeBuffer, 0);
		if (recieved < 0)
		{
			ThrowSocketExceptionWithCode("Error Recieve. WSAGetLastError:", WSAGetLastError());
		}
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
		{
			ThrowSocketExceptionWithCode("Error Recieve. WSAGetLastError:", WSAGetLastError());
		}
	}
	return sizeBuffer;
}

int WinSocket::Recv(char *buf, int size)
{

	int recieved = recv(sock, buf, size, 0);
	if (recieved < 0)
	{
		ThrowSocketExceptionWithCode("Error Recieve. WSAGetLastError:", WSAGetLastError());
	}
	return recieved;
}

int WinSocket::Send(char *buf, int size)
{
	int sended = send(sock, buf, size, 0);
	if (sended < 0)
	{
		ThrowSocketExceptionWithCode("Error Send. WSAGetLastError:", WSAGetLastError());
	}
	return sended;
}


int WinSocket::SendAll(char *buf, int bufLen)
{
	int sizeBuffer = 0;
	int lenght = bufLen;

	while (sizeBuffer < SIZE_FIRST_MESSAGE)
	{
		int sended = send(sock, (char*)&lenght + sizeBuffer, SIZE_FIRST_MESSAGE - sizeBuffer, 0);
		if (sended < 0)
		{
			ThrowSocketExceptionWithCode("Error Send. WSAGetLastError:", WSAGetLastError());
		}
		sizeBuffer += sended;
	}

	sizeBuffer = 0;

	while (sizeBuffer < lenght)
	{
		int sended = send(sock, buf + sizeBuffer, bufLen - sizeBuffer, 0);
		if (sended < 0)
		{
			ThrowSocketExceptionWithCode("Error Send. WSAGetLastError:", WSAGetLastError());
		}
		sizeBuffer += sended;
	}

	return sizeBuffer;
}

void WinSocket::Connect()
{
	int len = sizeof(sAddr);
	if (connect(sock, (sockaddr*)&sAddr, len))
	{
		Close();
		WSACleanup();
		ThrowSocketExceptionWithCode("Error Connect. WSAGetLastError:", WSAGetLastError());
	}
}


void WinSocket::ShutDown(How shutHow)
{
	if (shutdown(sock, shutHow))
	{
		Close();
		WSACleanup();
		ThrowSocketExceptionWithCode("Error ShutDown. WSAGetLastError:", WSAGetLastError());
	}
}

void WinSocket::Close()
{
	closesocket(sock);
}

void WinSocket::Bind()
{
	if (bind(sock, (sockaddr *)& sAddr, sizeof(sAddr)))
	{
		Close();
		WSACleanup();
		ThrowSocketExceptionWithCode("Error Bind. WSAGetLastError:", WSAGetLastError());
	}
}

void WinSocket::Listen(int len)
{
	if (listen(sock, len))
	{
		Close();
		WSACleanup();
		ThrowSocketExceptionWithCode("Error Listen. WSAGetLastError:", WSAGetLastError());
	}
}

ISocket* WinSocket::Accept()
{
	int sizeSAddr = sizeof(sAddr);
	SOCKET newS = accept(sock, (sockaddr*)&sAddr, &sizeSAddr);
	if (!newS) return NULL;
	if (newS == INVALID_SOCKET)
	{
		Close();
		WSACleanup();
		ThrowSocketExceptionWithCode("Error Accept. WSAGetLastError:", WSAGetLastError());
	}
	WinSocket *newSocket = new WinSocket(newS);
	newSocket->sAddr = sAddr;
	newSocket->wsaData = wsaData;
	return (ISocket*)newSocket;
}


int WinSocket::RecvFrom()
{
	return 0;
}
int WinSocket:: SendTo()
{
	return 0;
}