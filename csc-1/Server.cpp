#include "Server.h"

const int chunkSize = 1024;

template <typename TP>
std::time_t to_time_t(TP tp)
{
	using namespace std::chrono;
	auto sctp = time_point_cast<system_clock::duration>(tp - TP::clock::now()
		+ system_clock::now());
	return system_clock::to_time_t(sctp);
}

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
	char request[chunkSize];
	memset(request, 0, chunkSize);
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
	else if (strcmp(command, "LIST") == 0) {
		std::string response = "";
		for (const auto& entry : fs::directory_iterator(path)) {
		 	response += entry.path().string();
		 	response += "\n";
		}
		send(clientSocket, response.c_str(), response.size(), 0);
	}
	else if (strcmp(command, "DELETE") == 0) {
		if (std::remove(path) == 0) {
			const char* response = "File deleted successfully";
			send(clientSocket, response, sizeof(response), 0);
		}
		else {
			const char* response = "Failed to delete file";
			send(clientSocket, response, (int)strlen(response), 0);
		}
	}
	else if (strcmp(command, "PUT") == 0) {
		const char* response = "Ready to accept file";
		send(clientSocket, response, (int)strlen(response), 0);

		std::ofstream file(path);
		bytesReceived = chunkSize;
		while (bytesReceived == chunkSize) {
			char buffer[chunkSize+1];
			memset(buffer, 0, chunkSize+1);
			bytesReceived = recv(clientSocket, buffer, chunkSize, 0);
			file << buffer;
		}

		response = "File creation succeeded";
		send(clientSocket, response, (int)strlen(response), 0);
	}
	else if (strcmp(command, "INFO") == 0) {
		fs::directory_entry file{ path };
		std::string fileSize = static_cast<std::stringstream>(std::stringstream() << file.file_size()).str();

		fs::file_time_type fileTime = file.last_write_time();
		time_t tt = to_time_t(fileTime);
		std::stringstream buffer;
		struct tm gmt;
		gmtime_s(&gmt, &tt);
		buffer << std::put_time(&gmt, "%A, %d %B %Y %H:%M");
		std::string lastModified = buffer.str();
		std::string response = "File size: " + fileSize + "\n" + "Last modified: " + lastModified;
		send(clientSocket, response.c_str(), response.size(), 0);
	}
	return 0;
}
