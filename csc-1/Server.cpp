#include "Server.h"

int Server::Start() {
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		std::cerr << "WSAStartup failed" << std::endl;
		return 1;
	}
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket == INVALID_SOCKET)
	{
		std::cerr << "Error creating socket: " << WSAGetLastError() << std::endl;
		WSACleanup();
		return 1;
	}
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	serverAddr.sin_port = htons(port);
	if (bind(serverSocket, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr)) == SOCKET_ERROR)
	{
		std::cerr << "Bind failed with error: " << WSAGetLastError() << std::endl;
		closesocket(serverSocket);
		WSACleanup();
		return 1;
	}
	return 0;
}

int Server::ConnectClient() {
	if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR)
	{
		std::cerr << "Listen failed with error: " << WSAGetLastError() << std::endl;
		closesocket(serverSocket);
		WSACleanup();
		return 1;
	}
	std::cout << "Server listening on port " << port << std::endl;
	clientSocket = accept(serverSocket, nullptr, nullptr);
	if (clientSocket == INVALID_SOCKET)
	{
		std::cerr << "Accept failed with error: " << WSAGetLastError() << std::endl;
		closesocket(serverSocket);
		WSACleanup();
		return 1;
	}
	return 0;
}

void Server::Close() {
	closesocket(clientSocket);
	closesocket(serverSocket);
	WSACleanup();
}

int Server::Listen() {
	char request[1024];
	memset(request, 0, 1024);
	int bytesReceived = recv(clientSocket, request, sizeof(request), 0);
	if (bytesReceived == 0) {
		return 1;
	}
	char* next = NULL;
	char* command = strtok_s(request, " ", &next);
	char* path = strtok_s(NULL, " ", &next);
	if (strcmp(command, "GET") == 0) {

		HANDLE hFile = CreateFileA(path, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

		if (!TransmitFile(clientSocket, hFile, GetFileSize(hFile, NULL), 0, NULL, NULL, 0))
		{
			std::cerr << "File send error: " << WSAGetLastError() << std::endl;
			closesocket(serverSocket);
			WSACleanup();
			return 1;
		}
	}
	if (strcmp(command, "LIST") == 0) {
		std::string response = "";
		for (const auto& file : fs::directory_iterator(path)) {
			response += file.path().string();
			response += "\n";
		}
		send(clientSocket, response.c_str(), response.size(), 0);
	}
	return 0;
}
