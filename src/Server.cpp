#include "../include/Server.hpp"
#include <chrono>
/* ------------------------------------------------------------ */
/* 			CONSTRUCTOR DESTRUCTOR INITIALIZATION 				*/
/* ------------------------------------------------------------ */

Server::Server(t_serverInput *serverInput):serverInfo(*serverInput)
{
	data =  UsersData(serverInfo);
}

Server::~Server()
{
	if ((status = close(data[(pollfdIt)0].fd) == SERVER_FAILURE))
	{
		perror("socket closing failed");
		exit(EXIT_FAILURE);
	}
}

/* ------------------------------------------------------------	*/
/*							MAIN								*/
/* ------------------------------------------------------------	*/


void	Server::run(){
	
	int events;
	
	listenConnection();
	while(1)
	{
		printServerStatus();
		events = poll(data.getPollfdData(), static_cast<nfds_t>(data.size()), 200);
		if (events < 0)
		{
			perror("error-event detected");
			return;
		}
		if (events != 0)
			checkFds(events); 
	}
}



void	Server::checkFds(int events)
{
	pollfdIt i = 0;
	if (data[i].revents & POLLIN) //CONEXION REQUEST
	{
		printf("-------- INCOMING REQUEST RECEIVED --------\n");
		acceptConnection();
		events--;
	}
	for (i = 1; i < data.size() && events; i++) //INPUT REQUEST
	{
		handleEvents(i);
		events--;
	}
}


/* ------------------------------------------------------------ */
/*				LISTEN AND ACCEPT CONNECTION					*/
/* ------------------------------------------------------------ */


void	Server::listenConnection() {
	std::cout << "Server started, im listening" << std::endl;
	if (listen(data[(pollfdIt)0].fd, 3) == SERVER_FAILURE)
	{
		perror("listening process failure");
		exit(EXIT_FAILURE);
	}
}

void	Server::acceptConnection() {
	
	unsigned int size = static_cast<unsigned int>(sizeof(serverInfo.address));
	struct pollfd new_client;

	std::cout << "The connection has been accepted, continuing" << std::endl;
	if ((new_client.fd = accept(data[(pollfdIt)0].fd, (struct sockaddr *)&serverInfo.address, &size)) == SERVER_FAILURE)
	{
		perror("connection refused");
		return;
	}
	new_client.events = POLLOUT | POLLIN;
		///ESTO NO SE SI FUNCIONA EN EL COLOQUI
		sendMsgUser(new_client.fd, "Welcome to - A O I R C - \n");
	data.addClient(new_client, Client());
}

/* ------------------------------------------------------------ */
/*					    	NEW USER 							*/
/* ------------------------------------------------------------ */

void Server::handleNewUser(std::string &input, clientIt idx)
{
	std::cout << "NEW USER\n";
	std::vector<std::string> lines = split(input, '\n');

	for (uint32_t i = 0; i < lines.size();i++)
	{
		std::vector<std::string> arguments = splitIrcPrameters(lines[i], ' ');
		if (i == 1) //NICK
			saveNick(arguments, data[idx]);
		else if (i == 2) //USER
			saveUser(arguments, data[idx]);
	}
	//data.addClient(newUser);
}

void Server::saveNick(std::vector<std::string> &arguments, Client &client)
{
	client.setNickname(arguments[1]);
}

void Server::saveUser(std::vector<std::string> &arguments, Client &client)
{
	client.setUsername(arguments[1]);
	client.setRole(atoi(arguments[2].c_str()));
}



/* ---------------------------------------------------------------------------------------- */
/*						POLL() AND HANDLE EVENTS	 (incoming requests and inputs)			*/
/* ---------------------------------------------------------------------------------------- */




