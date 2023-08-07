#include <chrono>
#include <arpa/inet.h>
#include "../include/Server.hpp"

/* ------------------------------------------------------------ */
/* 			CONSTRUCTOR DESTRUCTOR INITIALIZATION 				*/
/* ------------------------------------------------------------ */

Server::Server(sd::t_serverInput &serverInput) :
	serverInfo(serverInput),serverData(serverInfo)
{
	if (DEBUG)
	{
		logFile.open(DEBUG_FILE, std::ios::in | std::ios::out);
		if (!logFile.is_open())
		{
			throw std::runtime_error(std::string("Failed to open file") + DEBUG_FILE);
		}
		logTimeVal("Server created.");
	}
}

Server::~Server()
{
	if ((status = close(serverData[(sd::pollfdIt)0].fd) == SERVER_FAILURE))
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
		events = poll(serverData.getPollfdData(), static_cast<nfds_t>(serverData.pollfdSize()), serverData.getConfig().maxusers);
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
	sd::pollfdIt i = 0;
	if (serverData[i].revents & POLLIN) //CONEXION REQUEST
	{
		acceptConnection();
		events--;
	}
	for (i = 1; i < serverData.pollfdSize() && events; i++) //INPUT REQUEST
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
	if (listen(serverData[(sd::pollfdIt)0].fd, 3) == SERVER_FAILURE)
	{
		std::cerr << color::red << "ERROR: listening process failure\n" << color::reset; 
		exit(EXIT_FAILURE);
	}
}

std::string getHostName(int fd)
{
	struct sockaddr_in clientAddr;
	socklen_t clientAddrLen = sizeof(clientAddr);
	if (getpeername(fd, (struct sockaddr *)&clientAddr, &clientAddrLen) == -1) {
		std::cerr << color::red << "ERROR: getpeername() failed\n" << color::reset; 
		exit(1);
	}
	char ipStr[INET_ADDRSTRLEN];// Convert the IP address from binary to string representation
	if (inet_ntop(AF_INET, &(clientAddr.sin_addr), ipStr, INET_ADDRSTRLEN) == nullptr) {
		std::cerr << color::red << "ERROR: inet process failure\n" << color::reset; 
		exit(1);
	}
	return std::string(ipStr);
}

void	Server::acceptConnection() {
	
	unsigned int size = static_cast<unsigned int>(sizeof(serverInfo.address));
	struct pollfd new_client;

	if ((new_client.fd = accept(serverData[(sd::pollfdIt)0].fd, (struct sockaddr *)&serverInfo.address, &size)) == SERVER_FAILURE)
	{
		std::cerr << color::red << "ERROR: Connection refused\n" << color::reset; 
		return;
	}
	new_client.events = POLLOUT | POLLIN;
	std::string hostName = getHostName(new_client.fd);
	serverData.addClient(new_client, Client(hostName));
}

void Server::serverConfig(sd::t_serverInput *serverInput)
{
	std::string file = utils::readFile("server.conf");
	std::vector<std::string>params = utils::split(file, '\n');
	try{
		for (std::vector<std::string>::iterator it = params.begin(); it != params.end(); it++)
		{
			std::vector<std::string> conf = utils::split(*it, '=');
			if (conf[0] == "CHANTYPES")
				serverInput->chantypes = conf[1];
			else if (conf[0] == "PREFIX")
				serverInput->prefix = conf[1];
			else if (conf[0] == "MODES")
				serverInput->modes = std::stoi(conf[1]);
			else if (conf[0] == "CHANLIMIT")
				serverInput->chanlimit = std::stoi(conf[1].substr(2));
			else if (conf[0] == "NICKLEN")
				serverInput->nicklen = std::stoi(conf[1]);
			else if (conf[0] == "USERLEN")
				serverInput->userlen = std::stoi(conf[1]);
			else if (conf[0] == "HOSTLEN")
				serverInput->hostlen = std::stoi(conf[1]);
			else if (conf[0] == "TOPICLEN")
				serverInput->topiclen = std::stoi(conf[1]);
			else if (conf[0] == "KICKLEN")
				serverInput->kicklen = std::stoi(conf[1]);
			else if (conf[0] == "CHANNELLEN")
				serverInput->channellen = std::stoi(conf[1]);
			else if (conf[0] == "MAXUSERS")
				serverInput->maxusers = std::stoi(conf[1]);
			else if (conf[0] == "MAXUSERSCHAN")
				serverInput->maxuserschan = std::stoi(conf[1]);
			else if (conf[0] == "VERSION")
				serverInput->version = conf[1];
			else if (conf[0] == "VERSION_COMMENTS")
				serverInput->versionComments = conf[1];
		}
	}
	catch(...)
	{
		throw std::runtime_error("failed to fill server conf");
	}
}

