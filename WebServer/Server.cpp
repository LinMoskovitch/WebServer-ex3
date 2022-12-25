#include "Server.h"


void Server::run()
{
	SocketState sockets[MAX_SOCKETS] = { 0 };
	int socketsCount = 0;

	cout << "Server start running" << endl;
	WSAData wsaData;

	if (NO_ERROR != WSAStartup(MAKEWORD(2, 2), &wsaData))
	{
		cout << "Http Server: Error at WSAStartup()\n";
		return;
	}

	SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == listenSocket)
	{
		cout << "Http Server: Error at socket(): " << WSAGetLastError() << endl;
		WSACleanup();
		return;
	}
	sockaddr_in serverService;
	// Address family (must be AF_INET - Internet address family).
	serverService.sin_family = AF_INET;
	serverService.sin_addr.s_addr = INADDR_ANY;
	serverService.sin_port = htons(HTTP_PORT);
	if (SOCKET_ERROR == bind(listenSocket, (SOCKADDR*)&serverService, sizeof(serverService)))
	{
		cout << "Http Server: Error at bind(): " << WSAGetLastError() << endl;
		closesocket(listenSocket);
		WSACleanup();
		return;
	}
	if (SOCKET_ERROR == listen(listenSocket, 5))
	{
		cout << "Http Server: Error at listen(): " << WSAGetLastError() << endl;
		closesocket(listenSocket);
		WSACleanup();
		return;
	}

	addSocket(listenSocket, LISTEN, sockets, &socketsCount);
	struct timeval timeout;

	// Accept connections and handles them one by one.
	while (true)
	{
		timeout.tv_sec = TWO_MINUTES;
		timeout.tv_usec = 0;
		fd_set waitRecv;
		FD_ZERO(&waitRecv);
		for (int i = 0; i < MAX_SOCKETS; i++)
		{
			if ((sockets[i].recv == LISTEN) || (sockets[i].recv == RECEIVE))
				FD_SET(sockets[i].id, &waitRecv);
		}


		fd_set waitSend;
		FD_ZERO(&waitSend);
		for (int i = 0; i < MAX_SOCKETS; i++)
		{
			if (sockets[i].send == SEND)
				FD_SET(sockets[i].id, &waitSend);
		}

		int nfd = select(0, &waitRecv, &waitSend, NULL, &timeout);

		if (nfd == SOCKET_ERROR)
		{
			cout << "Http Server: Error at select(): " << WSAGetLastError() << endl;
			WSACleanup();
			return;
		}
		checkTimeout(sockets, &socketsCount);

		for (int i = 0; i < MAX_SOCKETS && nfd > 0; i++)
		{
			if (FD_ISSET(sockets[i].id, &waitRecv))
			{
				nfd--;
				switch (sockets[i].recv)
				{
				case LISTEN:
					acceptConnection(i, sockets, &socketsCount);
					break;

				case RECEIVE:
					receiveMessage(i, sockets, &socketsCount);
					break;
				}
			}
		}
		for (int i = 0; i < MAX_SOCKETS && nfd > 0; i++)
		{
			if (FD_ISSET(sockets[i].id, &waitSend))
			{
				nfd--;
				switch (sockets[i].send)
				{
				case SEND:
					sendMessage(i, sockets, &socketsCount);
					break;
				}
			}
		}
	}
	// Closing connections and Winsock.
	cout << "Http Server: Closing Connection.\n";
	closesocket(listenSocket);
	WSACleanup();
	exit(1);
}

bool Server::addSocket(SOCKET id, int what, SocketState* sockets, int* socketsCount)
{
	unsigned long flag = 1;
	if (ioctlsocket(id, FIONBIO, &flag) != 0)
	{
		cout << "Http Server: Error at ioctlsocket(): " << WSAGetLastError() << endl;
	}

	for (int i = 0; i < MAX_SOCKETS; i++)
	{
		if (sockets[i].recv == EMPTY)
		{
			sockets[i].id = id;
			sockets[i].recv = what;
			sockets[i].send = IDLE;
			sockets[i].len = 0;
			sockets[i].lastMessageTime = time(0);
			(*socketsCount)++;
			return (true);
		}
	}
	return (false);
}

void Server::checkTimeout(SocketState* sockets, int* socketsCount)
{
	for (int i = 1; i < *socketsCount; i++)
	{
		int timeDifference = difftime(time(0), sockets[i].lastMessageTime);
		if (timeDifference >= 120)
		{
			cout << "deleteing socket :" << i << " time: " << timeDifference << endl;
			closesocket(sockets[i].id);
			removeSocket(i, sockets, socketsCount);
		}
	}
}

void Server::removeSocket(int index, SocketState* sockets, int* socketsCount)
{
	sockets[index].recv = EMPTY;
	sockets[index].send = EMPTY;
	(*socketsCount)--;
}

void Server::acceptConnection(int index, SocketState* sockets, int* socketsCount)
{
	SOCKET id = sockets[index].id;
	struct sockaddr_in from;		// Address of sending partner
	int fromLen = sizeof(from);

	SOCKET msgSocket = accept(id, (struct sockaddr*)&from, &fromLen);
	if (INVALID_SOCKET == msgSocket)
	{
		cout << "Http Server: Error at accept(): " << WSAGetLastError() << endl;
		return;
	}
	cout << "Http Server: Client " << inet_ntoa(from.sin_addr) << ":" << ntohs(from.sin_port) << " is connected." << endl;


	if (addSocket(msgSocket, RECEIVE, sockets, socketsCount) == false)
	{
		cout << "\t\tToo many connections, dropped!\n";
		closesocket(id);
	}
	return;
}

void Server::receiveMessage(int index, SocketState* sockets, int* socketsCount)
{
	SOCKET msgSocket = sockets[index].id;
	int len = sockets[index].len;
	int bytesRecv = recv(msgSocket, &sockets[index].buffer[len], sizeof(sockets[index].buffer) - len, 0);

	if (SOCKET_ERROR == bytesRecv)
	{
		cout << "Http Server: Error at recv(): " << WSAGetLastError() << endl;
		closesocket(msgSocket);
		removeSocket(index, sockets, socketsCount);
		return;
	}
	if (bytesRecv == 0)
	{
		closesocket(msgSocket);
		removeSocket(index, sockets, socketsCount);
		return;
	}
	else
	{
		sockets[index].buffer[len + bytesRecv] = '\0'; //add the null-terminating to make it a string
		sockets[index].len += bytesRecv;
		sockets[index].lastMessageTime = time(0);

		if (sockets[index].len > 0)
		{
			string socketBuffer = string(sockets[index].buffer);
			sockets[index].send = SEND;
		}
	}
}

void Server::sendMessage(int index, SocketState* sockets, int* socketsCount) {
	int bytesSent = 0;
	char sendBuff[10000];
	string httpResponse;
	SOCKET msgSocket = sockets[index].id;
	HTTPRequestHandler::DTO dto = HTTPRequestHandler::extractDataFromRequest(
		index, 
		sockets
	);
	httpResponse = HTTPRequestHandler::handleRequest(dto);
	strcpy(sendBuff, httpResponse.c_str());

	bytesSent = send(msgSocket, sendBuff, (int)strlen(sendBuff), 0);
	if (SOCKET_ERROR == bytesSent)
	{
		cout << "HTTP Server: Error at send(): " << WSAGetLastError() << endl;
		return;
	}

	if (sockets[index].len == 0)
	{
		sockets[index].send = IDLE;
	}
}


