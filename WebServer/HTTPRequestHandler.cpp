#include "HTTPRequestHandler.h"


string HTTPRequestHandler::makeRequestToString(int index, SocketState* sockets)
{
	string bufferStr = string(sockets[index].buffer);
	int spaceIndex = bufferStr.find(" ");
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
	return firstRequest;
}

string HTTPRequestHandler::handleRequest(DTO dto)
{
	string httpResponse;
	if (dto.requestType == "GET") {
		httpResponse = HTTPFunctions::Get(dto.request);
	}
	else if (dto.requestType == "POST") {
		httpResponse = HTTPFunctions::Post(dto.request);
	}
	else if (dto.requestType == "HEAD") {
		httpResponse = HTTPFunctions::Head(dto.request);
	}
	else if (dto.requestType == "PUT") {
		httpResponse = HTTPFunctions::Put(dto.request);
	}
	else if (dto.requestType == "TRACE") {
		httpResponse = HTTPFunctions::Trace(dto.request);
	}
	else if (dto.requestType == "DELETE") {
		httpResponse = HTTPFunctions::Delete(dto.request);
	}
	else if (dto.requestType == "OPTIONS") {
		httpResponse = HTTPFunctions::Options(dto.request);
	}
	else { // unimplemented or illegal request type - return an html page with error message.
		httpResponse = HTTPFunctions::handleIllegalRequest(dto.request);
	}

	return httpResponse;
}

HTTPRequestHandler::DTO HTTPRequestHandler::extractDataFromRequest(int index, SocketState* sockets) {
	DTO dto;
	dto.request = makeRequestToString(index, sockets);
	dto.requestType = getRequestTypeFrom(dto.request);

	return dto;

}

string HTTPRequestHandler::getRequestTypeFrom(string request) {
	int spaceIndex = request.find(" ");
	string requestType = request.substr(0, spaceIndex);

	return requestType;
}
