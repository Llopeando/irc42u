/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ullorent <ullorent@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/31 12:36:06 by ullorent          #+#    #+#             */
/*   Updated: 2023/08/03 20:12:54 by ullorent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	
	serverInfo.IP = utils::printIp();
	try {
		serverCreateInfo(argv + 1, argc, &serverInfo);
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	Server	*server = new Server(serverInfo);
	server->run2();
	delete server;
	return (0);
}
