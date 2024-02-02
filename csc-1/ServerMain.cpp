#include "Server.h"

int main()
{
	
	Server server;
	if (server.Start() == 1) {
		return 1;
	}
	while (server.ListenForClientConnections() != 1) {}
	
	server.Close();
}