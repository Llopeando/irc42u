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


#include "../include/Server.hpp"
#include "time.h"
/* ------------------------------------------------------------ */
/* 			CONSTRUCTOR DESTRUCTOR INITIALIZATION 				*/
/* ------------------------------------------------------------ */

Server::Server()
{
	/*
		socket() Devuelve el fd, se guarda en pollfds[0] 
	*/
	pollfds.resize(1);
	pollfds[0].events = POLLIN | POLLOUT;
	
	if ((pollfds[0].fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == SERVER_FAILURE)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}
	commands.cmd[0] = "nickname_edit";
	commands.cmd[1] = "password_edit";
	commands.cmd[2] = "role_edit";
	commands.cmd[3] = "join_channel";
	commands.cmd[4] = "leave_server";
	commands.cmd[5] = "susurro";
	commands.cmd[6] = "leave_channel";
	commands.cmd[7] = "/0";
	//commands.func[0] = &Server::nickname_edit;
	//commands.func[1] = &Server::password_edit;
	//commands.func[2] = &Server::role_edit;
	//commands.func[3] = &Server::join_channel;
	//commands.func[4] = &Server::leave_channel;
	//commands.func[5] = &Server::leave_server;
	//commands.func[6] = &Server::susurro;
	//commands.func[7] = NULL;
	
	//reservar posicion 0 de todos los array como INTOCABLES
	t_tempClient temp;
	temp.newClient = false;
	temp.tempClient = Client();
	temp.tempClient.setUsername("server");
	tempClients.push_back(temp);

	t_activeIndex  activeTemp;
	activeTemp.index = 0;
	activeTemp.registered = true;
	actives.push_back(activeTemp);

	Client registeredTemp = Client();
	registeredTemp.setUsername("ROOT");
	registeredTemp.setNickname("Root");
	registered.push_back(registeredTemp);

	createChannel("Lobby", registered[0], 0);
}

Server::~Server()
{
	if ((status = close(pollfds[0].fd)) == SERVER_FAILURE)
	{
		perror("socket closing failed");
		exit(EXIT_FAILURE);
	}
}

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


/* ------------------------------------------------------------	*/
/*							MAIN								*/
/* ------------------------------------------------------------	*/


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