/* ---------------------------------------------------------------------------------------- */
/*				POLL() AND HANDLE EVENTS	 (incoming requests and inputs)	   				*/
/* ---------------------------------------------------------------------------------------- */

eFlags Server::handleInput(sd::clientIt index, std::string input)
{
	std::vector<std::string> arguments = utils::splitIrcPrameters(input, ' ');

	cmd::CmdInput package(arguments, serverData, index);

	if (serverData[index].getAuthentificied() != sd::eAuthentified && arguments[0] != "NICK" && arguments[0] != "USER" && arguments[0] != "PASS")
	{
		return eError;
	}
	if (DEBUG)
	{
		logServerStatus(input);
		//logTimeVal(input);
	}
	eFlags output = cmd::callFunction(arguments[0], package);
	
	// Algunas eFLAG necesitan gestion:
	if (output == eBack)
	{
		cmd::removeClientChannels(serverData, index);
		serverData.backClient(index);
	}
	else if (output == eExited)
	{
		cmd::removeClientChannels(serverData, index);
		serverData.removeClient(index);
	}
	else if (output == eRemoveClientChannel)
	{
		cmd::removeClientChannels(serverData, index);
	}
	else if (output == eNoSuchFunction && arguments.size() &&!arguments[0].empty())
	{
		error::error(package, error::ERR_UNKNOWNCOMMAND, arguments[0]);
	}
	return output;
}

void Server::handleEvents(sd::pollfdIt index)
{
	if (serverData[index].revents & POLLIN)
	{
		std::string input = readTCPInput(serverData[index].fd, (sd::clientIt)index);
		serverData[(sd::clientIt)index].addBuffer(input);
		if (input[input.size() - 1] != '\n')
			return;
		else
		{
			if (input == "QUIT\n" && serverData[(sd::clientIt)index].getAuthentificied() != sd::eAuthentified)
			{
				serverData.removeClient((sd::clientIt)index);
				return ;
			}
			input = serverData[(sd::clientIt)index].getBuffer();
			std::vector<std::string> lines = utils::split(input, '\n');
			bool exited = false;
			for (uint32_t i = 0; i < lines.size(); i++)
			{
				eFlags value;
				std::string username = serverData[(sd::clientIt)index].getUsername();
				value = handleInput((sd::clientIt)index, lines[i]);
				if (value == eReordered)
				{
					index = serverData.findUsername(username);
					if (index == 0)
						return;
				}
				exited = value == eBack || value == eExited;
			}
			if (!exited) 		//eSuccess / eError / eRemoveChannel / eNoSuchFUnction / eReordered / eRemoveClientChannel / eRemoveChannel -> todas son validas y siguen corriendo el bucle, emptyBuffer() y sigue
				serverData[(sd::clientIt)index].emptyBuffer();
			if (DEBUG)
				logServerStatus("FINISH");
		}
	}
}

std::string Server::getName()const
{
	return serverData.getName();
}

/* ------------------------------------------------------------ */
/*							UTILS								*/
/* ------------------------------------------------------------ */

std::string Server::readTCPInput(int client_fd, sd::clientIt index) {
	char echoBuffer[RCVBUFSIZE];
	int	recvMsgSize;
	(void)index;

	memset(echoBuffer, 0, RCVBUFSIZE);
	recvMsgSize = recv(client_fd, echoBuffer, sizeof(echoBuffer) - 1, 0);
	if (recvMsgSize == SERVER_FAILURE)
	{
		std::cerr << color::red << "ERROR: recv failed\n" << color::reset;
		return (std::string(""));
	}
	else if (recvMsgSize == 0) {
		return ("QUIT\n");
	}
	///////////////LIMPIACARRO///////////
	ssize_t j = 0;
	for(ssize_t i = 0; i < recvMsgSize; i++)
	{
		if (echoBuffer[i] != '\r')
		{
			echoBuffer[j] = echoBuffer[i];
			j++;
		}
	}
	return std::string(echoBuffer, j);
}

/* ---------------------------------------------------------------------------------------- */
/*										SERVER FROM ITERM									*/
/* ---------------------------------------------------------------------------------------- */

void Server::lauch()
{
	int events;

	listenConnection();
	while(1)
	{
		events = poll(serverData.getPollfdData(), static_cast<nfds_t>(serverData.pollfdSize()), serverData.getConfig().maxusers);
		if (events < 0)
		{
			std::cerr << color::red << "ERROR: poll() - error-event detected\n" << color::reset; 
			return;
		}
		if (events != 0)
			checkFds(events);
	}
}

