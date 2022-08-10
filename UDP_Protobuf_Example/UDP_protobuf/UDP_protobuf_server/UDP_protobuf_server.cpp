// UDP-Server.cpp : Defines the entry point for the console application.
//
#undef UNICODE

#define WIN32_LEAN_AND_MEAN

//#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

//#define DEFAULT_BUFLEN 256
#define DEFAULT_BUFLEN 1024
#define PORT 6666

int __cdecl main(void)
{
	std::cout << "****************\n*    SERVER    *\n****************\n";


	char str[INET_ADDRSTRLEN];

	//Initialize Winsock
	WSADATA wsaData;        // (version req,d , winsock data)
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != NO_ERROR) {
		std::cout << "WSAStartup failed with error: " << iResult << std::endl;
		return 1;
	}


	//Create a SOCKET for listening for incoming connections request
	SOCKET SrvSocket;
	SrvSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (SrvSocket == INVALID_SOCKET) {
		std::cout << "Socket failed with error: " << WSAGetLastError() << std::endl;
		WSACleanup();
		return 1;
	}


	//The sockaddr_in structure specifies the address family,
	//IP address, and port for the socket that is being bound

		// The htons() function converts a 16-bit short integer to a network
	// ordered 16bit integer. This is done to ensure that integers are read
	// in the proper endian format used on the network. In the case of WinSock,
	// that’s always big endian.

	sockaddr_in addrServer;
	addrServer.sin_family = AF_INET;
	InetPton(AF_INET, "127.0.0.1", &addrServer.sin_addr.s_addr);
	addrServer.sin_port = htons(PORT); // OR, you can do serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	memset(&(addrServer.sin_zero), '\0', 8);

	//Bind socket
	if (bind(SrvSocket, (SOCKADDR *)& addrServer, sizeof(addrServer)) == SOCKET_ERROR) {
		std::cout << "Bind failed with error: " << WSAGetLastError() << std::endl;
		closesocket(SrvSocket);
		WSACleanup();
		return 1;
	}

	std::cout << "Waiting for data..." << std::endl;

	//Variables for recieve
	int iSendResult;
	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;


	struct sockaddr_in clientAddr;
	int clientAddrLen = sizeof(clientAddr);
	// UDP data received from the client
	/* The recvfrom function is a blocking call, it will halt the calling thread until it
		receives a message on the port the socket is bound to. A buffer and relevant size
		of buffer is also needed to accept the incoming messages. Lastly, a separate address
		struct is needed to store incoming information from who sent the datagram. The original
		address struct used for the socket cannot be also utilized here because it would overwrite
		that existing, vital information.
	*/

	while (true)
	{
		ZeroMemory(&clientAddr, clientAddrLen); // Clear the client structure
		ZeroMemory(recvbuf, DEFAULT_BUFLEN); // Clear the receive buffer

		// Wait for message (recfrom for UDP client)
		int bytesIn = recvfrom(SrvSocket, recvbuf, DEFAULT_BUFLEN, 0, (struct sockaddr *)&clientAddr, &clientAddrLen);
		if (bytesIn == SOCKET_ERROR)
		{
			std::cout << "Error receiving from client " << WSAGetLastError() << std::endl;
			continue;
		}

		// Display message and client info
		ZeroMemory(str, INET_ADDRSTRLEN); // to string of characters

		inet_ntop(AF_INET, &clientAddr.sin_addr, str, INET_ADDRSTRLEN);
		std::cout << "Recieved name from: " << str << std::endl;
		std::cout << "Recieved: " << recvbuf << std::endl;



		////Create string
		//std::string filename;
		////std::cout << filename << std::endl;
		//for (int i = 0; i < bytesIn; i++) filename += recvbuf[i];

		////std::cout << "Opening file: " << filename << std::endl;


		////Open file and count bytes
		//std::fstream file;
		//file.open(filename, std::ios::in | std::ios::binary);

		//file.seekg(0, std::ios::end);
		//int fileSize = file.tellg();
		//file.close();
		//std::cout << "File \"" << filename << "\" size: " << fileSize << std::endl;


		////Convert to char array
		//std::string temp = std::to_string(fileSize);
		////std::cout << "Filesize in string: " << temp << std::endl;
		//char tempc[DEFAULT_BUFLEN];
		//int i = 0;
		//while (temp[i] != '\0') {
		//	tempc[i] = temp[i];
		//	i++;
		//}

		//tempc[i] = '\0';
		////std::cout << "Filesize in char array: " << tempc << std::endl;
		//const char* sendbuf = tempc;

		//inet_ntop(AF_INET, &clientAddr.sin_addr, str, INET_ADDRSTRLEN);
		//std::cout << "Sending size to: " << str << std::endl;

		//int c = strlen(sendbuf);
		//sendto(SrvSocket, sendbuf, c, 0, (struct sockaddr *)&clientAddr, clientAddrLen);
	}


	// cleanup
	closesocket(SrvSocket);
	WSACleanup();
	
	return 0;
}