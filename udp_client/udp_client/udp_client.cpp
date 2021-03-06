﻿#include "pch.h"
#include <iostream>
using namespace std;
// Don't forget to include "ws2_32.lib" in the library list.
#include <winsock2.h> 
#include <Ws2tcpip.h>
#include <string.h>
#include <string>

#pragma comment(lib,"ws2_32.lib")

int main(int argc, char *argv[])
{
	if (argc < 2) {
		fprintf(stderr, "Error: no server port provided\n");
		exit(-1);

	}

	int SERVER_PORT = atoi(argv[1]);
	// Initialize Winsock (Windows Sockets).

	WSAData wsaData;
	if (NO_ERROR != WSAStartup(MAKEWORD(2, 2), &wsaData))
	{
		cout << "Time Client: Error at WSAStartup()\n";
	}

	// Client side:
	// Create a socket and connect to an internet address.

	SOCKET connSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (INVALID_SOCKET == connSocket)
	{
		cout << "Time Client: Error at socket(): " << WSAGetLastError() << endl;
		WSACleanup();
		return(-1);
	}

	// For a client to communicate on a network, it must connect to a server.

	// Need to assemble the required data for connection in sockaddr structure.

	// Create a sockaddr_in object called server. 
	sockaddr_in server;
	server.sin_family = AF_INET;
	//server.sin_addr.s_addr = inet_addr("127.0.0.1");
	InetPton(AF_INET, L"127.0.0.1", &server.sin_addr.s_addr);
	server.sin_port = htons(SERVER_PORT);

	// Send and receive data.

	int bytesSent = 0;
	int bytesRecv = 0;
	char sendBuff[255] = "";
	char recvBuff[255];

	// Asks the server what's the currnet time.
	// The send function sends data on a connected socket.
	// The buffer to be sent and its size are needed.
	// The fourth argument is an idicator specifying the way in which the call is made (0 for default).
	// The two last arguments hold the details of the server to communicate with. 
	// NOTE: the last argument should always be the actual size of the client's data-structure (i.e. sizeof(sockaddr)).
	cout << "Please enter a request: ";
	string req;
	getline(cin, req);
	strcpy(sendBuff, req.c_str());
	bytesSent = sendto(connSocket, sendBuff, (int)strlen(sendBuff), 0, (const sockaddr *)&server, sizeof(server));
	if (SOCKET_ERROR == bytesSent)
	{
		cout << "File Client: Error at sendto(): " << WSAGetLastError() << endl;
		closesocket(connSocket);
		WSACleanup();
		return(-1);
	}
	cout << "File Client: Sent: " << bytesSent << "/" << strlen(sendBuff) << " bytes of \"" << sendBuff << "\" message.\n";

	// Gets the server's answer using simple recieve (no need to hold the server's address).
	bytesRecv = recv(connSocket, recvBuff, 255, 0);
	if (SOCKET_ERROR == bytesRecv)
	{
		cout << "File Client: Error at recv(): " << WSAGetLastError() << endl;
		closesocket(connSocket);
		WSACleanup();
		return(-1);
	}

	recvBuff[bytesRecv] = '\0'; //add the null-terminating to make it a string
	cout << "Data received from server: " << endl;
	cout << recvBuff << "\n";
	// Closing connections and Winsock.
	cout << "File Client: Closing Connection.\n";
	closesocket(connSocket);
}