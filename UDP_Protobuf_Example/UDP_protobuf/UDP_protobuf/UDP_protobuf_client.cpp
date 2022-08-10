// UDP-Client.cpp : Defines the entry point for the console application.
//
#undef UNICODE

// #define WIN32_LEAN_AND_MEAN

//#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <ctime>
#include <fstream>
#include <google/protobuf/util/time_util.h>
#include <cstdio>
#include <ctime>
#include "radio.pb.h"

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
//#pragma comment (lib, "Mswsock.lib")
//#pragma comment (lib, "AdvApi32.lib")


#define DEFAULT_BUFLEN 256
#define PORT 6666

using google::protobuf::util::TimeUtil;

// This function fills in a Person message based on user input.
void AddPhoneData(madera::Phone* phone, int id, std::string name, madera::Phone::PhoneNetwork phoneNet,
	bool activeStatus, madera::Phone::PhoneType phoneType) {

	// int id = 1111;
	phone->set_id(id);
	std::cout << "Set phone ID: " << phone->id() << std::endl;

	phone->set_name(name); // it will be initialized to an empty string automatically if not set
	std::cout << "Set phone name: " << phone->name() << std::endl;

	phone->set_network(phoneNet);
	std::cout << "Set phone network: " << phone->network() << std::endl;

	phone->set_active(activeStatus);
	std::cout << "Set phone active status: " << phone->active() << std::endl;

	phone->set_type(phoneType);
	std::cout << "Set phone type: " << phone->type() << std::endl;

	// Create the google:protobuf::Timestamp from date string
	//struct tm created;
	//strptime(doc["created"].GetString(), "%Y-%m-%dT%H:%M:%SZ", &created);

	//google::protobuf::Timestamp *created_timestamp = new google::protobuf::Timestamp();
	//created_timestamp->set_seconds(mktime(&created));
	//created_timestamp->set_nanos(0);
	//*phone->set_allocated_created();

	*phone->mutable_created() = TimeUtil::SecondsToTimestamp(time(NULL));
}

madera::Phone CreatePhone(int id, std::string name, madera::Phone::PhoneNetwork phoneNet,
	bool activeStatus, madera::Phone::PhoneType phoneType) {

	madera::Phone *phone = new madera::Phone;

	// int id = 1111;
	phone->set_id(id);
	std::cout << "Set phone ID: " << phone->id() << std::endl;

	phone->set_name(name); // it will be initialized to an empty string automatically if not set
	std::cout << "Set phone name: " << phone->name() << std::endl;

	phone->set_network(phoneNet);
	std::cout << "Set phone network: " << phone->network() << std::endl;

	phone->set_active(activeStatus);
	std::cout << "Set phone active status: " << phone->active() << std::endl;

	phone->set_type(phoneType);
	std::cout << "Set phone type: " << phone->type() << std::endl;

	// Create the google:protobuf::Timestamp from date string
	//struct tm created;
	//strptime(doc["created"].GetString(), "%Y-%m-%dT%H:%M:%SZ", &created);

	//google::protobuf::Timestamp *created_timestamp = new google::protobuf::Timestamp();
	//created_timestamp->set_seconds(mktime(&created));
	//created_timestamp->set_nanos(0);
	//*phone->set_allocated_created();

	*phone->mutable_created() = TimeUtil::SecondsToTimestamp(time(NULL));

	return *phone;
}

bool AddToPhoneList(madera::PhoneList* phoneList, std::vector<madera::Phone> listOfPhones, std::string fName)
{
	for (auto phone : listOfPhones)
	{
		// Add an address.
		AddPhoneData(phoneList->add_phones(), phone.id(), phone.name(),
			phone.network(), phone.active(), phone.type());

		// Write the new address book back to disk.
		// phone.SerializeToArray 
		std::fstream output(fName, std::ios::out | std::ios::trunc | std::ios::binary);
		if (!phoneList->SerializeToOstream(&output)) {
			std::cerr << "Failed to write phone message." << std::endl;
			return false;
		}

	}

	return true;
}



