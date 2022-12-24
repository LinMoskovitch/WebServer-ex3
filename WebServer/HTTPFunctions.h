#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <string>
using namespace std;
#include <string.h>
#include <iostream>
#include <fstream>

#define PATH "C:\\temp\\"
#define NOT_FOUND "404 Not Found"
#define SERVER_ERROR "500 Internal Server Error"
#define OK "200 OK"
#define NOT_IMPLEMENTED "501 Not Implemented"




class HTTPFunctions
{
public:
	static string Get(string HttpRequest);
	static string Post(string HttpRequest);
	static string Put(string HttpRequest);
	static string Head(string HttpRequest);
	static string Trace(string HttpRequest);
	static string Options(string HttpRequest);
	static string Delete(string HttpRequest);
	static string handleIllegalRequest(string HttpRequest);
	static string createFilePath(string fileName, string langParamValue);
	static string createDateHeader();
	static string createLastModifiedHeader(string filePath);
	static string getRequestBody(string httpRequest);
	static string createPostResponseIfFileExist(string filePath, string& responseStatus);
	static string createHttpResponse(string& responseStatus, string responseBody, int contentLength, string dateHeader,
	                                 string lastModifiedHeader);
	static string createTraceHttpResponse(string& responseStatus, string responseBody, int contentLength, string dateHeader,
	                                      string lastModifiedHeader);
	static string createOptionsHttpResponse(string& responseStatus, string responseBody, int contentLength, string dateHeader,
	                                        string lastModifiedHeader);
	static string processResponse(string filePath, string requestType, string httpRequest = "null");
	static string ExtractResourceName(string httpRequest);
	static string checkLangParameter(string file);
	static string createResponseBodyIfFileExist(string filePath, string& responseStatus);
	static string createDeleteResponseIfFileExist(string filePath, string& responseStatus);
	static string createPutResponse(string filePath, string& responseStatus, string requestBody);
	static string createUnimplementedResponse(string& responseStatus);

};

