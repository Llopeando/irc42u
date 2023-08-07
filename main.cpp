#include "include/Server.hpp"

void	serverCreateInfo(char **argv, int argc, sd::t_serverInput *serverInfo)
{
	if (argc != 3)
		throw std::runtime_error("bad arguments!"); 
	
	//RELLENAR STRUCT
	int port = std::atoi(argv[0]); 
	serverInfo->password = std::string(argv[1]);
	serverInfo->serverName = serverInfo->IP;
	serverInfo->address.sin_family = AF_INET;
	serverInfo->address.sin_addr.s_addr = INADDR_ANY;
	serverInfo->address.sin_port = htons( port );

	Server::serverConfig(serverInfo);
}

int	main(int argc, char *argv[])
{
	sd::t_serverInput serverInfo;
	Server	*server;
	
	serverInfo.IP = utils::printIp();
	try {
		serverCreateInfo(argv + 1, argc, &serverInfo);
		server = new Server(serverInfo);
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	//if (DEBUG)
	//	server->run2();
	//else
		server->run2();
	delete server;
	return (0);
}
