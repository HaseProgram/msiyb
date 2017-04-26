#pragma once

#include "../defines.h"
#include "../tools/exceptions/socketexception.h"


typedef enum End { SHUTRECV, SHUTSEND, SHUTBOTH } How;
typedef enum SocketFamily_
{
	SOCKETFAMILYSYSTEM,
	SOCKETFAMLIYINET,
	SOCKETFAMILYINET6
} SocketFamily;

typedef enum SocketType_
{

} SocketType;


typedef enum SocketProtocol_
{

} SocketProtocol;

class ISocket
{
public:
	virtual int RecvAll(char *buf) = 0;
	virtual int SendAll(char *buf, int size) = 0;
	virtual ~ISocket() {}
	virtual void Connect() = 0;
	virtual void Close() = 0;
	virtual void Bind() = 0;
	virtual void Listen(int len) = 0;
	virtual ISocket* Accept() = 0;
	virtual void SetDirectPort(short port) = 0;
	virtual int Recv(char *buf, int size) = 0;
	virtual int Send(char *buf, int size) = 0;
	virtual int RecvFrom() = 0;
	virtual int SendTo() = 0;
	virtual void ShutDown(How shutHow) = 0;
};
