#pragma once
#define _CRT_SECURE_NO_WARNINGS
using namespace std;
#include <string.h>
#include  "Server.h"
#include "HTTPFunctions.h"

class SocketState
{
public:
	SOCKET id;			// Socket handle
	int	recv;			// Receiving?
	int	send;			// Sending?
	int sendSubType;	// Sending sub-type
	char buffer[10000];
	int len;
	time_t lastMessageTime;
};


class HTTPRequestHandler
{
public:

	
	static string extractHTTPVerb(int index, SocketState* sockets);
	static string handleRequest(string currentRequest);
};