void *Server::lauchWrapper(void *data)
{
	Server *server = reinterpret_cast<Server *>(data);
	server->lauch();
	return nullptr;
}

void	Server::run2()
{
	if(pthread_create(&serverThread, NULL, lauchWrapper, this))
	{
		std::cerr << color::red << "ERROR: Error creating thread\n" << color::reset; 
		return ;
	}
	minishell();
	if(pthread_join(serverThread, NULL))
	{
		std::cerr << color::red << "ERROR: Error joining thread\n" << color::reset; 
		return ;
	}
}

/* ---------------------------------------------------------------------------------------- */
/*										SERVER SHELLL 										*/
/*							Allows to debug data while executing 							*/
/* ---------------------------------------------------------------------------------------- */

void Server::minishell()
{
	std::cout << std::endl;
	while (1)
	{
		std::string line;
		std::cout << color::boldwhite << "> " << color::reset;
		std::getline(std::cin, line);
		if (line == "quit")
			return ;
		else if (line == "ls ch")
			printAllChannNames(true);
		else if (line == "ls users")
			printAllUsers(true);
		else if (line == "ls back")
			printAllUsersBack(true);
		else if (line == "ls all")
		{
			printAllUsers(true);
			printAllUsersBack(true);
		}
		else if (line == "info ch")
		{
			std::cout << "ch name: ";
			std::getline(std::cin, line);
			printChannelInfo(line);
		}
		else if (line == "info user")
		{
			std::cout << "user name: ";
			std::getline(std::cin, line);
			printUserInfo(line);
		}
		else if (line == "info server")
			printServerInfo();
		else if (line == "info lobby")
			printLobbyInfo(true);
		else if (line == "info")
			printInfo();
		else if (line == "update conf")
		{
			updateConf();
		}
		else {
			std::cout << color::red << "║ Unknown command! Type 'info' for help.\n" << color::reset;
		}
	}
}

void Server::updateConf()
{
	try {
		sd::t_serverInput input;
		serverConfig(&input);
		serverInfo = input;
		serverData.setConfig(input);
		std::cout << color::green << "║ The config has been successfully updated with new server.conf changes\n" << color::reset;
	}
	catch (std::exception &e)
	{
		std::cout << color::red << "ERROR: " << e.what() << color::reset << '\n';
	}
}

void Server::printAllChannNames(bool color) const
{
	if (color)
	{
		for (sd::channIt ch = 0; ch < serverData.getNumOfChannels(); ch++)
		{
			std::cout << color::boldwhite << "║ [" << std::to_string(ch) << "]\t" << serverData[ch].getName() << "\n" << color::reset;
		}
	}
	else
	{
		for (sd::channIt ch = 0; ch < serverData.getNumOfChannels(); ch++)
		{
			std::cout << "║ [" << std::to_string(ch) << "]\t" << serverData[ch].getName() << "\n";
		}
	}
}

void Server::printAllUsers(bool color)const
{
	if (color)
	{
		for (sd::clientIt user = 0; user < serverData.getNumOfClients(); user++)
		{
			std::cout << color::boldwhite << "║ [" << std::setw(5) << std::to_string(user) << "] " << color::green << "Nick: [" << color::reset << serverData[user].getNickname() << "] - " << color::yellow << "Username: [" << color::reset << serverData[user].getUsername() << "] [" << serverData[(sd::pollfdIt)user].fd << "]" << color::reset << '\n';
		}
	}
	else
	{
		for (sd::clientIt user = 0; user < serverData.getNumOfClients(); user++)
		{
			std::cout << "║ [" << std::to_string(user) << "] "  << "Nick: [" << serverData[user].getNickname() << "] - "  << "Username: [" << serverData[user].getUsername() << "] [" << serverData[(sd::pollfdIt)user].fd << "]" << '\n';
		}
	}
}

void Server::printAllUsersBack(bool color)const
{
	if (color)
	{
		std::cout << color::boldwhite << "║ " << color::red << "Back Users: " << std::endl;
		for (sd::backIt user = 0; user < serverData.getNumOfClientsBack(); user++)
		{
			std::cout << color::boldwhite << "║ [" << std::to_string(user) << "] " << color::green << "Nick: [" << color::reset << serverData[user].getNickname() << "] - " << color::yellow << "Username: [" << color::reset << serverData[user].getUsername() << "]" <<  color::reset << "\n";
		}
	}
	else
	{
		std::cout << "║ " << "Back Users: " << std::endl;
		for (sd::backIt user = 0; user < serverData.getNumOfClientsBack(); user++)
		{
			std::cout << "║ [" << std::to_string(user) << "] " << "Nick: [" << serverData[user].getNickname() << "] - " << "Username: [" << serverData[user].getUsername() << "]" << "\n";
		}
	}
	
}


