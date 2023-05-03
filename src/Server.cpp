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

Server::Server(t_serverInput *serverInput)
{
	/*
		socket() Devuelve el fd, se guarda en pollfds[0] 
	*/
	/*pollfds.resize(1);
	pollfds[0].events = POLLIN | POLLOUT;
	
	if ((pollfds[0].fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == SERVER_FAILURE)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}*/

	//set UsersData
	data = new UsersData();

	//rellenar array de comandos 
	setCommands();
	

	//Client temp;
	//temp = Client();
	//temp.setUsername("server");
	//tempClients.push_back(temp);
//
	//t_activeIndex  activeTemp;
	//activeTemp.index = 0;
	//activeTemp.registered = true;
	//actives.push_back(activeTemp);
//
	//Client registeredTemp = Client();
	//registeredTemp.setUsername("ROOT");
	//registeredTemp.setNickname("Root");
	//registered.push_back(registeredTemp);

	createChannel("Lobby", (*data)[0], 0);
	
	setSocket(serverInput);
}

Server::~Server()
{
	if ((status = close(data->getFd(0))) == SERVER_FAILURE)
	{
		perror("socket closing failed");
		exit(EXIT_FAILURE);
	}
}

void	Server::setSocket(t_serverInput *serverInfo)
{
	/*
		1 SOCKET DE SERVIDOR - Lo crea con todos sus datos (fd, puerto, adress...) y bind()
		La estructura con el address (struct) y la password viene rellena y se asigna a la de nuestro server
		Bind [servidor - address] - union de fd y puerto
		Tenemos socket de servidor, con todos sus datos (fd, puerto, adress...) y bindeado
		Después de inicializar el socket con bind, lo seteamos a modo "No bloqueante" para el cliente
	*/

	this->serverInfo = *serverInfo;
	if (bind(data->getFd(0), (const struct sockaddr*)&serverInfo->address, sizeof(serverInfo->address)) == SERVER_FAILURE)
	{
		perror("socket bind failed");
		exit(EXIT_FAILURE);
	}
	
	fcntl(data->getFd(0), F_SETFL, O_NONBLOCK);
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
		events = poll(data->Pollfd().data(), static_cast<nfds_t>(data->sizeAct()), 200); 
		//std::cout << "Poll - numero de eventos recibidos : " << events << std::endl;
		if (events < 0)
		{
			perror("error-event detected");
			return;
		}
		checkFds(events); // 2a - EVENTOS
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
	if (data->Pollfd()[0].revents & POLLIN)
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
	if (listen(data->getFd(0), 3) == SERVER_FAILURE)
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
	if ((new_client.fd = accept(data->getFd(0), (struct sockaddr *)&serverInfo.address, &size)) == SERVER_FAILURE)
	{
		perror("connection refused");
		return;
	}
	
	
	data->newTempUser(new_client.fd);

	//new_client.events = POLLOUT | POLLIN;
	/*pollfds.push_back(new_client);

	t_activeIndex activeIndex;
	activeIndex.registered = false;
	activeIndex.index = static_cast<uint32_t>(actives.size());

	Client tempClient;

	tempClient = Client();
	actives.push_back(activeIndex);
	tempClients.push_back(tempClient);
	std::cout << "----------ARRAY ha entrado con bool [" << tempClients[tempClients.size() - 1].getInputBlock() << "]"<< std::endl;*/
	sendMsgUser(new_client.fd, "👋 Welcome! Please, introduce the " + color::boldwhite + "server password:" + color::reset + "\n🔐 ");

}


/* ------------------------------------------------------------ */
/*						CLIENT INPUT							*/
/* ------------------------------------------------------------ */