void Server::handleEvents(pollfdIt index)
{
	
	if (data[index].revents & POLLIN)
	{
		std::string input = readTCPInput(data[index].fd);
		handleNewUser(input, (clientIt)index); 
		
		
		//if (data->Actives()[indexAct].registered) //si esta aqui esta en el array REGISTERED
		//{
		//	//Client *registeredClient;
		//	//uint32_t channel = registered[actives[index].index].getChannel();
		//	//channels[channel].broadcast(index, input);
		//	microshell(indexAct, input);
		//}
		//else //si esta aqui esta en el array TEMPORAL
		//{
		//	Client *newClient;
		//	newClient = &(*data)[indexAct];
		//	switch (newClient->getState())
		//	{
		//		case CL_STATE_SERVER_PASSWORD:
		//			checkServerPassword(newClient, indexAct, input);
		//			break;
		//		case CL_STATE_LOG_IN:
		//			loginChoice(newClient, indexAct, input);
		//			break;
		//		case CL_STATE_SELECT_USERNAME:
		//			selectUsername(newClient, indexAct, input);
		//			break;
		//		case CL_STATE_INTRODUCE_PERSONAL_PASSWORD://Cuando completa este estado pasa al array de REGISTRADO
		//			checkPassword(newClient, indexAct, input);
		//			break;
		//		case CL_STATE_SELECT_NICKNAME: //ya esta en registrado y borrado de temp
		//			selectNickname(newClient, indexAct, input);
		//			break;
		//		default:
		//			break;
		//	}
		//}
	}
}


/* ------------------------------------------------------------ */
/*					CHECK LOG IN AND REGISTER					*/
/* ------------------------------------------------------------ */

/*
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
		sendMsgUser(data->getFd(indexAct), color::boldred + "❌  Incorrect option! Please, try again" + color::reset + "\n❓ ");
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
			sendMsgUser(data->getFd(indexAct), "🔐 Enter your " + color::boldwhite + "personal password" + color::reset + ": ");
			return true;
		}
		else //no username
		{
			sendMsgUser(data->getFd(indexAct), color::boldred + "❌ This user does not exist, please try again." + color::reset + "\n📝 ");
			return false;
		}
	}
	else // sign up
	{
		if (findUsername(input))
			sendMsgUser(data->getFd(indexAct), color::boldred + "❌ This username was already taken! Please, type another username." + color::reset + "\n📝 ");
		else//register
		{
			client->setUsername(input);
			client->setState(CL_STATE_INTRODUCE_PERSONAL_PASSWORD);
			sendMsgUser(data->getFd(indexAct), "🔐 Enter your " + color::boldwhite + "personal password" + color::reset + ": ");
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
			sendMsgUser(data->getFd(indexAct), "🧑 Enter your desired " + color::boldwhite + "nickname" + color::reset + "to chat: ");
			return true;
		}
		else // wrong
		{
			sendMsgUser(data->getFd(indexAct), color::boldred + "❌ Incorrect password! Please, try again" + color::reset + "\n🔐 ");
		}
	}
	else // sign up 
	{
		client->setPassword(input);
		client->setState(CL_STATE_SELECT_NICKNAME);
		sendMsgUser(data->getFd(indexAct), "🧑 Enter your desired " + color::boldwhite + "nickname" + color::reset + " to chat: ");
	}
	return false;
}



void Server::selectNickname(Client *client, uint32_t indexAct, std::string &input)
{
	input.erase(std::remove(input.begin(), input.end(), '\n'), input.end());
	
	client->setNickname(input);
	client->setState(CL_STATE_LOBBY);

	data->newRegisteredUser(indexAct);
	
	if (!client->getNewClient())//log in
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
	}
	sendMsgUser(data->getFd(indexAct), color::boldgreenback + "\n👋 Welcome to the " + color::yellow + "'A O I R C' \033[1;42;37mchat server!" + color::reset + "\n");
	channels[0].addClient(indexAct);//es bool, recoger el true? en el caso de lobby igual no 
	
}

*/
/* ------------------------------------------------------------ */
/*							MICROSHELL							*/
/* ------------------------------------------------------------ */


