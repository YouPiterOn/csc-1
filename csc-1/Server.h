#pragma once
#include <string>
#include <iostream>
#include <winsock2.h>
#include <mswsock.h>
#include <filesystem>
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Mswsock.lib")
namespace fs = std::filesystem;

class Server
{
	WSADATA wsaData;

	const int port = 12345;
	SOCKET serverSocket;
	sockaddr_in serverAddr;

	SOCKET clientSocket;

public:
	int Start();
	int ConnectClient();
	void Close();
	int Listen();
};

