#include "Client.h"

const int chunkSize = 1024;

int Client::Start() {
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		std::cerr << "WSAStartup failed" << std::endl;
		return 1;
	}
	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket == INVALID_SOCKET)
	{
		std::cerr << "Error creating socket: " << WSAGetLastError() << std::endl;
		WSACleanup();
		return 1;
	}
	return 0;
}

int Client::ConnectToServer() {
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	InetPton(AF_INET, serverIp, &serverAddr.sin_addr);
	if (connect(clientSocket, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr)) == SOCKET_ERROR)
	{
		std::cerr << "Connect failed with error: " << WSAGetLastError() << std::endl;
		closesocket(clientSocket);
		WSACleanup();
		return 1;
	}
	return 0;
}

void Client::Get(std::string pathOnServer, std::string pathOnClient) {
	std::string message = "GET " + pathOnServer;
	send(clientSocket, message.c_str(), message.size(), 0);

	std::ofstream file(pathOnClient);
	int bytesReceived = chunkSize;
	while (bytesReceived == chunkSize) {
		char buffer[chunkSize + 1];
		memset(buffer, 0, chunkSize + 1);
		bytesReceived = recv(clientSocket, buffer, chunkSize, 0);
		file << buffer;
	}
}

int Client::Put(std::string pathOnServer, std::string pathOnClient) {
	std::string message = "PUT " + pathOnServer;
	send(clientSocket, message.c_str(), message.size(), 0);

	int bytesReceived = chunkSize;
	while (bytesReceived == chunkSize) {
		char buffer[chunkSize + 1];
		memset(buffer, 0, chunkSize + 1);
		bytesReceived = recv(clientSocket, buffer, chunkSize, 0);
		std::cout << buffer << std::endl;
	}

	HANDLE hFile = CreateFileA(pathOnClient.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (!TransmitFile(clientSocket, hFile, GetFileSize(hFile, NULL), 0, NULL, NULL, 0))
	{
		std::cerr << "File send error: " << WSAGetLastError() << std::endl;
		closesocket(clientSocket);
		WSACleanup();
		return 1;
	}

	bytesReceived = chunkSize;
	while (bytesReceived == chunkSize) {
		char buffer[chunkSize + 1];
		memset(buffer, 0, chunkSize + 1);
		bytesReceived = recv(clientSocket, buffer, chunkSize, 0);
		std::cout << buffer;
	}
}

void Client::List(std::string pathOnServer) {
	std::string message = "LIST " + pathOnServer;
	send(clientSocket, message.c_str(), message.size(), 0);

	int bytesReceived = chunkSize;
	while (bytesReceived == chunkSize) {
		char buffer[chunkSize + 1];
		memset(buffer, 0, chunkSize + 1);
		bytesReceived = recv(clientSocket, buffer, chunkSize, 0);
		std::cout << buffer;
	}
}

void Client::Info(std::string pathOnServer) {
	std::string message = "INFO " + pathOnServer;
	send(clientSocket, message.c_str(), message.size(), 0);

	int bytesReceived = chunkSize;
	while (bytesReceived == chunkSize) {
		char buffer[chunkSize + 1];
		memset(buffer, 0, chunkSize + 1);
		bytesReceived = recv(clientSocket, buffer, chunkSize, 0);
		std::cout << buffer;
	}
}

void Client::Delete(std::string pathOnServer) {
	std::string message = "DELETE " + pathOnServer;
	send(clientSocket, message.c_str(), message.size(), 0);

	int bytesReceived = chunkSize;
	while (bytesReceived == chunkSize) {
		char buffer[chunkSize + 1];
		memset(buffer, 0, chunkSize + 1);
		bytesReceived = recv(clientSocket, buffer, chunkSize, 0);
		std::cout << buffer;
	}
}

void Client::Close() {
	closesocket(clientSocket);
	WSACleanup();
}
