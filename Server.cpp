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
	pollfds.reserve(10);
	pollfds.resize(1);
	pollfds[0].events = POLLIN | POLLOUT;
	
	//Crear socket - Devuelve el fd, se guarda en pollfds[0] 
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


// 1 SOCKET DE SERVIDOR - Lo crea con todos sus datos (fd, puerto, adress...) y bind()
void	Server::setSocket(t_serverInput serverInfo)
{
	//la estructura con el address (struct) y la password viene rellena y se asigna a la de nuestro server
	this->serverInfo = serverInfo;
	
	//Bind [servidor - address] - union de fd y puerto
	if (bind(pollfds[0].fd, (const struct sockaddr*)&serverInfo.address, sizeof(serverInfo.address)) == SERVER_FAILURE)
	{
		perror("socket bind failed");
		exit(EXIT_FAILURE);
	}
	//Tenemos socket de servidor, con todos sus datos (fd, puerto, adress...) y bindeado
}

void Server::iterFds(pollfd *add, size_t length)
{
	size_t i;
	
	i = 0;
	while (i < length)
	{
		writeFds(add[i]);
		i++;
	}
}

void Server::writeFds(pollfd &pollfd)
{
	if (pollfd.revents & POLLIN)
	{
		//printf("message receive\n");
		handleTCPClient(pollfd.fd);
	}
}


// 2a EVENTOS - Entradas, salidas y peticiones de escrituras 

void	Server::checkFds(int events)
{
	(void)events;
	//Eventos de entrada y salida: pollfd[0].fd que es el servidor
	if (pollfds[0].revents & POLLIN)
	{
		printf("--------server receive input\n");
		acceptConnection();
	}
	//Resto de eventos recibidos, chequear pollfd * a ver cual ha cambiado hasta llegar al numero de eventos recibidos (contar con los del server)
	/*while (events)
	{
		//iterar y chequear event y revent de cada cliente, si ha cambiado
		events--;
	}*/

	//prueba de eneko de mandar una a a todos los clientes:
	//writeFds<pollfd>(pollfds[1]);
	iterFds(pollfds.data() + 1, pollfds.size() - 1);
}

// 2 BUCLE CORE DEL PROGRAMA - Preparar el fd_server y el array de clientes para recibir eventos (listen(), poll() y accept()) 
void	Server::run(){
	
	// ESTO ES UNA PRUEBA DE LECTURA PARA CLIENTE
	//char c;
	//int current_fds;
	int events;
	
	listenConnection(); // abre el fd del server patra que quede a la espera
	while(1)
	{
		//Espera de escrituras y salidas de todos los clientes activos: rellena events y revents en cada cliente si recibe evento y devuelve numero de clientes con eventos
		events = poll(pollfds.data(), static_cast<nfds_t>(pollfds.size()), 200); //pregunta: gestiona las entradas y salidas o solo las escrituras y lecturas?????
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
	int client_fd; // el int que devuelve accept() es el fd del cliente que guardamos en nuestro vector (ClientFd)
	unsigned int size = static_cast<unsigned int>(sizeof(serverInfo.address)); // lo necesita accept() y lo rellena: the actual size of the peer address.
	
	std::cout << "accepting" << std::endl;
	//al aceptar a un cliente recibe un int = el fd del cliente 
	if ((client_fd = accept(pollfds[0].fd, (struct sockaddr *)&serverInfo.address, &size)) == SERVER_FAILURE)
	{
		perror("connection refused");
		return;
	}
	//Crear nueva structura pollfd (resize) y rellenar el fd de esa struct de cliente
	pollfds.resize(pollfds.size() + 1);
	pollfds[pollfds.size() - 1].fd = client_fd;
	pollfds[pollfds.size() - 1].events = POLLOUT | POLLIN;

	
	//Cliente ha entrado
	std::cout << "----------Cliente ha entrado en el fd : " << client_fd << " en la posicion " << pollfds.size() - 1 << std::endl;
}


void handleTCPClient(int client_fd) {
	char	echoBuffer[RCVBUFSIZE];
	int	recvMsgSize;

	// Aqui recibimos el mensaje del cliente
	if ((recvMsgSize = recv(client_fd, echoBuffer, RCVBUFSIZE, 0)) == SERVER_FAILURE)
		perror("recv failed");
	//if (send(client_fd, echoBuffer, recvMsgSize, 0) != recvMsgSize)
	//	perror("send failed");
	write(1, echoBuffer, recvMsgSize);
}