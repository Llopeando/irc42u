/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ullorent <ullorent@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 16:15:34 by ecamara           #+#    #+#             */
/*   Updated: 2023/03/10 18:59:222 by ullorent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/* SERVER
	AF_INET == ipv4
	SOCK_STREAM == best suited for chat irc server
	0 == let the computer handle it.

	Estructura a seguir para el servidor:
	socket()
	bind()
	listen()
	accept() -> Aquí después se coneta con el cliente para sincronizarlo
	recv() \ <- send() (cliente)
	send() / -> recv() (cliente) -> despues vuelve a rcv()
	close()
*/

/* --- Constructors and destructor --- */
Server::Server()
{
	//crear socket - Devuelve fd y se asigna
	 	if ((server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == SERVER_FAILURE)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}
}

Server::~Server()
{
	if ((status = close(server_fd)) == SERVER_FAILURE)
	{
		perror("socket closing failed");
		exit(EXIT_FAILURE);
	}
}

/* --- Functions --- */

void	Server::setSocket(t_serverInput serverInfo){
	
	//la estructura con el address (struct) y la password viene rellena y se asigna a la de nuestro server
	this->serverInfo = serverInfo;
	
	//bind [servidor - address] - union de fd y puerto
	if (bind(server_fd, (const struct sockaddr*)&serverInfo.address, sizeof(serverInfo.address)) == SERVER_FAILURE)
	{
		perror("socket bind failed");
		exit(EXIT_FAILURE);
	}
}

void	Server::run(){
	listenConnection();
	acceptConnection();
	// ESTO ES UNA PRUEBA DE LECTURA PARA CLIENTE
	char str[1];
	while(1){
		int fd = clientFd[0];
		read(fd, str, 1);
		std::cout << str[0];
		write(fd, str, 1);
	}
}

//FINAL DEL PROGRAMA
void	Server::listenConnection() {
	std::cout << "listening" << std::endl;
	if (listen(server_fd, 3) == SERVER_FAILURE)
	{
		perror("listening process failure");
		exit(EXIT_FAILURE);
	}
}
void	Server::acceptConnection() {
	int newSocket;
	unsigned int size = static_cast<unsigned int>(sizeof(serverInfo.address));
	// el int que devuelve es el fd del cliente que guardamos en nuestro vector (clientFd)

	std::cout << "accepting" << std::endl;
	if ((newSocket = accept(server_fd, (struct sockaddr *)&serverInfo.address, &size)) == SERVER_FAILURE)
	{
		perror("connection refused");
	}
	clientFd.push_back(newSocket);
}