/*
		Depende el estado en el que esta, solo va a comparar ciertos comandos. Estado: 4 o 5 

*/

/*
void Server::microshell(uint32_t indexAct, std::string &input)
{
	if (input == "\n")
		channels[(*data)[indexAct].getChannel()].refresh(indexAct);
	else if (input[0] == '<')//command
		command(indexAct, input);
	else
		channels[(*data)[indexAct].getChannel()].broadcast(indexAct, input);
}

std::string getCommand(std::string input)
{
	std::string result = input.substr(1, input.find('>') - 1);
	return result;
}
void Server::command(uint32_t indexAct, std::string &input)
{
	std::string command = getCommand(input);
	uint32_t i ;
	for (i = 0; i < commands.size;i++)
	{
		if (command == commands.cmd[i])
		{
			(this->*(commands.func[i]))(indexAct, input);
			break ;
	
		}
	}
	if (i == commands.size)
		sendMsgUser(data->getFd(indexAct), color::boldwhite + "❌ This command " + color::boldyellow + "(" + command + ")" + color::boldwhite + "does not exist!\n" + color::reset);
}
*/
/* 	------------------------------------------------------------ */
/*					MICROSHELL COMMADNS							*/
/* 	------------------------------------------------------------ */
/*
void Server::nickname_edit_m(uint32_t indexAct, std::string &argument)
{
	argument.erase(std::remove(argument.begin(), argument.end(), '\n'), argument.end());
	std::vector<std::string> words = split(argument, ' ');
	std::cout << "size = " << words.size() << '\n';
	if (words.size() != 3)
	{
		sendMsgUser(data->getFd(indexAct), color::boldred + "❌ Wrong arguments or command!\n" + color::reset);
		return ;
	}
	(*data)[indexAct].setNickname(words[2]);
	sendMsgUser(data->getFd(indexAct), color::boldred + "Nickname Changed to [" + words[2] + "]\n" + color::reset);
}
		
void Server::password_edit_m(uint32_t indexAct, std::string &argument)
{
	argument.erase(std::remove(argument.begin(), argument.end(), '\n'), argument.end());
	std::vector<std::string> words = split(argument, ' ');
	if (words.size() != 3)
	{
		sendMsgUser(data->getFd(indexAct), color::boldred + "❌ Wrong arguments or command!\n" + color::reset);
		return ;
	}
	(*data)[indexAct].setPassword(words[2]);
}
		
void Server::role_edit_m(uint32_t indexAct, std::string &argument)
{
	argument.erase(std::remove(argument.begin(), argument.end(), '\n'), argument.end());

	std::vector<std::string> words = split(argument, ' ');
	if (words.size() != 2)
	{
		sendMsgUser(data->getFd(indexAct), color::boldred + "❌ Wrong arguments or command!\n" + color::reset);
		return ;
	}
	//check ADMIN password
	if ((*data)[0].checkPassword(words[1]))
		(*data)[indexAct].setRole(CL_ROOT);
}

void Server::join_channel_m(uint32_t indexAct, std::string &argument)
{
	argument.erase(std::remove(argument.begin(), argument.end(), '\n'), argument.end());
	std::vector<std::string> words = split(argument, ' ');
	if (words.size() != 3)
	{
		sendMsgUser(data->getFd(indexAct), color::boldred + "❌ Wrong arguments or command!\n" + color::reset);
		return ;
	}

	uint32_t	channelIdx = findChannel(words[2]);
	if((*data)[indexAct].getState() == CL_STATE_IN_CHANNEL) 	//depende del estado (IN_CHANNEL)tiene que hacer leave+ join
		channels[channelIdx].removeClient(indexAct);
	if (channelIdx)
		channels[findChannel(words[2])].addClient(indexAct);
	else
		sendMsgUser(data->getFd(indexAct), color::boldred + "❌ This channel does not exist! Please, try again.\n" + color::reset);
}

void Server::leave_channel_m(uint32_t indexAct, std::string &argument)
{
	argument.erase(std::remove(argument.begin(), argument.end(), '\n'), argument.end());
	(void)argument;
	if ((*data)[indexAct].getState() != CL_STATE_IN_CHANNEL)
	{
		sendMsgUser(data->getFd(indexAct), color::boldred + "❌ You must be in a channel to use this command!\n" + color::reset);
		return;
	}
	channels[(*data)[indexAct].getChannel()].removeClient(indexAct);
}

void Server::leave_server_m(uint32_t indexAct, std::string &argument)
{
	argument.erase(std::remove(argument.begin(), argument.end(), '\n'), argument.end());
	(void)indexAct;
	(void)argument;
	//if((*data)[indexAct].getState() == CL_STATE_IN_CHANNEL) 	//depende del estado (IN_CHANNEL)tiene que hacer leave+ join
	//	(*data)[index].channel.removeClient(indexAct);
	//remove de pollfds
	//remove de Actives
	//exit 
}

void Server::susurro_m(uint32_t indexAct, std::string &argument)
{
	argument.erase(std::remove(argument.begin(), argument.end(), '\n'), argument.end());
	(void)indexAct;
	(void)argument;
}

void	Server::createChannel_m(uint32_t indexAct, std::string &argument)
{
	argument.erase(std::remove(argument.begin(), argument.end(), '\n'), argument.end());
	std::vector<std::string> words = split(argument, ' ');
	if (words.size() != 3)
	{
		sendMsgUser(data->getFd(indexAct), color::boldred + "❌ Wrong arguments or command!\n" + color::reset);
		return ;
	}
	if ((*data)[indexAct].getRole() != CL_ROOT)
	{
		sendMsgUser(data->getFd(indexAct), color::red + "❌ This is an admin only command! Check your roles and try again.\n" + color::reset);
		return ;
	}
	if (findChannel(words[2]))
	{
		sendMsgUser(data->getFd(indexAct), color::red + "❌ This channel already exists! Please, choose another channel name.\n" + color::reset);
		return ;
	}
	else
		channels.push_back(Channel(words[2], (*data)[indexAct].getUsername(), data));

}

void	Server::deleteChannel_m(uint32_t indexAct, std::string &argument)
{
	argument.erase(std::remove(argument.begin(), argument.end(), '\n'), argument.end());
	std::vector<std::string> words = split(argument, ' ');

	if ((*data)[indexAct].getRole() != CL_ROOT)
	{
		sendMsgUser(data->getFd(indexAct), color::red + "❌ This is an admin only command! Check your roles and try again.\n" + color::reset);
		return ;
	}
	std::deque<Channel>::iterator it;

	it += findChannel(words[2]);
	channels.erase(it);
}

void	Server::myInfo_m(uint32_t indexAct, std::string &argument)
{
	argument.erase(std::remove(argument.begin(), argument.end(), '\n'), argument.end());
	(void)argument;
	Client &user = (*data)[indexAct];
	std::string	msg = "\t\tYour user info:\n";
				msg += "\t\tUsername: " + user.getUsername() + "\n";
				msg += "\t\tCurrent Nickname: " + user.getNickname() + "\n";
				msg += "\t\tCurrent Channel: " + channels[user.getChannel()].getName() + "\n";
				msg += "\t\tCurrent role: " + std::to_string(user.getRole()) + "\n";
				msg += "\t\tCurrent state: " + std::to_string(user.getState()) + "\n";
				
	sendMsgUser(data->getFd(indexAct), msg);
}


bool Server::joinChannel(uint32_t indexAct, std::string input) {
	
	uint8_t channelIndex;

	channelIndex = findChannel(input);

	if (channelIndex < channels.size())
	{
		sendMsgUser(data->getFd(indexAct), color::boldgreen + "✅ You've been successfully connected to the channel!\n" + color::reset);
		channels[channelIndex].addClient(indexAct);
		//sendMsgUser(pollfds[indexAct].fd, color::red + registered[actives[indexAct].index].getUsername() + color::reset);
		//deberia mandar el mensaje el propio channel
		(*data)[indexAct].setState(CL_STATE_IN_CHANNEL);
	
	}
	else
		sendMsgUser(data->getFd(indexAct), color::red + "❌ Incorrect channel, please try again.\n-> " + color::reset);
		return false;
}*/

