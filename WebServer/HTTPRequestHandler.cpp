#include "HTTPRequestHandler.h"


string HTTPRequestHandler::extractHTTPVerb(int index, SocketState* sockets)
{
	string bufferStr = string(sockets[index].buffer);
	int spaceIndex = bufferStr.find(' ');
	string bufferRequestType = bufferStr.substr(spaceIndex);

	int firstRequestLastIndex;
	if (bufferRequestType.find("GET") != string::npos)
	{
		firstRequestLastIndex = bufferRequestType.find("GET") + spaceIndex;

	}
	else if (bufferRequestType.find("POST") != string::npos)
	{
		firstRequestLastIndex = bufferRequestType.find("POST") + spaceIndex;
	}
	else if (bufferRequestType.find("PUT") != string::npos)
	{
		firstRequestLastIndex = bufferRequestType.find("PUT") + spaceIndex;
	}
	else if (bufferRequestType.find("HEAD") != string::npos)
	{
		firstRequestLastIndex = bufferRequestType.find("HEAD") + spaceIndex;
	}
	else if (bufferRequestType.find("DELETE") != string::npos)
	{
		firstRequestLastIndex = bufferRequestType.find("DELETE") + spaceIndex;
	}
	else if (bufferRequestType.find("TRACE") != string::npos)
	{
		firstRequestLastIndex = bufferRequestType.find("TRACE") + spaceIndex;
	}
	else if (bufferRequestType.find("OPTIONS") != string::npos)
	{
		firstRequestLastIndex = bufferRequestType.find("OPTIONS") + spaceIndex;
	}
	else
	{
		firstRequestLastIndex = bufferRequestType.size() + spaceIndex - 1;
	}

	string firstRequest = bufferStr.substr(0, firstRequestLastIndex + 1);

	string remainedStrBuffer = bufferStr.substr(firstRequestLastIndex + 1);
	strcpy(sockets[index].buffer, remainedStrBuffer.c_str());
	sockets[index].len = remainedStrBuffer.size();

	spaceIndex = firstRequest.find(" ");
	string requestType = firstRequest.substr(0, spaceIndex);

	return requestType;
}

string HTTPRequestHandler::handleRequest(string currentRequest)
{
	string httpResponse;
	if (currentRequest == "GET") {
		httpResponse = HTTPFunctions::Get(currentRequest);
	}
	else if (currentRequest == "POST") {
		httpResponse = HTTPFunctions::Post(currentRequest);
	}
	else if (currentRequest == "HEAD") {
		httpResponse = HTTPFunctions::Head(currentRequest);
	}
	else if (currentRequest == "PUT") {
		httpResponse = HTTPFunctions::Put(currentRequest);
	}
	else if (currentRequest == "TRACE") {
		httpResponse = HTTPFunctions::Trace(currentRequest);
	}
	else if (currentRequest == "DELETE") {
		httpResponse = HTTPFunctions::Delete(currentRequest);
	}
	else if (currentRequest == "OPTIONS") {
		httpResponse = HTTPFunctions::Options(currentRequest);
	}
	else { // unimplemented or illegal request type - return an html page with error message.
		httpResponse = HTTPFunctions::handleIllegalRequest(currentRequest);
	}

	return httpResponse;
}


