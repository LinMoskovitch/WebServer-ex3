#include "HTTPFunctions.h"



string HTTPFunctions::Get(string HttpRequest)
{
	string fileName = ExtractResourceName(HttpRequest);
	string langParamValue = checkLangParameter(fileName);
	fileName = fileName.substr(0, fileName.find('?'));
	string filePath = createFilePath(fileName, langParamValue);
	string httpResponse = processResponse(filePath, "GET");

	return httpResponse;
}
string HTTPFunctions::Post(string HttpRequest)
{
	string fileName = ExtractResourceName(HttpRequest);
	fileName = fileName.substr(0, fileName.find('?'));
	string filePath = createFilePath(fileName, string("none"));
	string requestBody = getRequestBody(HttpRequest);
	cout << endl << "The request body is: " << (requestBody.empty() ? "Empty" : requestBody) << endl << endl;
	string httpResponse = processResponse(filePath, "POST");

	return httpResponse;
}

string HTTPFunctions::Head(string HttpRequest)
{
	string fileName = ExtractResourceName(HttpRequest);
	string langParamValue = checkLangParameter(fileName);
	fileName = fileName.substr(0, fileName.find('?'));
	string filePath = createFilePath(fileName, langParamValue);
	string httpResponse = processResponse(filePath, "HEAD");

	return httpResponse;
}

string HTTPFunctions::Put(string HttpRequest)
{
	string fileName = ExtractResourceName(HttpRequest);
	fileName = fileName.substr(0, fileName.find('?'));
	string filePath = createFilePath(fileName, string("none"));
	string httpResponse = processResponse(filePath, "PUT", HttpRequest);

	return httpResponse;
}

string HTTPFunctions::Trace(string HttpRequest)
{
	string fileName = ExtractResourceName(HttpRequest);
	fileName = fileName.substr(0, fileName.find('?'));
	string filePath = createFilePath(fileName, string("none"));
	string httpResponse = processResponse(filePath, "TRACE", HttpRequest);
	return httpResponse;
}

string HTTPFunctions::Delete(string HttpRequest)
{
	string fileName = ExtractResourceName(HttpRequest);
	fileName = fileName.substr(0, fileName.find('?'));
	string filePath = createFilePath(fileName, string("none"));
	string httpResponse = processResponse(filePath, "DELETE");
	return httpResponse;
}

string HTTPFunctions::Options(string HttpRequest)
{
	string fileName = ExtractResourceName(HttpRequest);
	fileName = fileName.substr(0, fileName.find('?'));
	string filePath = createFilePath(fileName, string("none"));
	string httpResponse = processResponse(filePath, "OPTIONS");
	return httpResponse;
}

string HTTPFunctions::handleIllegalRequest(string HttpRequest)
{
	string fileName = ExtractResourceName(HttpRequest);
	fileName = fileName.substr(0, fileName.find('?'));
	string filePath = createFilePath(fileName, string("none"));

	string illegalMethod = HttpRequest.substr(HttpRequest.find(' '));
	cout << endl << "ERROR: " << illegalMethod << " method is not implemented in our server!" << endl << endl;
	string httpResponse = processResponse(filePath, "ERROR");
	return httpResponse;
}


string HTTPFunctions::createFilePath(string fileName, string langParamValue)
{
	string filePath;
	if (langParamValue == "en" || langParamValue == "he" || langParamValue == "fr")
	{
		filePath = PATH + langParamValue + "_" + fileName;
	}

	else if (langParamValue == "none")
	{
		filePath = PATH + fileName;
	}
	else if (langParamValue == "error - lang")
	{
		filePath = PATH + string("404page.html");
	}

	return filePath;
}

string HTTPFunctions::createDateHeader()
{
	time_t timer;
	time(&timer);
	struct tm* info;
	info = localtime(&timer);
	char time[200];
	strftime(time, 200, "%a, %d %b %Y %H:%M:%S GMT", info);
	return string(time);
}

string HTTPFunctions::createLastModifiedHeader(string filePath)
{
	struct stat attrib;
	stat(filePath.c_str(), &attrib);
	struct tm* info;
	info = localtime(&attrib.st_mtime);
	char time[200];
	strftime(time, 200, "%a, %d %b %Y %H:%M:%S GMT", info);
	return string(time);
}