void Server::printLobbyInfo(bool color)const
{
	if (color)
	{
		sd::channIt channel = 0;
		std::cout << color::boldwhite << "║ " << color::yellow << "Name: " << color::reset << "Lobby" << '\n';
		std::cout << color::boldwhite << "║ " << color::yellow << "Topic: " << color::reset << serverData[channel].getTopic() << '\n';
		std::cout << color::boldwhite << "║ " << color::yellow << "Creator: " << color::reset << serverData[channel].getCreator() << '\n';
		std::cout << color::boldwhite << "║ " << color::yellow << "Creation Date: " << color::reset << serverData[channel].getCreationDate() << '\n';
		std::cout << color::boldwhite << "║ " << color::yellow << "Num of users: " << color::reset << serverData[channel].getNumUser() - 1 << '\n';
		std::cout << color::boldwhite << "║ " << color::yellow << "Users:\n" << color::reset;
		std::vector<std::string> names = utils::split(serverData.getUserList(channel), '\n');
		for (std::vector<std::string>::const_iterator name = names.begin(); name != names.end(); name++)
		{
			std::cout << color::cyan << "\t" << *name << '\n' << color::reset;
		}
	}
	else
	{
		sd::channIt channel = 0;
		std::cout << "║ " << "Name: " << "Lobby" << '\n';
		std::cout << "║ " << "Topic: " << serverData[channel].getTopic() << '\n';
		std::cout << "║ " << "Creator: " << serverData[channel].getCreator() << '\n';
		std::cout << "║ " << "Creation Date: " << serverData[channel].getCreationDate() << '\n';
		std::cout << "║ " << "Num of users: " << serverData[channel].getNumUser() - 1 << '\n';
		std::cout << "║ " << "Users:\n";
		std::vector<std::string> names = utils::split(serverData.getUserList(channel), '\n');
		for (std::vector<std::string>::const_iterator name = names.begin(); name != names.end(); name++)
		{
			std::cout << "\t" << *name << '\n';
		}
	}
	
}

void Server::printChannelInfo(const std::string& chName)const
{
	sd::channIt channel = serverData.findChannel(chName);
	if (channel == 0)
	{
		std::cout << color::red << "║ Channel '" << chName << "' does not exist\n" << color::reset;
		return ;
	}
	std::cout << color::boldwhite << "║ " << color::yellow << "Name: " << color::reset << chName << '\n';
	std::cout << color::boldwhite << "║ " << color::yellow << "Topic: " << color::reset << serverData[channel].getTopic() << '\n';
	std::cout << color::boldwhite << "║ " << color::yellow << "Creator: " << color::reset << serverData[channel].getCreator() << '\n';
	std::cout << color::boldwhite << "║ " << color::yellow << "Creation Date: " << color::reset << serverData[channel].getCreationDate() << '\n';
	std::cout << color::boldwhite << "║ " << color::yellow << "Num of users: " << color::reset << serverData[channel].getNumUser() - 1 << '\n';
	std::cout << color::boldwhite << "║ " << color::yellow << "Users:\n" << color::reset;
	std::vector<std::string> names = utils::split(serverData.getUserList(channel), '\n');
	for (std::vector<std::string>::const_iterator name = names.begin(); name != names.end(); name++)
	{
		std::cout << color::cyan << "\t" << *name << '\n' << color::reset;
	}
}

void Server::printUserInfo(const std::string& nickname)const
{
	sd::clientIt user = serverData.findNickname(nickname);
	if (user == 0)
	{
		std::cout << color::red << "║ Channel " << nickname << " does not exist\n" << color::reset;
		return ;
	}
	std::cout << color::boldwhite << "║ " << color::yellow << "Nickname: " << color::reset << nickname << '\n';
	std::cout << color::boldwhite << "║ " << color::yellow << "Username: " << color::reset << serverData[user].getUsername() << '\n';
	std::cout << color::boldwhite << "║ " << color::yellow << "Hostname: " << color::reset << serverData[user].getHostname() << '\n';
	std::cout << color::boldwhite << "║ " << color::yellow << "Role: " << color::reset << serverData[user].getRole() << '\n';
	std::cout << color::boldwhite << "║ " << color::yellow << "Authentified: " << color::reset << serverData[user].getAuthentificied() << '\n';
		std::cout << color::boldwhite << "║ " << color::yellow << "ChannelOps: " << color::reset;
		utils::printBinary(serverData[user].getChannelToOps());
		std::cout << '\n';
	std::cout << color::boldwhite << "║ " << color::yellow << "Away: " << color::reset << (bool)serverData[user].getAwayStatus();
	if (serverData[user].getAwayStatus())
		std::cout << color::boldwhite << "║ Message: " << color::reset << serverData[user].getAwayMsg();
	std::cout << '\n';
}

