#pragma once
#define _CRT_SECURE_NO_WARNINGS
using namespace std;
#include <string.h>
#include  "Server.h"
#include "HTTPFunctions.h"

class SocketState {
	public:
		SOCKET id;			// Socket handle
		int	recv;			// Receiving?
		int	send;			// Sending?
		int sendSubType;	// Sending sub-type
		char buffer[10000];
		int len;
		time_t lastMessageTime;
};


class HTTPRequestHandler {


	public:
		class DTO {
			public:
				string request;
				string requestType;
		};
	
	static string makeRequestToString(int index, SocketState* sockets);
	static string handleRequest(DTO dto);
	static HTTPRequestHandler::DTO extractDataFromRequest(int index, SocketState* sockets);
	static string getRequestTypeFrom(string request);
};