string HTTPFunctions::createUnimplementedResponse(string& responseStatus)
{
	responseStatus = NOT_IMPLEMENTED;
	string content = "ERROR: this method is not implemented in our server!";
	return content;
}

string HTTPFunctions::createDeleteResponseIfFileExist(string filePath, string& responseStatus)
{
	string resourceContent = "";
	ifstream infile(filePath.c_str());
	if (!infile) // file not exist
	{
		infile.close();
		responseStatus = NOT_FOUND;
		resourceContent = "ERROR: server failed to find the file";
	}
	else
	{
		infile.close();
		responseStatus = OK;
		resourceContent = "file in path: " + filePath + " deleted successfully.";
		if (remove(filePath.c_str()) != 0)
		{
			cout << "Error deleting file" << endl;
			responseStatus = SERVER_ERROR;
			resourceContent = "ERROR: server failed to delete file";
		}
	}

	return resourceContent;
}

string HTTPFunctions::getRequestBody(string httpRequest)
{
	string bodyRequest = "";
	if (httpRequest.find("\r\n\r\n") != string::npos)
	{
		bodyRequest = httpRequest.substr(httpRequest.find("\r\n\r\n") + 4);
	}
	return bodyRequest;
}

string HTTPFunctions::createPostResponseIfFileExist(string filePath, string& responseStatus)
{
	string resourceContent = "";
	ifstream infile(filePath.c_str());
	if (!infile) // file not exist
	{
		infile.close();
		responseStatus = NOT_FOUND;
		resourceContent = "ERROR: server failed to find the file";
	}
	else
	{
		infile.close();
		responseStatus = OK;
		resourceContent = "file in path: " + filePath + " found successfully.";
	}

	return resourceContent;
}


string HTTPFunctions::createPutResponse(string filePath, string& responseStatus, string requestBody)
{
	string resourceContent = "";
	ifstream infile(filePath.c_str());

	if (!infile) // file not exist - we will create it
	{
		infile.close();
		resourceContent = "server created a new file in path: " + filePath + " with the requested content";
	}
	else // file exist - open it
	{
		infile.close();
		resourceContent = "server updated the file in path: " + filePath + " with the requested content";
	}

	ofstream outfile(filePath.c_str());
	if (!outfile) // server internal error 
	{
		outfile.close();
		responseStatus = SERVER_ERROR;
		resourceContent = "ERROR: server failed to create/open the file in path " + filePath;
	}
	else // ok - write to the file
	{
		responseStatus = OK;
		outfile << requestBody;
		outfile.close();
	}

	return resourceContent;
}

string HTTPFunctions::createResponseBodyIfFileExist(string filePath, string& responseStatus)
{
	ifstream infile(filePath.c_str());
	string resourceContent = "";
	string errorFilePath;
	if (!infile) // file not exist
	{
		infile.close();
		errorFilePath = PATH + string("404page.html");
		infile.open(errorFilePath.c_str());
		if (!infile)
		{
			cout << "ERROR: page 404page.html must be under c:/temp directory" << endl;
			responseStatus = SERVER_ERROR;
		}
		else {
			responseStatus = NOT_FOUND;
		}

	}
	else
	{
		responseStatus = OK;
	}
	if (responseStatus != SERVER_ERROR)
	{
		getline(infile, resourceContent, '\0');
	}
	return resourceContent;
}

string HTTPFunctions::createHttpResponse(string& responseStatus, string responseBody, int contentLength, string  dateHeader, string lastModifiedHeader)
{
	string httpResponse = "";
	httpResponse += "HTTP/1.1 " + responseStatus + "\r\n" +
		"Connection: keep-alive" + "\r\n" +
		"Date: " + dateHeader + "\r\n" +
		"Server: S_Y_WebServer" + "\r\n" +
		"Last-Modified: " + lastModifiedHeader + "\r\n" +
		"Content-Length: " + to_string(contentLength) + "\r\n" +
		"Content-Type: text/html" + "\r\n\r\n" +
		responseBody;
	return httpResponse;
}