void Server::handleEvents(uint32_t indexAct)
{
	/*
		1- Puntero client. Chequear si el evento viene de un usuario registrado o de uno temporal. Ahora mismo el ciebte esta en active[]
			-> ¿true? == se ha registrado 

			-> ¿false? = no se ha registrado
				-accede al array de temporales para la info de cliente
		
	*/
	if (data->Pollfd()[indexAct].revents & POLLIN)
	{
		std::string input = readTCPInput(data->getFd(indexAct));
		
		if (data->Actives()[indexAct].registered) //si esta aqui esta en el array REGISTERED
		{
			//Client *registeredClient;
			//uint32_t channel = registered[actives[index].index].getChannel();
			//channels[channel].broadcast(index, input);
			microshell(indexAct, input);
		}
		else //si esta aqui esta en el array TEMPORAL
		{
			Client *newClient;
			newClient = &(*data)[indexAct];
			switch (newClient->getState())
			{
				case CL_STATE_SERVER_PASSWORD:
					checkServerPassword(newClient, indexAct, input);
					break;
				case CL_STATE_LOG_IN:
					loginChoice(newClient, indexAct, input);
					break;
				case CL_STATE_SELECT_USERNAME:
					selectUsername(newClient, indexAct, input);
					break;
				case CL_STATE_INTRODUCE_PERSONAL_PASSWORD://Cuando completa este estado pasa al array de REGISTRADO
					checkPassword(newClient, indexAct, input);
					break;
				case CL_STATE_SELECT_NICKNAME: //ya esta en registrado y borrado de temp
					selectNickname(newClient, indexAct, input);
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


bool Server::checkServerPassword(Client *client, uint32_t indexAct, std::string &input)
{
	input.erase(std::remove(input.begin(), input.end(), '\n'), input.end());
	if (input == serverInfo.password) {
		sendMsgUser(data->Pollfd()[indexAct].fd, "\n\033[1;42m✅ OK! Let's continue\033[0m\n");
		client->setState(CL_STATE_LOG_IN);
		sendMsgUser(data->getFd(indexAct), "❓ If you have an account on this chat, " + color::boldwhite + "write 0" + color::reset + ". If not, \033[1;37mwrite 1\033[0m: ");
		//sendMsgUser(this->pollfds[index].fd, "\n🧑 Enter your \033[1;37musername\033[0m: ");
		return true;
	}
	else {
		sendMsgUser(data->getFd(indexAct), "\033[1;31m❌ Incorrect password! Please, try again\033[0m\n🔐 ");
		return false;
	}
}

void Server::loginChoice(Client *client, uint32_t indexAct, std::string &input)
{
	input.erase(std::remove(input.begin(), input.end(), '\n'), input.end());
	if (input == "0")
	{
		client->setState(CL_STATE_SELECT_USERNAME);
		client->setNewClient(false);
		sendMsgUser(data->getFd(indexAct), "\n\033[1;32m👋 Welcome back! 👋\033[0m\n📝 Please, enter you \033[1;37musername\033[0m: ");
	}
	else if (input == "1")
	{
		client->setNewClient(true);
		client->setState(CL_STATE_SELECT_USERNAME);
		sendMsgUser(data->getFd(indexAct), "\n\033[1;32m🆕 Welcome to our IRC chat! 🆕\033[0m\n📝 Please, enter your \033[1;37mdesired username\033[0m: ");
	}
	else
	{
		sendMsgUser(data->getFd(indexAct), "\033[1;31m❌ Incorrect option! Please, try again\033[0m\n❓ ");
	}
}


bool Server::selectUsername(Client *client, uint32_t indexAct, std::string &input)
{
	input.erase(std::remove(input.begin(), input.end(), '\n'), input.end());
	std::cout << "username" << " [" << client->getNewClient() << "]["<< input <<"]\n";
	if (!client->getNewClient()) //log in
	{
		if (findUsername(input))
		{
			client->setState(CL_STATE_INTRODUCE_PERSONAL_PASSWORD);
			client->setUsername(input);
			sendMsgUser(data->getFd(indexAct), "\n🔐 Enter your \033[1;37mpersonal password\033[0m: ");
			return true;
		}
		else //no username
		{
			sendMsgUser(data->getFd(indexAct), "\033[1;31m❌ This user does not exist, please try again.\033[0m\n📝 ");
			return false;
		}
	}
	else // sign up
	{
		if (findUsername(input))
			sendMsgUser(data->getFd(indexAct), "\033[1;31m❌ This username has already been taken! Please, type another username.\033[0m\n📝 ");
		else//register
		{
			client->setUsername(input);
			client->setState(CL_STATE_INTRODUCE_PERSONAL_PASSWORD);
			sendMsgUser(data->getFd(indexAct), "🔐 Enter your \033[1;37mpersonal password\033[0m: ");
			return true;
		}
	}
	return false;
	
}

bool Server::checkPassword(Client *client, uint32_t indexAct, std::string &input)
{

	input.erase(std::remove(input.begin(), input.end(), '\n'), input.end());
	std::cout << "password" << input << " [" << client->getNewClient() << "]["<< input <<"]\n";
	if (!client->getNewClient()) //logging in
	{
		if (assertClientPassword(indexAct, input)) //pass ok
		{
			client->setState(CL_STATE_SELECT_NICKNAME);
			sendMsgUser(data->getFd(indexAct), "🧑 Enter your desired \033[1;37mnickname\033[0m to chat: ");
			return true;
		}
		else // wrong
		{
			sendMsgUser(data->getFd(indexAct), "\033[1;31m❌ Incorrect password! Please, try again\033[0m\n🔐 ");
		}
	}
	else // sign up 
	{
		client->setPassword(input);
		client->setState(CL_STATE_SELECT_NICKNAME);
		sendMsgUser(data->getFd(indexAct), "🧑 Enter your desired \033[1;37mnickname\033[0m to chat: ");
	}
	return false;
}



void Server::selectNickname(Client *client, uint32_t indexAct, std::string &input)
{
	input.erase(std::remove(input.begin(), input.end(), '\n'), input.end());
	
	client->setNickname(input);
	client->setState(CL_STATE_LOBBY);

	data->newRegisteredUser(indexAct);
	
	/*if (!client->getNewClient())//log in
	{
		tempToRegistered(indexAct);
		actives[indexAct].index = findUsername(client->getUsername());
	}
	else
	{
		//change to the registered array;
		////////////////////////////ERROR///////////////////////// CAMBIA INPUTBLOCK  A 0
		
		registered.push_back(*client);
		//std::cout << "----------Registered se ha creado con bool [" << registered[registered.size() - 1].getInputBlock() << "]"<< std::endl;
		tempToRegistered(indexAct);
		actives[indexAct].index = registered.size() - 1;

		///////////////////////////////////////////////////// 
	}*/
	sendMsgUser(data->getFd(indexAct), color::boldgreenback + "\n👋 Welcome to the \033[1;33m'A O I R C' \033[1;42;37mchat server!\033[0m\n" + color::reset);
	joinChannel(indexAct, "Lobby");//es bool, recoger el true? en el caso de lobby igual no 
	
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


void command(uint32_t index, std::string &input)
{
	//std::string comd = "";
	(void)input;
	(void)index;

}

void Server::microshell(uint32_t indexAct, std::string &input)
{
	if (input == "\n")
		channels[(*data)[indexAct].getChannel()].refresh(indexAct);
	else if (input[0] == '<')//command
		command(indexAct, input);
	else
	{
		channels[(*data)[indexAct].getChannel()].broadcast(indexAct, input);
	}
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
	if (client.getRole() == CL_ROOT)
	{
		if (findChannel(name))
		{
			sendMsgUser(data->getFd(indexAct), color::red + "❌ This channel already exists! Please, choose another channel name.\n" + color::reset);
		}
		else
			channels.push_back(Channel(name, client.getUsername(), data));
	}

}
/*
void	Server::deleteChannel(size_t channelIndex)
{
	std::deque<Channel>::iterator it;

	it += channelIndex;
	channels.erase(it);
}
*/
/* ------------------------------------------------------------ */
/*							OBSOLETE							*/
/* ------------------------------------------------------------ */


bool Server::joinChannel(uint32_t indexAct, std::string input) {
	
	uint8_t channelIndex;

	channelIndex = findChannel(input);

	if (channelIndex < channels.size())
	{
		sendMsgUser(data->getFd(indexAct), "✅ You've been successfully connected to the channel!\n");
		channels[channelIndex].addClient(indexAct);
		//sendMsgUser(pollfds[indexAct].fd, color::red + registered[actives[indexAct].index].getUsername() + color::reset);
		//deberia mandar el mensaje el propio channel
		(*data)[indexAct].setState(CL_STATE_IN_CHANNEL);
	
	}
	else
		sendMsgUser(data->getFd(indexAct), color::red + "❌ Incorrect channel, please try again.\n-> " + color::reset);
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
	for(uint32_t i = 1; i < data->sizeAct();i++)
	{
		handleEvents(i);
	}
}

std::string Server::readTCPInput(int client_fd) {
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
	for (uint32_t i =0;i < data->sizeReg();i++)
	{
		if ((data->Registered()[i].getUsername()) == username)
		{	
			return i;
		}
	}
	return (0);
}

bool	Server::assertClientPassword(uint32_t indexAct, const std::string &password) const
{
	uint32_t indexReg;

	indexReg = findUsername((*data)[indexAct].getUsername());
	if(indexReg == 0)
	{
		std::cout << "server crash\n";
	}
	else
	{
		if ((*data)[indexAct].checkPassword(password))
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

void Server::sendMsgUser(int fd, const std::string &str) const
{
	int buffer_size = 65536;
	setsockopt(fd, SOL_SOCKET, SO_SNDBUF, &buffer_size, sizeof(buffer_size));
	send(fd, str.c_str(), str.size(), 0);
}



//////////    INIT SETTINGS   ///////

void Server::setCommands()
{
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
}

/////////////////  PRINT INFO   /////////////////

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
	
		std::cout << "\033[1;37m🖥️  👥 SERVER ACTIVE USERS: \033[0m" << data->sizeAct() << '\n' << std::endl;

		std::cout << "\033[1;37m🔶 👥 TEMP USERS (waiting to login or register): \033[0m" << data->sizeTmp() << '\n';
		for(uint32_t i = 0; i < data->sizeTmp(); i++)
		{
			if (data->TempClients()[i].getUsername() == "server")
				std::cout << "\t\033[1;37m[" << i << "] \033[0m: username(\033[1;33m" << data->TempClients()[i].getUsername() << "\033[0m) new:(" << data->TempClients()[i].getNewClient() << ")" << std::endl;
			else
				std::cout << "\t\033[1;37m[" << i << "] \033[0m: username(\033[1;37m" << data->TempClients()[i].getUsername() << "\033[0m) new:(" << data->TempClients()[i].getNewClient() << ")" << std::endl;
		}

		std::cout << "\033[1;37m✅ 👥 REGISTERED USERS: \033[0m" << data->Registered().size() << "\n";
		for(uint32_t i = 0; i < data->Registered().size(); i++)
		{
			if (data->Registered()[i].getUsername() == "ROOT")
				std::cout << "\t\033[1;37m[" << i << "] \033[0m: username(\033[1;31m" << data->Registered()[i].getUsername() << "\033[0m) " << data->Registered()[i].getInputBlock() << " \n";
			else
				std::cout << "\t\033[1;37m[" << i << "] \033[0m: username(\033[1;37m" << data->Registered()[i].getUsername() << "\033[0m) " << data->Registered()[i].getInputBlock() << " \n";
		}

		std::cout << "\n\033[1;37m💭 CHANNELS LIST: \033[0m" << channels.size() << "\n";
		for(uint32_t i = 0; i < channels.size(); i++)
		{
			std::cout << "\t\033[1;37m[" << channels[i].getName() << "]\033[0m" << std::endl;
		}
		std::cout << "\n\033[1;33m🟨 DEBUG LOG (if any)\033[0m" << std::endl;
		last_time = now;
	}
}