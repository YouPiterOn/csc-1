#include <iostream>
#include <Ws2tcpip.h>
#include <winsock2.h>
#include <mswsock.h>
#include <filesystem>
#pragma comment(lib, "Mswsock.lib")
#pragma comment(lib, "ws2_32.lib")
int main()
{
	// Initialize Winsock
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		std::cerr << "WSAStartup failed" << std::endl;
		return 1;
	}
	// Client configuration
	int port = 12345;
	PCWSTR serverIp = L"127.0.0.1";
	SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket == INVALID_SOCKET)
	{
		std::cerr << "Error creating socket: " << WSAGetLastError() << std::endl;
		WSACleanup();
		return 1;
	}
	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	InetPton(AF_INET, serverIp, &serverAddr.sin_addr);
	// Connect to the server
	if (connect(clientSocket, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr)) == SOCKET_ERROR)
	{
		std::cerr << "Connect failed with error: " << WSAGetLastError() << std::endl;
		closesocket(clientSocket);
		WSACleanup();
		return 1;
	}
	// Send data to the server
	const char* message = "INFO D:\\text.txt";
	send(clientSocket, message, (int)strlen(message), 0);
	/*
	char response[1024];
	memset(response, 0, 1024);
	recv(clientSocket, response, sizeof(response), 0);
	std::cout << response << std::endl;

	HANDLE hFile = CreateFileA("D:\\clientInfo.txt", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (!TransmitFile(clientSocket, hFile, GetFileSize(hFile, NULL), 0, NULL, NULL, 0))
	{
		std::cerr << "File send error: " << WSAGetLastError() << std::endl;
		closesocket(clientSocket);
		WSACleanup();
		return 1;
	}
	*/
	int bytesReceived = 1024;
	while (bytesReceived == 1024) {
		char buffer[1024];
		memset(buffer, 0, 1024);
		bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
		std::cout << buffer << std::endl;
	}
	closesocket(clientSocket);
	WSACleanup();
	return 0;
}