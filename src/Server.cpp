#include "../include/Server.hpp"
#include <chrono>
/* ------------------------------------------------------------ */
/* 			CONSTRUCTOR DESTRUCTOR INITIALIZATION 				*/
/* ------------------------------------------------------------ */

Server::Server(t_serverInput *serverInput):serverInfo(*serverInput),serverName(serverInput->IP)
{
	data = UsersData(serverInfo);
	errorHandler.setData(&data, serverName);
	setCommands();
	channels.push_back(Channel("Lobby", data[(clientIt)0].getUsername() , &data));
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
		//printServerStatus();
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
		//printf("-------- INCOMING REQUEST RECEIVED --------\n");
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

#include <arpa/inet.h>

std::string getHostName(int fd)
{
	struct sockaddr_in clientAddr;
	socklen_t clientAddrLen = sizeof(clientAddr);
	if (getpeername(fd, (struct sockaddr *)&clientAddr, &clientAddrLen) == -1) {
		perror("getpeername error");
		exit(1);
	}

	// Convert the IP address from binary to string representation
	char ipStr[INET_ADDRSTRLEN];
	if (inet_ntop(AF_INET, &(clientAddr.sin_addr), ipStr, INET_ADDRSTRLEN) == nullptr) {
		perror("inet_ntop error");
		exit(1);
	}
	return std::string(ipStr);

/*
struct addrinfo {
               int              ai_flags;
               int              ai_family;
               int              ai_socktype;
               int              ai_protocol;
               socklen_t        ai_addrlen;
               struct sockaddr *ai_addr;
               char            *ai_canonname;
               struct addrinfo *ai_next;
           };

struct sockaddr {
        ushort  sa_family;
        char    sa_data[14];
};




*/




}

void	Server::acceptConnection() {
	
	unsigned int size = static_cast<unsigned int>(sizeof(serverInfo.address));
	struct pollfd new_client;

	std::cout << "The connection has been accepted, continuing" << std::endl;
	//////PINGGGGGGGGGGGGGGGGGG//////
	if ((new_client.fd = accept(data[(pollfdIt)0].fd, (struct sockaddr *)&serverInfo.address, &size)) == SERVER_FAILURE)
	{
		perror("connection refused");
		return;
	}
	new_client.events = POLLOUT | POLLIN;
	std::string hostName = getHostName(new_client.fd);
	
	data.addClient(new_client, Client(hostName));
	//sendMsgUser(data.size() - 1, "Welcome to - A O I R C - \n");
}

/* ---------------------------------------------------------------------------------------- */
/*						POLL() AND HANDLE EVENTS	 (incoming requests and inputs)	    //La nueva minishell		*/
/* ---------------------------------------------------------------------------------------- */

void printVector(std::vector<std::string> &input)
{
	(void)input;
	/*for (std::vector<std::string>::iterator word = input.begin(); word != input.end(); word++)
	{
		std::cout << color::yellow << "[" << *word << "]\n";
	}*/
}

void Server::handleInput(clientIt index, std::string input) 
{
	std::vector<std::string>arguments = splitIrcPrameters(input, ' ');
	printVector(arguments);
	//std::cout << "[" << arguments[0].c_str() << "]\n";
	//std::cout << "[" << arguments[1].c_str() << "]\n";


	void (Server::*func)(clientIt index, std::vector<std::string>& arguments) = commands.funcmap[arguments[0]];
	if (func == nullptr)
	{
		std::cout << color::red << "[ERROR DE COMANDO [" <<  input << "]\n" << color::reset ; ////////////ERROR DE COMANDO 
		return;
	}
	(this->*func)(index, arguments);

}

void Server::handleEvents(pollfdIt index)
{
	if (data[index].revents & POLLIN)
	{
		std::string input = readTCPInput(data[index].fd);
		std::cout << color::green << "INPUT:[" << color::boldwhite << input << "]\n" << color::reset;
		std::vector<std::string> lines = split(input, '\n');
		for (uint32_t i = 0; i < lines.size();i++)
		{
		//	std::cout << color::boldgreen << "[" << lines[i] << "]\n" << color::reset;
			handleInput((clientIt)index, lines[i]);
		}
	}
}


/* ---------------------------------------------------------------------------------------- */
/*						COMMAND FUNCTION													*/
/* ---------------------------------------------------------------------------------------- */

void	Server::pass(clientIt index, std::vector<std::string> &arguments)
{
	if (data[index].getAuthentificied())
		return ;
	if (arguments.size() < 2)
	{
		errorHandler.error(index, ERR_PASSWDMISMATCH);
		errorHandler.fatalError(index, ERR_BADPASSWORD);
		return ;
	}
	if (serverInfo.password == arguments[1])
	{
		data[index].setAuthentificied(true);
		return ;
	}
	else
	{
		errorHandler.error(index, ERR_PASSWDMISMATCH);
		errorHandler.fatalError(index, ERR_BADPASSWORD);
		//remove client from server
	}
}


void	Server::nick(clientIt index, std::vector<std::string> &arguments)
{
	if (arguments.size() < 2 || arguments[1].empty())
	{
		errorHandler.error(index, ERR_NONICKNAMEGIVEN);
		//nueva string par que pueda meter uno nuevo 
		return ;
	}
	if (arguments[1].size() == 0)
	{
		errorHandler.error(index, ERR_ERRONEUSNICKNAME);

		return;
	}
	for (std::string::iterator c = arguments[1].begin(); c != arguments[1].end(); c++)
	{
		if (*c == '#' || !std::isprint(*c))
		{
			errorHandler.error(index, ERR_ERRONEUSNICKNAME, arguments[1]); 
			return;
		}
	}
	//nickname repetido -> ERROR y cliente devielve de nuevo NICK + _ ?????
	if (data.findNickname(arguments[1])) 
	{
		errorHandler.error(index, ERR_NICKNAMEINUSE);
		return ;
	}
	else 
	{
		std::string message =  ":" + data[index].getUserMask() + " NICK :" + arguments[1] + "\r\n";
		data[index].setNickname(arguments[1]);
		channels[0].broadcast(0, message); 
		std::string mask = data[index].getUserMask();
	}
}

void	Server::user(clientIt index, std::vector<std::string> &arguments)
{
	if (arguments.size() < 5 || arguments[1].empty()) //NO ARGS
	{
		errorHandler.error(index, ERR_NEEDMOREPARAMS); 
		return ;
	}
	//The maximum length of <username> may be specified by the USERLEN RPL_ISUPPORT parameter.  MUST be silently truncated to the given length 
	// The minimum length of <username> is 1, ie. it MUST NOT be empty. 
	if (data.findUsername(arguments[1])) 
	{
		errorHandler.error(index, ERR_ALREADYREGISTERED); 
		return ;
	}
	data[index].setUsername(arguments[1]);

	if (data[index].getAuthentificied()) //ha autentificado, ha mandado PASS y es ok )
	{
		std::string message = ":" + data[index].getUserMask() + " MOTD\r\n";
		sendMsgUser(data[(pollfdIt)index].fd, message);
		channels[0].addClient(index); 
	}
	else
	{
		errorHandler.error(index, ERR_PASSWDMISMATCH);
		errorHandler.fatalError(index, ERR_BADPASSWORD);
		//remove client from server
	}
}

void	Server::privmsg(clientIt index, std::vector<std::string> &arguments)
{
	if (arguments.size() < 2 || arguments[1].empty()) //NO ARGS
	{
		//std::cout << color::red << "ERROR NORECIPIENT" << color::reset << std::endl;
		errorHandler.error(index, ERR_NORECIPIENT); 
		return ;
	}
	std::vector<std::string> targets = split(arguments[1], ',');
	std::set<std::string> uniqueNames;
	for (std::vector<std::string>::iterator target = targets.begin(); target != targets.end(); target++)
	{
		if (!uniqueNames.insert(*target).second) {
			// ERR_TOOMANYTARGETS (407) // <target> :Duplicate recipients. No message delivered
			errorHandler.error(index, ERR_TOOMANYTARGETS, *target);
			continue ;
		}
		std::string message = ":" + data[index].getNickname() +  " " +  arguments[0] + " " + *target + " :" + arguments[2] + "\r\n";
		if ((*target)[0] == '#') 	//  channel membership prefix character (@, +, etc) 
		{
			
			uint32_t channel = findChannel(target->substr(1));
			if(channel == 0)
			{
				//std::cout << color::red << "ERROR NOCHANNEL 404" << color::reset << std::endl;
				errorHandler.error(index, ERR_CANNOTSENDTOCHAN);//NO CHANNEL 404
				continue;
			}
			////CHECK BANNED??? MOD??? ->>>>>> ERR_CANNOTSENDTOCHAN (404)  
			channels[channel].broadcast(index, message); //PRIVMSG TO CHANNEL SUCCESS 
		}
		else 
		{
			clientIt user = data.findNickname(*target);
			if (user != 0)
			{
				if (data[user].getAwayStatus() == true) // RPL_AWAY (301)
				{
					sendMsgUser(data[(pollfdIt)index].fd, message);
					std::string away_msg = ":" + serverName + " 301 " + data[index].getNickname() + " " + data[user].getNickname() + " :" + data[user].getAwayMsg() + "\r\n";
					sendMsgUser(data[(pollfdIt)index].fd, away_msg);
				}
				else //PRIVMSG TO USER SUCCESS 
				{	
					if(arguments[2].empty()) //ERR_NOTEXTTOSEND (412) 
					{
					//	std::cout << color::red << "ERROR NOTEXT" << color::reset << std::endl;
						errorHandler.error(index, ERR_NOTEXTTOSEND); 
					}
					sendMsgUser(data[(pollfdIt)user].fd, message);
				}
			
			}
			else 
			{
				//std::cout << color::red << "ERROR NO NICK" << color::reset << std::endl;
				errorHandler.error(index, ERR_NOSUCHNICK);
			}
		}
	
	}
}

void	Server::join(clientIt index, std::vector<std::string> &arguments)
{
	if (arguments.size() < 2)
	{
		errorHandler.error(index, ERR_NEEDMOREPARAMS, "JOIN");
		return ;
	}
	std::vector<std::string>channelNames = split(arguments[1], ',');

	//////JOIN 0 TE PIRAS SDE TODOS LOS CANALES 

	for (uint32_t i = 0;i < channelNames.size();i++)
	{
		 //ARGUMENT ERROR 
		if (arguments[1][0] != '#' || arguments[1].size() < 2 || !std::isprint(arguments[1][1]))
		{
			errorHandler.error(index, ERR_BADCHANMASK, arguments[1]);
			continue ;
		}
		uint32_t channel = findChannel(channelNames[i].substr(1));
		//NO EXISTE CHANNEL ->se crea y se une 
		if(!channel) 
		{
			channels.push_back(Channel(channelNames[i].substr(1), data[index].getUsername(), &data));
			channel = channels.size() - 1;
			channels[channel].addClient(index);

			std::string back = ':' + data[index].getUserMask() + " JOIN " + "#" + channels[channel].getName() + "\r\n";//bien
			std::string back_mode = ':' + serverName + " MODE #" + channels[channel].getName() + " " + " +nt\r\n";
			std::string back_list = ':' + serverName + " 353 " + data[index].getNickname() + " = #" + channels[channel].getName() + " :" + channels[channel].getUserList() + "\r\n";//@for the operator
			std::string back_list_end = ':' + serverName + " 366 " + data[index].getNickname() + " #" + channels[channel].getName() + " :End of /NAMES list.\r\n";

			sendMsgUser(data[(pollfdIt)index].fd, back);
			sendMsgUser(data[(pollfdIt)index].fd, back_mode);
			sendMsgUser(data[(pollfdIt)index].fd, back_list);
			sendMsgUser(data[(pollfdIt)index].fd, back_list_end);
		}
		else // EXISTE CHANNEL ->se une
		{
			channels[channel].addClient(index);

			std::string back = ':' + data[index].getUserMask() + " JOIN #" + channels[channel].getName() + "\r\n";
			std::string back_topic = ':' + serverName + " 332 " + data[index].getNickname() + " #" + channels[channel].getName() + " :" + channels[channel].getTopic() + "\r\n";
			//std::string back_channel  = ':' + serverName + " 333 " + data[index].getNickname() + " #" + channelNames[i] + "quien lo ha creado (mask) y cuando"  + "\r\n";
			std::string back_list  = ':' + serverName + " 353 " + data[index].getNickname() + " = #" + channels[channel].getName() + " :" + channels[channel].getUserList() + "\r\n";
			std::string back_list_end = ':' + serverName + " 366 " + data[index].getNickname()  + " #"+ channels[channel].getName() + " :End of /NAMES list." + "\r\n";

			sendMsgUser(data[(pollfdIt)index].fd, back);
			sendMsgUser(data[(pollfdIt)index].fd, back_topic);
			sendMsgUser(data[(pollfdIt)index].fd, back_list);
			sendMsgUser(data[(pollfdIt)index].fd, back_list_end);
		}
		std::string message =  ":" +  data[index].getUserMask() +" JOIN :#" + channels[channel].getName() + "\r\n";
		channels[channel].broadcast(index, message);
	}
}

void	Server::list(clientIt index, std::vector<std::string> &arguments){

	////LISTA RESET EN LIME????
	// los demas parametros obviamos???
	(void)arguments; 
	std::string message = ":" + serverName + " 321: Channel Users Name\r\n";
	sendMsgUser(data[(pollfdIt)index].fd, message);
	for (uint32_t i = 1;i < channels.size() ; i++)
	{
		std::string back = ":" + serverName + " 322 " + data[index].getNickname() + " #" + channels[i].getName() + " " +  std::to_string(channels[i].getNumUser()) + " :" + channels[i].getTopic() + "\r\n";
		sendMsgUser(data[(pollfdIt)index].fd, back);
	}
}

void	Server::part(clientIt index, std::vector<std::string> &arguments)
{
	if (arguments.size() < 2)//ARGUMENT ERROR
	{
		errorHandler.error(index, ERR_NEEDMOREPARAMS , "PART");
		return;
	}
	std::string reason;
	if (arguments.size() > 2)//REASON
	{
		reason = joinStr(arguments, 2);
		//std::cout << "MESSAGE [ " << reason << "]\n";
	}
	std::vector<std::string> channelNames = split(arguments[1], ',');
	for (std::vector<std::string>::iterator channelName = channelNames.begin(); channelName != channelNames.end(); channelName++)
	{
		
		if(!((*channelName)[0] == '#' || (*channelName)[0] == '@')) //NO ES UN CHANNEL 
		{
			errorHandler.error(index, ERR_BADCHANMASK, *channelName);
			continue ;
		}
		uint32_t channel = findChannel(channelName->substr(1));
		if (channel == 0)//NO EXISTE CHANNEL 
		{
			errorHandler.error(index, ERR_NOSUCHCHANNEL, *channelName);
			continue ;
		}
		if(!channels[channel].findUser(index)) //NO ESTAS EN EL CHANNEL 
		{
			errorHandler.error(index, ERR_NOTONCHANNEL, *channelName);
			continue ;
		}
		std::string message;
		if (!reason.empty())
			message = ":" + data[index].getUserMask() + " PART #" + channels[channel].getName() + " :" + reason + "\r\n";
		else
			message = ":" + data[index].getUserMask() + " PART #" + channels[channel].getName() + "\r\n";
		channels[channel].broadcast(0, message);
		channels[channel].removeClient(index);
		if (channels[channel].getNumUser() == 0) //when all users leave a group channel, the channel is deleted
			deleteChannel(channel);
	}
}

void	Server::topic(clientIt index, std::vector<std::string> &arguments) {

	if (arguments.size() < 2)//ARGUMENT ERROR
	{
		errorHandler.error(index, ERR_NEEDMOREPARAMS , "TOPIC");
		return;
	}
	uint32_t channel = findChannel(arguments[1].substr(1));
	if(!(arguments[1][0] == '#' || arguments[1][0] == '@')) //NO ES UN CHANNEL 
	{
		errorHandler.error(index, ERR_BADCHANMASK, arguments[1]);
		return ;
	}
	if (channel == 0)//NO EXISTE CHANNEL 
	{
		errorHandler.error(index, ERR_NOSUCHCHANNEL, arguments[1]);
		return ;
	}
	if(!channels[channel].findUser(index)) //NO ESTAS EN EL CHANNEL 
	{
		errorHandler.error(index, ERR_NOTONCHANNEL, arguments[1]);
		return ;
	}
	//si no eres operator          
			// ERR_CHANOPRIVSNEEDED (482)
			// s-> :irc.example.com 482 dan #v5 :You're not channel operator

	if( arguments.size() >= 3) //SETTING topic
	{
		channels[channel].setTopic(joinStr(arguments, 2));
		std::string message = ":" + data[index].getUserMask() + " TOPIC #" + arguments[1].substr(1) + " :" + channels[channel].getTopic() + "\r\n"; //RPL_TOPIC (332)
		channels[channel].broadcast(0, message);
		channels[channel].setCreationDate(t_chrono::to_time_t(t_chrono::now()));
	}
	else	//VIEWING topic
	{
		if(channels[channel].getTopic().empty())
		{
			std::string no_topic  = ":" + serverName + " 331 " + data[index].getNickname() + " #" + channels[channel].getName() + " :No topic is set.\r\n"; //RPL_TOPIC (332)
			return ;
		}
		std::string message  = ":" + serverName + " 332 " + data[index].getNickname() + " #" + channels[channel].getName() + " :" + channels[channel].getTopic() + "\r\n"; //RPL_TOPIC (332)
		std::time_t date = channels[channel].getCreationDate();
		std::string message2 = ":" + serverName + " 332 " + data[index].getNickname() + " #" + channels[channel].getName() + " " +  channels[channel].getCreator() + " " +  std::ctime(&date) + "\r\n"; //RPL_TOPICWHOTIME (333) //cambiar la fecha a legible?
		channels[channel].broadcast(0, message);
		channels[channel].broadcast(0, message2);
	}
}


void	Server::notice(clientIt index, std::vector<std::string> &arguments)
{
	if (arguments.size() < 2 || arguments[1].empty()) //NO ARGS
	{
		std::cout << color::red << "ERROR NORECIPIENT" << color::reset << std::endl;
		errorHandler.error(index, ERR_NORECIPIENT); 
		return ;
	}
	std::vector<std::string> targets = split(arguments[1], ',');
	std::set<std::string> uniqueNames;
	for (std::vector<std::string>::iterator target = targets.begin(); target != targets.end(); target++)
	{
		if (!uniqueNames.insert(*target).second) {
			errorHandler.error(index, ERR_TOOMANYTARGETS, *target);
			continue ;
		}
		std::string message = ":" + serverName + " NOTICE " + " :-" + data[index].getNickname() + "- " + arguments[2] + "\r\n";
		
		if ((*target)[0] == '#' /*&& usuarioEsOperador()*/) //a un CHANNEL ---- OJO!!!!! El NOTICE para los CHANNELS solo lo pueden usar los OPERADORES
		{
			uint32_t channel = findChannel(target->substr(1));
			if (channel == 0) {
				errorHandler.error(index, ERR_CANNOTSENDTOCHAN);
				continue;
			}
			channels[channel].broadcast(index, message);
		}
		else 
		{
			clientIt user = data.findNickname(*target);
			if (user != 0)
			{
				if (arguments[2].empty()) {
					errorHandler.error(index, ERR_NOTEXTTOSEND); 
				}
				sendMsgUser(data[(pollfdIt)user].fd, message);
			}
			else {
				errorHandler.error(index, ERR_NOSUCHNICK);
			}
		}
	}
}

void	Server::quit(clientIt index, std::vector<std::string> &arguments)
{
	(void)index;
	(void)arguments;
}

void	Server::mode(clientIt index, std::vector<std::string> &arguments)
{
	(void)index;
	(void)arguments;
}

void	Server::names(clientIt index, std::vector<std::string> &arguments)
{
	if (arguments.size() == 1)
	{
		for (std::deque<Channel>::const_iterator target = channels.begin() + 1; target != channels.end(); target++)
		{
			std::string message = ':' + serverName + " 353 " + data[index].getNickname() + " @ #" + target->getName() + " :" + target->getUserList() + "\r\n";
			std::string back_list_end = ':' + serverName + " 366 " + data[index].getNickname() + " #" + target->getName() + " :End of /NAMES list.\r\n";
			sendMsgUser(data[(pollfdIt)index].fd, message);
			sendMsgUser(data[(pollfdIt)index].fd, back_list_end);
		}
		return ;
	}
	std::vector<std::string> targets = split(arguments[1], ',');
	for (std::vector<std::string>::const_iterator target = targets.begin(); target != targets.end(); target++)
	{
		uint32_t channel = findChannel(target->substr(1));
		if (!channel)
		{
			errorHandler.error(index, ERR_NOSUCHCHANNEL, arguments[1]);
			continue;
		}
		std::string message = ':' + serverName + " 353 " + data[index].getNickname() + " = #" + channels[channel].getName() + " :" + channels[channel].getUserList() + "\r\n";
		std::string back_list_end = ':' + serverName + " 366 " + data[index].getNickname() + " #" + channels[channel].getName() + " :End of /NAMES list.\r\n";
		sendMsgUser(data[(pollfdIt)index].fd, message);
		sendMsgUser(data[(pollfdIt)index].fd, back_list_end);
	}
}

void	Server::whois(clientIt index, std::vector<std::string> &arguments)
{
	(void)index;
	(void)arguments;
}

void	Server::kick(clientIt index, std::vector<std::string> &arguments)
{
	if (arguments.size() < 3)
	{
		errorHandler.error(index, ERR_NEEDMOREPARAMS , "KICK");
		return;
	}
	//REASON
	std::string reason;
	if (arguments.size() == 4)
	{
		reason = arguments[3];
	}
	else
		reason = "The kick hammer has spoken!";   //no me gustaaaaaaa
	uint32_t channel = findChannel(arguments[1].substr(1));
	if (channel == 0)
	{
		errorHandler.error(index, ERR_NOSUCHCHANNEL, arguments[1]);
		return ;
	}
	if (channels[channel].findUser(index) == 0) // NO ESTAS EN EL CHANNEL
	{
		errorHandler.error(index, ERR_NOTONCHANNEL, arguments[1]);
	}
	//ERR_CHANOPRIVSNEEDED (482) //no permissions
	std::vector<std::string>targets = split(arguments[2], ',');
	for(std::vector<std::string>::const_iterator target = targets.begin();target != targets.end(); target++)
	{
		clientIt clientIdx = data.findNickname(*target);
		if (clientIdx == 0) 									//no existe target
			errorHandler.error(index, ERR_NOSUCHNICK);
		else if (channels[channel].findUser(clientIdx) == 0) 	//TARGET no esta en el canal 
			errorHandler.error(index, ERR_USERNOTINCHANNEL);
		else
		{
			std::string broadcast_message = ":" + data[index].getUserMask() + " KICK " + " #" + channels[channel].getName() + " " + data[clientIdx].getNickname() + " :" + reason + "\r\n";
			channels[channel].broadcast(0, broadcast_message);
			channels[channel].removeClient(clientIdx);
		}
	}
}

void	Server::away(clientIt index, std::vector<std::string> &arguments)
{
	if (data[index].getAwayStatus() == true || arguments.size() < 2) {
		data[index].setAwayStatus(false);
		data[index].setAwayMsg("");
		std::string message  = ":" + data[index].getUserMask() + " 305 " + data[index].getNickname() + " :You are no longer marked as being away\r\n";
		sendMsgUser(data[(pollfdIt)index].fd, message);
		return ;
	}
	data[index].setAwayMsg(joinStr(arguments, 1));
	data[index].setAwayStatus(true);
	std::string message  = ":" + data[index].getUserMask() + " 306 " + data[index].getNickname() + " :You have been marked as being away\r\n";
	sendMsgUser(data[(pollfdIt)index].fd, message);
}

void	Server::invite(clientIt index, std::vector<std::string> &arguments)
{
	if (arguments.size() < 2)//ARGUMENT ERROR
	{
		errorHandler.error(index, ERR_NEEDMOREPARAMS , "INVITE");
		return;
	}
	else if (arguments.size() == 3)
	{
		clientIt target_user = data.findNickname(arguments[1]);
		//cliente no esta conectado
		std::string channel_name = arguments[2].substr(1);
		uint32_t channel = findChannel(channel_name);
		if (channel == 0)
			errorHandler.error(index, ERR_NOSUCHCHANNEL, arguments[2]); // NO EXISTE CHANNEL
		else if (!channels[channel].findUser(index))
			errorHandler.error(index, ERR_NOTONCHANNEL, data[index].getNickname()); //NO ESTAS EN ESE CHANNEL
		//ERR_CHANOPRIVSNEEDED (482)
		else if (channels[channel].findUser(target_user)) //TARGET YA ESTA EN EL CHANNEL
		{
			errorHandler.error(index, ERR_USERONCHANNEL, arguments[1]);
		}
		else
		{
			std::string message = ":" + data[index].getUserMask() + " " + arguments[0] + " "+ data[target_user].getNickname() + " " + arguments[2] + "\r\n"; //reply->  :dan-!d@localhost INVITE Wiz #test
			sendMsgUser(data[(pollfdIt)target_user].fd, message);  ///SUCCESSSSS
		}
	}
	/*else if (arguments.size() == 2)
	{
		//list //RPL_INVITELIST (336) // RPL_ENDOFINVITELIST (337)
	}
	*/

}

void	Server::motd(clientIt index, std::vector<std::string> &arguments) {
	(void)arguments;
	std::string message = ":" + data[index].getUserMask() + " 375 " + data[index].getNickname() + " :- " + SERVER_NAME + " Message of the day - \r\n";
	sendMsgUser(data[(pollfdIt)index].fd, message);

	std::vector<std::string> words = split(std::string(MOTD), '\n');
	for (std::vector<std::string>::iterator it = words.begin(); it != words.end(); it++) {
		std::string motd_message = ":" + data[index].getUserMask() + " 372 " + data[index].getNickname() + " : " + *it + "\r\n";
		sendMsgUser(data[(pollfdIt)index].fd, motd_message);
	}
	message = ":" + data[index].getUserMask() + " 376 " + data[index].getNickname() + " :End of /MOTD command.\r\n";
	sendMsgUser(data[(pollfdIt)index].fd, message);
}

void	Server::ping(clientIt index, std::vector<std::string> &arguments)
{
	if (arguments.size() < 2)//ARGUMENT ERROR 
	{
		errorHandler.error(index, ERR_NEEDMOREPARAMS , "PING"); // ERR_NOORIGIN (409) IN THE OLD IRCS 
		return;
	}
	std::string mask = data[index].getUserMask();
	std::string message = ":" + mask + arguments[0] + " PONG " + joinStr(arguments, 2) + "\r\n";
	sendMsgUser(data[(pollfdIt)index].fd, message);
}



/*--------------CAPABILITIES NEGOTIATION -------------*/


void	Server::cap(clientIt index, std::vector<std::string> &arguments) 
{
	void (Server::*cap_func)(clientIt index, std::vector<std::string>& arguments) = commands.cap_funcmap[arguments[0]];
	if (cap_func != nullptr)
	{
		(this->*cap_func)(index, arguments);
		return;
	}
}
//COMMAND CAP FUNCTIONS

void	Server::cap_req(clientIt index, std::vector<std::string> &arguments)
{

	//std::cout << "REQ REACHED\n";
	std::string ack = "CAP * ACK";
	std::string nack = "CAP * NACK";

	//en vez de esto podemos usar una funcion generica  Server::cap_available() que devuelva las capabilities disponibles, osea las que coinciden en un vector 

	for (uint32_t i = 3; i < arguments.size(); i++)
	{
		//std::cout << color::green <<"[" << i << "]"<< arguments[i] << "\n";
		bool found = false;
		//for (uint32_t j = 0; j < COMMANDS;j++)
		//{
		//0	std::cout << "	[" << j << "]"<<commands.cmd[j] << "\n" << color::reset;
			if (arguments[i] == "multi-prefix")
			{
				ack +=  " multi-prefix";
				found = true;
			}
			else
				nack += " " + arguments[i];
		/*}
		if (!found)
		{
		}*/
	}
	sendMsgUser(data[(pollfdIt)index].fd, ack);
	std::cout << "SENDED CAP [" << ack << "]\n";
	std::cout << "SENDED NACK [" << nack << "]\n";
	sendMsgUser(data[(pollfdIt)index].fd, nack);
	//sendMssgUser(data[(pollfdIt)index].fd, "CAP END")
}

void	Server::cap_ls(clientIt index, std::vector<std::string> &arguments)
{
	(void)arguments;
	//std::cout << "CAP LS REACHED\n";
	//mostrar las capabilities que ofrecemos, 	REPASARRRRRRRRRRRRRRRRRRRRRRRRRRRR
	std::string message = "CAP * LS :multi-prefix sasl ";//sasl account-notify extended-join away-notify chghost userhost-in-names cap-notify server-time message-tags invite-notify batch echo-message account-tag";
	sendMsgUser(data[(pollfdIt)index].fd, message);
}

void	Server::cap_end(clientIt index, std::vector<std::string> &arguments)
{
	(void)index;
	(void)arguments;	
	//mirar que es 001
	std::string message = "001 " + data[index].getNickname() + " :Welcome to the A O I R C server\n";
	sendMsgUser(data[(pollfdIt)index].fd, message);
}

void	Server::cap_ack(clientIt index, std::vector<std::string> &arguments)
{
	(void)arguments;
	std::string message = "CAP * ACK : multi-prefix sasl account-notify extended-join away-notify chghost userhost-in-names cap-notify server-time message-tags invite-notify batch echo-message account-tag";
	sendMsgUser(data[(pollfdIt)index].fd, message);
	//sendMsgUser(index, "CAP * END");
}
	
	
void	Server::cap_nak(clientIt index, std::vector<std::string> &arguments)
{

	(void)index;
	(void)arguments;
}
		
std::string Server::getName()const
{
	return serverName;
}

/*void	Server::cap_available(std::vector<std::string> &arguments)
{
		//va a devolver las capabilities disponibles, osea las que coinciden 

	std::vector<std::string> availables;


		

}*/


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

std::string Server::readTCPInput(int client_fd) {
	char echoBuffer[RCVBUFSIZE];
	int	recvMsgSize;

	memset(echoBuffer, 0, RCVBUFSIZE);
	recvMsgSize = recv(client_fd, echoBuffer, sizeof(echoBuffer) - 1, 0);
	if (recvMsgSize == SERVER_FAILURE)
	{
		perror("recv failed, debug here: ");
		//exit (EXIT_FAILURE);
		return (std::string(nullptr));
	}
	//////////////////
	//////////////////
	//////////////////SI SALE ALGUIEN DEL SERVIDOR, HAY QUE CERRAR EL FD CORRECTAMENTE PORQUE SI NO CRASHEA EL SERVIDOR
	//////////////////HAY QUE SOLUCIONAR ESTO!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//////////////////
	//////////////////
	else if (recvMsgSize == 0) {
		close(client_fd);
		return std::string("A client was disconnected from server");
	}
	///////////////LIMPIACARRO///////////
	ssize_t index = 0;
	for(ssize_t i = 0; i < recvMsgSize; i++)
	{
		if (echoBuffer[i] != '\r')
		{
			echoBuffer[index] = echoBuffer[i];
			index++;
		}
	}
	write(0, echoBuffer, index);
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

/*
bool checkAdmin(Client *client) {
	if (client->getRole() == CL_ROOT)
		return true;
	else
		return false;
}
*/


/* ---------------------------------------------------------------------------------------- */
/*						INIT SETTINGS													*/
/* ---------------------------------------------------------------------------------------- */



void Server::setCommands()
{
	commands.funcmap["PASS"]	= &Server::pass;
	commands.funcmap["NICK"]	= &Server::nick;
	commands.funcmap["USER"]	= &Server::user;
	commands.funcmap["JOIN"]	= &Server::join;
	commands.funcmap["PRIVMSG"]	= &Server::privmsg;
	commands.funcmap["NOTICE"]	= &Server::notice;
	commands.funcmap["QUIT"]	= &Server::quit;
	commands.funcmap["MODE"]	= &Server::mode;
	commands.funcmap["NAMES"]	= &Server::names;
	commands.funcmap["WHOIS"]	= &Server::whois;
	commands.funcmap["KICK"]	= &Server::kick;
	commands.funcmap["AWAY"]	= &Server::away;
	commands.funcmap["INVITE"]	= &Server::invite;
	commands.funcmap["PING"]	= &Server::ping;
	commands.funcmap["CAP"]		= &Server::cap;
	commands.funcmap["TOPIC"]	= &Server::topic;
	commands.funcmap["LIST"]	= &Server::list;
	commands.funcmap["PART"]	= &Server::part;
	commands.funcmap["MOTD"]	= &Server::motd;

	commands.cap_funcmap["CAP_REQ"]	= &Server::cap_req;
	commands.cap_funcmap["CAP_LS"]	= &Server::cap_ls;
	commands.cap_funcmap["CAP_END"]	= &Server::cap_end;
	commands.cap_funcmap["CAP_ACK"]	= &Server::cap_ack;
	commands.cap_funcmap["CAP_NAK"]	= &Server::cap_nak;
}


/* ---------------------------------------------------------------------------------------- */
/*						DEBUG PRINT													*/
/* ---------------------------------------------------------------------------------------- */


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
		printIp();
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

void	Server::deleteChannel(uint32_t channel)
{
	channels.erase(channels.begin() + channel);
}
