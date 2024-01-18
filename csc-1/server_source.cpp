#include "Server.h"

int main()
{
	
	Server server;
	if (server.Start() == 1) {
		return 1;
	}
	if (server.ConnectClient() == 1) {
		return 1;
	}
	server.Listen();
	server.Close();
	
}