void	Server::printServerStatus() const
{
	static clock_t last_time = 0;
	clock_t now ;
	double timeElapsed;

	if (last_time == 0)
		last_time = clock();
	now = clock();
	timeElapsed = (double)(now - last_time) / CLOCKS_PER_SEC;
	if (timeElapsed > 5)
	{
		system("clear");
		std::cout << "\033[1;37m≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡\033[0m 🖥️  \033[1;37;43mSERVER STATUS\033[0m ℹ️  \033[1;37m≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡\033[0m\n";
	
		std::cout << "\033[1;37m🖥️  👥 SERVER ACTIVE USERS: \033[0m" << actives.size() << '\n' << std::endl;

		std::cout << "\033[1;37m🔶 👥 TEMP USERS (waiting to login or register): \033[0m" << tempClients.size() << '\n';
		for(uint32_t i = 0; i < tempClients.size(); i++)
		{
			if (tempClients[i].tempClient.getUsername() == "server")
				std::cout << "\t\033[1;37m[" << i << "] \033[0m: username(\033[1;33m" << tempClients[i].tempClient.getUsername() << "\033[0m) new:(" << tempClients[i].newClient << ")" << std::endl;
			else
				std::cout << "\t\033[1;37m[" << i << "] \033[0m: username(\033[1;37m" << tempClients[i].tempClient.getUsername() << "\033[0m) new:(" << tempClients[i].newClient << ")" << std::endl;
		}

		std::cout << "\033[1;37m✅ 👥 REGISTERED USERS: \033[0m" << registered.size() << "\n";
		for(uint32_t i = 0; i < registered.size(); i++)
		{
			if (registered[i].getUsername() == "ROOT")
				std::cout << "\t\033[1;37m[" << i << "] \033[0m: username(\033[1;31m" << registered[i].getUsername() << "\033[0m)\n";
			else
				std::cout << "\t\033[1;37m[" << i << "] \033[0m: username(\033[1;37m" << registered[i].getUsername() << "\033[0m)\n";
		}

		std::cout << "\n\033[1;37m💭 CHANNELS LIST: \033[0m" << "\n";
		for(uint32_t i = 0; i < channels.size(); i++)
		{
			std::cout << "\t\033[1;37m[" << channels[i].getName() << "]\033[0m" << std::endl;
		}
		std::cout << "\n\033[1;33m🟨 DEBUG LOG (if any)\033[0m" << std::endl;
		last_time = now;
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
	printServerStatus();
	if (pollfds[0].revents & POLLIN)
	{
		printf("-------- INCOMING REQUEST RECEIVED --------\n");
		acceptConnection();
	}
	//chequea evento a evento o hacemos events--  ? para ir chequeando hasta que no haya y no tener que recorrrer todo

	iterFds(&Server::handleEvents);//arreglar
}


/* ------------------------------------------------------------ */
/*				LISTEN AND ACCEPT CONNECTION					*/
/* ------------------------------------------------------------ */


void	Server::listenConnection() {
	std::cout << "Server started, im listening" << std::endl;
	if (listen(pollfds[0].fd, 3) == SERVER_FAILURE)
	{
		perror("listening process failure");
		exit(EXIT_FAILURE);
	}
}

void	Server::acceptConnection() {
	/*
		client_fd -> El int que devuelve accept() es el fd del cliente que guardamos en nuestro vector (ClientFd)
		resize()  -> Crear nueva structura pollfd (resize) y rellenar el fd de esa struct de cliente
		accept()  -> Al aceptar a un cliente recibe un int = el fd del cliente 
		size      -> Lo necesita accept() y lo rellena: the actual size of the peer address.
	*/
	unsigned int size = static_cast<unsigned int>(sizeof(serverInfo.address));
	struct pollfd new_client;

	std::cout << "The connection has been accepted, continuing" << std::endl;
	if ((new_client.fd = accept(pollfds[0].fd, (struct sockaddr *)&serverInfo.address, &size)) == SERVER_FAILURE)
	{
		perror("connection refused");
		return;
	}
	new_client.events = POLLOUT | POLLIN;
	pollfds.push_back(new_client);

	t_activeIndex activeIndex;
	activeIndex.registered = false;
	activeIndex.index = static_cast<uint32_t>(actives.size());

	t_tempClient tempClient;

	tempClient.tempClient = Client();
	tempClient.newClient = true;

	actives.push_back(activeIndex);
	tempClients.push_back(tempClient);

	//std::cout << "----------Cliente ha entrado en el fd : " << new_client.fd << " en la posicion de pollfds y actives en [" << pollfds.size() - 1 << "]"<< std::endl;
	sendMsgUser(new_client.fd, "👋 Welcome! Please, introduce the \033[1;37mserver password\033[0m:\n🔐 ");

}


/* ------------------------------------------------------------ */
/*						CLIENT INPUT							*/
/* ------------------------------------------------------------ */


void Server::handleEvents(uint32_t index)
{
	/*
		1- Puntero client. Chequear si el evento viene de un usuario registrado o de uno temporal. Ahora mismo el ciebte esta en active[]
			-> ¿true? == se ha registrado 

			-> ¿false? = no se ha registrado
				-accede al array de temporales para la info de cliente
		
	*/
	if (pollfds[index].revents & POLLIN)
	{
		std::string input = readTCPInput(pollfds[index].fd);
		
		if (actives[index].registered) //si esta aqui esta en el array REGISTERED
		{
			//Client *registeredClient;
			uint32_t channel = registered[actives[index].index].getChannel();
			channels[channel].broadcast(index, input);
			//microshell(index, input);
		}
		else //si esta aqui esta en el array TEMPORAL
		{
			t_tempClient *newClient; 
			newClient = &tempClients[index];
			switch (newClient->tempClient.getState())
			{
				case CL_STATE_SERVER_PASSWORD:
					checkServerPassword(newClient, index, input);
					break;
				case CL_STATE_LOG_IN:
					loginChoice(newClient, index, input);
					break;
				case CL_STATE_SELECT_USERNAME:
					selectUsername(newClient, index, input);
					break;
				case CL_STATE_INTRODUCE_PERSONAL_PASSWORD://Cuando completa este estado pasa al array de REGISTRADO
					checkPassword(newClient, index, input);
					break;
				case CL_STATE_SELECT_NICKNAME: //ya esta en registrado y borrado de temp
					selectNickname(newClient, index, input);
					break;
				default:
					break;
			}
		}
	}
}


/* ------------------------------------------------------------ */
/*					CHECK LOG IN AND REGISTER					*/
/* ------------------------------------------------------------ */


bool Server::checkServerPassword(t_tempClient *client, uint32_t indexAct, std::string &input)
{
	input.erase(std::remove(input.begin(), input.end(), '\n'), input.end());
	if (input == serverInfo.password) {
		sendMsgUser(pollfds[indexAct].fd, "\n\033[1;42m✅ OK! Let's continue\033[0m\n");
		client->tempClient.setState(CL_STATE_LOG_IN);
		sendMsgUser(pollfds[indexAct].fd, "❓ If you have an account on this chat, \033[1;37mwrite 0\033[0m. If not, \033[1;37mwrite 1\033[0m: ");
		//sendMsgUser(this->pollfds[index].fd, "\n🧑 Enter your \033[1;37musername\033[0m: ");
		return true;
	}
	else {
		sendMsgUser(this->pollfds[indexAct].fd, "\033[1;31m❌ Incorrect password! Please, try again\033[0m\n🔐 ");
		return false;
	}
}

void Server::loginChoice(t_tempClient *client, uint32_t indexAct, std::string &input)
{
	input.erase(std::remove(input.begin(), input.end(), '\n'), input.end());
	if (input == "0")
	{
		client->tempClient.setState(CL_STATE_SELECT_USERNAME);
		client->newClient = false;
		sendMsgUser(pollfds[indexAct].fd, "\n\033[1;32m👋 Welcome back! 👋\033[0m\n📝 Please, enter you \033[1;37musername\033[0m: ");
	}
	else if (input == "1")
	{
		client->newClient = true;
		client->tempClient.setState(CL_STATE_SELECT_USERNAME);
		sendMsgUser(pollfds[indexAct].fd, "\n\033[1;32m🆕 Welcome to our IRC chat! 🆕\033[0m\n📝 Please, enter your \033[1;37mdesired username\033[0m: ");
	}
	else
	{
		sendMsgUser(pollfds[indexAct].fd, "\033[1;31m❌ Incorrect option! Please, try again\033[0m\n❓ ");
	}
}


bool Server::selectUsername(t_tempClient *client, uint32_t indexAct, std::string &input)
{
	input.erase(std::remove(input.begin(), input.end(), '\n'), input.end());
	std::cout << "username" << " [" << client->newClient << "]["<< input <<"]\n";
	if (!client->newClient) //log in
	{
		if (findUsername(input))
		{
			client->tempClient.setState(CL_STATE_INTRODUCE_PERSONAL_PASSWORD);
			client->tempClient.setUsername(input);
			sendMsgUser(this->pollfds[indexAct].fd, "\n🔐 Enter your \033[1;37mpersonal password\033[0m: ");
			return true;
		}
		else //no username
		{
			sendMsgUser(this->pollfds[indexAct].fd, "\033[1;31m❌ This user does not exist, please try again.\033[0m\n📝 ");
			return false;
		}
	}
	else // sign up
	{
		if (findUsername(input))
			sendMsgUser(this->pollfds[indexAct].fd, "\033[1;31m❌ This username has already been taken! Please, type another username.\033[0m\n📝 ");
		else//register
		{
			client->tempClient.setUsername(input);
			client->tempClient.setState(CL_STATE_INTRODUCE_PERSONAL_PASSWORD);
			sendMsgUser(this->pollfds[indexAct].fd, "🔐 Enter your \033[1;37mpersonal password\033[0m: ");
			return true;
		}
	}
	return false;
	
}

bool Server::checkPassword(t_tempClient *client, uint32_t indexAct, std::string &input)
{

	input.erase(std::remove(input.begin(), input.end(), '\n'), input.end());
	std::cout << "password" << input << " [" << client->newClient << "]["<< input <<"]\n";
	if (!client->newClient) //logging in
	{
		if (assertClientPassword(indexAct, input)) //pass ok
		{
			client->tempClient.setState(CL_STATE_SELECT_NICKNAME);
			sendMsgUser(this->pollfds[indexAct].fd, "🧑 Enter your desired \033[1;37mnickname\033[0m to chat: ");
			return true;
		}
		else // wrong
		{
			sendMsgUser(this->pollfds[indexAct].fd, "\033[1;31m❌ Incorrect password! Please, try again\033[0m\n🔐 ");
		}
	}
	else // sign up 
	{
		client->tempClient.setPassword(input);
		client->tempClient.setState(CL_STATE_SELECT_NICKNAME);
		sendMsgUser(this->pollfds[indexAct].fd, "🧑 Enter your desired \033[1;37mnickname\033[0m to chat: ");
	}
	return false;
}
void Server::selectNickname(t_tempClient *client, uint32_t indexAct, std::string &input)
{
	input.erase(std::remove(input.begin(), input.end(), '\n'), input.end());
	
	client->tempClient.setNickname(input);
	client->tempClient.setState(CL_STATE_LOBBY);
	if (!client->newClient)
	{
		/*uint32_t indexTemp = actives[indexAct].index;
		std::deque<t_tempClient>::iterator it = std::begin(tempClients) + indexTemp; 
		tempClients.erase(it);
		//-1 to every index of temps bigger than tempindex;
		for (uint32_t i = 0; i < actives.size(); i++)
		{
			if (!actives[i].registered && actives[i].index > indexTemp)
				actives[i].index--;
		}
		actives[indexAct].registered = true;*/
		tempToRegistered(indexAct);
		actives[indexAct].index = findUsername(client->tempClient.getUsername());
	}
	else
	{
		//change to the registered array;
		registered.push_back(client->tempClient);
		tempToRegistered(indexAct);
		actives[indexAct].index = registered.size() - 1;
	}
	sendMsgUser(pollfds[indexAct].fd, "\n\033[1;42m👋 Welcome to the \033[1;33m'A O I R C' \033[1;42;37mchat server!\033[0m\n");
	joinChannel(indexAct, "Lobby");//es bool, recoger el true? en el caso de lobby igual no 
	
}

void Server::tempToRegistered(uint32_t indexAct)
{
		//eliminar user de temp
		uint32_t indexTemp = actives[indexAct].index;
		std::deque<t_tempClient>::iterator it = std::begin(tempClients) + indexTemp; 
		tempClients.erase(it);
		//-1 to every index of temps bigger than tempindex;
		for (uint32_t i = 0; i < actives.size(); i++)
		{
			if (!actives[i].registered && actives[i].index > indexTemp)
				actives[i].index--;
		}
		actives[indexAct].registered = true;
}

/* ------------------------------------------------------------ */
/*							MICROSHELL							*/
/* ------------------------------------------------------------ */


/*	MICROSHELL
	-Es comand????
		Depende el estado en el que esta, solo va a comparar ciertos comandos. Estado: 4 o 5 
	-SI no es cmand
		escribir en canal 
*/
void Server::microshell(uint32_t index, std::string &input)
{
	std::string cmd = input.substr(0, input.find(' '));
	//std::string argument = input.substr(input.find(' '), );
	std::string argument;
	uint32_t max = 0;
	uint32_t i;

	if (registered[index].getState() == CL_STATE_LOBBY) 
		max = 5;
	else if (registered[index].getState() == CL_STATE_IN_CHANNEL)
		max = 8;
	for(i= 0; i < max; i++ )
		if (commands.cmd[i] == cmd)
			(this->*(Server::commands.func[i]))(index, argument);
	if (i == max)
		//quieroescribirenmicanal() ; ------------------------------------------------> REHACER;
		;
}

/*
void Server::nickname_edit(uint32_t index, std::string &argument){

	 
	
}
		
void Server::password_edit(uint32_t index, std::string &argument){

	
}
		
void Server::role_edit(uint32_t index, std::string &argument){

	
}

void Server::join_channel(uint32_t index, std::string &argument){

	//depende del estado (IN_CHANNEL)tiene que hacer leave + join
	
}

void Server::leave_channel(uint32_t index, std::string &argument){

	
}

void Server::leave_server(uint32_t index, std::string &argument){

	
}

void Server::susurro(uint32_t index, std::string &argument){

	
}
*/
void	Server::createChannel(std::string name, Client &client, uint32_t indexAct)
{
	//check role
	if (client.getRole() != CL_ROOT)
	{
		if (findChannel(name))
		{
			sendMsgUser(pollfds[indexAct].fd, "\033[1;31m❌ This channel already exists! Please, choose another channel name.\033[0m\n");
		}
		else
			channels.push_back(Channel(name, client.getUsername(), &pollfds, &actives, &registered));
	}

}

void	Server::deleteChannel(size_t channelIndex)
{
	std::deque<Channel>::iterator it;

	it += channelIndex;
	channels.erase(it);
}

/* ------------------------------------------------------------ */
/*							OBSOLETE							*/
/* ------------------------------------------------------------ */


bool Server::joinChannel(uint32_t indexAct, std::string input) {
	
	uint8_t channelIndex;

	channelIndex = findChannel(input);

	if (channelIndex < channels.size())
	{
		channels[channelIndex].addClient(indexAct);
		sendMsgUser(pollfds[indexAct].fd, "✅ You've been successfully connected to the channel!\n");
		//deberia mandar el mensaje el propio channel
		registered[actives[indexAct].index].setState(CL_STATE_IN_CHANNEL);
	
	}
	else
		sendMsgUser(pollfds[indexAct].fd, "❌ Incorrect channel, please try again.\n-> ");
		return false;
}

// void Server::writeInChannel(uint32_t index, uint)
// {

// }
/*
void Server::showChannelsUser(int fd) const
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
*/

/* ------------------------------------------------------------ */
/*							UTILS								*/
/* ------------------------------------------------------------ */

uint32_t	Server::findChannel(const std::string &name) const
{
	for (uint32_t i =0;i < channels.size();i++)
	{
		if (channels[i].getName() == name)
			return i;
	}
	return (0);
}

void Server::iterFds(void (Server::*func)(uint32_t index))
{
	(void )func;
	for(uint32_t i = 1; i < actives.size();i++)
	{
		handleEvents(i);
	}
/*
	it = pollfds.begin() + 1;
	while (it != pollfds.end())
	{
		handleEvents(index);
		//(this->*func)(static_cast<uint32_t>(it - pollfds.begin()));
		it++;
	}*/
}

std::string Server::readTCPInput(int client_fd) {
	/*
		recv() -> Recibimos el mensaje del cliente
	
	*/
	char echoBuffer[RCVBUFSIZE];
	int	recvMsgSize;

	memset(echoBuffer, 0, RCVBUFSIZE);
	recvMsgSize = recv(client_fd, echoBuffer, sizeof(echoBuffer), 0);
	if (recvMsgSize == SERVER_FAILURE)
	{
		perror("recv failed, debug here");
		//exit (EXIT_FAILURE);
		return (std::string(nullptr));
	}
	return std::string(echoBuffer, recvMsgSize);
}

uint32_t	Server::findUsername(const std::string &username) const
{
	for (uint32_t i =0;i < registered.size();i++)
	{
		if (registered[i].getUsername() == username)
		{	
			return i;
		}
	}
	return (0);
}

bool	Server::assertClientPassword(uint32_t indexAct, const std::string &password) const
{
	uint32_t indexReg;

	indexReg = findUsername(tempClients[actives[indexAct].index].tempClient.getUsername());
	if(indexReg == 0)
	{
		std::cout << "server crash\n";
	}
	else
	{
		if (registered[indexReg].checkPassword(password)) 
		{
			return(true);
			
		}
		else
		{
			return (false);
		}
	}
	return (false);
}

void Server::sendMsgUser(int fd, char const *str) const
{
	int buffer_size = 65536;
	setsockopt(fd, SOL_SOCKET, SO_SNDBUF, &buffer_size, sizeof(buffer_size));
	send(fd, str, strlen(str), 0);
}