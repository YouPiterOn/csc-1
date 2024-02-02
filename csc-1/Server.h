#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <chrono>
#include <time.h>
#include <thread>
#include <iostream>
#include <winsock2.h>
#include <mswsock.h>
#include <filesystem>
#include <list>
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Mswsock.lib")
namespace fs = std::filesystem;

class Server
{
	WSADATA wsaData;

	const int port = 12345;
	SOCKET serverSocket;
	sockaddr_in serverAddr;

public:
	int Start();
	int ConnectClient();
	void Close();
	int ListenForClientConnections();
	int Listen(SOCKET clientSocket);
};