/*void Server::showChannels_m(int fd) const
{
	sendMsgUser(fd, "💭 Chat channels:\n");
	for(std::deque<Channel>::iterator it = channels.begin() + 1; it != channels.end(); it++)
	{
		sendMsgUser(fd, "\t");
		sendMsgUser(fd, "➡️ ");
		sendMsgUser(fd, (*it).getName());
		sendMsgUser(fd, "\n");
	}
}*/


/* ------------------------------------------------------------ */
/*							UTILS								*/
/* ------------------------------------------------------------ */

/*
uint32_t	Server::findChannel(const std::string &name) const
{
	for (uint32_t i =0;i < channels.size();i++)
	{
		if (channels[i].getName() == name)
			return i;
	}
	return (0);
}
*/
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
/*
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
*/
void Server::sendMsgUser(int fd, const std::string &str) const
{
	int buffer_size = 65536;
	setsockopt(fd, SOL_SOCKET, SO_SNDBUF, &buffer_size, sizeof(buffer_size));
	send(fd, str.c_str(), str.size(), 0);
}
/*
bool checkAdmin(Client *client) {
	if (client->getRole() == CL_ROOT)
		return true;
	else
		return false;
}

//////////    INIT SETTINGS   ///////

void Server::setCommands()
{
	commands.cmd[0] = "change nickname";
	commands.cmd[1] = "change password";
	commands.cmd[2] = "root";
	commands.cmd[3] = "join channel";
	commands.cmd[4] = "leave server";
	commands.cmd[5] = "susurro";
	commands.cmd[6] = "leave channel";
	commands.cmd[7] = "my info";
	commands.cmd[8] = "create channel";
	commands.func[0] = &Server::nickname_edit_m;
	commands.func[1] = &Server::password_edit_m;
	commands.func[2] = &Server::role_edit_m;
	commands.func[3] = &Server::join_channel_m;
	commands.func[4] = &Server::leave_channel_m;
	commands.func[5] = &Server::leave_server_m;
	commands.func[6] = &Server::susurro_m;
	commands.func[7] = &Server::myInfo_m;
	commands.func[8] = &Server::createChannel_m;
}
*/
/////////////////  PRINT INFO   /////////////////

