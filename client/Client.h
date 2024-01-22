#include <iostream>
#include <fstream>
#include <Ws2tcpip.h>
#include <winsock2.h>
#include <mswsock.h>
#pragma comment(lib, "Mswsock.lib")
#pragma comment(lib, "ws2_32.lib")

class Client
{
	WSADATA wsaData;

	const int port = 12345;
	const PCWSTR serverIp = L"127.0.0.1";
	SOCKET clientSocket;
	sockaddr_in serverAddr;

public:
	int Start();
	int ConnectToServer();

	void Get(std::string pathOnServer, std::string pathOnClient);
	int Put(std::string pathOnServer, std::string pathOnClient);
	void List(std::string pathOnServer);
	void Info(std::string pathOnServer);
	void Delete(std::string pathOnServer);

	void Close();
};