void main(int argc, char *argv[])
{
	GOOGLE_PROTOBUF_VERIFY_VERSION;
	std::cout << "****************\n*    CLIENT    *\n****************\n";

	char str[INET_ADDRSTRLEN];


	//Initialize Winsock
	WSADATA wsaData;
	WORD version = MAKEWORD(2, 2);
	int iResult = WSAStartup(version, &wsaData);
	if (iResult != NO_ERROR) {
		std::cout << "WSAStartup Failed with error: " << iResult << std::endl;
		return;
	}
	/*        socket( socket family: IPv4 or IPv6 (AF_INET means IPv4.),
	//                socket type (UDP deals with datagrams so SOCK_DGRAM),
					  protocol (relatee to socket type) )
	*/
	SOCKET UDPSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); // 
	if (UDPSocket == INVALID_SOCKET) {
		std::cout << "Error at socket(): " << WSAGetLastError() << std::endl;
		WSACleanup();
		return;
	}

	// The sockaddr_in structure specifies the address family,
	// IP address, and port (6666) for the socket that is being bound.
	sockaddr_in addrServer;
	addrServer.sin_family = AF_INET;
	InetPton(AF_INET, "127.0.0.1", &addrServer.sin_addr.s_addr);
	//InetPton(AF_INET, "192.168.0.20", &addrServer.sin_addr.s_addr);

	// The htons() function converts a 16-bit short integer to a network
	// ordered 16bit integer. This is done to ensure that integers are read
	// in the proper endian format used on the network. In the case of WinSock,
	// that’s always big endian.
	addrServer.sin_port = htons(PORT); // OR, you can do serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	memset(&(addrServer.sin_zero), '\0', 8);

	//variables
	// char filename[DEFAULT_BUFLEN] = { 0 };
	char protobufferUDP[DEFAULT_BUFLEN] = { 0 };
	//std::cout << "Name of file: ";
	//std::cin.getline(filename, DEFAULT_BUFLEN, '\n');
	//std::cout << filename << std::endl;



	/*
	Add protobuf here...........
	Serialise object to buffer
	*/
	madera::PhoneList phoneList;

	// Add an address.
	AddPhoneData(phoneList.add_phones(), 0001, "Fresno", madera::Phone::EE, true, madera::Phone::ANDROID);
	AddPhoneData(phoneList.add_phones(), 0002, "Fresno2", madera::Phone::VODAPHONE, true, madera::Phone::APPLE);
	{
		// Write the new address book back to disk.
		// phone.SerializeToArray 
		std::streambuf buf() = protobufferUDP;
		std::ostream output(protobufferUDP, std::ios::out | std::ios::trunc | std::ios::binary);
		if (!phoneList.SerializeToOstream(&output)) {
			std::cerr << "Failed to write phone message." << std::endl;
			return;
		}
	}

	////////////////////////////////////////////////////////




	const char *sendbuf = filename;

	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;


	//std::cout << "Array lenght:" << (int)strlen(sendbuf) << std::endl;
	int srvAddrLen = sizeof(addrServer);

	// Send file name
	inet_ntop(AF_INET, &addrServer.sin_addr, str, INET_ADDRSTRLEN);
	std::cout << "Sending name to: " << str << std::endl;


	int count = strlen(filename);
	// udp client passes data via the sendto function
	sendto(UDPSocket, filename, count, 0, (struct sockaddr *)&addrServer, srvAddrLen);
	   	
	
	//// receive from server??? --- needed???

	int c = recvfrom(UDPSocket, recvbuf, DEFAULT_BUFLEN, 0, (struct sockaddr *)&addrServer, &srvAddrLen);
	//std::cout << recvbuf << std::endl;


	inet_ntop(AF_INET, &addrServer.sin_addr, str, INET_ADDRSTRLEN);
	std::cout << "Recieved size from: " << str << std::endl;

	std::string size;
	for (int i = 0; i < c; i++) size += recvbuf[i];
	if (size == "-1") std::cout << "No file named \"" << filename << "\"" << std::endl;
	else std::cout << "The size of file \"" << filename << "\" is: " << size << std::endl;


	// cleanup
	google::protobuf::ShutdownProtobufLibrary();
	closesocket(UDPSocket);
	WSACleanup();

	return;
}