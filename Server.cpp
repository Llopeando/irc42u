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
	/*
		socket() Devuelve el fd, se guarda en pollfds[0] 
	*/
	pollfds.resize(1);
	pollfds[0].events = POLLIN | POLLOUT;
	
	//
	if ((pollfds[0].fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == SERVER_FAILURE)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}
	//std::cout << "Fd de servidor : " << pollfds[0].fd << std::endl;
	
}

Server::~Server()
{
	if ((status = close(pollfds[0].fd)) == SERVER_FAILURE)
	{
		perror("socket closing failed");
		exit(EXIT_FAILURE);
	}
}

/* --- MAIN PROGRAM --- */

void	Server::setSocket(t_serverInput serverInfo)
{
	/*
		1 SOCKET DE SERVIDOR - Lo crea con todos sus datos (fd, puerto, adress...) y bind()
		La estructura con el address (struct) y la password viene rellena y se asigna a la de nuestro server
		Bind [servidor - address] - union de fd y puerto
		Tenemos socket de servidor, con todos sus datos (fd, puerto, adress...) y bindeado
	*/
	this->serverInfo = serverInfo;
	if (bind(pollfds[0].fd, (const struct sockaddr*)&serverInfo.address, sizeof(serverInfo.address)) == SERVER_FAILURE)
	{
		perror("socket bind failed");
		exit(EXIT_FAILURE);
	}
}

void	Server::checkFds(int events)
{
	/*
		EVENTOS - Entradas, salidas y peticiones de escrituras 
		Eventos de entrada y salida: pollfd[0].fd que es el servidor
		Resto de eventos recibidos, chequear pollfd * a ver cual ha cambiado hasta llegar al numero de eventos recibidos (contar con los del server)
	*/
	(void)events;

	if (pollfds[0].revents & POLLIN)
	{
		printf("--------server receive input\n");
		acceptConnection();
	}
	iterFds(&Server::readClientsInput);//arreglar
}

void	Server::run(){
	/*
		BUCLE CORE DEL PROGRAMA - Preparar el fd_server y el array de clientes para recibir eventos (listen(), poll() y accept()) 
		listenConnection() abre el fd del server patra que quede a la espera
		Espera de escrituras y salidas de todos los clientes activos: rellena events y revents en cada cliente si recibe evento y devuelve numero de clientes con eventos
		pregunta: gestiona las entradas y salidas o solo las escrituras y lecturas?????
	*/
	int events;
	
	listenConnection();
	while(1)
	{
		events = poll(&pollfds.front(), static_cast<nfds_t>(pollfds.size()), 200); 
		//std::cout << "Poll - numero de eventos recibidos : " << events << std::endl;
		if (events < 0)
		{
			perror("error-event detected");
			return;
		}
		
		//unsigned int	clntLen = sizeof(serverInfo.address); // length de la estructura de datos de address
		//std::cout << clntLen << std::endl;
		checkFds(events); // 2a - EVENTOS
	}
}

/* --- Functions --- */


void	Server::listenConnection() {
	std::cout << "listening" << std::endl;
	if (listen(pollfds[0].fd, 3) == SERVER_FAILURE)
	{
		perror("listening process failure");
		exit(EXIT_FAILURE);
	}
}
void	Server::acceptConnection() {
	/*
		client_fd El int que devuelve accept() es el fd del cliente que guardamos en nuestro vector (ClientFd)
		size Lo necesita accept() y lo rellena: the actual size of the peer address.
		accept() Al aceptar a un cliente recibe un int = el fd del cliente 
		resize() Crear nueva structura pollfd (resize) y rellenar el fd de esa struct de cliente
	*/
	unsigned int size = static_cast<unsigned int>(sizeof(serverInfo.address));
	struct pollfd new_client;
	
	std::cout << "accepting" << std::endl;
	if ((new_client.fd = accept(pollfds[0].fd, (struct sockaddr *)&serverInfo.address, &size)) == SERVER_FAILURE)
	{
		perror("connection refused");
		return;
	}
	new_client.events = POLLOUT | POLLIN;
	handleNewUser(new_client);

	pollfds.push_back(new_client);

	std::cout << "----------Cliente ha entrado en el fd : " << new_client.fd << " en la posicion " << pollfds.size() - 1 << std::endl;
}

int	Server::handleNewUser(struct pollfd pollfd)
{
	std::string password;

	//password() t_serverInput serverInfo
	
}

void handleTCPClient(int client_fd) {
	/*
		recv() Recibimos el mensaje del cliente
	
	*/
	char	echoBuffer[RCVBUFSIZE];
	int	recvMsgSize;

	if ((recvMsgSize = recv(client_fd, echoBuffer, RCVBUFSIZE, 0)) == SERVER_FAILURE)
		perror("recv failed");
	if (send(client_fd, echoBuffer, recvMsgSize, 0) != recvMsgSize)
		perror("send failed");
	write(1, echoBuffer, recvMsgSize);
}

/* -----UTILS -----*/

void Server::readClientsInput(pollfd &pollfd)
{
	if (pollfd.revents & POLLIN)
	{
		//printf("message receive\n");
		handleTCPClient(pollfd.fd);
	}
}

void Server::iterFds(void (Server::*func)(pollfd &pollfd))
{
	size_t i;
	std::deque<struct pollfd>::iterator it;

	it = pollfds.begin() + 1;
	i = 0;
	while (it != pollfds.end())
	{
		(this->*func)(*it);
		it++;
	}
}

/*----------- UTILS CHANNELS -----------*/

void	Server::createChannel(std::string name)
{
	channels.push_back(Channel(name, &pollfds));
}

void	Server::deleteChannel(size_t channelIndex)
{
	std::deque<Channel>::iterator it;

	it += channelIndex;
	channels.erase(it);
}
