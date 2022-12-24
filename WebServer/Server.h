#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_DEPRECATE
#include <iostream>
using namespace std;
#pragma comment(lib, "Ws2_32.lib")
#include <winsock2.h>
#include <string.h>
#include <time.h>
#include  "HTTPRequestHandler.h"

class SocketState;

#define MAX_SOCKETS 60
#define HTTP_PORT 8080
#define LISTEN 1
#define EMPTY  0
#define IDLE 3
#define RECEIVE 2
#define SEND 4





const static int TWO_MINUTES = 120;
const static int PORT = 8080;


class Server
{

public:
	static void run();
	static bool addSocket(SOCKET id, int what, SocketState* sockets, int* socketsCount);
	static void checkTimeout(SocketState* sockets, int* socketsCount);
	static void removeSocket(int index, SocketState* sockets, int* socketsCount);
	static void acceptConnection(int index, SocketState* sockets, int* socketsCount);
	static void receiveMessage(int index, SocketState* sockets, int* socketsCount);
	static void sendMessage(int index, SocketState* sockets, int* socketsCount);
};