string HTTPFunctions::createTraceHttpResponse(string& responseStatus, string responseBody, int contentLength, string  dateHeader, string lastModifiedHeader)
{
	string httpResponse = "";
	httpResponse += "HTTP/1.1 " + responseStatus + "\r\n" +
		"Connection: keep-alive" + "\r\n" +
		"Date: " + dateHeader + "\r\n" +
		"Server: S_Y_WebServer" + "\r\n" +
		"Last-Modified: " + lastModifiedHeader + "\r\n" +
		"Content-Length: " + to_string(contentLength) + "\r\n" +
		"Content-Type: message/http" + "\r\n\r\n" +
		responseBody;
	return httpResponse;
}


string HTTPFunctions::createOptionsHttpResponse(string& responseStatus, string responseBody, int contentLength, string  dateHeader, string lastModifiedHeader)
{
	string httpResponse = "";
	httpResponse += "HTTP/1.1 " + responseStatus + "\r\n" +
		"Allow: GET,POST,HEAD,DELETE,PUT,TRACE,OPTIONS" + "\r\n" +
		"Connection: keep-alive" + "\r\n" +
		"Date: " + dateHeader + "\r\n" +
		"Server: S_Y_WebServer" + "\r\n" +
		"Last-Modified: " + lastModifiedHeader + "\r\n" +
		"Content-Length: " + to_string(contentLength) + "\r\n" +
		"Content-Type: text/html" + "\r\n\r\n" +
		responseBody;
	return httpResponse;
}

string HTTPFunctions::processResponse(string filePath, string requestType, string httpRequest) // third parameter is optional
{
	string responseStatus;
	string httpResponse;
	string responseBody;
	string lastModifiedHeader = createLastModifiedHeader(filePath);
	string requestBody = getRequestBody(httpRequest);

	if (requestType == "GET") {
		responseBody = createResponseBodyIfFileExist(filePath, responseStatus);
	}
	else if (requestType == "POST") {
		responseBody = createPostResponseIfFileExist(filePath, responseStatus);
	}
	else if (requestType == "HEAD") {
		responseBody = createResponseBodyIfFileExist(filePath, responseStatus);
	}
	else if (requestType == "PUT") {
		responseBody = createPutResponse(filePath, responseStatus, requestBody);
	}
	else if (requestType == "TRACE") {
		responseBody = httpRequest;
		responseStatus = OK;
	}
	else if (requestType == "DELETE") {
		responseBody = createDeleteResponseIfFileExist(filePath, responseStatus);
	}
	else if (requestType == "OPTIONS") {
		responseBody = " The server's options are: GET,POST,HEAD,DELETE,PUT,TRACE,OPTIONS";
		responseStatus = OK;
	}
	else 
	{// unimplemented or illegal request type - return a reponse with error message.
		responseBody = createUnimplementedResponse(responseStatus);
	}

	int contentLength = responseBody.size();
	if (requestType == "HEAD")
	{
		responseBody = "";
	}
	string dateHeader = createDateHeader();
	if (requestType == "TRACE")
	{
		httpResponse = createTraceHttpResponse(responseStatus, responseBody, contentLength, dateHeader, lastModifiedHeader);
	}

	else if (requestType == "OPTIONS")
	{
		httpResponse = createOptionsHttpResponse(responseStatus, responseBody, contentLength, dateHeader, lastModifiedHeader);
	}

	else
	{
		httpResponse = createHttpResponse(responseStatus, responseBody, contentLength, dateHeader, lastModifiedHeader);
	}

	return httpResponse;
}


string HTTPFunctions::ExtractResourceName(string httpRequest)
{
	int startIndex = httpRequest.find("/") + 1;
	int endIndex = httpRequest.find(" HTTP/") - 1;
	string fileName = httpRequest.substr(startIndex, endIndex - startIndex + 1);
	return fileName;
}

string HTTPFunctions::checkLangParameter(string file)
{
	int langParamIndex = file.find("lang");
	if (file.find("lang") != string::npos)
	{
		if (file.substr(langParamIndex).find("en") != string::npos)
		{
			return "en";
		}

		else if (file.substr(langParamIndex).find("fr") != string::npos)
		{
			return "fr";
		}

		else if (file.substr(langParamIndex).find("he") != string::npos)
		{
			return "he";
		}
		else
		{
			return "error-lang";
		}
	}

	else
	{
		return "none";
	}

}