void	Server::printServerStatus() const
{
	static std::chrono::steady_clock::time_point last_time = std::chrono::high_resolution_clock::now();
	std::chrono::steady_clock::time_point now;
	double timeElapsed;

	now = std::chrono::high_resolution_clock::now();
	timeElapsed = std::chrono::duration_cast<std::chrono::duration<double> >(now - last_time).count();
	if (timeElapsed > 1)
	{
		system("clear");
		std::cout << "≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡ 🖥️  SERVER STATUS ℹ️  ≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡\n";
	
		//std::cout << "🖥️  👥 SERVER ACTIVE USERS: " << data.size() << '\n' << std::endl;

		std::cout << "USERS (waiting to login or register): " << data.size() << '\n';
		for(clientIt i = 0; i < data.size(); i++)
		{
			std::cout << "\t[" << i << "]  username: " << data[i].getUsername() << " nickname: " << data[i].getNickname() << " role: " << data[i].getRole() << "\n";
		}

		//std::cout << "\t\t CHANNELS LIST:" << channels.size() << "\n";
		//for(uint32_t i = 0; i < channels.size(); i++)
		//{
		//	std::cout << "[" << channels[i].getName() << "]" << std::endl;
		//}
		std::cout << "DEBUG LOG (if any)" << std::endl;
		last_time = now;
	}
}