#include <iostream>
#include <Ws2tcpip.h>
#include <winsock2.h>
#include <mswsock.h>
#include <filesystem>
#include "Client.h"
#pragma comment(lib, "Mswsock.lib")
#pragma comment(lib, "ws2_32.lib")
int main()
{
	Client client;
	if (client.Start() == 1) {
		return 1;
	}
	if (client.ConnectToServer() == 1) {
		return 1;
	}
	std::string command = "";
	while (true) {
		std::cout << "Enter command: ";
		std::cin >> command;
		std::cout << std::endl;
		if (command == "GET") {
			std::string pathOnClient = "";
			std::string pathOnServer = "";
			std::cout << "Enter path to file on client: ";
			std::cin >> pathOnClient;
			std::cout << "Enter path to file on server: ";
			std::cin >> pathOnServer;
			client.Get(pathOnServer, pathOnClient);
		}
		else if (command == "PUT") {
			std::string pathOnClient = "";
			std::string pathOnServer = "";
			std::cout << "Enter path to file on client: ";
			std::cin >> pathOnClient;
			std::cout << "Enter path to file on server: ";
			std::cin >> pathOnServer;
			if (client.Put(pathOnServer, pathOnClient) == 1) {
				return 1;
			}
		}
		else if (command == "LIST") {
			std::string pathOnServer = "";
			std::cout << "Enter path to file on server: ";
			std::cin >> pathOnServer;
			client.List(pathOnServer);
		}
		else if (command == "INFO") {
			std::string pathOnServer = "";
			std::cout << "Enter path to file on server: ";
			std::cin >> pathOnServer;
			client.Info(pathOnServer);
		}
		else if (command == "DELETE") {
			std::string pathOnServer = "";
			std::cout << "Enter path to file on server: ";
			std::cin >> pathOnServer;
			client.Delete(pathOnServer);
		}
		else if (command == "EXIT") {
			break;
		}
		std::cout << std::endl;
	}
	client.Close();
	return 0;
}