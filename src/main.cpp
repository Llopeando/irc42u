/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecamara <ecamara@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 17:22:37 by ullorent          #+#    #+#             */
/*   Updated: 2023/04/13 20:09:05 by ecamara          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"

void    serverCreateInfo(char **argv, int argc, t_serverInput *serverInfo)
{
	//check ( argumentos = 2 , puerto y password, puerto valido,...)
    if (argc != 3)
        throw std::runtime_error("bad arguments!"); //ya veremos si hacer excepciones definidas

    //RELLENAR STRUCT
    int port = std::atoi(argv[0]); //y de paso chequea si es un int valido
    serverInfo->password = std::string(argv[1]);
	serverInfo->address.sin_family = AF_INET;
    serverInfo->address.sin_addr.s_addr = INADDR_ANY;
    serverInfo->address.sin_port = htons( port ); //mirar htons
}

int	main(int argc, char *argv[])
{
    t_serverInput serverInfo;

    try {
        serverCreateInfo(argv + 1, argc, &serverInfo);
    }
    catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	Server	*server = new Server();
	server->setSocket(serverInfo);
    server->run();
    delete server;
    return (0);
}
