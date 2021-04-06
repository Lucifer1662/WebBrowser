#include "Connection.h"
#include <string>
#include <iostream>
#include "HttpResponseHeader.h"

#ifndef UNICODE
#define UNICODE 1
#endif

// link with Ws2_32.lib
#pragma comment(lib,"Ws2_32.lib")

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>   // Needed for _wtoi


addrinfo* getAddress(const char* address,
	const char* port) {
	struct addrinfo* result = NULL;


	struct addrinfo hints;

	//--------------------------------
	// Setup the hints address info structure
	// which is passed to the getaddrinfo() function
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;



	//--------------------------------
	// Call getaddrinfo(). If the call succeeds,
	// the result variable will hold a linked list
	// of addrinfo structures containing response
	// information
	int dwRetval = getaddrinfo(address, port, &hints, &result);
	if (dwRetval != 0) {
		printf("getaddrinfo failed with error: %d\n", dwRetval);
	}

	return result;
}

#define HEADER_END "\r\n\r\n"

std::string read(SOCKET sockfd) {
	std::string content;
	char buf[1000];
	memset(buf, 0, sizeof(buf));


	// get header
	while (recv(sockfd, buf, sizeof(buf) - 1, 0) > 0) {
		std::cout << buf;
		content.append(buf);
		memset(buf, 0, sizeof(buf));
	}

	return content;
}


std::pair<HttpResponseHeader, std::string> seperateContent(const std::string& content) {
	int headerEnd = content.find(HEADER_END);
	auto httpFactory = HttpResponseHeaderFactory();
	auto header = httpFactory.create(content.substr(0, headerEnd));

	auto body = content.substr(headerEnd, content.size() - headerEnd);
	

	return std::make_pair(header, body);
}


std::pair<HttpResponseHeader, std::string> connection()
{

	//-----------------------------------------
	// Declare and initialize variables
	WSADATA wsaData = { 0 };
	int iResult = 0;


	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		wprintf(L"WSAStartup failed: %d\n", iResult);
	}



	int iFamily = iFamily = AF_INET;
	int iType = SOCK_STREAM;
	int iProtocol = IPPROTO_TCP;

	SOCKET sock = socket(iFamily, iType, iProtocol);



	int sockfd;


	struct addrinfo* res = getAddress("www.ibdhost.com", "80");
	sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	int j = connect(sockfd, res->ai_addr, res->ai_addrlen);
	freeaddrinfo(res);


	std::string header_format = 
		"GET %s HTTP/1.1\r\n"
		"Host: %s\r\n"
		"%s"
		"User-Agent: webBrowser"
		"\r\n\r\n";


	std::string header = 
		"GET / HTTP/1.1\r\n"
		"Host: www.ibdhost.com\r\n"
		"User-Agent: webBrowser\r\n"
		"\r\n";

	send(sockfd, header.c_str(), header.size(), 0);
	
	auto content = read(sockfd);

	return seperateContent(content);
}


