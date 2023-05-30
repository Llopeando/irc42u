#include "../include/Server.hpp"
#include <chrono>
/* ------------------------------------------------------------ */
/* 			CONSTRUCTOR DESTRUCTOR INITIALIZATION 				*/
/* ------------------------------------------------------------ */

Server::Server(t_serverInput *serverInput):serverInfo(*serverInput),serverName(serverInput->IP)
{
	data =  UsersData(serverInfo);
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
	for (uint32_t i = 0; i < COMMANDS; i++)
	{
		if (commands.cmd[i] == arguments[0])
		{
			//std::cout << "command reached\n";
			//////////////KAREN/////////////////////
			(this->*(commands.func[i]))(index, arguments);
			return ;
		}
	}	
	//std::cout << "[Received message : " << input << "]\n";
	//AQUI TENEMOS QUE HACER BROADCAST
	
}

void Server::handleEvents(pollfdIt index)
{
	if (data[index].revents & POLLIN)
	{
		std::string input = readTCPInput(data[index].fd);
		std::cout << color::green << "INPUT:[" << color::boldwhite << input << "]\n" << color::reset;
	//	std::cout <<"\n" << color::reset;
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
	
	data[index].setUsername(arguments[1]);
	//sendMsgUser(data[(pollfdIt)index].fd, message);
	std::string message = "001 " + data[index].getNickname() + " :Welcome to A O I R C\n" ;
	sendMsgUser(data[(pollfdIt)index].fd, message);
	channels[0].addClient(index); //join to lobby
	//std::cout << "----------New user changed: " << data[index].getUsername() << "]\n";

}


// REPLY //sendMsgUser(fd, STRING);
void	Server::privmsg(clientIt index, std::vector<std::string> &arguments)
{
	//revisar opciones de MSG , gestion de error de USER no existente etc
	std::string message = ":" + data[index].getNickname() +  " " +  arguments[0] + " " + arguments[1] + " :" + joinStr(arguments, 2) + "\r\n";
 
	if (arguments[1][0] == '#') 	//to a CHANNEL 
	{
		uint32_t channel = findChannel(arguments[1].substr(1, arguments[1].size() - 1));
		if(channel == 0)
		{
			errorHandler.error(index, ERR_CANNOTSENDTOCHAN);//NO CHANNEL 404
			return;
		}
		channels[channel].broadcast(index, message);
	}
/*	if (MENSAJE AL SERVIDOR)
	{
		402 ERR_NOSUCHSERVER
	}
	*/
	else 	//to an USER 
	{
		clientIt user = data.findNickname(arguments[1]);
		if (user == 0)
		{
			errorHandler.error(index, ERR_NOSUCHNICK);
			return;
		}
		if (data[user].getAwayStatus() == true)
		{
			sendMsgUser(data[(pollfdIt)index].fd, message);
			std::string mask = data[index].getUserMask();
			std::string away_msg = ":" + serverName + " 301 " + data[index].getNickname() + " " + data[index].getNickname() + " :" + data[index].getAwayMsg() + "\r\n";
			sendMsgUser(data[(pollfdIt)index].fd, away_msg);
		}
		else
		{
			sendMsgUser(data[(pollfdIt)user].fd, message);
		}
	}
}
/*

ERR_NOSUCHNICK (401)
ERR_NOSUCHSERVER (402)
ERR_CANNOTSENDTOCHAN (404)
ERR_TOOMANYTARGETS (407)
ERR_NORECIPIENT (411)
ERR_NOTEXTTOSEND (412)
ERR_NOTOPLEVEL (413)
ERR_WILDTOPLEVEL (414)
RPL_AWAY (301)

*/

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
		uint32_t channel = findChannel(channelNames[i].substr(1, channelNames[i].size() - 1));
		//NO EXISTE CHANNEL ->se crea y se une 
		if(!channel) 
		{
			channels.push_back(Channel(channelNames[i].substr(1, channelNames[i].size() - 1), data[index].getUsername(), &data));
			channel = channels.size() - 1;
			channels[channel].addClient(index);

			std::string back = ':' + data[index].getUserMask() + " JOIN " + "#" + channels[channel].getName() + "\r\n";//bien
			std::string back_mode = ':' + std::string(SERVER_NAME) + " MODE #" + channels[channel].getName() + " " + " +nt\r\n";
			std::string back_list = ':' + std::string(SERVER_NAME) + " 353 " + data[index].getNickname() + " = #" + channels[channel].getName() + " :" + channels[channel].getUserList() + "\r\n";//@for the operator
			std::string back_list_end = ':' + std::string(SERVER_NAME) + " 366 " + data[index].getNickname() + " #" + channels[channel].getName() + " :End of /NAMES list.\r\n";

			sendMsgUser(data[(pollfdIt)index].fd, back);
			sendMsgUser(data[(pollfdIt)index].fd, back_mode);
			sendMsgUser(data[(pollfdIt)index].fd, back_list);
			sendMsgUser(data[(pollfdIt)index].fd, back_list_end);
		}
		else // EXISTE CHANNEL ->se une
		{
			channels[channel].addClient(index);

			std::string back = ':' + data[index].getUserMask() + " JOIN #" + channels[channel].getName() + "\r\n";
			std::string back_topic = ':' + std::string(SERVER_NAME) + " 332 " + data[index].getNickname() + " #" + channels[channel].getName() + " :" + channels[channel].getTopic() + "\r\n";
			//std::string back_channel  = ':' + std::string(SERVER_NAME) + " 333 " + data[index].getNickname() + " #" + channelNames[i] + "quien lo ha creado (mask) y cuando"  + "\r\n";
			std::string back_list  = ':' + std::string(SERVER_NAME) + " 353 " + data[index].getNickname() + " @ #" + channels[channel].getName() + " :" + channels[channel].getUserList() + "\r\n";
			std::string back_list_end = ':' + std::string(SERVER_NAME) + " 366 " + data[index].getNickname()  + " #"+ channels[channel].getName() + " :End of /NAMES list." + "\r\n";

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
	std::string message = ":" + std::string(SERVER_NAME) + " 321: Channel Users Name\r\n";
	sendMsgUser(data[(pollfdIt)index].fd, message);
	for (uint32_t i = 1;i < channels.size() ; i++)
	{
		std::string back = ":" + std::string(SERVER_NAME) + " 322 " + data[index].getNickname() + " #" + channels[i].getName() + " " +  std::to_string(channels[i].getNumUser()) + " :" + channels[i].getTopic() + "\r\n";
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
		std::cout << "MESSAGE [ " << reason << "]\n";
	}
	std::vector<std::string> channelNames = split(arguments[1], ',');
	for (std::vector<std::string>::iterator channelName = channelNames.begin(); channelName != channelNames.end(); channelName++)
	{
		
		if(!((*channelName)[0] == '#' || (*channelName)[0] == '@')) //NO ES UN CHANNEL 
		{
			errorHandler.error(index, ERR_BADCHANMASK, *channelName);
			continue ;
		}
		uint32_t channel = findChannel(channelName->substr(1, channelName->size() - 1));
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

////////////////////////////////////////////

void	Server::topic(clientIt index, std::vector<std::string> &arguments) {

	if (arguments.size() < 2)//ARGUMENT ERROR
	{
		errorHandler.error(index, ERR_NEEDMOREPARAMS , "TOPIC");
		return;
	}
	uint32_t channel = findChannel(arguments[1].substr(1, arguments[1].size() - 1));
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
		std::string message = ":" + data[index].getUserMask() + " TOPIC #" + arguments[1].substr(1, arguments[1].size() - 1) + " :" + channels[channel].getTopic() + "\r\n"; //RPL_TOPIC (332)
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
	std::string message = ":" + serverName +  " NOTICE " +  arguments[0] + " :" + joinStr(arguments, 1) + "\r\n";
	if (arguments[1][0] == '#' /*&& usuarioEsOperador()*/) //a un CHANNEL ---- OJO!!!!! El NOTICE para los CHANNELS solo lo pueden usar los OPERADORES
	{
		uint32_t channel = findChannel(arguments[1].substr(1, arguments[1].size() - 1));
		if(channel == 0)
		{
			errorHandler.error(index, ERR_CANNOTSENDTOCHAN);//NO CHANNEL 404
			return;
		}
		channels[channel].broadcast(index, message);
	}
/*	if (MENSAJE AL SERVIDOR)
	{
		402 ERR_NOSUCHSERVER
	}
	*/
	else	//to a USER
	{
		clientIt user = data.findNickname(arguments[1]);
		if (user == 0) {
			errorHandler.error(index, ERR_NOSUCHNICK);
			return;
		}
		sendMsgUser(data[(pollfdIt)user].fd, message);
	}
}

// void	Server::quit(clientIt index, std::vector<std::string> &arguments)
// {
// 	(void)index;
// 	(void)arguments;
// }

// void	Server::mode(clientIt index, std::vector<std::string> &arguments)
// {
// 	(void)index;
// 	(void)arguments;
// }

// void	Server::names(clientIt index, std::vector<std::string> &arguments)
// {
// 	(void)index;
// 	(void)arguments;
// }

// void	Server::whois(clientIt index, std::vector<std::string> &arguments)
// {
// 	(void)index;
// 	(void)arguments;
// }

// void	Server::kick(clientIt index, std::vector<std::string> &arguments)
// {
// 	(void)index;
// 	(void)arguments;
// }

void	Server::away(clientIt index, std::vector<std::string> &arguments)
{
	std::string mask = data[index].getUserMask();
	if (data[index].getAwayStatus() == true || arguments.size() < 2) {
		data[index].setAwayStatus(false);
		data[index].setAwayMsg("");
		std::string message  = ":" + mask + " 305 " + data[index].getNickname() + " :You are no longer marked as being away\r\n";
		sendMsgUser(data[(pollfdIt)index].fd, message);
		return ;
	}
	data[index].setAwayMsg(joinStr(arguments, 1));
	data[index].setAwayStatus(true);
	std::string message  = ":" + mask + " 306 " + data[index].getNickname() + " :You have been marked as being away\r\n";
	sendMsgUser(data[(pollfdIt)index].fd, message);
}
/*	
void	Server::invite(clientIt index, std::vector<std::string> &arguments)
{
	(void)index;
	(void)arguments;
}

*/
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
	//std::cout << "CAP REACHED[" << arguments[1] << "]\n";
	return;

	for (uint32_t i = 0; i < CAP_COMMANDS;i++)
	{
		if (commands.cap_cmd[i] == arguments[1])
		{
			(this->*(commands.cap_func[i]))(index, arguments);
		}
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
	commands.cmd[0]  = NICK;
	commands.cmd[1]  = USER;
	commands.cmd[2]  = JOIN;
	commands.cmd[3]  = PART;
	commands.cmd[4]  = PRIVMSG;
	commands.cmd[5]  = NOTICE;
	//commands.cmd[6]  = QUIT;
	//commands.cmd[8]  = MODE;
	//commands.cmd[9]  = NAMES;
	//commands.cmd[10] = WHOIS;
	//commands.cmd[11] = KICK;
	commands.cmd[6] = AWAY;
	//commands.cmd[13] = INVITE;
	commands.cmd[7] = PING;
	commands.cmd[8] = CAP;
	commands.cmd[9] = TOPIC;
	commands.cmd[10] = LIST;

	commands.func[0] = &Server::nick;
	commands.func[1] = &Server::user;
	commands.func[2] = &Server::join;
	commands.func[3] = &Server::part;
	commands.func[4] = &Server::privmsg;
	commands.func[5] = &Server::notice;
	commands.func[6] = &Server::away;
	commands.func[7] = &Server::ping;
	commands.func[8] = &Server::cap;
	commands.func[9]  = &Server::topic;
	commands.func[10]  = &Server::list;
	

	//commands.func[5]  = &Server::notice;
	//commands.func[6]  = &Server::quit;
	//commands.func[8]  = &Server::mode;
	//commands.func[9]  = &Server::names;
	//commands.func[10] = &Server::whois;
	//commands.func[11] = &Server::kick;
	//commands.func[13] = &Server::invite;


	commands.cap_cmd[0]  = CAP_REQ;
	commands.cap_cmd[1]  = CAP_LS;
	commands.cap_cmd[2]  = CAP_END;
	commands.cap_cmd[3]  = CAP_ACK;
	commands.cap_cmd[4]  = CAP_NAK;

	commands.cap_func[0] = &Server::cap_req;
	commands.cap_func[1] = &Server::cap_ls;
	commands.cap_func[2] = &Server::cap_end;
	commands.cap_func[3] = &Server::cap_ack;
	commands.cap_func[4] = &Server::cap_nak;
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