void Server::rmrf(const std::string& nickname)
{
	sd::clientIt user = serverData.findNickname(nickname);
	if (user == 0)
	{
		std::cout << color::red << "║ User " << nickname << " does not exist\n" << color::reset;
		return ;
	}
	std::vector<std::string> arguments;
	arguments.push_back("QUIT");
	cmd::CmdInput input(arguments, serverData, user);
	cmd::callFunction("QUIT", input);
	
	std::cout << color::green << "║ User " << nickname << " has been successfully killed\n" << color::reset;
}

void Server::printServerInfo()const
{
	std::cout << color::boldwhite << "║ " << color::yellow << "CHANTYPES: " << color::reset << serverInfo.chantypes << '\n';
	std::cout << color::boldwhite << "║ " << color::yellow << "PREFIX: " << color::reset << serverInfo.prefix << '\n';
	std::cout << color::boldwhite << "║ " << color::yellow << "MODES: " << color::reset << serverInfo.modes << '\n';
	std::cout << color::boldwhite << "║ " << color::yellow << "CHANLIMIT: " << color::reset << serverInfo.chanlimit << '\n';
	std::cout << color::boldwhite << "║ " << color::yellow << "NICKLEN: " << color::reset << serverInfo.nicklen << '\n';
	std::cout << color::boldwhite << "║ " << color::yellow << "USERLEN: " << color::reset << serverInfo.userlen << '\n';

	std::cout << color::boldwhite << "║ " << color::yellow << "HOSTLEN: " << color::reset << serverInfo.hostlen << '\n';
	std::cout << color::boldwhite << "║ " << color::yellow << "TOPICLEN: " << color::reset << serverInfo.topiclen << '\n';
	std::cout << color::boldwhite << "║ " << color::yellow << "CHANNELLEN: " << color::reset << serverInfo.channellen << '\n';
	std::cout << color::boldwhite << "║ " << color::yellow << "MAXUSERS: " << color::reset << serverInfo.maxusers << '\n';
	std::cout << color::boldwhite << "║ " << color::yellow << "MAXUSERSCHAN: " << color::reset << serverInfo.maxuserschan << '\n';
	std::cout << color::boldwhite << "║ " << color::yellow << "VERSION: " << color::reset << serverInfo.version << '\n';
	std::cout << color::boldwhite << "║ " << color::yellow << "USERLEN: " << color::reset << serverInfo.userlen << '\n';
	std::cout << color::boldwhite << "║ " << color::yellow << "VERSION_COMMENTS: " << color::reset << serverInfo.versionComments << '\n';
}

void Server::printInfo()const
{
	std::cout << color::boldwhite << "║ " << color::yellow << "ls ch: " << color::reset << "list all channels" << '\n';
	std::cout << color::boldwhite << "║ " << color::yellow << "ls users: " << color::reset << "list all users" << '\n';
	std::cout << color::boldwhite << "║ " << color::yellow << "info user: " << color::reset << "print user info" << '\n';
	std::cout << color::boldwhite << "║ " << color::yellow << "info ch: " << color::reset << "print channel info" << '\n';
	std::cout << color::boldwhite << "║ " << color::yellow << "info server: " << color::reset << "print server info" << '\n';
}

void Server::log(const std::string str)
{
	logFile << str << std::endl;
}

void Server::logTimeVal(const std::string str)
{
	const std::time_t now = utils::t_chrono::to_time_t(utils::t_chrono::now());
	std::string nowS =  std::ctime(&now);
	log('[' + nowS.substr(0, nowS.size() - 1) + "]: " + str);
}

void Server::logServerStatus(std::string str)
{
	std::streambuf *coutbuf = std::cout.rdbuf();
	std::cout.rdbuf(logFile.rdbuf());
	const std::time_t now = utils::t_chrono::to_time_t(utils::t_chrono::now());
	std::string nowS =  std::ctime(&now);
	std::cout << '[' << nowS.substr(0, nowS.size() - 1) << "]: " << str << '\n';

	printAllUsers(false);
	printAllUsersBack(false);
	std::cout << "\n";
	printAllChannNames(false);
	std::cout << "\n";
	printLobbyInfo(false);
	std::cout << "\n\n";

	std::cout.rdbuf(coutbuf);
}