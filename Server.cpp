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
		Después de inicializar el socket con bind, lo seteamos a modo "No bloqueante" para el cliente
	*/
	this->serverInfo = serverInfo;
	if (bind(pollfds[0].fd, (const struct sockaddr*)&serverInfo.address, sizeof(serverInfo.address)) == SERVER_FAILURE)
	{
		perror("socket bind failed");
		exit(EXIT_FAILURE);
	}
	
	fcntl(pollfds[0].fd, F_SETFL, O_NONBLOCK);
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
	iterFds(&Server::handleEvents);//arreglar
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
	//handleNewUser(new_client);
	pollfds.push_back(new_client);
	clients.push_back(Client());
	std::cout << "----------Cliente ha entrado en el fd : " << new_client.fd << " en la posicion " << pollfds.size() - 1 << std::endl;
	sendMsgUser(new_client.fd, "👋 Welcome! Please, introduce the server password:\n");
}



std::string Server::readTCPInput(int client_fd) {
	/*
		recv() Recibimos el mensaje del cliente
	
	*/
	char echoBuffer[RCVBUFSIZE];
	int	recvMsgSize;

	memset(echoBuffer, 0, RCVBUFSIZE);
	recvMsgSize = recv(client_fd, echoBuffer, sizeof(echoBuffer), 0);
	if (recvMsgSize == SERVER_FAILURE)
	{
		perror("recv failed, debug here");
		return (NULL);
	}
	return std::string(echoBuffer);
}

/* -----UTILS -----*/
bool Server::checkPassword(uint32_t index, std::string input) {
	//input = input.substr(0, input.size() - 2);
	input.erase(std::remove(input.begin(), input.end(), '\n'), input.end());
	std::cout << "[" << input << "]" << "[" << this->serverInfo.password << "]" << std::endl;
	if (input == this->serverInfo.password) {
		sendMsgUser(this->pollfds[index].fd, "✅ SUCCESS... Connecting to the server...\n\n // Please, select a channel by writing the number on the right //\n[CH1] ➡️  1 \n[CH2] ➡️  2\n[CH3] ➡️  3\n-> ");
		clients[index].setState(CL_STATE_SELECT_CHANNEL);
		//showChannelsUser(this->pollfds[index].fd);
	}
	else
		sendMsgUser(this->pollfds[index].fd, "❌ Incorrect password! Please, try again\n");
		return false;
}

bool Server::joinChannel(uint32_t index, std::string input) {
	uint8_t channelIndex;

	try
	{
		channelIndex = std::atoi(input.c_str());
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}

	if (channelIndex < channels.size()) {
		sendMsgUser(this->pollfds[index].fd, "✅ SUCCESS\nYou've been successfully connected to the channel, WELCOME!");
		//channels[channelIndex].addClient();
		clients[index].setState(CL_STATE_IN_CHANNEL);
	
	}
	else
		sendMsgUser(this->pollfds[index].fd, "❌ Incorrect channel, please try again.\n-> ");
		return false;
}

// void Server::writeInChannel(uint32_t index, uint)
// {

// }

void Server::showChannelsUser(int fd)
{
	sendMsgUser(fd, "💭 Chat channels:\n");
	for(std::deque<Channel>::iterator it = channels.begin() + 1; it != channels.end(); it++)
	{
		sendMsgUser(fd, "\t");
		sendMsgUser(fd, "➡️ ");
		sendMsgUser(fd, (*it).getName());
		sendMsgUser(fd, "\n");
	}
}



void Server::handleEvents(uint32_t index)
{
	/*
		Despues d ela password, de alguna manera hay que rellenar la info de cliente (nickname, username, role.....), como rellenamos el objeto creadoclient sin abrir input y solo con eventos ??
	*/
	if (pollfds[index].revents & POLLIN)
	{
		//printf("message receive\n");
		std::string input = readTCPInput(pollfds[index].fd);
		switch (clients[index].getState())
		{
			default:
				break;
			case CL_STATE_PASSWORD:
				checkPassword(index, input);
				break;
		
			case CL_STATE_SELECT_CHANNEL:
				joinChannel(index, input);
				break;
			// case CL_STATE_IN_CHANNEL:
			// 	writeInChannel(input);
			// 	break;
		}
	}
}

void Server::iterFds(void (Server::*func)(uint32_t index))
{
	std::deque<struct pollfd>::iterator it;

	it = pollfds.begin() + 1;
	while (it != pollfds.end())
	{
		(this->*func)(static_cast<uint32_t>(it - pollfds.begin()));
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

void Server::sendMsgUser(int fd, char const *str) {
	send(fd, str, strlen(str), 0);
}
// bool Server::writeInChannel(std::string input)
// {
	

// }